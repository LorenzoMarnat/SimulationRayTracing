#include "lodepng.h"
#include "Sphere.h"
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

float raySphereIntersect(Vector3 r0, Vector3 rd, vector<Sphere> vs0/*, vector<float> vsr*/) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.

    for (int i = 0; i < vs0.size(); i++)
    {
        Vector3 s0 = vs0[i].GetCentre();
        float sr = vs0[i].GetRayon();

        float a = rd.dot(rd);
        Vector3 s0_r0 = r0 - s0;
        float b = 2.0 * rd.dot(s0_r0);
        float c = s0_r0.dot(s0_r0) - (sr * sr);
        if (b * b - 4.0 * a * c >= 0.0)
        {
            float r = (-b - sqrt((b * b) - 4.0 * a * c)) / (2.0 * a);
            if(r >= 0)
                return r;
        }
    }
    return -1.0;
}

int main(int argc, char* argv[]) {
        const char* filename = argc > 1 ? argv[1] : "test.png";

        unsigned width = 512, height = 512;
         
        vector<unsigned char> image;
        image.resize(width * height * 4);

        vector<Sphere> spheres;
        //spheres.resize(2);

        Vector3 plan = Vector3(0, 0, 0);

        Sphere sphere = Sphere(100, Vector3(200, 100, 150));
        addSphere(&spheres, sphere);

        Sphere sphere2 = Sphere(60, Vector3(400, 350, 200));
        addSphere(&spheres, sphere2);

        for (unsigned y = 0; y < width; y++)
        {
            for (unsigned x = 0; x < width; x++) 
            {

                Vector3 rayon = Vector3(plan.x + x, plan.y + y, plan.z);
                float inter = raySphereIntersect(rayon, Vector3(0, 0, 1), spheres);
                if (inter >= 0)
                {
                    color(&image, 4 * width * y + 4 * x, 255 * (inter / 255), 255 * (inter / 255), 255 * (inter / 255), 255);
                }
                else
                {
                    if (inter == -1)
                    {
                        color(&image, 4 * width * y + 4 * x, 255, 0, 0, 255);
                    }
                }
            }
        }

        encodeOneStep(filename, image, width, height);
}
