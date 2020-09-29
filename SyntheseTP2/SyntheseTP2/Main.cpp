#include "lodepng.h"
#include "Mirroir.h"
#include "SphereCouleur.h"
#include "Lampe.h"
#include "Camera.h"
#include <math.h>
#include <iostream>
#include <random>
#include <ctime>

#define PI 3.141593

using namespace std;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void normalizeColor(vector<unsigned char>* imageOut, vector<double>* image)
{
    double max = 0;
    for (int i = 0; i < (int)image->size(); i += 4)
    {
        if (image->at(i) > max)
            max = image->at(i);

        if (image->at(i+1) > max)
            max = image->at(i+1);

        if (image->at(i+2) > max)
            max = image->at(i+2);
    }
    
    max = max / 4;
    for (int i = 0; i < (int)image->size(); i += 4)
    {
        if ((int)((image->at(i) / max) * 255) > 255)
            imageOut->at(i) = 255;
        else
            imageOut->at(i) = (int)((image->at(i) / max) * 255);

        if ((int)((image->at(i+1) / max) * 255) > 255)
            imageOut->at(i+1) = 255;
        else
            imageOut->at(i+1) = (int)((image->at(i+1) / max) * 255);

        if ((int)((image->at(i+2) / max) * 255) > 255)
            imageOut->at(i+2) = 255;
        else
            imageOut->at(i+2) = (int)((image->at(i+2) / max) * 255);

         imageOut->at(i+3) = image->at(i+3);
    }

}
void color(vector<double>* img, int index, Couleur couleur)
{
    img->at(index) += couleur.red;

    img->at(index + 1) += couleur.green;

    img->at(index + 2) += couleur.blue;

    img->at(index + 3) = couleur.alpha;
}

Couleur colorOnSurface(Lampe lampe, Sphere* sphere)
{
    float distanceCarre = (lampe.GetDistance())* (lampe.GetDistance());

    Vector3 normaleSurface = lampe.GetOrigin() - sphere->GetCentre();
    normaleSurface = normaleSurface.normalize();

    double scalaire = abs(normaleSurface.dot(lampe.GetDirection()));

    double intensity = (scalaire * lampe.intensity) / (PI * distanceCarre);

    return Couleur(sphere->albedo.red * intensity, sphere->albedo.green * intensity, sphere->albedo.blue * intensity);
}

void addSphere(vector<Sphere*>* spheres, Sphere* sphere)
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

