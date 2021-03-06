#include "Model3D.h"


Model3D::Model3D(const std::string& filename)
{
    loadFromFile(filename);
}

bool Model3D::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        triangles.clear();
        materials.clear();
        min = std::numeric_limits<float>::max();
        max = -std::numeric_limits<float>::max();

        std::string trash;

        // Read object name, nothing to do with it.
        std::getline(file, trash);
        // Read number of triangles and materials
        int num_triangles, num_materials;
        file >> trash >> trash >> trash >> num_triangles;
        file >> trash >> trash >> trash >> num_materials;

        // Read materials
        for (int i = 0; i < num_materials; ++i)
        {
            Vector3f ambient, diffuse, specular;
            float shine;
            file >> trash >> trash >> ambient.x >> ambient.y >> ambient.z;
            file >> trash >> trash >> diffuse.x >> diffuse.y >> diffuse.z;
            file >> trash >> trash >> specular.x >> specular.y >> specular.z;
            file >> trash >> trash >> shine;
            materials.push_back(Material(ambient, diffuse, specular, shine));
        }

        // Find whether object is textured
        std::string tex;
        file >> trash >> trash >> tex;
        bool textured = false;
        if (tex == "YES")
            textured = true;

        // Read until end of line and next line which contains comments
        std::getline(file, trash);
        std::getline(file, trash);

        // Read triangles
        for (int i = 0; i < num_triangles; ++i)
        {
            Triangle3D new_tri;
            for (int j = 0; j < 3; ++j)
            {
                file >> trash >> new_tri.vertex[j].x >> new_tri.vertex[j].y >> new_tri.vertex[j].z;
                file >> new_tri.normal[j].x >> new_tri.normal[j].y >> new_tri.normal[j].z;
                new_tri.normal[j].normalize();
                file >> new_tri.material_index[j];
                if (textured)
                {
                    file >> new_tri.texture[j].x >> new_tri.texture[j].y;
                }
                else
                {
                    switch (j)
                    {
                    case 0:
                        new_tri.texture[j] = { 0.f, 0.f };
                        break;
                    case 1:
                        new_tri.texture[j] = { 0.f, 1.f };
                        break;
                    case 2:
                        new_tri.texture[j] = { 1.f, 0.f };
                        break;
                    }                    
                }
                updateBoundingBox(new_tri.vertex[j]);
            }
            file >> trash >> trash >> new_tri.face_normal.x >> new_tri.face_normal.y >> new_tri.face_normal.z;
            triangles.push_back(new_tri);
        }
        float s = 1000.f / (max.y - min.y);
        Matrix<3, 3, float> scale = 
            {{ s, 0, 0}, 
             { 0, s, 0},
             { 0, 0, s} };
        min = std::numeric_limits<float>::max();
        max = -std::numeric_limits<float>::max();
        for (Triangle3D &tri : triangles)
        {
            for (int i = 0; i < 3; ++i)
            {
                tri.vertex[i] = scale*tri.vertex[i];
                updateBoundingBox(tri.vertex[i]);
            }
        }
        return true;
    }
    else
        return false;
}

void Model3D::updateBoundingBox(Vector3f v)
{
    if (v.x < min.x) min.x = v.x;
    if (v.y < min.y) min.y = v.y;
    if (v.z < min.z) min.z = v.z;
    if (v.x > max.x) max.x = v.x;
    if (v.y > max.y) max.y = v.y;
    if (v.z > max.z) max.z = v.z;
}
