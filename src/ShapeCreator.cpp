#include "ShapeCreator.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "Vector3D.h"
#include "Box.h"
#include "ImplicitSphere.h"
#include "InstancedObject.h"
#include "Triangle.h"

using namespace sivelab;

void ShapeCreator::instance(const ptree::value_type& v)
{
    std::istringstream buf;
    std::string name;
    std::string type;
    Shader *shaderRef;
    glm::vec3 minPt;
    glm::vec3 maxPt;

    name = v.second.get<std::string>("<xmlattr>.name");
    type = v.second.get<std::string>("<xmlattr>.type");

    std::cout << "Parsing " << type << ": " << name << "... ";

    // get the shader info
    ptree::const_assoc_iterator it = v.second.find("shader");
    std::string shaderName = it->second.get<std::string>("<xmlattr>.ref");
    shaderRef = scene->getShader(shaderName);

    SceneElement *e = NULL;
    if (type == "sphere")
    {
        glm::vec3 center;
        float radius;

        buf.str(v.second.get<std::string>("center"));
        buf >> center.x; buf >> center.y; buf >> center.z;
        buf.clear();

        buf.str(v.second.get<std::string>("radius"));
        buf >> radius;
        buf.clear();

        e = new ImplicitSphere(name, type, shaderRef, center, radius);
        scene->add(e);
    }
    else if (type == "triangle")
    {
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;

        buf.str(v.second.get<std::string>("v0"));
        buf >> v0.x; buf >> v0.y; buf >> v0.z;
        buf.clear();

        buf.str(v.second.get<std::string>("v1"));
        buf >> v1.x; buf >> v1.y; buf >> v1.z;
        buf.clear();

        buf.str(v.second.get<std::string>("v2"));
        buf >> v2.x; buf >> v2.y; buf >> v2.z;
        buf.clear();

        e = new Triangle(name, type, shaderRef, v0, v1, v2);
        if (v.first == "instance")
        {
            scene->addBase(e);
        }
        else
        {
            scene->add(e);
        }
    }
    else if (type == "box")
    {
        glm::vec3 minPt;
        glm::vec3 maxPt;

        buf.str(v.second.get<std::string>("minPt"));
        buf >> minPt.x; buf >> minPt.y; buf >> minPt.z;
        buf.clear();

        buf.str(v.second.get<std::string>("maxPt"));
        buf >> maxPt.x; buf >> maxPt.y; buf >> maxPt.z;
        buf.clear();

        e = new Box(name, type, shaderRef, minPt, maxPt);
        if (v.first == "instance")
        {
            std::cout << "Adding box instance..." << std::endl;
            scene->addBase(e);
            std::cout << "Box instance added." << std::endl;
        }
        else
        {
            scene->add(e);
        }
    }
    else if (type == "instance")
    {
        std::cout << "Parsing instance..." << std::endl;
        std::string id;
        id = v.second.get<std::string>("<xmlattr>.id");

        // find the object to reference for this instance
        Shape* shapeRef = scene->getInstance(id);

        // TODO parse transformation information
        glm::mat4 m;
        /*m.setIdentity();
        ptree::const_assoc_iterator it = v.second.find("transform");
        if (it != v.second.not_found()) {
            m = parseTransform(*it);
        }

        // assume it found the reference
        e = new InstancedObject(name, shapeRef->getType(), shapeRef, m);
        scene->add(e);
        */
    }
    else if (type == "mesh")
    {
        std::cout << "Not Parsing mesh because this hasn't been implemented..." << std::endl;
    }

    std::cout << "finished." << std::endl;
}

const glm::mat4 ShapeCreator::parseTransform(const ptree::value_type& v)
{
    glm::mat4 m;
    std::string name;
    name = v.second.get<std::string>("<xmlattr>.name");

    std::istringstream buf;
/*    ptree::const_iterator nodeIterator;
    for (nodeIterator = v.second.back();; --nodeIterator) {
        if (nodeIterator->first == "translate") {
            // From stackoverflow:
            // http://stackoverflow.com/questions/4597048/boost-property-tree-iterators-how-to-handle-them
            //
            // The iterator returns a value_type object which resembles a
            // pair, and has the first and second accessors. v.first is an
            // std::string holding the parent node (in my case "object1",
            // "object2"), and v.second is a boost::property_tree::ptree,
            // which can be used to parse the fields of the object.

            Vector3D trans;
            buf.str(nodeIterator->second.get_value<std::string>());
            buf >> trans;
            buf.clear();

            std::cout << "Found translational transform..." << std::endl;
            m.makeTranslation(trans[0], trans[1], trans[2]);
        }
        else if (nodeIterator->first == "rotate") {
            float rot;
            buf.str(nodeIterator->second.get_value<std::string>());
            buf >> rot;
            buf.clear();

            // TODO convert rot to radians?

            std::string axis = nodeIterator->second.get<std::string>("<xmlattr>.axis");
            if (axis == "X") {
                std::cout << "Found rotation about X transform..." << std::endl;
                m.makeRotationX(rot);
            }
            else if (axis == "Y") {
                std::cout << "Found rotation about Y transform..." << std::endl;
                m.makeRotationY(rot);
            }
            else if (axis == "Z") {
                std::cout << "Found rotation about Z transform..." << std::endl;
                m.makeRotationZ(rot);
            }

        }
        else if (nodeIterator->first == "scale") {
            Vector3D scale;
            buf.str(nodeIterator->second.get_value<std::string>());
            buf >> scale;
            buf.clear();

            std::cout << "Found scale transform..." << std::endl;
            m.makeScale(scale[0], scale[1], scale[2]);
        }

        if (nodeIterator == v.second.begin()) {
            break;
        }
    }*/

    return m;   // this is where you'd want to return your matrix
}
