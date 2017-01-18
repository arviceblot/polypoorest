add_executable(ray-tracer
    CameraCreator.cpp CameraCreator.h
    LightCreator.cpp LightCreator.h
    RasterizerArgs.cpp RasterizerArgs.h
    RenderWorkerThread.h
    ShaderCreator.cpp ShaderCreator.h
    ShapeCreator.cpp ShapeCreator.h
    ThreadData.h
    ray-tracer.cpp
)

target_link_libraries(ray-tracer
    sive-rtutil
    graphics-lib
    ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    ${Boost_THREAD_LIBRARIES}
    ${Boost_SYSTEM_LIBRARIES}
    ${PNG_LIBRARY}
    ${ZLIB_LIBRARY}
    pthread
)
