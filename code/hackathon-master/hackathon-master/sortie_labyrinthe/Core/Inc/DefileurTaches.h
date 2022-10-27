#ifndef DEFILEURTACHES_H
#define DEFILEURTACHES_H

#include "Reglages.h"
#include "FileTaches.h"
#include "ControleurPID.h"
#include "sonar.h"


class DefileurTaches
{
    public:

        DefileurTaches(FileTaches *file_taches,  ControleurPID *controleur, Sonar* sonar);

        bool update();
        void restart();
        void transition(Tache new_tache);
    private:
        FileTaches *_file_taches;
        Sonar* _sonar;
        bool sonarbool;


        ControleurPID *_controleur;


        Odometrie* _odometrie;
        Tache _current_tache;



};

#endif
