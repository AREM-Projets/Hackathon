#include "EtatDynamiqueRobot.h"

EtatDynamiqueRobot::EtatDynamiqueRobot(double x, double y, double theta) : x(x), y(y), theta(theta)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(theta <= - PI) { theta += 2.0 * PI; }
    while(theta > PI) { theta -= 2.0 * PI; }

    v_xy = 0;
    v_theta = 0;

    for(int i = 0; i < MEAN_SAMPLES; i++)
    {
        v_moteurG[i] = 0.0;
        v_moteurD[i] = 0.0;
    }

    c_v_moteurG = 0;
    c_v_moteurD = 0;

    last_dt = 1000*Time;
}

void EtatDynamiqueRobot::versPosition(double n_x, double n_y, double n_theta, int dt)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(n_theta <= - PI) { n_theta += 2.0 * PI; }
    while(n_theta > PI) { n_theta -= 2.0 * PI; }

    v_xy = sqrt( pow(n_x - x, 2) + pow(n_y - y, 2)) / dt;
    v_theta = (n_theta - theta) / dt;

    x = n_x;
    y = n_y;
    theta = n_theta;

    last_dt = dt;
}

void EtatDynamiqueRobot::setPosition(double n_x, double n_y, double n_theta)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(n_theta <= - PI) { n_theta += 2.0 * PI; }
    while(n_theta > PI) { n_theta -= 2.0 * PI; }

    x = n_x;
    y = n_y;
    theta = n_theta;

    v_xy = 0;
    v_theta = 0;

    v_moteurG[c_v_moteurG] = 0;
    v_moteurD[c_v_moteurD] = 0;
    c_v_moteurG = (c_v_moteurG + 1) % MEAN_SAMPLES;
    c_v_moteurD = (c_v_moteurD + 1) % MEAN_SAMPLES;
}


void EtatDynamiqueRobot::translatePosition(double n_x, double n_y, double n_theta)
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(n_theta <= - PI) { n_theta += 2.0 * PI; }
    while(n_theta > PI) { n_theta -= 2.0 * PI; }

    x = n_x;
    y = n_y;
    theta = n_theta;
}

void EtatDynamiqueRobot::setVMG(double vm)
{
    v_moteurG[c_v_moteurG] = vm;
    c_v_moteurG = (c_v_moteurG + 1) % MEAN_SAMPLES;
}

void EtatDynamiqueRobot::setVMD(double vm)
{
    v_moteurD[c_v_moteurD] = vm;
    c_v_moteurD = (c_v_moteurD + 1) % MEAN_SAMPLES;
}

/**
 * @brief Calcul l'angle le plus petit pour atteindre la cible depuis l'angle initial. Le résultat est garanti dans ]-PI, PI].
 */
double EtatDynamiqueRobot::deltaAngleTo(double initial, double target) const
{
    // On s'assure que les angles soient bien dans ]-PI, PI]
    while(initial <= - PI) { initial += 2.0 * PI; }
    while(initial > PI) { initial -= 2.0 * PI; }

    while(target <= - PI) { target += 2.0 * PI; }
    while(target > PI) { target -= 2.0 * PI; }

    // Chemin naïf vers l'objectif
    double delta = (target - initial);
    
    // Dans le cas ou il serait plus simple de partir dans l'autre sens
    if(delta <= -PI)
        delta += 2.0 * PI;
    if(delta > PI)
        delta -= 2.0 * PI;

    return delta;
    
}

double EtatDynamiqueRobot::getThetaRadian() const
{
    return theta;
}

double EtatDynamiqueRobot::getX() const
{
    return x;
}

double EtatDynamiqueRobot::getY() const
{
    return y;
}

double EtatDynamiqueRobot::getV() const
{
    return v_xy;
}

double EtatDynamiqueRobot::getVTheta() const
{
    return v_theta;
}

double EtatDynamiqueRobot::getVMG() const
{
    // Calcul moyenne
    double moyenne = 0.0;
    for(int i = 0; i < MEAN_SAMPLES; i++)
        moyenne += v_moteurG[i];
    moyenne /= MEAN_SAMPLES;
    return moyenne;
}

double EtatDynamiqueRobot::getVMD() const
{
    // Calcul moyenne
    double moyenne = 0.0;
    for(int i = 0; i < MEAN_SAMPLES; i++)
        moyenne += v_moteurD[i];
    moyenne /= MEAN_SAMPLES;
    return moyenne;
}
