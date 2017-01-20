find_package(SFML 1 COMPONENTS system window graphics audio REQUIRED)

add_library(graphics-lib
    AreaLight.cpp AreaLight.h
    Basis.cpp Basis.h
    BlinnPhongShader.cpp BlinnPhongShader.h
    BoundingBox.cpp BoundingBox.h
    Box.cpp Box.h
    BVHNode.cpp BVHNode.h
    Camera.cpp Camera.h
    DielectricShader.cpp DielectricShader.h
    ImplicitSphere.cpp ImplicitSphere.h
    InstancedObject.cpp InstancedObject.h
    LambertianShader.cpp LambertianShader.h
    Light.cpp Light.h
    Mathf.h Mathf.cpp
    PerspectiveCamera.cpp PerspectiveCamera.h
    PointLight.cpp PointLight.h
    Ray.h
    RaycastHit.h
    RenderWindow.cpp RenderWindow.h
    Scene.cpp Scene.h
    SceneElement.cpp SceneElement.h
    Shader.cpp Shader.h
    Shape.cpp Shape.h
    Triangle.cpp Triangle.h
    )

target_link_libraries(graphics-lib
    ${SFML_LIBRARIES}
    )
