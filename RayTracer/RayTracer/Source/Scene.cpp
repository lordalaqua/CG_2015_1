#include "Scene.h"



Scene::Scene(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        qDebug() << "Unable to open input File";
        throw 1; // TODO: Proper Exceptions
    }
    else
        parse(file);
}



Scene::~Scene()
{
    for (auto& obj : objects_)
    {
        if (obj != nullptr)
            free(obj);
    }
}


void Scene::parse(std::ifstream& file)
{
    YAML::Parser parser(file);
    YAML::Node document;
    parser.GetNextDocument(document);

    if (document.FindValue("RenderMode")) 
    {
        std::string renderMode;
        document["RenderMode"] >> renderMode;
        if (renderMode == "phong" or renderMode == "\"phong\"")
            render_mode_ = RenderMode::PHONG;
    }

    if (document.FindValue("Shadows")) 
    {
        std::string shadows;
        document["Shadows"] >> shadows;
        if (shadows == "true") use_shadows_ = true;
        else use_shadows_ = false;
    }

    if (document.FindValue("SuperSampling")) 
    {
        const auto& ss = document["SuperSampling"];
        int factor;
        ss["factor"] >> factor;
        supersampling_factor_ = factor;
    }

    if (document.FindValue("Camera")) 
    {
        camera_.read(document["Camera"]);
    }

    if (document.FindValue("Eye")) 
    {
        Vector3f eye;
        readVector3f(document["Eye"], eye);
        camera_.position(eye);
    }

    if (document.FindValue("MaxRecursionDepth"))
    {
        int max_depth = 1;
        document["MaxRecursionDepth"] >> max_depth;
        max_recursion_depth_ = max_depth;
    }

    /* Read and parse the scene objects */
    if (document.FindValue("Objects"))
    {
        const YAML::Node& objs = document["Objects"];
        for (YAML::Iterator it = objs.begin(); it != objs.end(); ++it) 
        {
            std::vector<Object*> objs = ObjectFactory::createObject(*it);
            for (auto& obj : objs)
                objects_.push_back(obj);
        }
    }

    /* Read and parse light definitions */
    if (document.FindValue("Lights"))
    {

        const YAML::Node& lights = document["Lights"];
        for (YAML::Iterator it = lights.begin(); it != lights.end(); ++it) 
        {
            light_sources_.push_back(LightSource(*it));
        }
    }
}

void Scene::render(QImage& img)
{
    int w = img.width();
    int h = img.height();
    size_t ss = supersampling_factor_;

    std::vector<std::vector<Vector3f>> 
       color_buffer(ss * w, std::vector<Vector3f>(ss * h));

    double totalTime = 0;
    for (int yy = 0; yy < h; yy++) 
    {
        for (int xx = 0; xx < w; xx++) 
        {
            for (unsigned i = 0; i < ss; ++i) 
            {
                for (unsigned j = 0; j < ss; ++j) 
                {
                    Vector3f subpixel_color ;
                    double x = xx + (1.0 / double(ss)) * (i + 0.5),
                        y = yy + (1.0 / double(ss)) * (j + 0.5);

                    Ray ray = camera_.getRay(x + 0.5, h - 1 - y + 0.5);

                    Hit hit = getIntersection(ray);

                    if (render_mode_ == RenderMode::PHONG)
                        subpixel_color  += traceRay(ray, hit);

                    color_buffer[xx * ss + i][yy * ss + j] = subpixel_color;
                }
            }
        }
    }

    /* Average results from supersampling */
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            Vector3f  pixel_color;
            for (unsigned i = 0; i < ss; ++i) {
                for (unsigned j = 0; j < ss; ++j) {
                    pixel_color  += color_buffer[xx * ss + i][yy * ss + j];
                }
            }
            pixel_color /= (ss * ss);
            img.setPixel(xx, yy, 
                qRgb(pixel_color[0]*255, pixel_color[1]*255, pixel_color[2]*255));
        }
    }
}

Hit Scene::getIntersection(const Ray& ray, const Object* ignore /*= NULL*/, 
    double max_distance /*= INF*/)
{
    Hit closestHit;
    for (unsigned i = 0; i < objects_.size(); ++i) {
        if (objects_[i] == ignore) continue;
        Hit h = objects_[i]->intersect(ray);
        if (h.isHit() and h < closestHit and h.getDistance() < max_distance) {
            assert(h.getObject() == objects_[i]);
            closestHit = h;
        }
    }
    return closestHit;
}

Vector3f Scene::traceRay(const Ray& ray, const Hit& hit, int recursion_depth /*= 0*/)
{
    if (recursion_depth >= max_recursion_depth_)
        return Vector3f();
    const Object* object = hit.getObject();
    const Material& mat = object->getMaterial();
    Vector3f point = ray.at(hit.getDistance());
    Vector3f colorAtHitpoint = object->colorAt(point);
    const Vector3f& N = hit.getNormal();
    Vector3f V = -ray.getDirection();
    Vector3f color;

    /* Ambient component */
    color = mat.ambient * colorAtHitpoint;
    for (unsigned i = 0; i < light_sources_.size(); ++i) {
        LightSource& light = light_sources_[i];
        auto L = (light.getPosition() - point).normalize();
        auto dist = (light.getPosition() - point).length();

        if (use_shadows_) {
            /* If the ray to the light hits another object,
            * don't use this light */
            auto maybeHit = getIntersection(Ray(point, L), object, dist);
            if (maybeHit.isHit()/* and maybeHit.getObject() != object*/)
                continue;
        }

        /* Diffuse component */
        Vector3f diffuse = std::max(0.f, dotProduct(N,L)) * mat.diffuse * colorAtHitpoint;

        /* Specular component */
        auto R = ((2 * (dotProduct(L,N) * N)) - L);
        Vector3f specular = pow(std::max(0.f, dotProduct(R, V)), mat.shine) * 
            mat.specular * light.getColor();
        color += light.getColor() * (specular + diffuse);
    }

    /* Calculate reflection */
    Vector3f D = ray.getDirection();
    Vector3f R = D - N * 2 * dotProduct(N, D);

    Ray newRay(point, R.normalize());
    auto newHit = getIntersection(newRay, object);
    auto reflectionColor = traceRay(newRay, newHit, recursion_depth + 1);

    color += reflectionColor * mat.specular;
    clamp(color);
    return color;
}

void Scene::clamp(Vector3f& color)
{
    for (int i = 0; i < 3; ++i)
    if (color[i] > 1.f)
        color[i] = 1.f;
}
