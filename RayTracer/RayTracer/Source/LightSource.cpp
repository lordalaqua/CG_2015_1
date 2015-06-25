#include "LightSource.h"
#include "yaml/yaml.h"

LightSource::LightSource(const YAML::Node& node) {
    read(node);
}

void LightSource::read(const YAML::Node& node) {
    readVector3f(node["position"], position);
    readVector3f(node["color"], color);
}