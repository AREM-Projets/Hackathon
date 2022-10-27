#include "ControleurPID.h"

extern float xglob;
extern float yglob;
extern float thetaglob;
extern float targetdistglob;
extern float targetangleglob;
extern float distglob;
extern float angleglob;
extern float CG;
extern float CD;
extern float CorAngle;
extern float CorDist;

ControleurPID::ControleurPID(BlocMoteurs *p_moteurs, Odometrie* p_odometrie) : objectif(0, 0, 0), start(INIT_X, INIT_Y, INIT_THETA)
{
    _min_update_period = 10000;

    // On a choisi de contenir ces éléments dans cette classe puisque le reste du code ne doit pas 
    // y avoir accès sans passer par le controleur
    moteurs = p_moteurs;

    odometrie = p_odometrie;




    _date_start = Time;

    // Initialisation des dates.
    // Il s'agit de quelque chose d'important pour que la première consigne ne soit
    // pas bizarre à cause d'une initialisation à 0, et donc d'un premier delta t absurde.
    uint32_t date = 1000*Time;
    _last_update_consigne = date;

    objectif_atteint = true;
    objectif_is_rotation = false;
    PauseLineForRotation = false;

    // Les coefficients de l'asservissement polaire
    Pdist = 0.0065;
    Idist = 0.000055;//4;
    Ddist = 0;



    Pcap = 15 ;
    Icap = 0.0;//2;//4;
    Dcap = 0;

    Pangle = 15 ;
    Iangle = 0.0;//2;//4;
    Dangle = 0;

    /*
    Pcap = 0.07;
    Icap = 0.0002;//2;//4;
    Dcap = 0.05;
    */

    sum_erreur_polaire.dist = 0.0;
    sum_erreur_polaire.cap = 0.0;
    sum_erreur_polaire.terr = 0.0;
    _OscCounter=0;
    lastx = odometrie->getX();
    lasty = odometrie->getY();

}

ControleurPID::~ControleurPID()
{
    // Ces objects sont détruits ici mais ils ne sont pas faits pour être recréés plusieurs fois.
    // Il doit donc a priori s'agir de la fin de tout le programme.
    // Dans le cas contraire le bon comportement des périphériques de la carte n'est pas garenti
    delete moteurs;
    delete odometrie;
}

/**
 * @brief A appeler le plus souvent possible. C'est la fonction elle-même qui tient compte de l'horloge système pour mettre à jour
 *          ou non l'odométrie et les consignes asservies des moteurs.
 */
bool ControleurPID::update()
{
    uint32_t date = 1000*Time;

    // l'actualisation de la position doit être effectué le plus souvent possible
    odometrie->update();

    // l'actualisation des vitesses moteurs se font à intervalles de temps réguliers
    // (actualiser le plus souvent possible n'est pas nécessaire, voir pas forcément désirable)
    if(date - _last_update_consigne >= DELTA_T_ASSER_us)
    {
        // Détermination des erreurs par rapport à l'objectif
        _processErreursPolaires();

        // Application des coefficients pour obtenir les consignes de vitesse
        // + Fin de l'objectif si suffisament proche
        if(objectif_is_rotation)
            _processPIDRotation();
        else
            _processPID();

        //_processProfileVitesse();
        
        //QuickDebug *qe = (QuickDebug *) e.currentPage;
        //qe->printe( (int)(etat.getX() / 10.0), 1 );
        //qe->printe( (int)(etat.getY() / 10.0), 2 );
        //qe->printe( (int)(etat.getThetaRadian() * 180.0 / PI), 3 );

        /*Serial.print("x:");
        Serial.println(odometrie->getX());*/

        // Envoie des vitesses aux moteurs
        // + Arret des moteurs si objectif atteint
        _applyConsignes();

        _last_update_consigne = date;
    }



    if(objectif_atteint)
        return true;
    else
        return false;
}

/**
 * @brief Met à jour l'odométrie du robot (sa position, la vitesse des moteurs et son orientation).Doit être appelé souvent car on fait l'hypothèse d'une 
 *          évolution linéaire des vitesses depuis le dernier appel.
 */


/**
 * @brief Détermine les erreurs polaires entre la position et l'orientation du robot et l'objectif fixé. Si l'objectif est derrière le robot (+/- 90°), alors l'erreur de distance 
 *          devient négative (comme si on échangeait l'avant et l'arrière du robot).
 */
