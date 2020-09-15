#include "lodepng.h"
#include "Sphere.h"
#include "Rayon.h"
#include <math.h>
#include <iostream>
using namespace std;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void color(vector<unsigned char> *img, int index, float r, float g, float b, float a)
{
    img->at(index) = r;
    img->at(index+1) = g;
    img->at(index+2) = b;
    img->at(index+3) = a;
}

void addSphere(vector<Sphere>* spheres, Sphere sphere)
{
    int size = spheres->size();
    spheres->resize(size + 1);
    spheres->at(size) = sphere;
}

bool raySphereIntersect(Rayon r, Sphere s, float *rayon) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.
    bool inte = false;

    Vector3 s0 = s.GetCentre();
    float sr = s.GetRayon();
    float a = r.GetDirection().dot(r.GetDirection());
    Vector3 s0_r0 = r.GetOrigine() - s0;
    float b = 2.0 * r.GetDirection().dot(s0_r0);
    float c = s0_r0.dot(s0_r0) - (sr * sr);
    if (b * b - 4.0 * a * c >= 0.0)
    {
        *rayon = (-b - sqrt((b * b) - 4.0 * a * c)) / (2.0 * a);
        if(*rayon >= 0)
            inte = true;
    }
    return inte;
}

bool intersectSpheres(Rayon r, vector<Sphere> spheres, float* rayon)
{
    bool t = false;
    for(int i = 0;i< spheres.size();i++)
    {
        float rr;
        bool inteSphere = raySphereIntersect(r, spheres[i], &rr);
        if (inteSphere)
        {
            if (!t)
            {
                *rayon = rr;
            }
            else
            {
                if (rr < *rayon)
                    *rayon = rr;
            }
            t = true;
        }
    }
    return t;
}
int main(int argc, char* argv[]) {
        const char* filename = argc > 1 ? argv[1] : "test.png";

        unsigned width = 512, height = 512;
         
        vector<unsigned char> image;
        image.resize(width * height * 4);

        vector<Sphere> spheres;

        Vector3 plan = Vector3(0, 0, 0);

        Sphere sphere = Sphere(100, Vector3(200, 100, 200));
        addSphere(&spheres, sphere);

        Sphere sphere2 = Sphere(60, Vector3(200, 300, 200));
        addSphere(&spheres, sphere2);

        for (unsigned y = 0; y < width; y++)
        {
            for (unsigned x = 0; x < width; x++) 
            {
                float minRayon;

                Rayon rayon = Rayon(Vector3(0, 0, 1), Vector3(plan.x + x, plan.y + y, plan.z));
                bool inter = intersectSpheres(rayon, spheres, &minRayon);

                //cout << inte << endl;
                //cout << inter << endl;
                if (inter)
                {
                   // 
                    float mnRayon;
                    Rayon lampee = Rayon(Vector3(1, 0, 0), Vector3(x,y,minRayon-0.02));
                    bool inter2 = intersectSpheres(lampee, spheres, &mnRayon);
                    //cout << mnRayon << endl;
                    if (!inter2)
                        color(&image, 4 * width * y + 4 * x, 255 - minRayon, 255 - minRayon, 255 - minRayon, 255);
                    else
                        color(&image, 4 * width * y + 4 * x, 255, 0, 0, 255);
                }
                else
                {
                    color(&image, 4 * width * y + 4 * x, 0, 0, 127, 255);
                }
            }
        }

        encodeOneStep(filename, image, width, height);
}
