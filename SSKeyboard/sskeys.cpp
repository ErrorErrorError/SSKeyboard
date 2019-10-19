
//
//  KeyEffects.cpp
//  SSKeyboard
//
//  Created by Erik Bautista on 10/14/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include "sskeys.h"
Keys::Keys() {}
Keys::Keys(uint8_t keyCode, char *letter, uint8_t location, RGB steadyColor) {
    keycode = keyCode;
    keyLetter = letter;
    region = location;
    mainColor = steadyColor;
    mode = Steady;
}

Keys::Keys(uint8_t keyCode, char *letter, uint8_t location, RGB active, RGB rest, uint16_t duration) {
    keycode = keyCode;
    keyLetter = letter;
    region = location;
    mainColor = rest;
    activeColor = active;
    speed = duration;
    mode = Reactive;
}

void Keys::setReactiveKey(RGB active, RGB rest, uint16_t duration) {
    resetModesColor();
    mainColor = rest;
    activeColor = active;
    speed = duration;
    mode = Reactive;
}

void Keys::setSteadyKey(RGB steadyColors) {
    resetModesColor();
    mainColor = steadyColors;
    mode = Steady;
}

RGB Keys::getMainColor() {
    return mainColor;
}
RGB Keys::getActiveColor() {
    return activeColor;
}

void Keys::resetModesColor() {
    mainColor = RGB();
    activeColor = RGB();
    speed = 0;
}

PerKeyModes Keys::getMode() {
    return mode;
}

void Keys::disableKey() {
    resetModesColor();
    mode = Disabled;
}


void Keys::setDuration(uint16_t duration) {
    speed = duration;
}
uint16_t Keys::getDuration() {
    return speed;
}
