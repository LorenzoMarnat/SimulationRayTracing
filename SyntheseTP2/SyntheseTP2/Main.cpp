#include "lodepng.h"
#include "Sphere.h"
#include "Lampe.h"
#include <math.h>
#include <iostream>
using namespace std;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void color(vector<unsigned char> *img, int index, Couleur couleur, float distance)
{
    float red = couleur.red - ((50 * distance) / 255);
    if (red < 0)
        red = 0;
    if (red > 255)
        red = 255;
    if (img->at(index) == 256)
    {
        img->at(index) = red;
    }
    else
    {
        img->at(index) = (img->at(index) + red) / 2;
    }

    float green = couleur.green - ((50 * distance) / 255);
    if (green < 0)
        green = 0;
    if (green > 255)
        green = 255;
    if (img->at(index+1) == 256)
    {
        img->at(index+1) = green;
    }
    else
    {
        img->at(index+1) = (img->at(index+1) + green) / 2;
    }

    float blue = couleur.blue - ((50 * distance) / 255);
    if (blue < 0)
        blue = 0;
    if (blue > 255)
        blue = 255;
    if (img->at(index+2) == 256)
    {
        img->at(index+2) = blue;
    }
    else
    {
        img->at(index+2) = (img->at(index+2) + blue) / 2;
    }

    img->at(index+3) = couleur.alpha;
}

void addSphere(vector<Sphere>* spheres, Sphere sphere)
{
    int size = spheres->size();
    spheres->resize(size + 1);
    spheres->at(size) = sphere;
}

void addLampe(vector<Lampe>* lampes, Lampe lampe)
{
    int size = lampes->size();
    lampes->resize(size + 1);
    lampes->at(size) = lampe;
}
bool raySphereIntersect(Rayon r, Sphere s, float *distance) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.
    bool intersect = false;

    Vector3 s0 = s.GetCentre();
    float sr = s.GetRayon();
    float a = r.GetDirection().dot(r.GetDirection());
    Vector3 s0_r0 = r.GetOrigine() - s0;
    float b = 2.0 * r.GetDirection().dot(s0_r0);
    float c = s0_r0.dot(s0_r0) - (sr * sr);
    if (b * b - 4.0 * a * c >= 0.0)
    {
        *distance = (-b - sqrt((b * b) - 4.0 * a * c)) / (2.0 * a);
        if(*distance >= 0)
            intersect = true;
    }
    return intersect;
}

bool intersectSpheres(Rayon r, vector<Sphere> spheres, float* distance)
{
    bool intersect = false;
    for(int i = 0;i< spheres.size();i++)
    {
        float minDistance;
        bool inteSphere = raySphereIntersect(r, spheres[i], &minDistance);
        if (inteSphere)
        {
            if (!intersect)
            {
                *distance = minDistance;
            }
            else
            {
                if (minDistance < *distance)
                    *distance = minDistance;
            }
            intersect = true;
        }
    }
    return intersect;
}
int main(int argc, char* argv[]) {
        const char* filename = argc > 1 ? argv[1] : "test.png";

        unsigned width = 512, height = 512;
         
        vector<unsigned char> image;
        image.resize(width * height * 4);

        for(int i=0;i< width * height * 4;i++)
            image[i] = 256;

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
                float minDistance;

                Rayon rayon = Rayon(Vector3(0, 0, 1), Vector3(plan.x + x, plan.y + y, plan.z));
                bool inter = intersectSpheres(rayon, spheres, &minDistance);

                if (inter)
                {
                    vector<Lampe> lampes;
                    float distanceLampe;
                    
                    Lampe lampe1 = Lampe(Vector3(300, 220, 200), Vector3(x,y,minDistance-0.02),Couleur(0,0,255));
                    lampe1.intersection = intersectSpheres((Rayon)lampe1, spheres, &distanceLampe);
                    addLampe(&lampes, lampe1);

                    Lampe lampe2 = Lampe(Vector3(-300, 220, 200), Vector3(x, y, minDistance - 0.02), Couleur(0, 255, 0));
                    lampe2.intersection = intersectSpheres((Rayon)lampe2, spheres, &distanceLampe);
                    addLampe(&lampes, lampe2);

                    bool noIntersection = true;
                    for (Lampe lampe : lampes)
                    {
                        if (!lampe.intersection)
                        {
                            color(&image, 4 * width * y + 4 * x, lampe.GetCouleur(), lampe.GetDistance());
                            noIntersection = false;
                        }
                    }
                    if (noIntersection)
                    {
                        color(&image, 4 * width * y + 4 * x, Couleur(25, 25, 25), 0);
                    }
                }
                else
                {
                    color(&image, 4 * width * y + 4 * x, Couleur(127,0,0),0);
                }
            }
        }

        encodeOneStep(filename, image, width, height);

}
