#include "Scene.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "RaycastHit.h"
#include "LambertianShader.h"
#include "PerspectiveCamera.h"
#include "SceneElement.h"
#include "Triangle.h"
#include "Mathf.h"

using namespace sivelab;

Shape *Scene::lastHit = NULL;

Scene::Scene(const GraphicsArgs *args)
    : args(args)
{
    ambient = glm::vec3(0.1f);
}

Scene::~Scene()
{
    // delete the cameras
    if (!cameras.empty())
    {
        for (std::vector<Camera*>::iterator it; it != cameras.end();)
        {
            delete (*it);
            it = cameras.erase(it);
        }
    }

    // delete the lights
    if (!lights.empty())
    {
        for (std::vector<Light*>::iterator it; it != lights.end();)
        {
            delete (*it);
            it = lights.erase(it);
        }
    }

    // delete the shapes
    if (!shapes.empty())
    {
        for (std::vector<Shape*>::iterator it; it != shapes.end();)
        {
            delete (*it);
            it = shapes.erase(it);
        }
    }

    // delete the shaders
    if (!shaders.empty())
    {
        for (std::map<std::string, Shader*>::iterator it; it != shaders.end(); it++)
        {
            delete it->second;
        }
    }

    // delete the referenced shapes for instances
    if (!bases.empty())
    {
        for (std::map<std::string, Shape*>::iterator it; it != bases.end(); it++)
        {
            delete it->second;
        }
    }
}

void Scene::add(SceneElement *e)
{
    if (e == NULL)
    {
        return;
    }

    if (e->getSceneElementType() == SceneElement::Type::CAMERA)
    {
        auto camera = static_cast<Camera*>(e);
        // make sure we set the size, this can probably be moved elsewhere
        camera->setSize(glm::vec2(args->width, args->height));
        cameras.push_back(camera);
    }
    else if (e->getSceneElementType() == SceneElement::Type::LIGHT)
    {
        lights.push_back(static_cast<Light*>(e));
    }
    else if (e->getSceneElementType() == SceneElement::Type::SHADER)
    {
        shaders[e->getName()] = static_cast<Shader*>(e);
    }
    else if (e->getSceneElementType() == SceneElement::Type::SHAPE)
    {
        shapes.push_back(static_cast<Shape*>(e));
    }
}

void Scene::addBase(SceneElement *e)
{
    if (e == NULL)
    {
        return;
    }

    if (e->getSceneElementType() == SceneElement::Type::SHAPE)
    {
        bases[e->getName()] = static_cast<Shape*>(e);
    }
}

void Scene::buildBVH()
{
    root = new BVHNode(shapes, 1);
}

Shader * Scene::getShader(const std::string &ref)
{
    std::map<std::string, Shader *>::iterator it = shaders.find(ref);
    if (it == shaders.end())
    {
        std::cerr << "Shader: " << ref << " not found!" << std::endl;
        return NULL;
    }
    else
    {
        return it->second;
    }
}

Shape * Scene::getInstance(const std::string &ref)
{
    std::map<std::string, Shape*>::iterator it = bases.find(ref);
    if (it == bases.end())
    {
        std::cerr << "Refenced Shape: " << ref << " not found!" << std::endl;
        return NULL;
    }
    else
    {
        return it->second;
    }
}

void Scene::computeRay(Ray &ray, float i, float j)
{
    cameras[0]->computeRay(ray, i, j);
}

bool Scene::isHit(const Ray& r, float tMin, float tMax)
{
    RaycastHit hit;

    // check last hit cache
    if (lastHit != NULL)
    {
        if (lastHit->isClosestHit(r, tMin, tMax, hit))
        {
            return true;
        }
    }

    if (root->isClosestHit(r, tMin, tMax, hit))
    {
        lastHit = hit.shape;
        return true;
    }
    return false;
}

glm::vec3 Scene::computeColor(Ray &r, float tMin, float tMax, int depth)
{
    if (depth == -1)
    {
        depth = args->recursionDepth;
    }

    // should set default color to background
    auto color = ambient;

    if (depth > 0)
    {
        RaycastHit hit;
        if (root->isClosestHit(r, tMin, tMax, hit))
        {
            color = hit.shader->apply(this, hit, depth);
        }
    }
    return color;
}

