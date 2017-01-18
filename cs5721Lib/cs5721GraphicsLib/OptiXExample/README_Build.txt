Here's the cmake line you want to use:

mkdir build
cd build
cmake .. -DOptiX_INCLUDE=/home/cs/software/sivelab/OptiX/include -DOptiX_INSTALL_DIR=/home/cs/software/sivelab/OptiX -DCUDA_TOOLKIT_ROOT_DIR=/home/cs/software/sivelab/cuda/cuda -DCUDA_SDK_ROOT_DIR=/home/cs/software/sivelab/cuda/sdk

then you can make and run the example:
./OptiXExample
