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
    Steady,
    ColorShift,
    Breathing,
    Reactive,
    Disabled,
};

#ifdef __cplusplus
class Keys {
private:
    RGB mainColor = RGB();
    PerKeyModes mode = PerKeyModes::Steady;
    RGB activeColor = RGB();
    void resetModesColor();
public:
    uint8_t keycode = 0;
    char *keyLetter;
    uint8_t region = 0;
    uint8_t effect_id = 0;
    uint16_t duration = 0;
    Keys();
    /// Creates a key object and sets the key as steady mode
    /// @param keyCode sets the key code of the key
    /// @param location sets the location of the key in the region
    /// @param steadyColor sets the steady color of the key
    Keys(uint8_t keyCode, uint8_t location, RGB steadyColor);
    
    /// Creates a key object and sets the key as reactive mode
    /// @param keyCode sets the keycode of the key
    /// @param location sets the location of the key in the region
    /// @param active sets the active color
    /// @param rest sets the rest color
    /// @param duration sets the duration of the effect
    Keys(uint8_t keyCode, uint8_t location, RGB active, RGB rest, uint16_t duration);
    /// Sets the key to reactive mode
    /// @param active sets the active color to activeColor
    /// @param rest sets the rest color to mainColor
    /// @param duration sets the duration of the effect
    void setReactiveKey(RGB active, RGB rest, uint16_t duration);
    /// This method sets the key to Steady mode
    /// @param steadyColor the steady color to assign
    void setSteadyKey(RGB steadyColor);
    /// Returns the main color
    RGB getMainColor();
    /// Returns the active color
    RGB getActiveColor();
    /// Returns the mode of the key
    PerKeyModes getMode();
    /// Turns the key off
    void disableKey();
};
#endif
