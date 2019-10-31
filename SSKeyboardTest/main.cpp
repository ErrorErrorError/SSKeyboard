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
    
    RGB solidColor = {0x00, 0x00,0x00};
    uint8_t id = 1;
    
    PerKeyModes modes = Steady;
    
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

    
    
    KeyTransition transitions[3]{};
    
    RGB firstColor =  {0xff,0x00,0xe1};
    
    transitions[0].color = firstColor;
    transitions[0].duration = 0xa8;
    
    transitions[1].color = {0xff,0xe4,0x00};
    transitions[1].duration = 0xa8;
    
    transitions[2].color = {0x00,0xcc,0xff};
    transitions[2].duration = 0xad;

    keyboardMgr.setSleepInMillis(200);
    keyboardMgr.sendColorKeys(keysModifiers, false);
    keyboardMgr.sendColorKeys(keysAlpha, false);
    keyboardMgr.sendColorKeys(keysEnter, false);
    keyboardMgr.sendColorKeys(keysSpecial, true);

    
    KeyEffect keyEffect = KeyEffect(id, transitions, sizeof(transitions) / sizeof(transitions[0]));
    keyboardMgr.sendEffectKeys(&keyEffect, false);

    return 0;
}