void ControleurPID::_processErreursPolaires()
{
    // La position et l'orientation actuelle du robot
    double x = odometrie->getX();
    double y = odometrie->getY();
    double theta = odometrie->getThetaRadian();
    xglob=x;
    yglob=y;
    thetaglob=theta;



    // La position et l'orientation de l'objectif
    double c_x = objectif.getX();
    double c_y = objectif.getY();
    double c_theta = objectif.getThetaRadian();
    targetangleglob=c_theta;

    // dist en mm
    // cap en radians
    // terr en radians
    double dist = sqrt(pow(c_x - x, 2) + pow(c_y - y, 2));


    if(dist > 300)
    {
        dist = 300;
    }
    if(dist > 100)
    {
    	sum_erreur_polaire.dist = 0;
    }
    /*
    if(dist < 40)
    {
    	Pcap=1;
    }
    if(dist < 300 && dist > 200)
    {
    	Pcap=60;
    }
    if(dist <200 && dist > 130)
    {
    	Pcap=45;
    }
    if(dist < 130 && dist > 80)
    {
    	Pcap=30;
    }
    if( dist < 80 && dist > 40)
    {
    	Pcap = 10;
    }
    */


    distglob=dist;


    double terr = -odometrie->deltaAngleTo(theta, c_theta);
    double cap = -odometrie->deltaAngleTo(theta, atan2(c_y - y, c_x - x));

    if(objectif_is_rotation)
    {
        if(PauseLineForRotation)
        {
            if(abs(cap) < ANGLE_MIN_ROTATION_SEULE) // ecriture pas opti mais plus lisible
                PauseLineForRotation = false;
        }
        else
        {
            if(abs(cap) > ANGLE_MAX_ROTATION_SEULE)
                PauseLineForRotation = true;
        }
    }

    // Pour éviter que le robot cherche à se retourner lorsqu'une marche arrière est plus adaptée
    if(cap < - PI/2.0)
    {
        cap += PI;
        dist *= -1.0;
    }
    else if(PI/2.0 < cap)
    {
        cap -= PI;
        dist *= -1.0;
    }

    // Sauvegarde des erreurs calculées
    erreur_polaire.dist = dist;
    erreur_polaire.cap = cap;
    erreur_polaire.terr = terr;
}

/**
 * @brief [Pour l'instant non fonctionnelle et non utilisée] Modifie les consignes calculées pour les moteurs afin de limiter l'accélération max du robot et pour
 *          faire arriver les deux moteurs à leur vitesse de consigne en même temps.
 */
