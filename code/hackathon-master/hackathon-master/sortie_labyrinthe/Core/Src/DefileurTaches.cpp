#include "DefileurTaches.h"



DefileurTaches::DefileurTaches(FileTaches *file_taches,  ControleurPID *controleur, Sonar* sonar)
{
    _file_taches = file_taches;
    _controleur = controleur;
    _sonar = sonar;
    sonarbool = false;




    _current_tache.type = TacheType::VIDE;
}


bool DefileurTaches::update()
{
    switch(_current_tache.type)
    {
        case(TacheType::WAIT):
            // update
            _current_tache.timer = max(_current_tache.timer - 1, 0);
            // transition
            if(_current_tache.timer <= 0)
                transition(_file_taches->popTache());
            break;
        
        case(TacheType::LIGNE):
            if(_controleur->update()) // update
            {
                // transition
                transition(_file_taches->popTache());
            }
            break;
        case(TacheType::ROTATION):
            if(_controleur->update()) // update
            {
                // transition
                transition(_file_taches->popTache());
            }
            break;
        case(TacheType::SONAR):
        	_sonar->update();
        	sonarbool=true;
                // transition
            transition(_file_taches->popTache());

            break;
        default:
        	transition(_file_taches->popTache());
        	break;

    }

    return _file_taches->isDone() && sonarbool;
}

void DefileurTaches::transition(Tache new_tache)
{
    switch(new_tache.type)
    {
        case TacheType::WAIT:
            // rien à faire
            break;
        case TacheType::LIGNE:
            _controleur->setObjectifLigne(new_tache.x, new_tache.y);
            break;
        case TacheType::ROTATION:
            _controleur->setObjectifRotation(new_tache.theta);
            break;
        case TacheType::SONAR:
        	sonarbool=false;
        default :
            break;

    }

    _current_tache = new_tache;
}

void DefileurTaches::restart()
{
    _file_taches->restart();
}
