/*
 *  test_XMLSceneParse.cpp
 *
 *  Created by Pete Willemsen on 10/6/09.
 *  Copyright 2009 Department of Computer Science, University of Minnesota-Duluth. All rights reserved.
 *
 * This file is part of CS5721 Computer Graphics library (cs5721Graphics).
 *
 * cs5721Graphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cs5721Graphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with cs5721Graphics.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cassert>

#include "Vector3D.h"
#include "XMLSceneParser.h"
#include "handleGraphicsArgs.h"

using namespace sivelab;

// Here's an example

class CameraCreator : public SceneElementCreator
{
public:
  CameraCreator() {}
  ~CameraCreator() {}

  void instance( ptree::value_type const &v )
  {
    std::istringstream buf;

    std::string name, type;
    Vector3D position, viewDir, lookatPoint;
    bool lookatSet = false;
    float focalLength;
    float imagePlaneWidth;

    name = v.second.get<std::string>("<xmlattr>.name");
    type = v.second.get<std::string>("<xmlattr>.type");

    buf.str( v.second.get<std::string>("position") );
    buf >> position;
    buf.clear();

    // LookAtPoint and ViewDir are optional, but one should be specified.
    boost::optional<std::string> plookatPoint = v.second.get_optional<std::string>("lookatPoint");
    boost::optional<std::string > pviewDir = v.second.get_optional<std::string>("viewDir");

    if (plookatPoint) {
      lookatSet = true;
      buf.str( *plookatPoint );
      buf >> lookatPoint; 
      buf.clear();
    } else if (pviewDir) {
      buf.str( *pviewDir );
      buf >> viewDir; 
      buf.clear();
    }

    buf.str( v.second.get<std::string>("focalLength") );
    buf >> focalLength;
    buf.clear();

    buf.str( v.second.get<std::string>("imagePlaneWidth") );
    buf >> imagePlaneWidth;
    buf.clear();

    std::cout << "Camera: name=" << name << ", type=" << type << std::endl;
    std::cout << "\tposition: " << position << std::endl;
    if (lookatSet)
      std::cout << "\tlook at point: " << lookatPoint << std::endl;
    else 
      std::cout << "\tview direction: " << viewDir << std::endl;
    std::cout << "\tfocal length: " << focalLength << std::endl;
    std::cout << "\timage plane width: " << imagePlaneWidth << std::endl;
  }

private:
};

class ContainerClass : public SceneElementCreator
{
public:
  ContainerClass() {}
  ~ContainerClass() {}

  void instance( ptree::value_type const &v )
  {
    std::istringstream buf;

    //
    // Scene Parameters
    //
    if (v.first == "sceneParameters") {

      bool useEnvMap = false;
      Vector3D bgColor;
      std::string envmapPrefix;

      // LookAtPoint and ViewDir are optional, but one should be specified.
      boost::optional<std::string> pBGColor = v.second.get_optional<std::string>("bgColor");
      boost::optional<std::string > pEnvMapPrefix = v.second.get_optional<std::string>("envMapPrefix");
      
      if (pBGColor) {
	buf.str( *pBGColor );
	buf >> bgColor;
	buf.clear();
      } 

      if (pEnvMapPrefix) {
	buf.str( *pEnvMapPrefix );
	buf >> envmapPrefix;
	buf.clear();
	
	useEnvMap = true;
      }
    }
    
    //
    // Light
    //
    if (v.first == "light") {

      std::string type;
      sivelab::Vector3D position, intensity;

      type = v.second.get<std::string>("<xmlattr>.type");

      buf.str( v.second.get<std::string>("position") );
      buf >> position;
      buf.clear();

      buf.str( v.second.get<std::string>("intensity") );
      buf >> intensity;
      buf.clear();

      if (type == "point") {
	// Instance the new Point light...
	std::cout << "Found a point light..." << std::endl;
      }
      else if (type == "area") {
	sivelab::Vector3D normal;
	buf.str( v.second.get<std::string>("normal") );
	buf >> normal;
	buf.clear();

	float width = 1.0, length = 1.0;
	width = v.second.get<float>("width");
	length = v.second.get<float>("length");

	std::cout << "Found an area light..." << std::endl;

	// 
	// Create two triangles aligned with normal to represent the
	// light with an emissive shader.
	// 
	// pos is the center of the light at this point (the area light
	// class manipulates it to the lower left of the light corner),
	// so lower left corner is
	// 
	// pos - aLight->binormal()*width/2.0 - aLight->tangent()*length/2.0;
      }
    }

    // 
    // Shader
    // 
    if (v.first == "shader") {
      parseShaderData(v);
    }

    // 
    // Shape or Instance
    // 
    if (v.first == "shape") {
      parseShapeData(v);
    }
  }

  // Hint!! this function could be modified to return the matrix!
  int parseTransformData( ptree::value_type const &v )
  {
    std::string name;
    name = v.second.get<std::string>("<xmlattr>.name");

    std::istringstream buf;
    ptree::const_iterator nodeIterator;
    for (nodeIterator=v.second.begin(); nodeIterator!=v.second.end(); ++nodeIterator)
      {
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
	  buf.str( nodeIterator->second.get_value<std::string>() );
	  buf >> trans;
	  buf.clear();

	  std::cout << "Found translational transform..." << std::endl;
	}
	else if (nodeIterator->first == "rotate") {

	  float rot;
	  buf.str( nodeIterator->second.get_value<std::string>() );
	  buf >> rot;
	  buf.clear();

	  std::string axis = nodeIterator->second.get<std::string>("<xmlattr>.axis");
	  if (axis == "X") {
	    std::cout << "Found rotation about X transform..." << std::endl;
	  } else if (axis == "Y") {
	    std::cout << "Found rotation about Y transform..." << std::endl;
	  } else if (axis == "Z") {
	    std::cout << "Found rotation about Z transform..." << std::endl;
	  }

	}
	else if (nodeIterator->first == "scale") {
	  sivelab::Vector3D scale;
	  buf.str( nodeIterator->second.get_value<std::string>() );
	  buf >> scale;
	  buf.clear();

	  std::cout << "Found scale transform..." << std::endl;
	}
      }

    return 1;  // this is where you'd want to return your matrix
  }

  void parseShapeData( ptree::value_type const &v )
  {
    bool notInstance = true;
    if (v.first == "instance")
      notInstance = false;

    // Shapes and instances are basically the same thing, but need to be
    // added to different structures within the scene

    std::string type, name;
    type = v.second.get<std::string>("<xmlattr>.type");
    name = v.second.get<std::string>("<xmlattr>.name");

    // Can optionally have a transform associated with it...
    // 
    // Need to find the transform in the second part of the shape pair
    int m;
    ptree::const_assoc_iterator it = v.second.find("transform");
    if( it != v.second.not_found() )
      {
	m = parseTransformData( *it );
      }
  
    // Make sure to find the shader data objects; HAVE to include a
    // shader in this version; Need to find the shader in the second part
    // of the shape pair
    it = v.second.find("shader");
    if( it != v.second.not_found() )
      {
	parseShaderData( *it );
      }

    std::istringstream buf;
    if (type == "instance") {
      // check for the instance id
      boost::optional<std::string> optInstanceId = v.second.get_optional<std::string>("<xmlattr>.id");    

      // 
      // locate instance in map and add to scene
      // 
    }

    if (type == "sphere") {
      float radius;
      sivelab::Vector3D center;

      buf.str( v.second.get<std::string>("center") );
      buf >> center;
      buf.clear();

      radius = v.second.get<float>("radius");
    }

    if (type == "box") {

      sivelab::Vector3D minPt, maxPt;

      buf.str( v.second.get<std::string>("minPt") );
      buf >> minPt;
      buf.clear();

      buf.str( v.second.get<std::string>("maxPt") );
      buf >> maxPt;
      buf.clear();
    }

    if (type == "triangle") {
      sivelab::Vector3D v0, v1, v2;

      buf.str( v.second.get<std::string>("v0") );
      buf >> v0;
      buf.clear();

      buf.str( v.second.get<std::string>("v1") );
      buf >> v1;
      buf.clear();

      buf.str( v.second.get<std::string>("v2") );
      buf >> v2;
      buf.clear();
    }

    if (type == "mesh") {
      std::string mesh_filename = v.second.get<std::string>("file");
    }

    if (notInstance) {
      // Add the object to the object list
      std::cout << "Ready to place object on the shape list!" << std::endl;
    }
    else {
      // Otherwise, this is an instance we should deal with...
      std::cout << "Ready to add instanced object..." << std::endl;
    }
  }

  void parseShaderData( ptree::value_type const &v )
  {
    std::istringstream buf;

    std::string type, name;
    boost::optional<std::string> optShaderRef = v.second.get_optional<std::string>("<xmlattr>.ref");

    if (!optShaderRef) {

      type = v.second.get<std::string>("<xmlattr>.type");
      name = v.second.get<std::string>("<xmlattr>.name");

    }
    // else 
    // std::cout << "Found optional shader ref: " << *optShaderRef << std::endl;

    // if name exists in the shader map and this is a ref, return the shader pointer
    // otherwise, add the shader if it NOT a ref and the name doesn't exist
    // final is to throw error

    if (!optShaderRef) {

      // Did not find the shader and it was not a reference, so create and return

      if (type == "Lambertian") {
	sivelab::Vector3D kd;
	buf.str( v.second.get<std::string>("diffuse") );
	buf >> kd;
	buf.clear();

	// std::string texName = v.second.get<std::string>("diffuse.<xmlattr>.tex");
	boost::optional<std::string> pTexName = v.second.get_optional<std::string>("diffuse.<xmlattr>.tex");
	if (pTexName) {
	  // find the texture ptr
	  ;
	}
	else {
	  ;
	}
      }
      else if (type == "LambertianPT") {
	sivelab::Vector3D kd;
	buf.str( v.second.get<std::string>("diffuse") );
	buf >> kd;
	buf.clear();

	// std::string texName = v.second.get<std::string>("diffuse.<xmlattr>.tex");
	boost::optional<std::string> pTexName = v.second.get_optional<std::string>("diffuse.<xmlattr>.tex");
	if (pTexName) {
	  // find the texture ptr
	  ;
	}
	else {
	  ;
	}
      }
      else if (type == "CoolToWarm") {
	sivelab::Vector3D kd;
	buf.str( v.second.get<std::string>("diffuse") );
	buf >> kd;
	buf.clear();

	// std::string texName = v.second.get<std::string>("diffuse.<xmlattr>.tex");
	boost::optional<std::string> pTexName = v.second.get_optional<std::string>("diffuse.<xmlattr>.tex");
	if (pTexName) {
	  // find the texture ptr
	  ;
	}
	else {
	  ;
	}
      }
      else if (type == "BlinnPhong" || type == "Phong") {
	sivelab::Vector3D d, s;
	float phongExp;

	buf.str( v.second.get<std::string>("diffuse") );
	buf >> d;
	buf.clear();

	boost::optional<std::string> pTexName = v.second.get_optional<std::string>("diffuse.<xmlattr>.tex");
	if (pTexName) {
	  ;
	}
	else {
	  ;
	}

	buf.str( v.second.get<std::string>("specular") );
	buf >> s;
	buf.clear();

	pTexName = v.second.get_optional<std::string>("specular.<xmlattr>.tex");
	if (pTexName) {
	  ;
	}
	else {
	  ;
	}

	phongExp = v.second.get<float>("phongExp");

	if (type == "BlinnPhong") {
	  std::cout << "create blinn phong shader..." << std::endl;
	}
	else 
	  std::cout << "create phong shader..." << std::endl;
      }
      else if (type == "Mirror") {
	std::cout << "create mirror shader..." << std::endl;
      }
      else if (type == "Glaze") {
	sivelab::Vector3D kd;
	buf.str( v.second.get<std::string>("diffuse") );
	buf >> kd;
	buf.clear();

	float mirrorCoef = 1.0;
	buf.str( v.second.get<std::string>("mirrorCoef") );
	buf >> mirrorCoef;
	buf.clear();

      }
      else if (type == "BlinnPhongMirrored") {
	sivelab::Vector3D kd;
	buf.str( v.second.get<std::string>("diffuse") );
	buf >> kd;
	buf.clear();

	sivelab::Vector3D ks;
	buf.str( v.second.get<std::string>("specular") );
	buf >> ks;
	buf.clear();

	float phongExp = 1.0;
	phongExp = v.second.get<float>("phongExp");

	float mirrorCoef = 1.0;
	buf.str( v.second.get<std::string>("mirrorCoef") );
	buf >> mirrorCoef;
	buf.clear();

      }
      else if (type == "Dielectric") {
	float rI = v.second.get<float>("refractiveIndex");

	sivelab::Vector3D attenCoef;
	boost::optional<std::string > pAttenCoef = v.second.get_optional<std::string>("attenuationCoef");
	if (pAttenCoef) {
	  buf.str( *pAttenCoef );
	  buf >> attenCoef; 
	  buf.clear();
	}
      }
     
      return;
    }
    else if (optShaderRef) {
    
      // optShaderRef is true, so this should be a shader reference.
      // Attempt to find the shader in the map and return it.
      return;
    }
    else 
      std::cerr << "Error that should be caught!" << std::endl;

    return;
  }
  
private:

};


int main(int argc, char *argv[])
{
  GraphicsArgs args;
  args.process(argc, argv);

  XMLSceneParser xmlScene;

  // register object creation function with xmlScene
  xmlScene.registerCallback("camera", new CameraCreator);

  ContainerClass *cc = new ContainerClass();
  xmlScene.registerCallback("light", cc);
  xmlScene.registerCallback("shader", cc);
  xmlScene.registerCallback("shape", cc);

  if (args.inputFileName != "")
    xmlScene.parseFile( args.inputFileName );
  else
    {
      std::cerr << "Need input file!" << std::endl;
      exit(EXIT_FAILURE);
    }

  exit(EXIT_SUCCESS);
}


