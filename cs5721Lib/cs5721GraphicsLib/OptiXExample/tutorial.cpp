
/*
 * Copyright (c) 2008 - 2009 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and proprietary
 * rights in and to this software, related documentation and any modifications thereto.
 * Any use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from NVIDIA Corporation is strictly
 * prohibited.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
 * AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
 * SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
 * BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
 * INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES
 */


// 0 - normal shader

#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <iostream>
#include <GLUTDisplay.h>
#include <ImageLoader.h>
#include "commonStructs.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <math.h>

using namespace optix;

static float rand_range(float min, float max)
{
  return min + (max - min) * (float) rand() / (float) RAND_MAX;
}


//-----------------------------------------------------------------------------
// 
// Whitted Scene
//
//-----------------------------------------------------------------------------

class Tutorial : public SampleScene
{
public:
  Tutorial(int tutnum, const std::string& texture_path)
    : SampleScene(), _tutnum(tutnum), _width(1080u), _height(720u), texture_path( texture_path )
  {}
  
  // From SampleScene
  void   initScene( InitialCameraData& camera_data );
  void   trace( const RayGenCameraData& camera_data );
  void   doResize( unsigned int width, unsigned int height );
  void   setDimensions( const unsigned int w, const unsigned int h ) { _width = w; _height = h; }
  Buffer getOutputBuffer();

private:
  std::string texpath( const std::string& base );
  void createGeometry();

  unsigned int _tutnum;
  unsigned int _width;
  unsigned int _height;
  std::string   texture_path;
  std::string  _ptx_path;
};


void Tutorial::initScene( InitialCameraData& camera_data )
{
  // set up path to ptx file associated with tutorial number
  std::stringstream ss;
  ss << "tutorial" << _tutnum << ".cu";
  _ptx_path = ptxpath( "programs/OptiXExample", ss.str() );

  // context 
  _context->setRayTypeCount( 2 );
  _context->setEntryPointCount( 1 );
  _context->setStackSize( 4640 );

  _context["max_depth"]->setInt(100);
  _context["radiance_ray_type"]->setUint(0);
  _context["shadow_ray_type"]->setUint(1);
  _context["frame_number"]->setUint( 0u );
  _context["scene_epsilon"]->setFloat( 1.e-3f );
  _context["importance_cutoff"]->setFloat( 0.01f );
  _context["ambient_light_color"]->setFloat( 0.31f, 0.33f, 0.28f );

  _context["output_buffer"]->set( createOutputBuffer(RT_FORMAT_UNSIGNED_BYTE4, _width, _height) );

  // Ray gen program
  std::string camera_name = "pinhole_camera";
  Program ray_gen_program = _context->createProgramFromPTXFile( _ptx_path, camera_name );
  _context->setRayGenerationProgram( 0, ray_gen_program );

  // Exception / miss programs
  Program exception_program = _context->createProgramFromPTXFile( _ptx_path, "exception" );
  _context->setExceptionProgram( 0, exception_program );
  _context["bad_color"]->setFloat( 0.0f, 1.0f, 0.0f );

  std::string miss_name = "miss";
  _context->setMissProgram( 0, _context->createProgramFromPTXFile( _ptx_path, miss_name ) );
  _context["bg_color"]->setFloat( make_float3( 0.34f, 0.55f, 0.85f ) );

  // Lights
  BasicLight lights[] = { 
    { make_float3( -5.0f, 60.0f, -16.0f ), make_float3( 1.0f, 1.0f, 1.0f ), 1 }
  };

  Buffer light_buffer = _context->createBuffer(RT_BUFFER_INPUT);
  light_buffer->setFormat(RT_FORMAT_USER);
  light_buffer->setElementSize(sizeof(BasicLight));
  light_buffer->setSize( sizeof(lights)/sizeof(lights[0]) );
  memcpy(light_buffer->map(), lights, sizeof(lights));
  light_buffer->unmap();

  _context["lights"]->set(light_buffer);

  // Set up camera
  camera_data = InitialCameraData( make_float3( 7.0f, 9.2f, -6.0f ), // eye
                                   make_float3( 0.0f, 4.0f,  0.0f ), // lookat
                                   make_float3( 0.0f, 1.0f,  0.0f ), // up
                                   60.0f );                          // vfov

  _context["eye"]->setFloat( make_float3( 0.0f, 0.0f, 0.0f ) );
  _context["U"]->setFloat( make_float3( 0.0f, 0.0f, 0.0f ) );
  _context["V"]->setFloat( make_float3( 0.0f, 0.0f, 0.0f ) );
  _context["W"]->setFloat( make_float3( 0.0f, 0.0f, 0.0f ) );

  // Populate scene hierarchy
  createGeometry();

  // Prepare to run
  _context->validate();
  _context->compile();
}


