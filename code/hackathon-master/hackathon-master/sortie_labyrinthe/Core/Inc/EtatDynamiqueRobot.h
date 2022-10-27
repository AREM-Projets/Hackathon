#ifndef ETATDYNAMIQUEROBOT
#define ETATDYNAMIQUEROBOT



#define MEAN_SAMPLES 35
#include <stdlib.h>
#include "Reglages.h"
#include "main.h"



using namespace std;

extern volatile unsigned int Time;


class EtatDynamiqueRobot
{
    public:
        EtatDynamiqueRobot(double x, double y, double theta);

        // Pour passer vers un nouvel etat
        // Effectue le calcul des nouvelles vitesses
        void versPosition(double n_x, double n_y, double n_theta, int dt);
        void setPosition(double n_x, double n_y, double n_theta);
        void translatePosition(double n_x, double n_y, double n_theta);
        void setVMG(double vm);
        void setVMD(double vm);

        double getThetaRadian() const;
        double getX() const;
        double getY() const;
        double getV() const;
        double getVTheta() const;
        double getVMG() const;
        double getVMD() const;

        double deltaAngleTo(double initial, double target) const;

    private:
        double x;
        double y;
        double theta;
        
        double v_xy;
        double v_theta;

        double v_moteurG[MEAN_SAMPLES];
        int c_v_moteurG;
        double v_moteurD[MEAN_SAMPLES];
        int c_v_moteurD;

        int last_dt;
};

#endif
