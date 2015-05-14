#include "AlmostGLPipeline.h"
#include <algorithm>

namespace AlmostGL
{
    std::vector<Triangle4D> runVertexPipeline(const Model3D& model, const Camera& camera,
        ViewPort vp, WindingOrder order)
    {

        // Compute Model/View Matrix
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

        // Transform model vertices to homogeneous coordinates
        std::vector<Triangle4D> triangles;
        triangles.resize((int)model.triangles.size());
        for (int i = 0; i < model.triangles.size(); ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                triangles[i].vertex[j] = model.triangles[i].vertex[j];
            }
            triangles[i].clipped = false;
        }

        // Multiply every vertex by the matrix
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                triangle.vertex[i] = PM*triangle.vertex[i];

                // Mark triangles outside the view volume to be clipped
                float x = abs(triangle.vertex[i][0]);
                float y = abs(triangle.vertex[i][1]);
                float z = abs(triangle.vertex[i][2]);
                float w = abs(triangle.vertex[i][3]);
                if (x > w || y > w || z > w)
                    triangle.clipped = true;
            }            
        }

        // Remove clipped triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [](Triangle4D& tri) { return tri.clipped; }), triangles.end());

        // Perform the perspective division
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                triangle.vertex[i][0] /= triangle.vertex[i][3];
                triangle.vertex[i][1] /= triangle.vertex[i][3];
                triangle.vertex[i][2] /= triangle.vertex[i][3];
                triangle.vertex[i][3] /= triangle.vertex[i][3];
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
                triangle.vertex[i] = viewport_matrix*triangle.vertex[i];
            }
            // Cull back facing polygons
            int area = 0;
            for (int i = 0; i < 3; ++i)
            {
                int j = (i + 1) % 3;
                area += (triangle.vertex[i].x * triangle.vertex[j].y)
                    - (triangle.vertex[j].x * triangle.vertex[i].y);
            }
            area = area / 2; // unnecessary but keeping for math correctness
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

    void runRasterization(const std::vector<Triangle4D>& triangles, 
        FrameBuffer& buffer, PolygonMode mode, Vector3f ambient, Vector3f diffuse, 
        Vector3f specular)
    {
        for (auto triangle : triangles)
        {

            std::sort(std::begin(triangle.vertex), std::end(triangle.vertex),
                [](const Vector<4,float>& v1, const Vector<4,float>& v2) { return v1.y > v2.y; });
            if (mode == POINTS)
            {
                for (int i = 0; i < 3; ++i)
                {
                    // Points polygon mode
                    buffer(triangle.vertex[i].x, triangle.vertex[i].y, AlmostGL::RED) = ambient[0];
                    buffer(triangle.vertex[i].x, triangle.vertex[i].y, AlmostGL::GREEN) = ambient[1];
                    buffer(triangle.vertex[i].x, triangle.vertex[i].y, AlmostGL::BLUE) = ambient[2];
                }
            }
            else if (mode == WIREFRAME)
            {
                bresenham(triangle.vertex[0], triangle.vertex[1], buffer);
                bresenham(triangle.vertex[1], triangle.vertex[2], buffer);
                bresenham(triangle.vertex[2], triangle.vertex[0], buffer);
            }            
            else if (mode == FILL)
            {
                fillTriangle(triangle, buffer);
            }
        }
    }

    void fillTriangle(Triangle4D triangle, FrameBuffer& buffer)
    {
        std::sort(std::begin(triangle.vertex), std::end(triangle.vertex),
            [](const Vector4f& a, const Vector4f& b) { return a.y < b.y; });
        if ((int)triangle.vertex[1].y == (int)triangle.vertex[2].y)
        {
            fillFlatBottomTriangle(triangle, buffer);
        }
        else if ((int)triangle.vertex[0].y == (int)triangle.vertex[2].y)
        {
            fillFlatTopTriangle(triangle, buffer);
        }
        else
        {
            float middle_y = triangle.vertex[1].y;
            float middle_x = triangle.vertex[0].x + (middle_y - triangle.vertex[0].y)
                / (triangle.vertex[2].y - triangle.vertex[0].y) *
                (triangle.vertex[2].x - triangle.vertex[0].x);
            //todo: interpolate rest of info
            Triangle4D upper, lower;
            lower.vertex[0] = triangle.vertex[0];
            lower.vertex[1] = triangle.vertex[1];
            lower.vertex[2] = { middle_x, middle_y, 0, 0 };
            upper.vertex[0] = triangle.vertex[1];
            upper.vertex[1] = { middle_x, middle_y, 0, 0 };
            upper.vertex[2] = triangle.vertex[2];
            fillFlatBottomTriangle(lower, buffer);
            fillFlatTopTriangle(upper, buffer);
        }
    }
    void fillFlatBottomTriangle(Triangle4D t, FrameBuffer& buffer)
    {
        float slopeA = (t.vertex[1].x - t.vertex[0].x) / (t.vertex[1].y - t.vertex[0].y);
        float slopeB = (t.vertex[2].x - t.vertex[0].x) / (t.vertex[2].y - t.vertex[0].y);

        float xA = t.vertex[0].x;
        float xB = t.vertex[0].x;

        for (int scanline = t.vertex[0].y; scanline <= t.vertex[2].y; scanline++)
        {
            bresenham({ xA, (float)scanline, 0, 0 }, { xB, (float)scanline, 0, 0 }, buffer);
            xA += slopeA;
            xB += slopeB;
        }
    }

    void fillFlatTopTriangle(Triangle4D t, FrameBuffer& buffer)
    {
        float slopeA = (t.vertex[2].x - t.vertex[0].x) / (t.vertex[2].y - t.vertex[0].y);
        float slopeB = (t.vertex[2].x - t.vertex[1].x) / (t.vertex[2].y - t.vertex[1].y);

        float xA = t.vertex[2].x;
        float xB = t.vertex[2].x;

        for (int scanline = t.vertex[2].y; scanline > t.vertex[0].y; scanline--)
        {
            xA -= slopeA;
            xB -= slopeB;
            bresenham({ xA, (float)scanline, 0, 0 }, { xB, (float)scanline, 0, 0 }, buffer);
        }
    }


    void bresenham(Vector4f start, Vector4f end, FrameBuffer& buffer)
    {
        float deltax = (int)end.x - (int)start.x;
        float deltay = (int)end.y - (int)start.y;
        if (deltax == 0 && deltay == 0) // single point
        {
            buffer(start.x, start.y, AlmostGL::RED) = 1.0;
        }
        else if (deltay == 0) // horizontal line
        {
            int y = start.y;
            int x = start.x;
            int inc_x = deltax > 0 ? 1 : -1;
            while (inc_x == 1 ? x <= end.x : x >= end.x)
            {
                buffer(x, y, AlmostGL::RED) = 1.0;
                x = x + inc_x;
            }
        }
        else if (deltax == 0) // vertical line
        {
            int y = start.y;
            int x = start.x;
            int inc_y = deltay > 0 ? 1 : -1;
            while (inc_y == 1 ? y <= end.y : y >= end.y)
            {
                buffer(x, y, AlmostGL::RED) = 1.0;
                y = y + inc_y;
            }
        }
        else //(deltax != 0 && deltay != 0)
        {
            float error = 0;
            float deltaerr = abs(deltay / deltax);
            int y = start.y;
            int x = start.x;
            int inc_x = deltax > 0 ? 1 : -1;
            int inc_y = deltay > 0 ? 1 : -1;
            while (inc_x == 1 ? x <= end.x : x >= end.x)
            {
                buffer(x, y, AlmostGL::RED) = 1.0;
                error = error + deltaerr;
                while (error >= 0.5 && (inc_y == 1 ? y <= end.y : y >= end.y))
                {
                    buffer(x, y, AlmostGL::RED) = 1.0;
                    y = y + inc_y;
                    error = error - 1.0;
                }
                x = x + inc_x;
            }
        }
    }

    float degreeToRadians(float angle)
    {
        return angle*(3.14159265 / 180);
    }
}// namespace AlmostGL
