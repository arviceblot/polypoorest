#include <iostream>

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

int CameraCallbackTest( ptree::value_type const &v )
{
  std::cout << "Camera" << std::endl;
  std::cout << "\tName: " << v.second.get<std::string>("<xmlattr>.name") << ", Type: " << v.second.get<std::string>("<xmlattr>.type") << std::endl;
  std::cout << "\tPosition: " << v.second.get<std::string>("position") << std::endl;

  return 1;
}

int main(int argc, char *argv[])
{
  // populate the ptree structure
  ptree pt;

  read_xml(argv[1], pt);

  BOOST_FOREACH( ptree::value_type const &v, pt.get_child("scene") ) {

    // v.first is the name of the child
    // v.second is the child tree

    if (v.first == "camera") {
      CameraCallbackTest( v );
    }

    if (v.first == "light") {
      std::cout << "Light" << std::endl;
      std::cout << "\tType: " << v.second.get<std::string>("<xmlattr>.type") << std::endl;
      std::cout << "\tPosition: " << v.second.get<std::string>("position") << std::endl;
    }

  }
}
