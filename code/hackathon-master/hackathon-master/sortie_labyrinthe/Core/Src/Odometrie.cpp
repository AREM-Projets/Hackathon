#include "Odometrie.h"


Odometrie::Odometrie(uint32_t min_update_period) : _min_update_period(min_update_period), etat(INIT_X, INIT_Y, INIT_THETA)
{
    encodeurs = new HardEncodeurs();

    _last_update_etat = 1000*Time;
}


Odometrie::~Odometrie()
{
    delete encodeurs;
}

void Odometrie::update()
{
    uint32_t date = 1000*Time;
    // Limitation de la fréquence d'update de l'odométrie (pas de limite si _min_update_period = 0)
    if(date - _last_update_etat > _min_update_period)
    {
        // (On suppose les vitesses constantes pendant t, la trajectoire decrite par le robot est alors un cercle)
        // on manipule des entiers le plus possible, c'est pourquoi l'unité est le micrometre

        uint32_t date = 1000*Time;

        // position et orientation actuelle du robot
        double x = etat.getX();
        double y = etat.getY();
        double theta = etat.getThetaRadian();

        // nouvelle position et orientation du robot
        double n_x = 0;
        double n_y = 0;
        double n_theta = 0;

        // recuperation de la distance parcourue par chaque roue encodeur
        int64_t deltaTicksG = encodeurs->getDeltaTicksG();
        int64_t deltaTicksD = encodeurs->getDeltaTicksD();

        double deltaRoueG = deltaTicksG * DISTANCE_PAR_TICK_G; // en mm
        double deltaRoueD = deltaTicksD * DISTANCE_PAR_TICK_D; // en mm

        //////////////////////////////////////////////////////////////////////////////////////////
        // calcul de la nouvelle position en ligne droite ou en parcourant un arc ////////////////

        if(deltaRoueG == deltaRoueD)
        {
            // dans le cas très particulier où les deux encodeurs ont retourné le même déplacement 
            // (ligne droite parfaite, R serait infini)
            n_x = x + deltaRoueG * cos(theta);
            n_y = y + deltaRoueD * sin(theta);

            n_theta = theta;
        }
        else // determination du cercle décrit par la trajectoire et de la vitesse du robot sur ce cercle
        {
            double R = 0;  // rayon du cercle decrit par la trajectoire
            double delta_theta = 0; // angle parcouru

            R = (ECART_ROUES / 2.0) * ((deltaRoueD + deltaRoueG) / (deltaRoueD - deltaRoueG)); // rayon de l'arc parcouru

            if(deltaTicksD == -deltaTicksG)
                delta_theta = deltaRoueD / (ECART_ROUES / 2.0);
            else
                delta_theta = (deltaRoueD - deltaRoueG) / (ECART_ROUES);//(deltaRoueD + deltaRoueG) / (2.0 * R);

            ////////////////////////////////////////////////////////////////////////////////////////////
            // Pour éviter les changements rapides et grands incoherents avec les encodeurs magnetiques
            if(abs(delta_theta) > 0.1)
                delta_theta = 0.0;
            ////////////////////////////////////////////////////////////////////////////////////////////

            n_x = etat.getX() + R * (- sin(theta) + sin(theta + delta_theta));
            n_y = etat.getY() + R * (cos(theta) - cos(theta + delta_theta));

            n_theta = theta + delta_theta;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        // determination et mise à jour des vitesses indépendantes des moteurs (vitesses en mm/s)
        etat.setVMG(deltaRoueG / ((double)(date - _last_update_etat) / 1000.0 / 1000.0));
        etat.setVMD(deltaRoueD / ((double)(date - _last_update_etat) / 1000.0 / 1000.0));

        // application de la nouvelle position calculée et mise à jour des vitesses
        etat.versPosition(n_x, n_y, n_theta, date - _last_update_etat);
        _last_update_etat = date;
    
    }
}

double Odometrie::getThetaRadian() const
{
    return etat.getThetaRadian();
}

double Odometrie::getX() const
{
    return etat.getX();
}

double Odometrie::getY() const
{
    return etat.getY();
}

double Odometrie::getV() const
{
    return etat.getV();
}

double Odometrie::getVTheta() const
{
    return etat.getVTheta();
}

double Odometrie::getVMG() const
{
    return etat.getVMG();
}

double Odometrie::getVMD() const
{
    return etat.getVMD();
}

double Odometrie::deltaAngleTo(double initial, double target) const
{
    return etat.deltaAngleTo(initial, target);
}



void Odometrie::setX(double x)
{
    etat.translatePosition(x, etat.getY(), etat.getThetaRadian());
}

void Odometrie::setY(double y)
{
    etat.translatePosition(etat.getX(), y, etat.getThetaRadian());
}

void Odometrie::setTheta(double theta)
{
    etat.translatePosition(etat.getX(), etat.getY(), theta);
}
