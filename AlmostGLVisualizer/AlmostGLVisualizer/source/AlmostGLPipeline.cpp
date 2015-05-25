#include "AlmostGLPipeline.h"
#include <algorithm>

namespace AlmostGL
{
    std::vector<Triangle4D> runVertexPipeline(const Model3D& model, 
        const Camera& camera, ViewPort vp, WindingOrder order, 
        Material material, const LightParameters& light)
    {

        // Transform model vertices to homogeneous coordinates and calculate color
        std::vector<Triangle4D> triangles;
        triangles.resize((int)model.triangles.size());
        for (int i = 0; i < model.triangles.size(); ++i)
        {
            for (int j = 0; j < 3; ++j) // iterate over vertices in the triangle
            {
                triangles[i].v[j].pos = model.triangles[i].vertex[j];

                // Calculate color
                if (light.mode == NONE)
                {
                    triangles[i].v[j].color = material.ambient;
                }
                else // Lighting is on
                {
                    triangles[i].v[j].color = calculateIllumination(
                        model.triangles[i].vertex[j], 
                        model.triangles[i].normal[j],
                        material, camera, light);
                }
            }            
            // When shading model is flat, make all vertices
            // in the triangle the same color (average)
            if (light.mode == FLAT)
            {
                Vector3f average_color = (triangles[i].v[0].color +
                    triangles[i].v[1].color + triangles[i].v[2].color) / 3.f;
                for (int j = 0; j < 3; ++j)
                    triangles[i].v[j].color = average_color;
            }
            triangles[i].clipped = false;
        }

        // Compute View Matrix
        Vector3f u = camera.u(), v = camera.v(), n = camera.n();
        Vector3f position = camera.position();
        Matrix4x4f view_matrix = {
            { u.x, u.y, u.z, dotProduct(-position, u) },
            { v.x, v.y, v.z, dotProduct(-position, v) },
            { n.x, n.y, n.z, dotProduct(-position, n) },
            { 0, 0, 0, 1 } };

        /* Compute Projection Matrix
        
            -Shearing element was removed for performance since interface provided
            enforces a symmetric field of view
            -Scale elements were substituted by equivalents of 1/(r-l) and 1/(t-b)
            obtained calculating tangent of field of view angle.
        */

        float znear = camera.Znear();
        float zfar = camera.Zfar();
        float Vfov_rad = degreeToRadians(camera.Vfov() / 2.f);
        float Hfov_rad = degreeToRadians(camera.Hfov() / 2.f);
        float topBottom = 1.f / tan(Vfov_rad);
        float leftRight = 1.f / tan(Hfov_rad);
        Matrix4x4f projection_matrix = {
            { leftRight, 0, 0, 0 },
            { 0, topBottom, 0, 0 },
            { 0, 0, -(zfar + znear) / (zfar - znear), -(2.f*zfar*znear) / (zfar - znear) },
            { 0, 0, -1, 0 } };

        // Compute the projection/model/view matrix
        Matrix4x4f PM = projection_matrix*view_matrix;

        // Multiply every vertex by the matrix
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                triangle.v[i].pos = PM*triangle.v[i].pos;

                // Mark triangles outside the view volume to be clipped
                float x = abs(triangle.v[i].pos.x);
                float y = abs(triangle.v[i].pos.y);
                float z = abs(triangle.v[i].pos.z);
                float w = abs(triangle.v[i].pos.w);
                if (x > w || y > w || z > w)
                    triangle.clipped = true;
            }            
        }

        // Remove clipped triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [](Triangle4D& tri) { return tri.clipped; }), triangles.end());

        // Perform the perspective division of all elements by w;
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                float w = triangle.v[i].pos.w;
                triangle.v[i].pos.x /= w;
                triangle.v[i].pos.y /= w;
                triangle.v[i].pos.z /= w;
                triangle.v[i].pos.w /= w;

                triangle.v[i].color[0] /= w;
                triangle.v[i].color[1] /= w;
                triangle.v[i].color[2] /= w;

                triangle.v[i].interpolation_factor = 1/w;

                // TODO: add texture coordinates

            }
        }

        // Compute the viewport matrix
        Matrix4x4f viewport_matrix = {
            { (vp.right - vp.left) / 2.f, 0, 0, (vp.right + vp.left) / 2.f },
            { 0, (vp.top - vp.bottom) / 2.f, 0, (vp.top + vp.bottom) / 2.f },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 } };

        // Perform translation and scaling of normalized device coordinates by
        // the viewport.
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                triangle.v[i].pos = viewport_matrix*triangle.v[i].pos;
            }

            // Cull back facing polygons (technique from OpenGL Red Book)
            int area = 0;
            for (int i = 0; i < 3; ++i)
            {
                int j = (i + 1) % 3;
                area += (triangle.v[i].pos.x * triangle.v[j].pos.y)
                    - (triangle.v[j].pos.x * triangle.v[i].pos.y);
            }
            // Dividing the area by 2 (area = area / 2;) was deemed unnecessary
            // due to not affecting the signal of the value, so was removed for
            // performance reasons
            if (order == CCW)
            {
                if (area < 0)
                    triangle.clipped = true;
            }
            else // order == CW
            {
                if (area > 0)
                    triangle.clipped = true;
            }
        }
        // Remove culled triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [](Triangle4D& tri) { return tri.clipped; }), triangles.end());

        return triangles;
    }

    Vector3f calculateIllumination(const Vector3f& vertex, 
        const Vector3f& normal, const Material& material, const Camera& camera, 
        const LightParameters& light)
    {
        Vector3f color = { 0.f, 0.f, 0.f };
        Vector3f V = (camera.position() - vertex).normalize();
        Vector3f N = getNormalized(normal);
        // Iterate over colors
        for (int i = 0; i < 3; ++i)
        {
            // Calculate ambient component and add to final color
            color[i] += light.ambient_color[i] * material.ambient[i];

            // Calculate diffuse and specular component for each light source
            for (LightSource source : light.sources)
            {
                Vector3f L = (source.position - vertex);
                float dist = L.length();
                float att_factor = 1 / (
                    light.constant_attenuation +
                    light.linear_attenuation * dist +
                    light.quadratic_attenuation * dist*dist);

                att_factor = std::min(att_factor, 1.f);
                L.normalize();
                Vector3f R = ((2 * dotProduct(N, L)) * N - L).normalize();

                float component = att_factor * source.color[i] * (
                    material.diffuse[i] * (dotProduct(N, L)) +
                    material.specular[i] * pow(dotProduct(V, R), material.shine));

                // Add source diffuse+specular component to final color
                color[i] += component;
            }
        }
        return color;
    }

    void runRasterization(const std::vector<Triangle4D>& triangles,
        FrameBuffer& buffer, PolygonMode mode)
    {
        for (auto triangle : triangles)
        {
            if (mode == POINTS)
            {
                for (int i = 0; i < 3; ++i)
                {
                    // Points polygon mode
                    triangle.v[i].color /= triangle.v[i].interpolation_factor;
                    buffer(triangle.v[i].pos.x, triangle.v[i].pos.y, AlmostGL::RED) = triangle.v[i].color[0];
                    buffer(triangle.v[i].pos.x, triangle.v[i].pos.y, AlmostGL::GREEN) = triangle.v[i].color[1];
                    buffer(triangle.v[i].pos.x, triangle.v[i].pos.y, AlmostGL::BLUE) = triangle.v[i].color[2];
                }
            }
            else if (mode == WIREFRAME || mode == FILL)
            {
                rasterTriangle(triangle, buffer, mode);
            }
        }
    }

    void rasterTriangle(Triangle4D& triangle, FrameBuffer& buffer, PolygonMode mode)
    {
        // Sort vertices by ascending y then by ascending x
        std::sort(std::begin(triangle.v), std::end(triangle.v),
            [](const Vertex& v1, Vertex& v2)
            {
                if (v1.pos.y == v2.pos.y)
                    return v1.pos.x < v2.pos.x;
                else
                    return v1.pos.y < v2.pos.y;
            });
        std::vector<std::vector<Vertex>> edges(3);
        // Run interpolations for triangle edges
        edges[0] = bresenham(triangle.v[0], triangle.v[1],buffer,true);
        edges[1] = bresenham(triangle.v[0], triangle.v[2],buffer,true);
        edges[2] = bresenham(triangle.v[1], triangle.v[2],buffer,true);
        // Fill triangle
        if (mode == FILL)
        {
            int e0 = 0;
            int e1 = 1;
            bool changed = false, end = false;
            int i0 = 0, i1 = 0;
            do
            {
                if (i0 < edges[e0].size())
                {
                    if (i1 < edges[e1].size())
                    {
                        bresenham(edges[e0][i0], edges[e1][i1], buffer);
                        ++i0; ++i1;
                    }
                    else if (!changed)
                    {
                        e1 = 2;
                        i1 = 0;
                        changed = true;
                    }
                    else
                    {
                        end = true;
                    }
                }
                else if (!changed)
                {
                    e0 = 2;
                    i0 = 0;
                    changed = true;
                }
                else
                {
                    end = true;
                }
            } while (!end);
        }
    }
    
    std::vector<Vertex> bresenham(const Vertex& start, const Vertex& end,
        FrameBuffer& buffer, bool output_vertices /*= false*/)
    {
        float deltax = (int)end.pos.x - (int)start.pos.x;
        float deltay = (int)end.pos.y - (int)start.pos.y;
        std::vector<Vertex> result;
        if (output_vertices)
        {
            result.reserve(std::max(fabs(deltax), fabs(deltay)));
        }        
        if (deltax == 0 && deltay == 0) // single point
        {
            return result; // Return empty
        }
        else if (deltay == 0) // horizontal line
        {
            int y = start.pos.y;
            int x = start.pos.x;
            int inc_x = deltax > 0 ? 1 : -1;
            while (inc_x == 1 ? x <= end.pos.x : x >= end.pos.x)
            {
                writeToBuffer(createInterpolated(start, end, x, y), buffer);
                x = x + inc_x;
            }
        }
        else if (deltax == 0) // vertical line
        {
            int y = start.pos.y;
            int x = start.pos.x;
            int inc_y = deltay > 0 ? 1 : -1;
            while (inc_y == 1 ? y <= end.pos.y : y >= end.pos.y)
            {
                Vertex v = createInterpolated(start, end, x, y);
                writeToBuffer(v,buffer);
                if (output_vertices)
                    result.push_back(v);
                y = y + inc_y;
            }
        }
        else //(deltax != 0 && deltay != 0)
        {
            float error = 0;
            float deltaerr = abs(deltay / deltax);
            int y = start.pos.y;
            int x = start.pos.x;
            int inc_x = deltax > 0 ? 1 : -1;
            int inc_y = deltay > 0 ? 1 : -1;
            while (inc_x == 1 ? x <= end.pos.x : x >= end.pos.x)
            {
                writeToBuffer(createInterpolated(start, end, x, y), buffer);
                error = error + deltaerr;
                while (error >= 0.5 && (inc_y == 1 ? y <= end.pos.y : y >= end.pos.y))
                {
                    Vertex v = createInterpolated(start, end, x, y);
                    writeToBuffer(v, buffer);
                    if (output_vertices)
                        result.push_back(v);
                    y = y + inc_y;
                    error = error - 1.0;
                }
                x = x + inc_x;
            }
        }
        return result;
    }

    float degreeToRadians(float angle)
    {
        return angle*(3.14159265 / 180);
    }

    AlmostGL::Vertex createInterpolated(const Vertex& start, const Vertex& end, int x, int y)
    {
        Vertex v;
        float alpha = sqrtf(pow((x-start.pos.x), 2) + pow((y-start.pos.y), 2)
            / (pow(end.pos.x-start.pos.x, 2) + pow(end.pos.y-start.pos.y, 2)));
        v.pos = { (float)x, (float)y,
            (1 - alpha)*start.pos.z + alpha*start.pos.z,
            (1 - alpha)*start.pos.w + alpha*start.pos.w };
        v.color = (1 - alpha)*start.color + alpha*end.color;
        v.interpolation_factor = (1 - alpha)*start.interpolation_factor
            + alpha*end.interpolation_factor;
        return v;
    }

    void writeToBuffer(Vertex& v, FrameBuffer& buffer)
    {
        v.color = v.color / v.interpolation_factor;
        buffer(v.pos.x, v.pos.y, AlmostGL::RED) = v.color[0];
        buffer(v.pos.x, v.pos.y, AlmostGL::GREEN) = v.color[1];
        buffer(v.pos.x, v.pos.y, AlmostGL::BLUE) = v.color[2];
    }

}// namespace AlmostGL
