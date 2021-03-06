#include "Pipeline.h"
namespace AlmostGL
{
	/* Run Geometry Pipeline*/
    void Pipeline::runVertexPipeline(const Model3D& model)
    {
        convertTrianglesAndCalculateColor(model);
        modelViewProjectionTransform();
        clipping();
        perspectiveDivision();
        viewportTransform();
        backFaceCulling();       
    }

	/* Transform Model Triangles into AlmostGL triangles and calculate color
	or illumination for each vertex */
    void Pipeline::convertTrianglesAndCalculateColor(const Model3D& model)
    {
        triangles.clear();
        triangles.resize((int)model.triangles.size());
        for (int i = 0; i < model.triangles.size(); ++i)
        {
            for (int j = 0; j < 3; ++j) // iterate over vertices in the triangle
            {
                triangles[i].v[j].pos = model.triangles[i].vertex[j];
                triangles[i].v[j].texture = model.triangles[i].texture[j];
                // Calculate color
                if (light.mode == NONE)
                {
                    triangles[i].v[j].color = model.material.ambient;
                    if (FUN)
                    {
                        triangles[i].v[j].color = { 0, 0, 0 };
                        triangles[i].v[j].color[j] = 1.f;
                    }
                }
                else if (light.mode == FLAT)
                {
                    triangles[i].v[j].color = calculateIllumination(
                        model.triangles[i].vertex[j],
                        model.triangles[i].face_normal,
                        model.material);
                }
                else // SMOOTH shading
                {
                    triangles[i].v[j].color = calculateIllumination(
                        model.triangles[i].vertex[j],
                        model.triangles[i].normal[j],
                        model.material);
                }
            }
            triangles[i].removed = false;
        }
    }
	