void ControleurPID::_processProfileVitesse()
{
    uint32_t date = Time - _date_start;

    if(date <= 5000.0)
    {
        consigneG = 0.0;
        consigneD = 0.0;
    }
    else if(date > 5000.0 && date <= 6000.0)
    {
        consigneG = (date - 5000.0) * ACCEL_MAX/100.0;
        consigneD = (date - 5000.0) * ACCEL_MAX/100.0;
    }
    else if(date > 6000.0 && date <= 10000.0)
    {
        consigneG = 1000.0 * ACCEL_MAX/100.0;
        consigneD = 1000.0 * ACCEL_MAX/100.0;
    }
    else if(date > 10000.0 && date <= 11000.0)
    {
        consigneG = 1000.0 * ACCEL_MAX/100.0 - (date - 10000.0) * ACCEL_MAX/100.0;
        consigneD = 1000.0 * ACCEL_MAX/100.0 - (date - 10000.0) * ACCEL_MAX/100.0;
    }
    else
    {
        consigneG = 0.0;
        consigneD = 0.0;
    }
    


    double consigne_originaleG = consigneG;
    double consigne_originaleD = consigneD;

    // la vitesse actuelle réelle des roues en mm/s
    double mesureG = odometrie->getVMG();
    double mesureD = odometrie->getVMD();

    // le signe des consignes
    int sensG = consigneG >= 0.0 ? 1.0 : -1.0;
    int sensD = consigneD >= 0.0 ? 1.0 : -1.0;

    if(consigneG * mesureG < 0.0) // si le moteur gauche doit passer par la vitesse nulle
    {
        consigneG = 0.0;
    }
    if(consigneD * mesureD < 0.0) // si le moteur droit doit passer par la vitesse nulle
    {
        consigneD = 0.0;
    }

    // l'information du signe a été sauvegardée dans sensG et sensD, on utilise désormais les valeurs absolues
    consigneG = abs(consigneG);
    consigneD = abs(consigneD);
    mesureG = abs(mesureG);
    mesureD = abs(mesureD);

    bool accelerationG = (consigneG - mesureG) >= 0;
    bool accelerationD = (consigneD - mesureD) >= 0;
    
    double time_to_consigne, time_to_consigneG, time_to_consigneD;

    // En fonction de si on a besoin d'accélérer ou de ralentir
    time_to_consigneG = (consigneG - mesureG) / ACCEL_MAX;
    time_to_consigneD = (consigneD - mesureD) / ACCEL_MAX;

    time_to_consigne = max(time_to_consigneG, time_to_consigneD);

    double accelG, accelD; // en mm/s

    if(time_to_consigneG > time_to_consigneD) // le moteur gauche limite l'accélération du droit
    {
        if(accelerationG)
            accelG = ACCEL_MAX;
        else
            accelG = - ACCEL_MAX;
        
        if(accelerationD)
            accelD = abs(consigneD - mesureD) / time_to_consigne;
        else
            accelD = - abs(consigneD - mesureD) / time_to_consigne;

    }
    else // le moteur droit limite l'accélération du gauche
    {
        if(accelerationG)
            accelG = abs(consigneG - mesureG) / time_to_consigne;
        else
            accelG = - abs(consigneG - mesureG) / time_to_consigne;
        
        if(accelerationD)
            accelD = ACCEL_MAX;
        else
            accelD = - ACCEL_MAX;
    }

    // JUSTE POUR TEST
    if(accelerationG)
            accelG = ACCEL_MAX;
        else
            accelG = - ACCEL_MAX;

    if(accelerationD)
            accelD = ACCEL_MAX;
        else
            accelD = - ACCEL_MAX;

    // finalement, on peut déterminer les vraies consignes qui respecte les limites d'accélération et décélération max
    consigneG = (mesureG + accelG * DELTA_T_ASSER_us / 1000.0 / 1000.0) * sensG;
    consigneD = (mesureD + accelD * DELTA_T_ASSER_us / 1000.0 / 1000.0) * sensD;

    /*if(sensG == 1.0)
        if(accelerationG)
            consigneG = min(consigne_originaleG, consigneG);
        else
            consigneG = max(consigne_originaleG, consigneG);
    else
        if(accelerationG)
            consigneG = max(consigne_originaleG, consigneG);
        else
            consigneG = min(consigne_originaleG, consigneG);

    if(sensD == 1.0)
        if(accelerationD)
            consigneD = min(consigne_originaleD, consigneD);
        else
            consigneD = max(consigne_originaleD, consigneD);
    else
        if(accelerationD)
            consigneD = max(consigne_originaleD, consigneD);
        else
            consigneD = min(consigne_originaleD, consigneD);*/

    /*Serial.print("CG:");
    Serial.print(consigneG);
    Serial.print(",CD:");
    Serial.print(consigneD);*/

    //Serial.print(",MG:");
    //Serial.print(mesureG);
    //Serial.print(",MD:");
    //Serial.print(mesureD);

    /*Serial.print(",RG:");
    Serial.print(consigne_originaleG);
    Serial.print(",RD:");
    Serial.println(consigne_originaleD);*/

    consigneG /= MAX_VITESSE;
    consigneD /= MAX_VITESSE;
}

/**
 * @brief Ajoute un nouvel objectif qui écrase l'ancien. La méthode _processPID sera appelé pour corriger la distance et l'orientation vers l'objectif.
 */
void ControleurPID::setObjectifLigne(double x, double y)
{
    // Il ne s'agit pas d'une rotation seule
    objectif_is_rotation = false;

    // Mise à jour de la nouvelle position de départ et du nouvel objectif
    objectif.setPosition(x, y, odometrie->getThetaRadian());
    start.setPosition(odometrie->getX(), odometrie->getY(), odometrie->getThetaRadian());
    
    targetdistglob = x;


    objectif_atteint = false;

    // Remise à zéro des valeurs qui dépendent des coups précédents de l'asser
    last_erreur_polaire.dist = 0;
    last_erreur_polaire.cap = 0;
    last_erreur_polaire.terr = 0;

    sum_erreur_polaire.dist = 0;
    sum_erreur_polaire.cap = 0;
    sum_erreur_polaire.terr = 0;

    _OscCounter=0;
}

/**
 * @brief Ajoute un nouvel objectif qui écrase l'ancien. La méthode _processPIDRotation sera appelé pour corriger la différence avec l'angle visé.
 */
