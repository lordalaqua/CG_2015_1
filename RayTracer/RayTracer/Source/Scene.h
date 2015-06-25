#pragma once

#include <QDebug>
#include <QImage>

#include "yaml/yaml.h"
#include <fstream>
#include <vector>

#include "Vector.h"
#include "Camera.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "LightSource.h"

class Scene 
{
public:
    enum RenderMode { PHONG };
    Scene(const std::string& filename);
    ~Scene();
    void render(QImage& img);
private:    
    void parse(std::ifstream& file);
    Hit getIntersection(const Ray& ray, const Object* ignore = NULL,
        double max_distance = INF);
    Vector3f traceRay(const Ray& ray, const Hit& hit, int recursion_depth = 0);

    void clamp(Vector3f& color);
private:
    Camera camera_;
    std::vector<Object*> objects_;
    std::vector<LightSource> light_sources_;

    RenderMode render_mode_;
    double supersampling_factor_;
    int max_recursion_depth_;
    bool use_shadows_;
};