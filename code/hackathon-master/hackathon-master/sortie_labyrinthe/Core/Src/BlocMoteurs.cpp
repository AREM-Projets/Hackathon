// Code récupéré dans hardware.cpp de l'année dernière et adapté pour être inclut dans une classe
#include "BlocMoteurs.h"

extern float COEF2D;
extern float COEF2G;

BlocMoteurs::BlocMoteurs()
{
    moteurs_arret = 1;

    // Initializing Motor
    //TODO
    //Initializing Motor


    pourcentMaxSpeed = 1.0;
}

BlocMoteurs::~BlocMoteurs()
{

}

void BlocMoteurs::commande_vitesses(float vitesse_normalisee_G, float vitesse_normalisee_D)
{ 
    /////////////////////////////////////////////////////////////
    // Assurance que les parametres sont entre -1 et 1 ////////////////
    vitesse_normalisee_G = fmin(1.0f, vitesse_normalisee_G);
    vitesse_normalisee_G = fmax(-1.0f, vitesse_normalisee_G);

    vitesse_normalisee_D = fmin(1.0f, vitesse_normalisee_D);
    vitesse_normalisee_D = fmax(-1.0f, vitesse_normalisee_D);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////

    // Détermination du signe des vitesses (logique inversée entre les deux moteurs car placés symétriquement sur le robot et branchement identique)
    int sens_G = avant;
    if(vitesse_normalisee_G < 0)
    {
    	sens_G = arriere;
    }
    int sens_D = avant;
    if(vitesse_normalisee_D < 0)
    {
    	sens_D = arriere;
    }

    // Détermination des valeurs de vitesses réelles à envoyer au shield + Recalage de la normalisation (le moteur ne fait avancer les chemilles qu'à partir de 0.2*MAX_VITESSE en statique et posé sur le sol)
    //

    int vitesse_G = COEF2G*MAX_VITESSE*(abs(vitesse_normalisee_G)*(1-COEF_G) + COEF_G);
    int vitesse_D = COEF2D*MAX_VITESSE*(abs(vitesse_normalisee_D)*(1-COEF_D) + COEF_D);
    /////////////////////////////////////////////////////////////
    // Au cas où le shield a un comportement indésirable lorsque les vitesses 
    // envoyées sont trop proches de 0 (vibration et bruit des moteurs); 
    // il peut valoir mieux arrêter les moteurs qu'envoyer une vitesse de 0.
    //vitesse_G = max(vitesse_G, 10);
    //vitesse_D = max(vitesse_D, 10);
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    
    set_vitesse_moteur_G(vitesse_G * pourcentMaxSpeed, sens_G);
    set_vitesse_moteur_D(vitesse_D * pourcentMaxSpeed, sens_D);
}

void BlocMoteurs::set_vitesse_moteur_G(int vitesse, int dir)
{
    // verification de la validité de la vitesse a envoyer
    vitesse = abs(vitesse);
    vitesse = fmin(vitesse, (int)MAX_VITESSE);
    //vitesse = vitesse > 10 ? vitesse : 0;

    if (!moteurs_arret)
    {
    	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint16_t ) vitesse);
    	HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, (GPIO_PinState) dir );

    }


}

void BlocMoteurs::set_vitesse_moteur_D(int vitesse, int dir)
{
    // verification de la validité de la vitesse a envoyer
    vitesse = abs(vitesse);
    vitesse = fmin(vitesse, (int)MAX_VITESSE);
    //vitesse = vitesse > 10 ? vitesse : 0;

    if (!moteurs_arret)
    {
    	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, (uint16_t ) vitesse);
    	HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, (GPIO_PinState) dir );

    }


}


void BlocMoteurs::motors_on() // il faut activer les moteurs pour qu'il puisse recevoir des commandes vitesse.
{
    moteurs_arret = 0;
}

void BlocMoteurs::motors_stop() // coupe les moteurs
{

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    moteurs_arret = 1;
}



void BlocMoteurs::setPourcentMaxSpeed(double p_pourcentMaxSpeed)
{
    // limitation du coefficient entre 0 et 1.0
    pourcentMaxSpeed = fmax(fmin(1.0, p_pourcentMaxSpeed), 0.0);
}