void ControleurPID::setObjectifRotation(double theta)
{
    // Il s'agit d'une rotation seule
    objectif_is_rotation = true;

    // Mise à jour de la nouvelle position de départ et du nouvel objectif
    objectif.setPosition(odometrie->getX(), odometrie->getY(), theta);
    start.setPosition(odometrie->getX(), odometrie->getY(), odometrie->getThetaRadian());



    objectif_atteint = false;

    // Remise à zéro des valeurs qui dépendent des coups précédents de l'asser
    last_erreur_polaire.dist = 0;
    last_erreur_polaire.cap = 0;
    last_erreur_polaire.terr = 0;

    sum_erreur_polaire.dist = 0;
    sum_erreur_polaire.cap = 0;
    sum_erreur_polaire.terr = 0;

    PauseLineForRotation=0;

    _OscCounter=0;
}

void ControleurPID::setObjectif(Objectif target)
{
    // Il s'agit d'une rotation seule
    objectif_is_rotation = target.rotation_only;

    // Mise à jour du nouvel objectif
    if(objectif_is_rotation)
        objectif.setPosition(odometrie->getX(), odometrie->getY(), target.theta);
    else
        objectif.setPosition(target.x, target.y, odometrie->getThetaRadian());

    // Mise à jour de la nouvelle position de départ de l'objectif
    start.setPosition(odometrie->getX(), odometrie->getY(), odometrie->getThetaRadian());

    objectif_atteint = false;

    // Remise à zéro des valeurs qui dépendent des coups précédents de l'asser
    last_erreur_polaire.dist = 0;
    last_erreur_polaire.cap = 0;
    last_erreur_polaire.terr = 0;

    sum_erreur_polaire.dist = 0;
    sum_erreur_polaire.cap = 0;
    sum_erreur_polaire.terr = 0;

    PauseLineForRotation=0;
}

/**
 * @brief Doit être appelé après la mise à jour de l'erreur polaire. Applique la correction P(ID) et en tire une consigne pour chaque moteur. Se charge également de vérifier les condition de réussite de l'objectif défini.
 */
void ControleurPID::_processPID()
{
    double target_dist = 0;
    double target_cap = 0;

    double dist = erreur_polaire.dist;
    distglob=dist;
    
    double cap = erreur_polaire.cap;
    angleglob=cap;



    sum_erreur_polaire.dist += dist;
    sum_erreur_polaire.cap += cap;

    /////////////////////////////////////////////////////////////
    // Application des coefficients
    target_dist = Pdist * dist 
                + Idist * sum_erreur_polaire.dist
                - Ddist * odometrie->getV();
    
    target_cap = Pcap * cap 
                + Icap * sum_erreur_polaire.cap
                - Dcap * odometrie->getVTheta();

    /*
    if(abs(cap) > PI/20)
    {
        target_dist=0;
        PauseLineForRotation=1;
    

        
        if(printing)
        {
            Serial.print("AACCHHHHTUUUNNG");
        }
    }
    else
    {
        PauseLineForRotation=0;
    }
    */

    

    /////////////////////////////////////////////////////////////
    // Si le robot doit trop tourner pour être en face de son 
    // objectif, il tourne sur place jusqu'à un seuil donné.



    /*
    Serial.print(", dist: ");
    Serial.print(dist);
    Serial.print(", cap : ");
    Serial.println(cap);
    */
   
    
    /*Serial.print("$");
    Serial.print(100*dist);
    Serial.println(";");*/
    



    // Si le robot se trouve suffisament proche de l'objectif, 
    // il considère l'objectif comme atteint.
    if(abs(dist) < ERROR_MIN_DIST && abs(cap)< ERROR_MIN_ANGLE)
    {
        objectif_atteint = true;

    }

    /*

    Serial.print("odometrie x: ");
    Serial.print(odometrie->getX());
    Serial.print(" y: ");
    Serial.println(odometrie->getY());

    */
    


    lastx = odometrie->getX();
    lasty = odometrie->getY();
    

    /////////////////////////////////////////////////////////////
    // Mise à jour des consignes de vitesse pour les moteurs
    consigneG = target_dist + target_cap;
    consigneD = target_dist - target_cap;

    CorDist=target_dist;
    CorAngle=target_cap;

    CG=consigneG;
    CD=consigneD;

    /*
    if(printing)
    {
        Serial.print(", G : ");
        Serial.print(consigneG);
        Serial.print(", D : ");
        Serial.println(consigneD);

    }
    */

    /*

    if(last_erreur_polaire.dist > 0)
    {
        if(dist < 0)
        {
            if(_OscCounter++>MAXOSC)
            {
                objectif_atteint=true;
                _OscCounter=0;
            }
        }
    }
    else
    {
        if(dist > 0)
        {
            if(_OscCounter++>MAXOSC)
            {
                objectif_atteint=true;
                _OscCounter=0;
            }

        }
    }

    if(last_erreur_polaire.cap > 0)
    {
        if(cap < 0)
        {
            if(_OscCounter++>MAXOSC)
            {
                objectif_atteint=true;
                _OscCounter=0;
            }
        }
    }
    else
    {
        if(cap > 0)
        {
            if(_OscCounter++>MAXOSC)
            {
                objectif_atteint=true;
                _OscCounter=0;
            }

        }
    }
    */

    



    _normalizeConsignes();

    /////////////////////////////////////////////////////////////
    // Sauvegarde de l'erreur polaire calculée pour le coup suivant
    last_erreur_polaire.dist = dist;
    last_erreur_polaire.cap = cap;
}

