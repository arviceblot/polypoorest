#include "ShaderCreator.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/vec3.hpp>
#include "Vector3D.h"
#include "BlinnPhongShader.h"
#include "LambertianShader.h"

using namespace sivelab;

ShaderCreator::ShaderCreator(Scene *scene)
    : scene(scene)
{
}

ShaderCreator::~ShaderCreator()
{
}

void ShaderCreator::instance(const ptree::value_type & v)
{
    std::istringstream buf;
    std::string name;
    std::string type;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float phongExp;
    float mirrorCoef = 0.0f;
    float roughness = 0.0f;
    bool hasMirror = false;
    std::shared_ptr<Shader> e = NULL;

    name = v.second.get<std::string>("<xmlattr>.name");
    type = v.second.get<std::string>("<xmlattr>.type");

    std::cout << "Parsing " << type << ": " << name << "... ";
    if (type == "Mirror")
    {
        e = std::make_shared<BlinnPhongShader>(name, type, diffuse, specular, phongExp, 1.0f);
        scene->add(e);
        std::cout << "finished." << std::endl;
        return;
    }

    buf.str(v.second.get<std::string>("diffuse"));
    buf >> diffuse.x;
    buf >> diffuse.y;
    buf >> diffuse.z;
    buf.clear();

    boost::optional<std::string> pSpecular = v.second.get_optional<std::string>("specular");
    boost::optional<std::string> pPhongExp = v.second.get_optional<std::string>("phongExp");
    boost::optional<std::string> pMirrorCoef = v.second.get_optional<std::string>("mirrorCoef");
    boost::optional<std::string> pRoughness = v.second.get_optional<std::string>("roughness");

    if (pSpecular)
    {
        buf.str(*pSpecular);
        buf >> specular.x;
        buf >> specular.y;
        buf >> specular.z;
        buf.clear();
    }
    if (pPhongExp)
    {
        buf.str(*pPhongExp);
        buf >> phongExp;
        buf.clear();
    }
    if (pMirrorCoef)
    {
        hasMirror = true;
        buf.str(*pMirrorCoef);
        buf >> mirrorCoef;
        buf.clear();
    }
    if (pRoughness)
    {
        buf.str(*pRoughness);
        buf >> roughness;
        buf.clear();
    }

    if (type == "Lambertian" || type == "Glaze")
    {
        e = std::make_shared<LambertianShader>(name, type, diffuse, mirrorCoef);
    }
    else if (type == "BlinnPhong" || type == "Phong" || type == "BlinnPhongMirrored")
    {
        if (hasMirror)
        {
            e = std::make_shared<BlinnPhongShader>(name, type, diffuse, specular, phongExp, mirrorCoef, roughness);
        }
        else
        {
            e = std::make_shared<BlinnPhongShader>(name, type, diffuse, specular, phongExp);
        }
    }
    scene->add(e);
    std::cout << "finished." << std::endl;
}