Buffer Tutorial::getOutputBuffer()
{
  return _context["output_buffer"]->getBuffer();
}


void Tutorial::trace( const RayGenCameraData& camera_data )
{
  _context["eye"]->setFloat( camera_data.eye );
  _context["U"]->setFloat( camera_data.U );
  _context["V"]->setFloat( camera_data.V );
  _context["W"]->setFloat( camera_data.W );

  Buffer buffer = _context["output_buffer"]->getBuffer();
  RTsize buffer_width, buffer_height;
  buffer->getSize( buffer_width, buffer_height );

  _context->launch( 0, static_cast<unsigned int>(buffer_width),
                      static_cast<unsigned int>(buffer_height) );
}


void Tutorial::doResize( unsigned int width, unsigned int height )
{
  // output buffer handled in SampleScene::resize
}

std::string Tutorial::texpath( const std::string& base )
{
  return texture_path + "/" + base;
}

float4 make_plane( float3 n, float3 p )
{
  n = normalize(n);
  float d = -dot(n, p);
  return make_float4( n, d );
}

void Tutorial::createGeometry()
{
  std::string box_ptx( ptxpath( "programs/OptiXExample", "box.cu" ) ); 
  Program box_bounds = _context->createProgramFromPTXFile( box_ptx, "box_bounds" );
  Program box_intersect = _context->createProgramFromPTXFile( box_ptx, "box_intersect" );

  // Create box
  Geometry box = _context->createGeometry();
  box->setPrimitiveCount( 1u );
  box->setBoundingBoxProgram( box_bounds );
  box->setIntersectionProgram( box_intersect );
  box["boxmin"]->setFloat( -2.0f, 0.0f, -2.0f );
  box["boxmax"]->setFloat(  2.0f, 7.0f,  2.0f );

  // Floor geometry
  std::string pgram_ptx( ptxpath( "programs/OptiXExample", "parallelogram.cu" ) );
  Geometry parallelogram = _context->createGeometry();
  parallelogram->setPrimitiveCount( 1u );
  parallelogram->setBoundingBoxProgram( _context->createProgramFromPTXFile( pgram_ptx, "bounds" ) );
  parallelogram->setIntersectionProgram( _context->createProgramFromPTXFile( pgram_ptx, "intersect" ) );
  float3 anchor = make_float3( -64.0f, 0.01f, -64.0f );
  float3 v1 = make_float3( 128.0f, 0.0f, 0.0f );
  float3 v2 = make_float3( 0.0f, 0.0f, 128.0f );
  float3 normal = cross( v2, v1 );
  normal = normalize( normal );
  float d = dot( normal, anchor );
  v1 *= 1.0f/dot( v1, v1 );
  v2 *= 1.0f/dot( v2, v2 );
  float4 plane = make_float4( normal, d );
  parallelogram["plane"]->setFloat( plane );
  parallelogram["v1"]->setFloat( v1 );
  parallelogram["v2"]->setFloat( v2 );
  parallelogram["anchor"]->setFloat( anchor );

  // Materials
  std::string box_chname = "closest_hit_normalMapShader";
  Material box_matl = _context->createMaterial();
  Program box_ch = _context->createProgramFromPTXFile( _ptx_path, box_chname );
  box_matl->setClosestHitProgram( 0, box_ch );
  box_matl["Ka"]->setFloat( 0.3f, 0.3f, 0.3f );
  box_matl["Kd"]->setFloat( 0.6f, 0.7f, 0.8f );
  box_matl["Ks"]->setFloat( 0.8f, 0.9f, 0.8f );
  box_matl["phong_exp"]->setFloat( 88 );
  box_matl["reflectivity_n"]->setFloat( 0.2f, 0.2f, 0.2f );

  std::string floor_chname = "closest_hit_normalMapShader";
  Material floor_matl = _context->createMaterial();
  Program floor_ch = _context->createProgramFromPTXFile( _ptx_path, floor_chname );
  floor_matl->setClosestHitProgram( 0, floor_ch );
  floor_matl["Ka"]->setFloat( 0.3f, 0.3f, 0.1f );
  floor_matl["Kd"]->setFloat( 194/255.f*.6f, 186/255.f*.6f, 151/255.f*.6f );
  floor_matl["Ks"]->setFloat( 0.4f, 0.4f, 0.4f );
  floor_matl["reflectivity"]->setFloat( 0.1f, 0.1f, 0.1f );
  floor_matl["reflectivity_n"]->setFloat( 0.05f, 0.05f, 0.05f );
  floor_matl["phong_exp"]->setFloat( 88 );
  floor_matl["tile_v0"]->setFloat( 0.25f, 0, .15f );
  floor_matl["tile_v1"]->setFloat( -.15f, 0, 0.25f );
  floor_matl["crack_color"]->setFloat( 0.1f, 0.1f, 0.1f );
  floor_matl["crack_width"]->setFloat( 0.02f );

  // Create GIs for each piece of geometry
  std::vector<GeometryInstance> gis;
  gis.push_back( _context->createGeometryInstance( box, &box_matl, &box_matl+1 ) );
  gis.push_back( _context->createGeometryInstance( parallelogram, &floor_matl, &floor_matl+1 ) );

  // Place all in group
  GeometryGroup geometrygroup = _context->createGeometryGroup();
  geometrygroup->setChildCount( static_cast<unsigned int>(gis.size()) );
  geometrygroup->setChild( 0, gis[0] );
  geometrygroup->setChild( 1, gis[1] );

  // You can modify nvidia's BVH/spatial data structures through this
  geometrygroup->setAcceleration( _context->createAcceleration("NoAccel","NoAccel") );

  _context["top_object"]->set( geometrygroup );
  _context["top_shadower"]->set( geometrygroup );
}


