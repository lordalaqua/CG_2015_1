#include "AlmostGLPipeline.h"

namespace AlmostGL
{
    std::vector<Triangle4D> runPipeline(const Model3D& model, const Camera& camera,
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
        double Vfov_rad = degreeToRadians(camera.Vfov() / 2.f);
        float aspect = camera.Hfov() / camera.Vfov();
        float topBottom = (float) (1.0 / tan(Vfov_rad));
        float leftRight = topBottom / aspect;
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

    float degreeToRadians(float angle)
    {
        return angle*(3.14159265 / 180);
    }

}// namespace AlmostGL