/**
 * @brief Doit être appelé après la mise à jour de l'erreur polaire. Applique la correction P(ID) et en tire une consigne pour chaque moteur. Se charge également de vérifier les condition de réussite de l'objectif défini.
 *         Ne tient compte que de l'erreur par rapport à l'angle visé (le robot reste donc plus ou moins sur place).
 */
void ControleurPID::_processPIDRotation()
{
    double target_cap = 0.0;

    double terr = erreur_polaire.terr;


    sum_erreur_polaire.terr += terr;

    /////////////////////////////////////////////////////////////
    // Application des coefficients
    target_cap = Pangle * erreur_polaire.terr
            + Iangle * sum_erreur_polaire.terr
            - Dangle * odometrie->getVTheta();

    /////////////////////////////////////////////////////////////
    // Si l'angle du robot est suffisament proche de l'angle visé, l'objectif est considéré comme étant atteint.
    if(abs(erreur_polaire.terr) < ERROR_MIN_ANGLE)
    {
        objectif_atteint = true;
    }

    /////////////////////////////////////////////////////////////
    // Mise à jour des consignes de vitesse pour les moteurs
    consigneG = target_cap;
    consigneD = - target_cap;





    CG=consigneG;
    CD=consigneD;


    _normalizeConsignes();

    /////////////////////////////////////////////////////////////
    // Sauvegarde de l'erreur polaire calculée pour le coup suivant
    last_erreur_polaire.terr = terr;
}

/**
 * @brief S'assure de normaliser les consignes en fonction de la plus grande si elle dépasse 1. Le but est d'éviter de couper une
 *          partie de la différence entre les deux consignes si l'une au moins dépasse 1 (on parle ici des valeurs absolues).
 */
void ControleurPID::_normalizeConsignes()
{
    if(abs(consigneG) > abs(consigneD))
    {
        if(abs(consigneG) > 1.0)
        {
            consigneD = consigneD / abs(consigneG);
            consigneG = consigneG / abs(consigneG);
        }
    }
    else
    {
        if(abs(consigneD) > 1.0)
        {
            consigneG = consigneG / abs(consigneD);
            consigneD = consigneD / abs(consigneD);
        }
    }
}

/**
 * @brief Doit être appelé après le calcul des consignes moteur. Envoi ces consignes au shield moteurs si l'objectif n'a pas été 
 *          marqué comme atteint.
 */
void ControleurPID::_applyConsignes()
{
    if(!objectif_atteint)
    {
        // Au cas où les moteurs avait été arrêté avant
        moteurs->motors_on();
        // Les consignes en paramètre doivent êtres normalisées, autrement elles seront simplement coupées par la fonction
        moteurs->commande_vitesses(consigneG, consigneD);
    }
    else
    {

        moteurs->motors_stop();
    }
}

BlocMoteurs* ControleurPID::getBlocMoteurs()
{
    return moteurs;
}

Odometrie* ControleurPID::getOdometrie()
{
    return odometrie;
}

void ControleurPID::commande_vitesses(double consigneG, double consigneD)
{
    moteurs->motors_on();
    moteurs->commande_vitesses(consigneG, consigneD);
}

void ControleurPID::stop_moteurs()
{
    moteurs->motors_stop();
}
