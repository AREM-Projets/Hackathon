#ifndef ODOMETRIE_H
#define ODOMETRIE_H


#include "Reglages.h"
#include "EtatDynamiqueRobot.h"


#include "HardEncodeurs.h"
#include "main.h"

extern volatile unsigned int Time;

class Odometrie
{
    public:

        Odometrie(uint32_t min_update_period);
        ~Odometrie();
        void update();
        
        double getThetaRadian() const;
        double getX() const;
        double getY() const;
        double getV() const;
        double getVTheta() const;
        double getVMG() const;
        double getVMD() const;
        double deltaAngleTo(double initial, double target) const;

        void debug();
        
        void setX(double x);
        void setY(double y);
        void setTheta(double theta);

    private:


        HardEncodeurs *encodeurs;

        EtatDynamiqueRobot etat;

        uint32_t _min_update_period;
        uint32_t _last_update_etat; // il s'agit de la date de la dernière mise à jour de position/rotation/vitesse
};

#endif
