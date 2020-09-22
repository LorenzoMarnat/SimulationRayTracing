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

void color(vector<unsigned char>* img, int index, Couleur couleur)
{
    img->at(index) += couleur.red;
    if (img->at(index) > 255)
        img->at(index) = 255;

    img->at(index+1) += couleur.green;
    if (img->at(index + 1) > 255)
        img->at(index+1) = 255;

    img->at(index+2) += couleur.blue;
    if (img->at(index + 2) > 255)
        img->at(index+2) = 255;

    img->at(index + 3) = couleur.alpha;
}

Couleur colorOnSurface(Lampe lampe, Sphere sphere)
{
    float distanceCarre = sqrt(lampe.GetDistance());

    Vector3 normaleSurface = lampe.GetOrigine() - sphere.GetCentre();
    normaleSurface = normaleSurface.normalize();

    double scalaire = abs(normaleSurface.dot(lampe.GetDirection()));

    float intensity = (lampe.intersection * scalaire * lampe.intensity) / (2 * acos(0.0) * distanceCarre);

    return Couleur(sphere.albedo.red * intensity, sphere.albedo.green * intensity, sphere.albedo.blue * intensity);
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
    double b = 2.0 * r.GetDirection().dot(s0_r0);
    float c = s0_r0.dot(s0_r0) - (sr * sr);
    if (b * b - 4.0 * a * c >= 0.0)
    {
        *distance = (-b - sqrt((b * b) - 4.0 * a * c)) / (2.0 * a);
        if(*distance >= 0)
            intersect = true;
    }
    return intersect;
}

int intersectSpheres(Rayon r, vector<Sphere> spheres, float* distance)
{
    int intersect = -1;
    for(int i = 0;i< spheres.size();i++)
    {
        float minDistance;
        bool inteSphere = raySphereIntersect(r, spheres[i], &minDistance);
        if (inteSphere)
        {
            if (intersect == -1)
            {
                intersect = i;
                *distance = minDistance;
            }
            else
            {
                if (minDistance < *distance)
                {
                    intersect = i;
                    *distance = minDistance;
                }
            }
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
            image[i] = 0;

        Vector3 plan = Vector3(0, 0, 0);

        vector<Sphere> spheres;

        Sphere sphere = Sphere(100, Vector3(200, 100, 200),Couleur(1,0,0));
        addSphere(&spheres, sphere);

        Sphere sphere2 = Sphere(60, Vector3(200, 300, 200),Couleur(0,0,1));
        addSphere(&spheres, sphere2);

        vector<Lampe> lampes;

        Lampe lampe1 = Lampe(Vector3(300, 220, 200), 100);
        addLampe(&lampes, lampe1);

        Lampe lampe2 = Lampe(Vector3(-300, 220, 200), 200);
        addLampe(&lampes, lampe2);

        Lampe lampe3 = Lampe(Vector3(200, -100, 200), 200);
        addLampe(&lampes, lampe3);

        for (unsigned y = 0; y < width; y++)
        {
            for (unsigned x = 0; x < width; x++) 
            {
                float minDistance;

                Rayon rayon = Rayon(Vector3(0, 0, 1), Vector3(plan.x + x, plan.y + y, plan.z));
                int sphereIntersect = intersectSpheres(rayon, spheres, &minDistance);

                if (sphereIntersect != -1)
                {
                    Vector3 pointIntersection = Vector3(x, y, minDistance - 0.02);

                    bool noIntersection = true;
                    for (Lampe lampe : lampes)
                    {
                        lampe.SetOrigine(pointIntersection);

                        float distanceLampe;
                        int idSphere = intersectSpheres((Rayon)lampe, spheres, &distanceLampe);

                        if (idSphere == -1)
                        {
                            lampe.intersection = 1;

                            Couleur surface = colorOnSurface(lampe, spheres[sphereIntersect]);
                            color(&image, 4 * width * y + 4 * x, surface);

                            noIntersection = false;

                        }
                    }
                    if (noIntersection)
                    {
                        color(&image, 4 * width * y + 4 * x, Couleur(0, 0, 0));
                    }
                }
                else
                {
                    color(&image, 4 * width * y + 4 * x, Couleur());
                }
            }
        }
        encodeOneStep(filename, image, width, height);

}
