#ifndef CONTROLEURPID
#define CONTROLEURPID


#include "Reglages.h"
#include "Odometrie.h"
#include "EtatDynamiqueRobot.h"
#include "Trajet.h"
#include "BlocMoteurs.h"
#include "HardEncodeurs.h"
#include "main.h"


extern volatile unsigned int Time;




struct ErreurPolaire
{
    double dist;
    double cap;
    double terr;
};

class ControleurPID
{
    public:
		ControleurPID(BlocMoteurs *p_moteurs, Odometrie* p_odometrie);
        ~ControleurPID();
        void setObjectifLigne(double x, double y);
        void setObjectifRotation(double theta);
        void setObjectif(Objectif target);
        bool update();

        // Pas le plus propre du monde mais pas encore d'autre solution
        BlocMoteurs* getBlocMoteurs();
        Odometrie* getOdometrie();
        void commande_vitesses(double consigneG, double consigneD);
        void stop_moteurs();


    private:
        BlocMoteurs *moteurs;
        Odometrie *odometrie;

        double Pdist;
        double Idist;
        double Ddist;

        double Pcap;
        double Icap;
        double Dcap;

        double Pangle;
		double Iangle;
		double Dangle;

        ErreurPolaire erreur_polaire;
        ErreurPolaire last_erreur_polaire;
        ErreurPolaire sum_erreur_polaire;
        void _processErreursPolaires();

        double consigneG;
        double consigneD;
        void _processPID();
        void _processPIDRotation();

        void _processProfileVitesse();

        void _normalizeConsignes();

        void _applyConsignes();

        uint32_t _min_update_period;
        bool objectif_atteint;
        bool objectif_is_rotation;
        bool PauseLineForRotation;
        EtatDynamiqueRobot objectif;
        EtatDynamiqueRobot start;



        uint32_t _date_start;
        uint32_t _OscCounter;

        uint32_t _last_update_consigne; // il s'agit de la date de la dernière mise à jour de la consigne aux moteurs
        double lastx;
        double lasty;


};

#endif
