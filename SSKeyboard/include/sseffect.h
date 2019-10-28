//
//  sseffect.hpp
//  SSKeyboard
//
//  Created by Erik Bautista on 10/24/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#ifndef sseffect_hpp
#define sseffect_hpp

#include <stdio.h>

// ifdef sskeys_h
#include "sskeys.h"
// #endif

#ifdef __cplusplus
extern "C" {
#endif

struct KeyTransition {
    struct RGB color;
    uint16_t duration;
};

struct KeyPoint {
    uint16_t x;
    uint16_t y;
};

#ifdef __cplusplus
}
#endif

enum WaveDirection {
    Outward = 0x0,
    Inward = 0x01
};

enum WaveRadControl {
    X,
    Y,
    XY
};

#ifdef __cplusplus
class KeyEffect {
    RGB startColor = RGB();
    KeyTransition *transitions;
    uint8_t sizeOfTransitions = 0;
    uint8_t effectId = 0;
    bool waveModeActive = false;
    KeyPoint waveOrigin = KeyPoint();
    WaveRadControl waveRadControl = XY;
    uint16_t waveLength = 0;
    WaveDirection waveDirection = Inward;
public:
    KeyEffect();
    
    KeyEffect(uint8_t _id, RGB initialColor, KeyTransition *keyTransition, uint8_t transitionSize);
    
    void setWaveMode(KeyPoint origin, WaveRadControl radControl, WaveDirection direction);
    void disableWavemode();
    void setEffectId(uint8_t _id);
    void setTransitions(KeyTransition *keyTransition, uint8_t size);
    
    RGB getStartColor();
    KeyTransition *getTransitions();
    uint8_t getTransitionSize();
    uint8_t getEffectId();
    bool isWaveModeActive();
    KeyPoint getWaveOrigin();
    WaveRadControl getWaveRadControl();
    uint16_t getWaveLength();
    WaveDirection getWaveDirection();

};
#endif

#endif /* sseffect_hpp */
