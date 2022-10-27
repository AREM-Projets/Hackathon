#ifndef HARDENCODEURS_H
#define HARDENCODEURS_H

#include "Reglages.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

class HardEncodeurs
{
    public:
        HardEncodeurs();
        int64_t getDeltaTicksG();
        int64_t getDeltaTicksD();
        int64_t getSumTicksG();
        int64_t getSumTicksD();


        void debugPrint(uint16_t*  cG, uint16_t* cD);
    private:
        int64_t _processDeltaTicksG();
        int64_t _processDeltaTicksD();
        int64_t _processDelta(uint16_t ticks, uint16_t lastTicks);



        uint32_t _lastTicksG;
        uint32_t _lastTicksD;
        int64_t _sumTicksG;
        int64_t _sumTicksD;
};


#endif
