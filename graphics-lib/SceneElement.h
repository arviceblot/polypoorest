#pragma once

#include <string>

class SceneElement
{
public:
    enum Type
    {
        UNKNOWN,
        SHADER,
        LIGHT,
        CAMERA,
        SHAPE,
    };

    SceneElement(const std::string &name = "", const std::string &type = "",
                 Type sType = Type::UNKNOWN)
        : name(name), type(type), sType(sType) {}
    virtual ~SceneElement() = 0;

    inline const std::string & getName() const
    {
        return name;
    }
    inline const std::string & getType() const
    {
        return type;
    }
    inline Type getSceneElementType() const
    {
        return sType;
    }

protected:
    std::string name;
    std::string type;
    Type sType;

private:
    SceneElement();
};