//-----------------------------------------------------------------------------
//
// Main driver
//
//-----------------------------------------------------------------------------

void printUsageAndExit( const std::string& argv0, bool doExit = true )
{
  std::cerr
    << "Usage  : " << argv0 << " [options]\n"
    << "App options:\n"
    << "  -h  | --help                               Print this usage message\n"
    << "        --dim=<width>x<height>               Set image dimensions\n"
    << std::endl;

  if ( doExit ) exit(1);
}


int main( int argc, char** argv )
{
  GLUTDisplay::init( argc, argv );

  unsigned int width = 1080u, height = 720u;

  std::string texture_path;
  int tutnum = 0;
  for ( int i = 1; i < argc; ++i ) {
    std::string arg( argv[i] );
    if ( arg == "--help" || arg == "-h" ) {
      printUsageAndExit( argv[0] );
    } else if ( arg.substr( 0, 6 ) == "--dim=" ) {
      std::string dims_arg = arg.substr(6);
      if ( sutilParseImageDimensions( dims_arg.c_str(), &width, &height ) != RT_SUCCESS ) {
        std::cerr << "Invalid window dimensions: '" << dims_arg << "'" << std::endl;
        printUsageAndExit( argv[0] );
      }
    } else if ( arg == "-t" || arg == "--texture-path" ) {
      if ( i == argc-1 ) {
        printUsageAndExit( argv[0] );
      }
      texture_path = argv[++i];
    } else {
      std::cerr << "Unknown option: '" << arg << "'\n";
      printUsageAndExit( argv[0] );
    }
  }

  if( !GLUTDisplay::isBenchmark() ) printUsageAndExit( argv[0], false );

  std::stringstream title;
  title << "OptiXExample";
  try {
    Tutorial scene(tutnum, texture_path);
    scene.setDimensions( width, height );
    GLUTDisplay::run( title.str(), &scene );
  } catch( Exception& e ){
    sutilReportError( e.getErrorString().c_str() );
    exit(1);
  }
  return 0;
}
