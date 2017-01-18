#include "CameraCreator.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Vector3D.h"
#include "PerspectiveCamera.h"

using namespace sivelab;

void CameraCreator::instance(const ptree::value_type& v)
{
    std::istringstream buf;

    std::string name;
    std::string type;
    glm::vec3 position;
    glm::vec3 viewDir;
    glm::vec3 lookatPoint;
    bool lookatSet = false;
    float focalLength, imagePlaneWidth;
    SceneElement *e = NULL;

    name = v.second.get<std::string>("<xmlattr>.name");
    type = v.second.get<std::string>("<xmlattr>.type");

    std::cout << "Parsing " << type << ": " << name << "... ";

    buf.str(v.second.get<std::string>("position"));
    buf >> position.x;
    buf >> position.y;
    buf >> position.z;
    buf.clear();

    boost::optional<std::string> plookatPoint = v.second.get_optional<std::string>("lookatPoint");
    boost::optional<std::string> pviewDir = v.second.get_optional<std::string>("viewDir");

    if (plookatPoint)
    {
        lookatSet = true;
        buf.str( *plookatPoint );
        buf >> lookatPoint.x;
        buf >> lookatPoint.y;
        buf >> lookatPoint.z;
        buf.clear();
        viewDir = lookatPoint - position;
        viewDir = glm::normalize(viewDir);
    }
    else if (pviewDir)
    {
        buf.str( *pviewDir );
        buf >> viewDir.x;
        buf >> viewDir.y;
        buf >> viewDir.z;
        buf.clear();
    }

    buf.str(v.second.get<std::string>("focalLength"));
    buf >> focalLength;
    buf.clear();

    buf.str(v.second.get<std::string>("imagePlaneWidth"));
    buf >> imagePlaneWidth;
    buf.clear();

    // TODO: Add orthographic camera option here
    if (type == "perspective")
    {
        e = new PerspectiveCamera(name, type, position, viewDir, focalLength, imagePlaneWidth);
    }
    scene->add(e);
    std::cout << "finished." << std::endl;
}
