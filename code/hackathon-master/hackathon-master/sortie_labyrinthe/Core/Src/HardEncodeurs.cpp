#include "HardEncodeurs.h"
#include "main.h"


#ifdef KRAKEN

extern int TICKG;
extern int TICKD;


HardEncodeurs::HardEncodeurs()
{
    _lastTicksG = 0;
    _lastTicksD = 0;

    _sumTicksG = 0;
    _sumTicksD = 0;

    // Configuration de la stm32 pour utiliser le mode encoder hardware sur les timers 4 et 8.
    // La fonction HAL_init doit normalement être appelée avant tout autre appel à une fonction HAL




    // Pour activer les channels 1 et 2 de chaque timer (chacune reliée à un pin du signal de l'encodeur)

}

int64_t HardEncodeurs::_processDeltaTicksG()
{
    uint32_t ticksG = __HAL_TIM_GET_COUNTER(&htim4); // obtention du compteur (registre CNT) du timer
    TICKG=ticksG;
    // (comme les timers sont configurés comme des encodeurs, ce compteur est tenu à jour indépendamment du CPU de la stm32)
    
    // calcul du delta en prenant en compte au mieux les overflow du timer
    int64_t delta = _processDelta(ticksG, _lastTicksG);

    _lastTicksG = ticksG;
    _sumTicksG += delta;

    return delta;
}

int64_t HardEncodeurs::_processDeltaTicksD()
{
    uint32_t ticksD = __HAL_TIM_GET_COUNTER(&htim3); // obtention du compteur (registre CNT) du timer
    TICKD=ticksD;
    // (comme les timers sont configurés comme des encodeurs, ce compteur est tenu à jour indépendamment du CPU de la stm32)
    
    // calcul du delta en prenant en compte au mieux les overflow du timer
    int64_t delta = _processDelta(ticksD, _lastTicksD);

    _lastTicksD = ticksD;
    _sumTicksD += delta;

    return delta;
}

int64_t HardEncodeurs::_processDelta(uint16_t ticks, uint16_t lastTicks)
{
    int64_t delta = 0;
    
    // On détecte du mieux que l'on peut les problèmes d'overflow du compteur CNT
    if(ticks < 0x0FFF && lastTicks > 0x8FFF) // CNT a surement overflow à droite
    {
        delta = (int64_t) ( (0xFFFF - lastTicks) + ticks + 1);
    }
    else if(ticks > 0x8FFF && lastTicks < 0x0FFF) // CNT a surement overflow à gauche
    {
        delta = - (int64_t) ( (0xFFFF - ticks) + lastTicks + 1);
    }
    else
    {
        delta = ((int64_t) ticks) - ((int64_t) lastTicks);
    }

    return delta;
}

int64_t HardEncodeurs::getDeltaTicksG()
{
    return _processDeltaTicksG();
}

int64_t HardEncodeurs::getDeltaTicksD()
{
    return _processDeltaTicksD();
}

int64_t HardEncodeurs::getSumTicksG()
{
    _processDeltaTicksG();
    return _sumTicksG;
}

int64_t HardEncodeurs::getSumTicksD()
{
    _processDeltaTicksD();
    return _sumTicksD;
}



void HardEncodeurs::debugPrint(uint16_t*  cG, uint16_t* cD)
{

    *cG = __HAL_TIM_GET_COUNTER(&htim4);
    *cD = __HAL_TIM_GET_COUNTER(&htim3);

    /*int64_t deltaG = _processDeltaTicksG();
    int64_t deltaD = _processDeltaTicksD();
    
    Serial.println("====================");
    Serial.println("Debug encodeurs :");
    Serial.print("Delta G = "); Serial.print(deltaG); Serial.print(" ; Delta D = "); Serial.println(deltaD);
    Serial.print("Sum G = "); Serial.print(_sumTicksG); Serial.print(" ; Sum D = "); Serial.println(_sumTicksD);
    Serial.print("Angle G = "); Serial.print(_sumTicksG * 360 / 4000); Serial.print(" ; Angle D = "); Serial.println(_sumTicksD * 360 / 4000);
    Serial.println("====================");*/
}




#endif
