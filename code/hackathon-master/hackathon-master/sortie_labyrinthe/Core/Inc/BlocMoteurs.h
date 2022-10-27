#ifndef BLOCMOTEURS_H
#define BLOCMOTEURS_H


#include "Reglages.h"
#include "main.h"
#include "tim.h"
#include <stdlib.h>


enum
{
	arriere,
	avant
};

class BlocMoteurs
{
	public:
		// Constructeur
		BlocMoteurs();
		~BlocMoteurs();
		
		// Pour envoyer une consigne de vitesse aux moteurs
		// les paramètres sont des flottants entre -1 et 1
		// (si la vitesse max est trop faible, il faut modifier la constante MAX_VITESSE dans Reglages.h)
		// (ne pas oublier d'utiliser la méthode motors_on si les moteurs ont été arrêtés avant)
		void commande_vitesses(float vitesse_normalisee_G, float vitesse_normalisee_D);

		// Methodes pour bloquer les moteurs
		// (il faut appeler la méthode motors_on pour pouvoir de nouveau envoyer des consignes
		// de vitesse aux moteurs)
		void motors_stop();
		
		// Methode pour autoriser la rotation des moteurs
		void motors_on();

		void setPourcentMaxSpeed(double p_pourcentMaxSpeed);

	private:
		// Methodes pour définir la tension affectée à chaque moteur
		void set_vitesse_moteur_G(int vitesse, int dir);
		void set_vitesse_moteur_D(int vitesse, int dir);
		
		bool moteurs_arret;

		// Un coefficient entre 0 et 1.0 pour limiter facilement la vitesse pendant un mouvement particulier
		double pourcentMaxSpeed;




};



#endif
