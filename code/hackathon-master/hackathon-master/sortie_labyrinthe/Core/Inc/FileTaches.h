#ifndef FILETACHES_H
#define FILETACHES_H

#include "Trajet.h"
#include "Reglages.h"
#include <stdlib.h>
#include "sonar.h"

using namespace std;

#define MAX_TACHES 190

enum TacheType
{
    LIGNE,
    ROTATION,
    WAIT,
	SONAR,
    VIDE,
	CONDITIONNAL
};



enum CalibrationType
{
    X, MINUS_X,
    Y, MINUS_Y
};


struct Tache
{
    TacheType type;
    
    double x;
    double y;
    double theta;
    int timer; // Pour une tache de type WAIT



};

class FileTaches
{
    public:
        FileTaches();
        void addLigne(double x, double y);
        void addRotation(double theta);
        void addWait(int timer);
        void addTache(Tache tache);
        void sonarscan();
        Tache popTache();
        void clear();
        void restart();
        bool isDone();
    private:
        Tache taches[MAX_TACHES];

        int curseur_taches;
        int nb_taches;
};

#endif
