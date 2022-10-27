/*
 * const.h
 *
 *  Created on: 11 août 2021
 *      Author: adrie
 */

#ifndef SRC_CONST_H_
#define SRC_CONST_H_

volatile unsigned int Time;
volatile unsigned int TLoop;

float xglob = 0;
float yglob = 0;
float thetaglob = 0;
float targetdistglob;
float targetangleglob;
float distglob;
float angleglob;
float CG;
float CD;
int TICKG=0;
int TICKD=0;
float CorAngle = 0;
float CorDist = 0;
float COEF2D=1;
float COEF2G=0.866666675;
float tempmult = 1;
float erreurdist=0;
float erreurangle=0;

float sonarmeasure=0;


#endif /* SRC_CONST_H_ */
