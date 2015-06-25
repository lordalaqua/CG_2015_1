#include "Model.h"
#include "yaml/yaml.h"


Model::Model(const std::string& filename)
{
    loadFromFile(filename);
}

Model::Model(const YAML::Node& node)
{
    smooth = false;
    readVector3f(node["position"],position);
    node["scale"] >> scale;
    std::string filename;
    node["filename"] >> filename;
    if (node.FindValue("smooth")) {
        std::string ss; node["smooth"] >> ss;
        if (ss == "true") smooth = true;
        else smooth = false;
    }
    loadFromFile(filename);
}

bool Model::loadFromFile(const std::string& filename)
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
            file >> trash >> trash >> ambient[0] >> ambient[1] >> ambient[2];
            file >> trash >> trash >> diffuse[0] >> diffuse[1] >> diffuse[2];
            file >> trash >> trash >> specular[0] >> specular[1] >> specular[2];
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
            Triangle new_tri;
            for (int j = 0; j < 3; ++j)
            {
                file >> trash >> new_tri.vertex[j][0] >> new_tri.vertex[j][1] >> new_tri.vertex[j][2];
                file >> new_tri.normal[j][0] >> new_tri.normal[j][1] >> new_tri.normal[j][2];
                new_tri.normal[j].normalize();
                file >> new_tri.material_index[j];
                if (textured)
                {
                    file >> new_tri.texture[j][0] >> new_tri.texture[j][1];
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
            file >> trash >> trash >> new_tri.face_normal[0] >> new_tri.face_normal[1] >> new_tri.face_normal[2];
            triangles.push_back(new_tri);
        }
        float s = scale / (max[1] - min[1]);
        Matrix<3, 3, float> scale = 
            {{ s, 0, 0}, 
             { 0, s, 0},
             { 0, 0, s} };
        min = std::numeric_limits<float>::max();
        max = -std::numeric_limits<float>::max();
        for (Triangle &tri : triangles)
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

void Model::updateBoundingBox(Vector3f v)
{
    if (v[0] < min[0]) min[0] = v[0];
    if (v[1] < min[1]) min[1] = v[1];
    if (v[2] < min[2]) min[2] = v[2];
    if (v[0] > max[0]) max[0] = v[0];
    if (v[1] > max[1]) max[1] = v[1];
    if (v[2] > max[2]) max[2] = v[2];
}
