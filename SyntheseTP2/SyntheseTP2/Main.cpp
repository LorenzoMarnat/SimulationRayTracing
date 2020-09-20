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
    img->at(index) = couleur.red - distance;
    img->at(index+1) = couleur.green - distance;
    img->at(index+2) = couleur.blue - distance;
    img->at(index+3) = couleur.alpha;
}

void addSphere(vector<Sphere>* spheres, Sphere sphere)
{
    int size = spheres->size();
    spheres->resize(size + 1);
    spheres->at(size) = sphere;
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
                    float minDist;
                    Lampe lampee = Lampe(Vector3(1000, 200, 200), Vector3(x,y,minDistance-0.02),Couleur(255,255,255));
                    //cout << "Direction :" << lampee.GetDirection().x << " " << lampee.GetDirection().y << " " << lampee.GetDirection().z << endl;
                    //cout << "Origine :" << lampee.GetOrigine().x << " " << lampee.GetOrigine().y << " " << lampee.GetOrigine().z << endl;
                    //cout << "Position :" << lampee.GetPosition().x << " " << lampee.GetPosition().y << " " << lampee.GetPosition().z << endl;
                    inter = intersectSpheres((Rayon)lampee, spheres, &minDist);

                    if (!inter)
                        color(&image, 4 * width * y + 4 * x, rayon.GetCouleur(),minDistance);
                    else
                        color(&image, 4 * width * y + 4 * x, Couleur(25,25,25),0);
                }
                else
                {
                    color(&image, 4 * width * y + 4 * x, Couleur(127,0,0),0);
                }
            }
        }

        encodeOneStep(filename, image, width, height);
}
