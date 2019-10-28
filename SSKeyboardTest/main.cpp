//
//  main.cpp
//  SSKeyboardTest
//
//  Created by Erik Bautista on 10/27/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include <iostream>
#include "sskeyboard.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    SSKeyboard keyboardMgr = SSKeyboard();
    std::cout << keyboardMgr.getKeyboardModel() << endl;
    
    RGB solidColor = {0x00, 0x00,0x00};
    uint8_t id = 1;
    
    
    Keys keysModifiers[kModifiersSize + 1];
    Keys keyModifier = Keys(regions[0], regions[0], solidColor);
    keyModifier.setEffectKey(id, ColorShift);
    keysModifiers[0] = keyModifier;
    for (uint8_t i = 0; i < kModifiersSize; i++) {
        Keys key = Keys(modifiers[i], regions[0], solidColor);
        key.setEffectKey(id, ColorShift);
        keysModifiers[i + 1] = key;
    }
    
    Keys keysAlpha[kAlphanumsSize + 1];
    Keys keyAlpha = Keys(regions[1], regions[1], solidColor);
    keyAlpha.setEffectKey(id, ColorShift);
    keysAlpha[0] = keyAlpha;
    for (uint8_t i = 0; i < kAlphanumsSize; i++) {
        Keys key = Keys(alphanums[i], regions[1], solidColor);
        key.setEffectKey(id, ColorShift);
        keysAlpha[i + 1] = key;
    }
    
    Keys keysEnter[kEnterSize + 1];
    Keys keyEnter = Keys(regions[2], regions[2], solidColor);
    keyEnter.setEffectKey(id, ColorShift);
    keysEnter[0] = keyEnter;
    for (uint8_t i = 0; i < kEnterSize; i++) {
        Keys key = Keys(enter[i], regions[2], solidColor);
        key.setEffectKey(id, ColorShift);
        keysEnter[i + 1] = key;
    }

    Keys keysSpecial[kSpecialSize + 1];
    Keys keySpecial = Keys(regions[3], regions[3], solidColor);
    keySpecial.setEffectKey(id, ColorShift);
    keysSpecial[0] = keySpecial;
    for (uint8_t i = 0; i < kSpecialSize; i++) {
        Keys key = Keys(special[i], regions[3], solidColor);
        key.setEffectKey(id, ColorShift);
        keysSpecial[i + 1] = key;
    }

    KeyTransition transitions[5];
    
    transitions[0] = KeyTransition();
    transitions[1] = KeyTransition();
    transitions[2] = KeyTransition();
    transitions[3] = KeyTransition();
    transitions[4] = KeyTransition();

    
    RGB firstColor = {0xff, 0x00, 0xff};
    
    transitions[0].color = {0x0,0xff,0x00};
    transitions[0].duration = 200;
    
    transitions[1].color = {0x00,0x00,0xff};
    transitions[1].duration = 200;
    
    transitions[2].color = {0x00,0xff,0xff};
    transitions[2].duration = 200;
    
    transitions[3].color = {0x00,0x00,0x00};
    transitions[3].duration = 200;
    
    transitions[4].color = firstColor;
    transitions[4].duration = 200;

    keyboardMgr.setSleepInMillis(200);
    keyboardMgr.sendColorKeys(keysModifiers, false);
    keyboardMgr.sendColorKeys(keysAlpha, false);
    keyboardMgr.sendColorKeys(keysEnter, false);
    keyboardMgr.sendColorKeys(keysSpecial, true);

    KeyEffect keyEffect = KeyEffect(id, firstColor, transitions, 5);
    keyEffect.setWaveMode({0x06c1, 0x026e}, X, WaveDirection::Outward);
    keyboardMgr.sendEffectKeys(&keyEffect, true);
    
    return 0;
}
