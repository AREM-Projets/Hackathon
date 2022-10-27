#include "FileTaches.h"

FileTaches::FileTaches()
{
    curseur_taches = 0;
    nb_taches = 0;
}

void FileTaches::addLigne(double x, double y)
{
    Tache tache;
    tache.type = TacheType::LIGNE;

    tache.x = x;
    tache.y = y;

    addTache(tache);
}

void FileTaches::addRotation(double theta)
{
    Tache tache;
    tache.type = TacheType::ROTATION;



    tache.theta = theta;

    addTache(tache);
}

void FileTaches::addWait(int timer)
{
    Tache tache;
    tache.type = TacheType::WAIT;
    tache.timer = timer;
    addTache(tache);
}

void FileTaches::sonarscan()
{
	Tache tache;
	tache.type = TacheType::SONAR;
	addTache(tache);
}


void FileTaches::addTache(Tache tache)
{
    if(nb_taches < MAX_TACHES) // Limité par la taille du tableau des taches
    {
        taches[nb_taches] = tache;
        nb_taches = min(nb_taches + 1, MAX_TACHES);
    }
}

Tache FileTaches::popTache()
{
    if(nb_taches > 0 && curseur_taches < nb_taches) // S'il y a au moins une tache
    {
        Tache tache = taches[curseur_taches];

        curseur_taches = min(curseur_taches + 1, nb_taches);

        return tache;
    }
    else
    {
        Tache tache_vide;
        tache_vide.type = TacheType::VIDE;
        return tache_vide;
    }
}

void FileTaches::clear()
{
    nb_taches = 0;
    curseur_taches = 0;
}

void FileTaches::restart()
{
    curseur_taches = 0;
}

bool FileTaches::isDone()
{
    return curseur_taches == nb_taches;
}
