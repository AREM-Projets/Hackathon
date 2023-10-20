# Hackathon - Public

Ce repo contient l'ensemble du code utilisé par les élèves de première année lors du Hackathon AREM. 

Ce README contient des ressources permettant d'utiliser le code.

## Vidéos de présentation 

https://www.youtube.com/playlist?list=PLIzFWC3vmmyL8_5sqXsGWLc__HhRqmanS

## Fonctions à utiliser

Les fonctions suivantes peuvent être utilisées :
```cpp
/* Actions */
taches->addLigne(double X, double Y);
taches->addRotation(double Theta); // En radians
taches->addWait(int number_ms); 
taches->sonarscan();

/* Récupération de valeurs */
float sonar->getValue(directionSonar dir); // dir peut être left, front ou right 
double odometrie->getX(); 
double odometrie->getY(); 
double odometrie->getThetaRadian(); 
```

Une description plus détaillée de certaines fonctions se trouve ci-dessous

### Actions : déplacements

Tous les déplacements sont **absolus**, et la posision de départ du robot est toujours `X = 0; Y = 0; Theta = 0`.

On a ainsi au démarrage :

Les fonctions de déplacement peuvent donc être décrites ainsi :
- **addLigne(X, Y)** - faire une ligne entre les coordonnées actuelles du robot et le point (X, Y)
- **addRotation(Theta)** - regarder l'angle Theta

**Attention** : addRotation(PI/2) dit au robot de **s'orienter dans la direction PI/2**, et non de tourner de PI/2

Par exemple, le code suivant permet au robot de dessiner un carré :
```cpp
taches->addLigne(400, 0);
taches->addRotation(PI/2);
taches->addLigne(400, 400);
taches->addRotation(PI);
taches->addLigne(0, 400);
taches->addRotation(-PI/2);
taches->addLigne(0, 0);
taches->addRotation(0);
```
### Récupération de valeurs

**Les valeurs renvoyées par les fonctions de mesure ne sont pas mises à jour instantanément.** 

Par mesure de sécurité, il est conseillé de considérer qu'elles sont mises à jour à chaque itération de la boucle de fonctionnement.

Dans le code du robot :
```cpp
// Mauvais code : récupère les valeurs d'il y a 1s
if(defileur->update()) 
{
    taches->sonarscan();
    taches->addWait(1000); 
    dist = sonar->getValue(front);
}

// Bon code : récupère les valeurs qui viennent d'être mesurées
if(defileur->update()) 
{
    dist = sonar->getValue(front); // récup des valeurs au début
    taches->addWait(1000); 
    taches->sonarscan(); // mesures à la fin
}
```

La fonction **sonar->getValue(dir)** récupère les **dernières valeurs mesurées** par le sonar.

Pour lancer une mesure, utiliser **taches->sonarscan()**. Si aucune mesure n'a été faite avant la récupération des valeurs, vous récupérez essentiellement des valeurs aléatoires (welcome to the beautiful world of *undefined behavior*).

## Troubleshooting

Attention, le robot peut ne pas démarrer si aucune action (voir fonctions à utiliser) ne se trouve avant la boucle while
