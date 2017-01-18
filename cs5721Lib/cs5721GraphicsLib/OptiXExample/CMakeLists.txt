cmake_minimum_required (VERSION 2.8) 
PROJECT (OptiXExample) 

SET(BUNDLEOPTS MACOSX_BUNDLE)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

FIND_PACKAGE(CUDA REQUIRED)
IF(CUDA_FOUND)
   INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
   INCLUDE_DIRECTORIES(${CUDA_SDK_ROOT_DIR}/common/inc)

  # Add the need flags for atomic operations in CUDA.
  #   if you know what you're doing...
  #   SET(CUDA_NVCC_FLAGS -arch=compute_13)
ELSE (CUDA_FOUND)
   MESSAGE(FATAL_ERROR "CUDA is required for compiling this project.  Please install CUDA or re-run cmake with -i to specify the CUDA directories.")
ENDIF(CUDA_FOUND)

# Turn off the warning that NVCC issues when generating PTX from our CUDA samples.  This
# is a custom extension to the FindCUDA code distributed by CMake.
OPTION(CUDA_REMOVE_GLOBAL_MEMORY_SPACE_WARNING "Suppress the \"Advisory: Cannot tell what pointer points to, assuming global memory space\" warning nvcc makes." ON)
set(CUDA_REMOVE_GLOBAL_MEMORY_SPACE_WARNING @CUDA_REMOVE_GLOBAL_MEMORY_SPACE_WARNING@)

FIND_PACKAGE(OptiX REQUIRED)
INCLUDE_DIRECTORIES("${OptiX_INCLUDE}")

FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/programs")

#########################################################
# OPTIX_add_sample_executable
#
# Convience function for adding samples to the code.  You can copy the contents of this
# funtion into your individual project if you wish to customize the behavior.  Note that
# in CMake, functions have their own scope, whereas macros use the scope of the caller.
function(OPTIX_add_sample_executable target_name)

  # These calls will group PTX and CUDA files into their own directories in the Visual
  # Studio projects.
  source_group("PTX Files"  REGULAR_EXPRESSION ".+\\.ptx$")
  source_group("CUDA Files" REGULAR_EXPRESSION ".+\\.cu$")

  # Separate the sources from the CMake and CUDA options fed to the macro.  This code
  # comes from the CUDA_COMPILE_PTX macro found in FindCUDA.cmake.  We are copying the
  # code here, so that we can use our own name for the target.  target_name is used in the
  # creation of the output file names, and we want this to be unique for each target in
  # the SDK.
  CUDA_GET_SOURCES_AND_OPTIONS(source_files cmake_options options ${ARGN})

  # Create the rules to build the PTX from the CUDA files.
  CUDA_WRAP_SRCS( ${CMAKE_BUILD_DIR}/programs/${target_name} PTX generated_files ${source_files} ${cmake_options}
    OPTIONS ${options} )

  # Here is where we create the rule to make the executable.  We define a target name and
  # list all the source files used to create the target.  In addition we also pass along
  # the cmake_options parsed out of the arguments.
  add_executable(${target_name}
    ${source_files}
    ${generated_files}
    ${cmake_options}
    )

  target_link_libraries( ${target_name}
    optix
    ${optix_rpath}
    )
endfunction()

# Your programs can be defined down here....

INCLUDE_DIRECTORIES(/home/cs/software/sivelab/OptiX/SDK/sutil)
LINK_DIRECTORIES(/home/cs/software/sivelab/OptiX/lib64)

OPTIX_add_sample_EXECUTABLE(OptiXExample
  tutorial.h tutorial.cpp
  tutorial0.cu
  box.cu
  parallelogram.cu
  )

TARGET_LINK_LIBRARIES(OptiXExample sutil)