void Scene::rasterize(png::image<png::rgb_pixel>* image)
{
    /**
    double nx = static_cast<double>(args->width);
    double ny = static_cast<double>(args->height);

    std::cout << "[x, y]: [" << nx << ", " << ny << "]" << std::endl;

    // init color image background
    for (int x = 0; x < args->width; x++) {
        for (int y = 0; y < args->height; y++) {
            (*image)[y][x] = png::rgb_pixel(25, 25, 25);
        }
    }

    // 1. construct Mvp
    glm::mat4 mvp = glm::mat4(nx/2.0,    0.0, 0.0, (nx-1)/2.0,
                     0.0, ny/2.0, 0.0, (ny-1)/2.0,
                     0.0,    0.0, 1.0,        0.0,
                     0.0,    0.0, 0.0,        1.0);
    std::cout << "Mvp:" << std:: endl;
    //mvp.print();
    // 2. construct Mortho
    // assume symetric camera
    Camera* cam = cameras[0];
    double l = cam->getImagePlaneWidth() * -0.5;
    double r = -l;
    double t = (cam->getImagePlaneWidth() / cam->getAspectRatio()) * 0.5;
    double b = -t;
    double n = -cam->getFocalLength();
    double f = n - 256.0;

    // init the z-buffer
    std::cout << "Initializing Z-buffer..." << std::endl;
    double* zbuff = new double[args->width * args->height];
    for (int x = 0; x < args->width; x++) {
        for (int y = 0; y < args->height; y++) {
            zbuff[x + y * args->width] = -f;
        }
    }

    glm::mat4 mortho = glm::mat4(2.0/(r-l),       0.0,       0.0, -(r+l)/(r-l),
                           0.0, 2.0/(t-b),       0.0, -(t+b)/(t-b),
                           0.0,       0.0, 2.0/(n-f), -(n+f)/(n-f),
                           0.0,       0.0,       0.0,          1.0);
    std::cout << "Mortho:" << std::endl;
    //mortho.print();
    // 3. construct P
    glm::mat4 p = glm::mat4(  n, 0.0, 0.0, 0.0,
                0.0,   n, 0.0, 0.0,
                0.0, 0.0, n+f, -f*n,
                0.0, 0.0, 1.0, 0.0);
    std::cout << "P:" << std::endl;
    p.print();
    // 4. construct Mcam
    Basis bas = cam->getBasis();
    glm::mat4 mcam1 = glm::mat4(bas.getU()[0], bas.getU()[1], bas.getU()[2], 0.0,
                    bas.getV()[0], bas.getV()[1], bas.getV()[2], 0.0,
                    bas.getW()[0], bas.getW()[1], bas.getW()[2], 0.0,
                              0.0,           0.0,           0.0, 1.0);
    auto orig = cam->getPosition();
    auto mcam2 = glm::mat4(1.0, 0.0, 0.0, -orig[0],
                    0.0, 1.0, 0.0, -orig[1],
                    0.0, 0.0, 1.0, -orig[2],
                    0.0, 0.0, 0.0,     1.0);
    auto mcam = mcam1 * mcam2;
    std::cout << "Mcam:" << std::endl;
    mcam.print();
    // Mper = MorthoP
    auto mper = mortho * p;
    std::cout << "Mper:" << std::endl;
    //mper.print();
    // M = MvpMperMcam
    auto m = mvp * mper * mcam;
    std::cout << "M:" << std::endl;
    //m.print();
    // for all triangles, t
    // assume all shapes are triangles :(
    std::vector<glm::vec3> fragments;
    std::vector<glm::vec3> colors;
    for (
        std::vector<Shape*>::iterator sit = shapes.begin();
        sit != shapes.end();
        sit++
    ) {
        std::vector<Triangle*> triangles = (*sit)->getTriangles();
        for (
            std::vector<Triangle*>::iterator tit = triangles.begin();
            tit != triangles.end();
            tit++
        ) {
            std::vector<glm::vec3> verticies = (*tit)->getVerticies();
            for (
                std::vector<glm::vec3>::iterator vit = verticies.begin();
                vit != verticies.end();
                vit++
            ) {
                // shaders
                RaycastHit hit;
                hit.point = *vit;
                hit.normal = (*tit)->getSurfaceNormal();
                Vector3D color = (*tit)->getShader()->apply(this, hit, 1);
                colors.push_back(color);

                double w = 1.0;
    //                std::cout << "Before:" << std::endl
    //                    << "\t" << *vit << std::endl
    //                    << "\t" << w << std::endl;
                // v' = Mv
                Vector3D v_i = m.multVector(*vit, w);
                // convert image plane coord
                // (v'x  v'y)
                // (---, ---)
                // (v'w  v'w)
                v_i[0] = v_i[0]/w;
                v_i[1] = v_i[1]/w;
    //                std::cout << "After:" << std::endl
    //                    << "\t" << v_i << std::endl
    //                    << "\t" << w << std::endl;
                fragments.push_back(v_i);
            }
        }
    }

    // TODO clipping here

    std::cout << "Fragments: " << fragments.size() << std::endl
        << "Colors: " << colors.size() << std::endl;

    // for all fragments
    for (
        std::pair<std::vector<Vector3D>::iterator, std::vector<Vector3D>::iterator>
            fit(fragments.begin(), colors.begin());
        ;
        fit.first += 3, fit.second += 3
    ) {
        //
        // in order for the implicit line equations to be valid, we need x0 <= x1
        // if this is not the case, swap them
        //
        if ((*fit.first)[0] > (*(fit.first + 1))[0]) {
            std::iter_swap(fit.first, fit.first + 1);
            std::iter_swap(fit.second, fit.second + 1);
        }
        Vector3D v0 = *fit.first;
        Vector3D v1 = *(fit.first + 1);
        Vector3D v2 = *(fit.first + 2);

        std::vector<Vector3D> verticies;
        verticies.push_back(v0);
        verticies.push_back(v1);
        verticies.push_back(v2);

        // xmin = floor(xi)
        // xmax = ceiling(xi)
        // ymin = floor(yi)
        // ymax = ceiling(yi)
        std::vector<int> xfloors;
        std::vector<int> xceilings;
        std::vector<int> yfloors;
        std::vector<int> yceilings;
        for (
            std::vector<Vector3D>::iterator it = verticies.begin();
            it != verticies.end();
            it++
        ) {
            xfloors.push_back(static_cast<int>(floor((*it)[0])));
            xceilings.push_back(static_cast<int>(ceil((*it)[0])));
            yfloors.push_back(static_cast<int>(floor((*it)[1])));
            yceilings.push_back(static_cast<int>(ceil((*it)[1])));
        }

        int xmin = *std::min_element(xfloors.begin(), xfloors.end());
        int xmax = *std::max_element(xceilings.begin(), xceilings.end());
        int ymin = *std::min_element(yfloors.begin(), yfloors.end());
        int ymax = *std::max_element(yceilings.begin(), yceilings.end());

    //       std::cout << "Fragment: " << std::distance(fragments.begin(), fit.first) << std::endl
    //          << "\tx: " << "[" << xmin << ", " << xmax << "]" << std::endl
    //         << "\ty: " << "[" << ymin << ", " << ymax << "]" << std::endl
    //        << "\t" << v0 << std::endl
     //       << "\t" << v1 << std::endl
      //      << "\t" << v2 << std::endl;
        //
        // Reference:
        // f01(x, y) = (y0 - y1)x + (x1 - x0)y + x0y1 - x1y0
        // f12(x, y) = (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1
        // f20(x, y) = (y2 - y0)x + (x0 - x2)y + x2y0 - x0y2
        //
        // falpha = f12(x0, y0)
        float falpha = (v1[1] - v2[1])*v0[0] + (v2[0] - v1[0])*v0[1] + v1[0]*v2[1] - v2[0]*v1[1];
        // fbeta = f20(x1, y1)
        float fbeta = (v2[1] - v0[1])*v1[0] + (v0[0] - v2[0])*v1[1] + v2[0]*v0[1] - v0[0]*v2[1];
        // fgamma = f01(x2, y2)
        float fgamma = (v0[1] - v1[1])*v2[0] + (v1[0] - v0[0])*v2[1] + v0[0]*v1[1] - v1[0]*v0[1];
        // check if fgamma = 0
        if (Util::areLike(fgamma, 0.0f)) {
            continue;
        }

        // for y = ymin to yamx
        for (int y = ymin; y < ymax; y++) {
            // for x = xmin to xmax
            for (int x = xmin; x < xmax; x++) {
                // alpha = f12(x, y)/falpha
                float alpha = ((v1[1] - v2[1])*x + (v2[0] - v1[0])*y + v1[0]*v2[1] - v2[0]*v1[1])/falpha;
                // beta = f20(x, y)/fbeta
                float beta = ((v2[1] - v0[1])*x + (v0[0] - v2[0])*y + v2[0]*v0[1] - v0[0]*v2[1])/fbeta;
                // gamma = f01(x, y)/fgamma
                float gamma = ((v0[1] - v1[1])*x + (v1[0] - v0[0])*y + v0[0]*v1[1] - v1[0]*v0[1])/fgamma;

                // if (alpha > 0 or falpha * f12(-1, -1) > 0)
                //      and (beta > 0 or fbeta * f20(-1, -1) > 0)
                //      and (gamma > 0 or fgamma * f01(-1, -1) > 0) then
                if (
                    alpha > 0.0f
                    && beta > 0.0f
                    && gamma > 0.0f
                ) {
                    // z-buffer calculation
                    double depth = (v0[2] * alpha)
                        + (v1[2] * beta)
                        + (v2[2] * gamma);
                    if (zbuff[x + y * args->width] > depth) {
                        zbuff[x + y * args->width] = depth;
                        // color = v0color alpha + v1color beta + v2color gamma
                        // drawpixel(x, y) with color
                        Vector3D c = ((*fit.second) * alpha)
                            + (*(fit.second + 1) * beta)
                            + (*(fit.second + 2) * gamma);
                        // cap the colors
                        for (int i = 0; i < 3; i++) {
                            c[i] = std::min(1.0, std::max(0.0, c[i]));
                        }
                        (*image)[args->height - y - 1][x] = png::rgb_pixel(
                            static_cast<int>(std::floor(c[0]*255)),
                            static_cast<int>(std::floor(c[1]*255)),
                            static_cast<int>(std::floor(c[2]*255))
                        );
                    } // end zbuff check
                } // end alpha beta gamma check
            } // end x
        } // end y

        // loop end condition
        if (
            fit.first + 3 == fragments.end()
            || fit.second + 3 == colors.end()
        ) {
            break;
        }
    }

    delete [] zbuff;
    */
}
