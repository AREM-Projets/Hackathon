#ifndef TRAJET_H
#define TRAJET_H

#include "vector"
#include <math.h>

using namespace std;


struct Objectif
{
    double x;
    double y;
    double theta;
    bool rotation_only;
};

class Trajet
{
    public:
        Trajet(double max_dist = NULL);
        void addLigne(double x, double y);
        void addRotation(double theta);
        bool estVide() const;
        Objectif getObjectif() const;
        Objectif popObjectif();
        void restart();
        void clear();
    private:
        void _addObjectif(double x, double y, double theta, bool rotation_only);

        std::vector<Objectif> objectifs;
        int curseur;

        double _max_dist;
};

#endif
