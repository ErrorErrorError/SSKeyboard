
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
    effectId = 0;
    duration = 0;
}
Keys::Keys(uint8_t keyCode, uint8_t location, RGB steadyColor) {
    keycode = keyCode;
    region = location;
    mainColor = steadyColor;
    mode = PerKeyModes::Steady;
    effectId = 0x00;
    duration = 0x012c;
}

Keys::Keys(uint8_t keyCode, uint8_t location, RGB active, RGB rest, uint16_t duration) {
    keycode = keyCode;
    region = location;
    mainColor = rest;
    activeColor = active;
    this->duration = duration;
    mode = PerKeyModes::Reactive;
    effectId = 0x00;
}

void Keys::setReactiveKey(RGB active, RGB rest, uint16_t duration) {
    resetModesColor();
    mainColor = rest;
    activeColor = active;
    this->duration = duration;
    mode = PerKeyModes::Reactive;
    effectId = 0xff;
}

void Keys::setSteadyKey(RGB steadyColors) {
    resetModesColor();
    mainColor = steadyColors;
    mode = PerKeyModes::Steady;
    effectId = 0x00;
    duration = 0x012c;
}

void Keys::setEffectKey(uint8_t _id, PerKeyModes breathOrShift) {
    resetModesColor();
    effectId = _id;
    mode = breathOrShift;
}

void Keys::resetModesColor() {
    mainColor = RGB();
    activeColor = RGB();
    this->duration =0x012c;
}

void Keys::disableKey() {
    resetModesColor();
    mode = PerKeyModes::Disabled;
    effectId = 0x00;
}

RGB Keys::getMainColor() {
    return mainColor;
}
RGB Keys::getActiveColor() {
    return activeColor;
}

PerKeyModes Keys::getMode() {
    return mode;
}

uint8_t Keys::getKeycode() {
    return keycode;
}

uint8_t Keys::getRegion() {
    return region;
}

uint8_t Keys::getEffectId() {
    return effectId;
}

uint16_t Keys::getDuration() {
    return duration;
}
