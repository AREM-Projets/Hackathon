#include "Trajet.h"

Trajet::Trajet(double max_dist)
{
    
    curseur = 0;
    _max_dist = max_dist; // en mm
}

bool Trajet::estVide() const
{
    return objectifs.size() == 0 || curseur == objectifs.size();
}

void Trajet::_addObjectif(double x, double y, double theta, bool rotation_only)
{
    Objectif n_objectif = {x, y, theta, rotation_only};
    objectifs.push_back(n_objectif);
}

void Trajet::addLigne(double x, double y)
{
    if(false == true)//_max_dist != NULL)
    {
        // Si une distance de déplacement max a été définie, le trajet peut-être découpé
        Objectif objPrec = objectifs.at(curseur);
        double dist = sqrt( pow(x - objPrec.x, 2.0) + pow(y - objPrec.y, 2.0) );

        // Tant que la distance max est dépassée par l'objectif par rapport au précédent
        double dx = x - objPrec.x;
        double dy = y - objPrec.y;
        double target_x = x;
        double target_y = y;

        while(dist > _max_dist)
        {
            x = objPrec.x + dx * _max_dist / dist;
            y = objPrec.y + dy * _max_dist / dist;
            _addObjectif(x, y, 0, false);

            dist = sqrt( pow(target_x - x, 2.0) + pow(target_y - y, 2.0) );
        }

        _addObjectif(target_x, target_y, 0, false);
    }
    else
    {
        _addObjectif(x, y, 0, false);
    }
}

void Trajet::addRotation(double theta)
{
    _addObjectif(0, 0, theta, true);
}

Objectif Trajet::getObjectif() const
{
    if(objectifs.size() > 0 && curseur < objectifs.size())
    {
        return objectifs.at(curseur);
    }
    else
    {
        Objectif resultat_vide = {0, 0, 0, true};
        return resultat_vide;
    }
}

Objectif Trajet::popObjectif()
{
    Objectif obj = getObjectif();
    curseur = min((curseur + 1), (int) objectifs.size());
    return obj;
}

void Trajet::restart()
{
    curseur = 0;
}

void Trajet::clear()
{
    curseur = 0;
    objectifs.clear();
}
