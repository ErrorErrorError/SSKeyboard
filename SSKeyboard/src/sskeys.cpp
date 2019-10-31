
//
//  KeyEffects.cpp
//  SSKeyboard
//
//  Created by Erik Bautista on 10/14/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include "sskeys.h"
Keys::Keys() {
    keycode = 0;
    region = 0;
    effect_id = 0;
    duration = 0;
}
Keys::Keys(uint8_t keyCode, uint8_t location, RGB steadyColor) {
    keycode = keyCode;
    region = location;
    mainColor = steadyColor;
    mode = PerKeyModes::Steady;
    effect_id = 0x00;
    duration = 0x012c;
}

Keys::Keys(uint8_t keyCode, uint8_t location, RGB active, RGB rest, uint16_t duration) {
    keycode = keyCode;
    region = location;
    mainColor = rest;
    activeColor = active;
    this->duration = duration;
    mode = PerKeyModes::Reactive;
    effect_id = 0x00;
}

void Keys::setReactiveKey(RGB active, RGB rest, uint16_t duration) {
    resetModesColor();
    mainColor = rest;
    activeColor = active;
    this->duration = duration;
    mode = PerKeyModes::Reactive;
    effect_id = 0xff;
}

void Keys::setSteadyKey(RGB steadyColors) {
    resetModesColor();
    mainColor = steadyColors;
    mode = PerKeyModes::Steady;
    effect_id = 0x00;
    duration = 0x012c;
}

void Keys::setEffectKey(uint8_t _id, PerKeyModes breathOrShift) {
    resetModesColor();
    effect_id = _id;
    mode = breathOrShift;
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
    this->duration =0x012c;
}

PerKeyModes Keys::getMode() {
    return mode;
}

void Keys::disableKey() {
    resetModesColor();
    mode = PerKeyModes::Disabled;
    effect_id = 0x00;
}
