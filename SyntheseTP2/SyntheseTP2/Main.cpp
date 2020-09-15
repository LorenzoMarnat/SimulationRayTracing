#include "lodepng.h"
#include "vector3.h"
#include <iostream>
using namespace std;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

float raySphereIntersect(Vector3 r0, Vector3 rd, Vector3 s0, float sr) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.
    float a = rd.dot(rd);
    Vector3 s0_r0 = r0 - s0;
    float b = 2.0 * rd.dot(s0_r0);
    float c = s0_r0.dot(s0_r0) - (sr * sr);
    if (b* b - 4.0 * a * c < 0.0) {
        return -1.0;
    }
    return (-b - sqrt((b * b) - 4.0 * a * c)) / (2.0 * a);
}

int main(int argc, char* argv[]) {
        //NOTE: this sample will overwrite the file or test.png without warning!
        const char* filename = argc > 1 ? argv[1] : "test.png";

        //generate some image
        unsigned width = 512, height = 512;
         
        std::vector<unsigned char> image;

        image.resize(width * height * 4);

        Vector3 plan = Vector3(0, 0, 0);

        Vector3 sphere = Vector3(200, 100, 150);
        float rayonSphere = 100;

        
        for (unsigned y = 0; y < width; y++)
        {
            for (unsigned x = 0; x < width; x++) 
            {
                Vector3 rayon = Vector3(plan.x + x, plan.y + y, plan.z);
                float inter = raySphereIntersect(rayon, Vector3(0, 0, 1), sphere, rayonSphere);

                if (inter >= 0)
                {
                    image[4 * width * y + 4 * x + 0] = 255 * (inter / 255);
                    image[4 * width * y + 4 * x + 1] = 255 * (inter / 255);
                    image[4 * width * y + 4 * x + 2] = 255 * (inter / 255);
                    image[4 * width * y + 4 * x + 3] = 255;
                }
                else
                {
                    if (inter == -1)
                    {
                        image[4 * width * y + 4 * x + 0] = 255;
                        image[4 * width * y + 4 * x + 1] = 0;
                        image[4 * width * y + 4 * x + 2] = 0;
                        image[4 * width * y + 4 * x + 3] = 255;
                    }
                    else
                        cout << "Erreur: sphere hors du plan" << endl;
                }
            }
        }

        encodeOneStep(filename, image, width, height);
}