    void Pipeline::modelViewProjectionTransform()
    {
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
            }
        }
    }

    void Pipeline::clipping()
    {
        // Multiply every vertex by the matrix
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                // Mark triangles outside the view volume to be clipped
                float x = abs(triangle.v[i].pos.x);
                float y = abs(triangle.v[i].pos.y);
                float z = abs(triangle.v[i].pos.z);
                float w = abs(triangle.v[i].pos.w);
                if (x > w || y > w || z > w)
                    triangle.removed = true;
            }
        }
        // Remove clipped triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [](Triangle& tri) { return tri.removed; }), triangles.end());
    }

    void Pipeline::perspectiveDivision()
    {
        // Perform the perspective division of all elements by w;
        for (auto& triangle : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                float w = triangle.v[i].pos.w;
                triangle.v[i].pos /= w;
                triangle.v[i].color /= w;
                triangle.v[i].texture /= w;
                triangle.v[i].interpolation_factor = 1 / w;
            }
        }
    }

    void Pipeline::viewportTransform()
    {
        // Compute the viewport matrix
        Matrix4x4f viewport_matrix = {
            { (viewport.right - viewport.left) / 2.f, 0, 0, (viewport.right + viewport.left) / 2.f },
            { 0, (viewport.top - viewport.bottom) / 2.f, 0, (viewport.top + viewport.bottom) / 2.f },
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
        }
    }

    void Pipeline::backFaceCulling()
    {
        for (auto& triangle : triangles)
        {
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
            if (winding_order == CCW)
            {
                if (area < 0)
                    triangle.removed = true;
            }
            else // order == CW
            {
                if (area > 0)
                    triangle.removed = true;
            }
        }
        // Remove culled triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [](Triangle& tri) { return tri.removed; }), triangles.end());
    }
	
	/* Calculate illumination for a vertex based on its properties and using the
	Phong Lighting Model, approximating OpenGL's */
    Vector3f Pipeline::calculateIllumination(const Vector3f& vertex,
        const Vector3f& normal, const Material& material)
    {
        Vector3f color = { 0.f, 0.f, 0.f };
        Vector3f V = (camera.position() - vertex).normalize();
        Vector3f N = getNormalized(normal);
        // Iterate over colors
        for (int i = 0; i < 3; ++i)
        {
            // Calculate ambient component and add to final color
            float ambient = light.ambient_color[i] * material.ambient[i];
            color[i] += ambient;

            // Calculate diffuse and specular component for each light source
            for (LightSource source : light.sources)
            {
                Vector3f L = (source.position-vertex);
                float dist = L.length();
                float att_factor = 1 / (
                    light.constant_attenuation +
                    light.linear_attenuation * dist +
                    light.quadratic_attenuation * dist*dist);

                att_factor = std::min(att_factor, 1.f);
                L.normalize();

                float dotNL = dotProduct(N, L);
                Vector3f S = (L + V).normalize();
                //Vector3f R = ((2 * dotNL) * N - (L)).normalize();

                float diffuse = std::max(0.f, dotNL)*material.diffuse[i];
                float specular = dotNL > 0 ? pow(std::max(0.f, dotProduct(S, N)),
                    material.shine)*material.specular[i]: 0.f;
                float component = att_factor*source.color[i]*(diffuse+specular);

                // Add source diffuse+specular component to final color
                color[i] += component;
            }
        }
        return color;
    }

	/* Draw 2D triangles as Points, outline (wireframe) or filled(rasterizing) */
    void Pipeline::runRasterization()
    {
        for (auto triangle : triangles)
        {
            if (polygon_mode == POINTS)
            {
                for (int i = 0; i < 3; ++i)
                {
                    // Points polygon mode
                    writeToBuffer(triangle.v[i], buffer);
                }
            }
            else if (polygon_mode == WIREFRAME)
            {
                bresenham(triangle.v[0], triangle.v[1], buffer);
                bresenham(triangle.v[0], triangle.v[2], buffer);
                bresenham(triangle.v[1], triangle.v[2], buffer);
            }
            if (polygon_mode == FILL)
            {
                rasterTriangle(triangle, buffer);
            }
        }
    }
	
	/* Rasterize a triangle onto the screen interpolating the colors of its 
	vertices */
    void Pipeline::rasterTriangle(Triangle& triangle, FrameBuffer& buffer)
    {
        std::sort(std::begin(triangle.v), std::end(triangle.v),
            [](const Vertex& a, const Vertex& b) { return a.pos.y < b.pos.y; });
        
        int y_start = triangle.v[0].pos.y;
        int y_end = triangle.v[2].pos.y;
        int start_a = 0, end_a = 1;
        int start_b = 0, end_b = 2;
        float slope_a = (triangle.v[end_a].pos.x - triangle.v[start_a].pos.x)
            / (triangle.v[end_a].pos.y - triangle.v[start_a].pos.y);
        float slope_b = (triangle.v[end_b].pos.x - triangle.v[start_b].pos.x)
            / (triangle.v[end_b].pos.y - triangle.v[start_b].pos.y);
        float x_a = triangle.v[start_a].pos.x;
        float x_b = triangle.v[start_b].pos.x;

        /* Tentative Mip Map storage variables, still not working*/
        mipmap_last_x = triangle.v[0].pos.x;
        mipmap_last_x_tex = { triangle.v[0].texture.x*texture.width(), triangle.v[0].texture.y*texture.height() };
        mipmap_last_y = triangle.v[0].pos.y;
        mipmap_last_y_tex = { triangle.v[0].texture.x*texture.width(), triangle.v[0].texture.y*texture.height() };

        for (int y = y_start; y <= y_end; ++y)
        {
            if ((int)triangle.v[end_a].pos.y == y)
            {
                start_a = 1; end_a = 2;
                slope_a = (triangle.v[end_a].pos.x - triangle.v[start_a].pos.x)
                    / (triangle.v[end_a].pos.y - triangle.v[start_a].pos.y);
                x_a = triangle.v[start_a].pos.x;
            }
            if ((int)triangle.v[end_b].pos.y == y)
            {
                start_b = 1;
                slope_b = (triangle.v[end_b].pos.x - triangle.v[start_b].pos.x)
                    / (triangle.v[end_b].pos.y - triangle.v[start_b].pos.y);
                x_b = triangle.v[start_b].pos.x;
            }
            Vertex v1 = createInterpolated(triangle.v[start_a], triangle.v[end_a], x_a, y);
            Vertex v2 = createInterpolated(triangle.v[start_b], triangle.v[end_b], x_b, y);
            bresenham(v1, v2, buffer);
            x_a += slope_a;
            x_b += slope_b;
            mipmap_last_y = y;
            mipmap_last_y_tex = { triangle.v[end_a].texture.x*texture.width(), triangle.v[end_a].texture.y*texture.height() };
        }
    }
	
	/* Bresenham's line drawing algorithm */
    void Pipeline::bresenham(const Vertex& start, const Vertex& end,
        FrameBuffer& buffer)
    {
        float deltax = (int)end.pos.x - (int)start.pos.x;
        float deltay = (int)end.pos.y - (int)start.pos.y;
        if (deltax == 0 && deltay == 0) // single point
        {
            return;
        }
        else if (deltay == 0) // horizontal line
        {
            int y = start.pos.y;
            int x = start.pos.x;
            int inc_x = deltax > 0 ? 1 : -1;
            while (inc_x == 1 ? x <= end.pos.x : x >= end.pos.x)
            {
                Vertex v = createInterpolated(start, end, x, y);
                writeToBuffer(v, buffer);
                mipmap_last_x = x;
                mipmap_last_x_tex = { v.texture.x / v.interpolation_factor, v.texture.x / v.interpolation_factor };
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
                writeToBuffer(v, buffer);
                y = y + inc_y;
            }
        }
        else // general case
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
                    y = y + inc_y;
                    error = error - 1.0;
                }
                x = x + inc_x;
            }
        }
    }
    
	/* Create an interpolated vertex based on start and end vertices and a 
	2d point guaranteed between the two. */
    Vertex Pipeline::createInterpolated(const Vertex& start,
        const Vertex& end, int x, int y)
    {
        Vertex v;
        float alpha =
            sqrtf(pow(((float)x - start.pos.x), 2) + pow(((float)y - start.pos.y), 2))
            / sqrtf(pow(end.pos.x - start.pos.x, 2) + pow(end.pos.y - start.pos.y, 2));
        v.pos = { (float)x, (float)y,
            (1 - alpha)*start.pos.z + alpha*start.pos.z,
            (1 - alpha)*start.pos.w + alpha*start.pos.w };
        v.color = (1 - alpha)*start.color + alpha*end.color;
        v.texture = (1 - alpha)*start.texture + alpha*end.texture;
        v.interpolation_factor = (1 - alpha)*start.interpolation_factor
            + alpha*end.interpolation_factor;
        return v;
    }
	
	/* Write a vertex to the frame buffer, resampling a texture when necessary. */
    void Pipeline::writeToBuffer(Vertex v, FrameBuffer& buffer)
    {
        if (texture.enabled)
        {
            v.texture /= v.interpolation_factor;
            if (v.texture.x > 1.f) v.texture.x = 1.f;
            if (v.texture.x < 0.f) v.texture.x = 0.f;
            if (v.texture.y > 1.f) v.texture.y = 1.f;
            if (v.texture.y < 0.f) v.texture.y = 0.f;
           
            Vector3f tex_color = calculateTextureColor(v.texture.x, v.texture.y);
            if (texture.mode == MODULATE)
            {
                v.color /= v.interpolation_factor;
                v.color *= tex_color;
            }
            else // if (texture.mode == DECAL)
            {
                v.color = tex_color;
            }                
        }
        else
        {
			// No texture mapping
            v.color /= v.interpolation_factor;
        }        
        buffer.writeVertex(v);
    }

    void Pipeline::bindTexture(std::string filename)
    {
        texture.loadImage(filename);
    }
	
	/* Resample texture in desired coordinates interpolating as necessary */
    Vector3f Pipeline::calculateTextureColor(float x, float y)
    {
        Vector3f color;
        int w = texture.width(), h = texture.height();
        if (texture.filter == NEAREST)
        {
            int x_nn = round(x*w);
            int y_nn = round(y*h);
            color = texture.getPixelColor(x_nn, y_nn);
        }
        else if (texture.filter == LINEAR)
        {
            color = bilinearInterpolatePixel(x, y);
        }
        else // MIPMAP
        {
            float p_x = x*texture.width(), p_y = y*texture.height();
            float dx = sqrt(pow(p_x - mipmap_last_x_tex.x, 2) + pow(p_y - mipmap_last_x_tex.y, 2));
            float dy = sqrt(pow(p_x - mipmap_last_y_tex.x, 2) + pow(p_y - mipmap_last_y_tex.y, 2));
            float level = log2f(std::max(dx, dy));
            int min_level = floor(level), max_level = ceil(level);
            float alpha = level - min_level;
            color = interpolate(bilinearInterpolatePixel(x, y, min_level),
                bilinearInterpolatePixel(x, y, max_level), alpha);
        }
        return color;
    }

    template<typename T>
    T Pipeline::interpolate(T start, T end, float alpha)
    {
        return (1 - alpha)*start + alpha*end;
    }


    float Pipeline::degreeToRadians(float angle)
    {
        return angle*(3.14159265 / 180);
    }
	
	/* Perform bilinear interpolation of texture values */
    Vector3f Pipeline::bilinearInterpolatePixel(float x, float y, int level/*=0*/)
    {
        float p_x = x*texture.width(), p_y = y*texture.height();
        float x0 = floor(p_x), x1 = ceil(p_x), y0 = floor(p_y), y1 = ceil(p_y);
        float alpha_x = p_x - x0, alpha_y = p_y - y0;
        Vector3f color_y0 = interpolate(texture.getPixelColor(x0, y0,level),
            texture.getPixelColor(x1, y0), alpha_x);
        Vector3f color_y1 = interpolate(texture.getPixelColor(x0, y1,level),
            texture.getPixelColor(x1, y1), alpha_x);
        return interpolate(color_y0, color_y1, alpha_y);
    }

}