//
//  sseffect.cpp
//  SSKeyboard
//
//  Created by Erik Bautista on 10/24/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include "sseffect.h"

KeyEffect::KeyEffect() {
    startColor = RGB();
    sizeOfTransitions = 0;
    effectId = 0;
    waveModeActive = false;
    waveOrigin = KeyPoint();
    waveRadControl = XY;
    waveLength = 0;
    waveDirection = Inward;
}

KeyEffect::KeyEffect(uint8_t _id, RGB initialColor, KeyTransition *keyTransition, uint8_t transitionSize) {
    effectId = _id;
    startColor = initialColor;
    transitions = keyTransition;
    sizeOfTransitions = transitionSize;
    waveModeActive = false;
}

void KeyEffect::setWaveMode(KeyPoint origin, WaveRadControl radControl, WaveDirection direction) {
    waveModeActive = true;
    waveOrigin = origin;
    waveRadControl = radControl;
    waveDirection = direction;
}

void KeyEffect::disableWavemode() {
    waveModeActive = false;
}

void KeyEffect::setEffectId(uint8_t _id) {
    effectId = _id;
}

void KeyEffect::setTransitions(KeyTransition *keyTransition, uint8_t size) {
    transitions = keyTransition;
    sizeOfTransitions = size;
}

RGB KeyEffect::getStartColor() {
    return startColor;
}

KeyTransition *KeyEffect::getTransitions() {
    return transitions;
}
uint8_t KeyEffect::getTransitionSize() {
    return sizeOfTransitions;
}
uint8_t KeyEffect::getEffectId() {
    return effectId;
}

bool KeyEffect::isWaveModeActive() {
    return waveModeActive;
}

KeyPoint KeyEffect::getWaveOrigin() {
    return waveOrigin;
}

WaveRadControl KeyEffect::getWaveRadControl() {
    return waveRadControl;
}

uint16_t KeyEffect::getWaveLength() {
    return waveLength;
}
WaveDirection KeyEffect::getWaveDirection() {
    return waveDirection;
}
