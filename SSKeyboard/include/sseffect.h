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

struct KeyTransition {
    struct RGB color;
    uint16_t duration;
};

struct KeyPoint {
    uint16_t x;
    uint16_t y;
};

enum WaveDirection {
    Outward = 0x0,
    Inward = 0x01
};

enum WaveRadControl {
    XY,
    X,
    Y
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
    /// This is the default constructor.
    KeyEffect();
    
    
    /// This constructor creates a KeyEffect with a unique ID to each effect.
    /// @param _id the unique Id.
    /// @param keyTransition the transitions.
    /// @param transitionSize size of the transitions. Should not be more than 16.
    KeyEffect(uint8_t _id, KeyTransition *keyTransition, uint8_t transitionSize);
    
    /// This sets the KeyEffect to wave
    /// @param origin origin of the wave
    /// @param waveLength length of wave
    /// @param radControl radControl description
    /// @param direction the direction of the wave.
    void setWaveMode(KeyPoint origin,uint16_t waveLength, WaveRadControl radControl, WaveDirection direction);
    void disableWavemode();
    void setEffectId(uint8_t _id);
    
    /// <#Description#>
    /// @param keyTransition array of transtions
    /// @param size size of transitions. Should not be more than 16.
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