bool raySphereIntersect(Rayon r, Sphere* s, float *distance) {
    bool intersect = false;

    Vector3 s0 = s->GetCentre();
    float sr = s->GetRayon();
    float a = r.GetDirection().dot(r.GetDirection());
    Vector3 s0_r0 = r.GetOrigin() - s0;
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

int intersectSpheres(Rayon r, vector<Sphere*> spheres, float* distance)
{
    int intersect = -1;
    for(int i = 0;i< (int)spheres.size();i++)
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

void mirrorRebound(Vector3 intersection, Rayon rayon, vector<Lampe> lamps, vector<Sphere*> spheres, int intersectSphere, vector<double>* image, int index)
{
    float minDistance;

    Vector3 normaleRayon = rayon.GetDirection();

    Vector3 normaleSurface = intersection - spheres[intersectSphere]->GetCentre();
    normaleSurface = normaleSurface.normalize();

    Vector3 directionRayon = (-normaleRayon.dot(normaleSurface)) * normaleSurface * 2 + normaleRayon;

    Rayon rayonSortant = Rayon(directionRayon, intersection);

    rayonSortant.rebound = ++rayon.rebound;

    int sphereIntersect = intersectSpheres(rayonSortant, spheres, &minDistance);
    if (sphereIntersect != -1)
    {
        Vector3 newIntersection = Vector3(minDistance * normaleRayon.x + intersection.x, minDistance * normaleRayon.y + intersection.y, (float)(minDistance * normaleRayon.z + intersection.z - 0.02));

        if (spheres[sphereIntersect]->IsMirror() && rayonSortant.rebound < Rayon::maxRebound)
        {
            mirrorRebound(newIntersection, rayonSortant, lamps, spheres, sphereIntersect, image, index);
        }
        else
        {
            intersectLamps(newIntersection,lamps,spheres,sphereIntersect,image,index);

            /*Lampe lampe = Lampe(intersection, newIntersection, 30000000);
            Couleur surface = colorOnSurface(lampe, spheres[sphereIntersect]);

            color(image, index, surface);*/
        }
    }
    else
    {
        color(image, index, Couleur(100, 100, 100));
    }
}

void intersectLamps(Vector3 intersection,vector<Lampe> lamps,vector<Sphere*> spheres, int intersectSphere,vector<double> *image, int index)
{
    Couleur surface = Couleur(0,0,0);

    int nbIntersection = 0;

    for (Lampe lampe : lamps)
    {
        lampe.SetOrigin(intersection);

        float distanceLampe;
        int idSphere = intersectSpheres(lampe, spheres, &distanceLampe);
        if (idSphere == -1 || distanceLampe > lampe.GetDistance())
        {
            surface = colorOnSurface(lampe, spheres[intersectSphere]);
            color(image, index, surface);

            nbIntersection++;
        }
    }
    if (nbIntersection == 0)
    {
        color(image, index, Couleur(0, 0, 0));
    }
}

float randomFloat(float min, float max) {

    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void checkNormale(Vector3* n)
{
    if (n->x > 1)
        n->x = 1;
    if (n->x < -1)
        n->x = -1;

    if (n->y > 1)
        n->y = 1;
    if (n->y < -1)
        n->y = -1;

    if (n->z > 1)
        n->z = 1;
    if (n->z < -1)
        n->z = -1;
}

int main(int argc, char* argv[]) {
        const char* filename = argc > 1 ? argv[1] : "test.png";

        int width = 1024, height = 1024;
         
        vector<unsigned char> imageOut;
        imageOut.resize(width * height * 4);

        vector<double> image;
        image.resize(width * height * 4);

        for (int i = 0; i < width * height * 4; i++)
            image[i] = 0;

        srand(time(NULL));

        Camera camera = Camera((float)width, (float)height, 1024, Vector3(0, 0, 0));

        vector<Sphere*> spheres;

        SphereCouleur rouge = SphereCouleur(200, Vector3(200, 0, 350),Couleur(1,0,0));

        SphereCouleur bleu = SphereCouleur(200, Vector3(200, 450, 250),Couleur(0,0,1));

        SphereCouleur vert = SphereCouleur(100, Vector3(850, 200, 150), Couleur(0, 1, 0));

        SphereCouleur cyan = SphereCouleur(150, Vector3(800, 800, 200), Couleur(0, 1, 1));

        SphereCouleur jaune = SphereCouleur(100, Vector3(0, 800, 300), Couleur(1, 1, 0));

        SphereCouleur sol = SphereCouleur(100000, Vector3(512, 101000, 500), Couleur(1, 0, 1));

        SphereCouleur mur = SphereCouleur(100000, Vector3(512, 512, -100200), Couleur(1, 0, 1));

        SphereCouleur plafond = SphereCouleur(100000, Vector3(512, -100500, 500), Couleur(1, 0, 1));

        SphereCouleur murFond = SphereCouleur(100000, Vector3(512, 512, 101000), Couleur(1, 0, 1));

        SphereCouleur murDroite = SphereCouleur(100000, Vector3(-100500, 512, 512), Couleur(1, 0, 1));

        SphereCouleur murGauche = SphereCouleur(100000, Vector3(101000, 512, 512), Couleur(1, 0, 1));

        Mirroir mirroir = Mirroir(200, Vector3(600, 500, 500), Couleur(1, 1, 1));

        addSphere(&spheres, &rouge);
        addSphere(&spheres, &bleu);
        addSphere(&spheres, &vert);
        addSphere(&spheres, &cyan);
        addSphere(&spheres, &jaune);
        addSphere(&spheres, &sol);
        addSphere(&spheres, &murDroite);
        addSphere(&spheres, &murFond);
        addSphere(&spheres, &murGauche);
        addSphere(&spheres, &mur);
        addSphere(&spheres, &plafond);
        addSphere(&spheres, &mirroir);

        vector<Lampe> lampes;

        Lampe lampe1 = Lampe(Vector3(500, 500, 100), 90000000);

        Lampe lampe2 = Lampe(Vector3(-300, 400, 300), 90000000);

        Lampe lampe3 = Lampe(Vector3(650, -100, 200), 90000000);

        addLampe(&lampes, lampe1);
        addLampe(&lampes, lampe2);
        addLampe(&lampes, lampe3);

        for (int y = 0; y < width; y++)
        {
            for (int x = 0; x < width; x++) 
            {
                float minDistance;

                Vector3 point = Vector3(camera.plan.x + x, camera.plan.y + y, camera.plan.z);
                Vector3 normale = camera.Normale(point);

                /*for (int i = 0; i < 5; i++)
                {
                    Vector3 normaleRand = Vector3(normale.x + randomFloat(-0.003, 0.003), normale.y + randomFloat(-0.003, 0.003), normale.z + randomFloat(-0.003, 0.003));
                    checkNormale(&normaleRand);
                    Rayon rayon = Rayon(normaleRand, point);*/
                Rayon rayon = Rayon(normale, point);
                    int sphereIntersect = intersectSpheres(rayon, spheres, &minDistance);
                    if (sphereIntersect != -1)
                    {
                        Vector3 pointIntersection = Vector3(minDistance * normale.x + x + camera.plan.x, minDistance * normale.y + y + camera.plan.z, (float)(minDistance * normale.z + camera.plan.z - 0.02));
                        if (!spheres[sphereIntersect]->IsMirror())
                        {
                            intersectLamps(pointIntersection, lampes, spheres, sphereIntersect, &image, 4 * width * y + 4 * x);
                        }
                        else
                        {
                            mirrorRebound(pointIntersection, rayon,lampes, spheres, sphereIntersect, &image, 4 * width * y + 4 * x);
                        }
                    }
                    else
                    {
                        color(&image, 4 * width * y + 4 * x, Couleur(100, 100, 100));
                    }
                //}
            }
        }
        normalizeColor(&imageOut, &image);
        encodeOneStep(filename, imageOut, width, height);
}
