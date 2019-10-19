//
//  KeyEffects.hpp
//  SSKeyboard
//
//  Created by Erik Bautista on 10/14/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifndef sskeys_h
#define sskeys_h
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
    
#ifdef __cplusplus
}
#endif

// Modes for PerKey/GS65
enum PerKeyModes {
    Steady = 0x01,
    //ColorShift,
    //Breathing,
    Reactive = 0xff,
    Disabled = 0x03,
};

#ifdef __cplusplus
class Keys {
private:
    RGB mainColor = RGB();
    PerKeyModes mode = Steady;
    RGB activeColor = RGB();
    uint16_t speed = 0;
    void resetModesColor();
public:
    uint8_t keycode = 0;
    char *keyLetter;
    uint8_t region = 0;
    Keys();
    Keys(uint8_t keyCode, char *letter, uint8_t location, RGB steadyColor);
    Keys(uint8_t keyCode, char *letter, uint8_t location, RGB active, RGB rest, uint16_t duration);
    void setReactiveKey(RGB active, RGB rest, uint16_t duration);
    void setSteadyKey(RGB steadyColors);
    RGB getMainColor();
    RGB getActiveColor();
    PerKeyModes getMode();
    void setDuration(uint16_t duration);
    uint16_t getDuration();
    void disableKey();
};
#endif
