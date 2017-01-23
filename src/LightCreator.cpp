#include "LightCreator.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/vec3.hpp>
#include "AreaLight.h"
#include "PointLight.h"

void LightCreator::instance(const ptree::value_type& v)
{
    std::istringstream buf;
    std::string type;
    glm::vec3 position;
    glm::vec3 intensity;
    std::shared_ptr<Light> e = NULL;

    type = v.second.get<std::string>("<xmlattr>.type");

    std::cout << "Parsing " << type << " light... ";

    buf.str(v.second.get<std::string>("position"));
    buf >> position.x;
    buf >> position.y;
    buf >> position.z;
    buf.clear();

    buf.str(v.second.get<std::string>("intensity"));
    buf >> intensity.x;
    buf >> intensity.y;
    buf >> intensity.z;
    buf.clear();

    if (type == "point")
    {
        e = std::make_shared<PointLight>(type, position, intensity);
    }
    else if (type == "area")
    {
        glm::vec3 normal;
        float width;
        float length;

        buf.str(v.second.get<std::string>("normal"));
        buf >> normal.x;
        buf >> normal.y;
        buf >> normal.z;
        buf.clear();

        width = v.second.get<float>("width");
        length = v.second.get<float>("length");
        e = std::make_shared<AreaLight>(type, position, intensity, normal, width, length);
    }
    scene->add(e);
    std::cout << "finished." << std::endl;
}
