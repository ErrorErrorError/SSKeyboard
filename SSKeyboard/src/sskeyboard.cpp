//
//  sskeybpard.cpp
//  SSKeyboard
//
//  Created by Erik Bautista on 9/28/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include "sskeyboard.h"
#include <chrono>
#include <thread>
using namespace std;

uint16_t times = 500;

static CFMutableDictionaryRef createMatchingDictionaryForKeyboard(uint16_t vendorID, uint16_t productID, uint16_t maxFeatureReport, uint16_t usagePage, uint16_t versionNumber)
{
    // create a dictionary to add usage page/usages to
    CFMutableDictionaryRef result = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    CFNumberRef vendorCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &vendorID);
    if (vendorCFNumberRef != NULL) {
        CFDictionarySetValue(result, CFSTR(kIOHIDVendorIDKey), vendorCFNumberRef);
        CFRelease(vendorCFNumberRef);
    }
    
    CFNumberRef productCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &productID);
    if (productCFNumberRef != NULL) {
        CFDictionarySetValue(result, CFSTR(kIOHIDProductIDKey), productCFNumberRef);
        CFRelease(productCFNumberRef);
    }

    CFNumberRef reportCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &maxFeatureReport);
    if (reportCFNumberRef != NULL) {
        CFDictionarySetValue(result, CFSTR(kIOHIDMaxFeatureReportSizeKey), reportCFNumberRef);
        CFRelease(reportCFNumberRef);
    }
    
    CFNumberRef usagePageCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &usagePage);
    if (usagePageCFNumberRef != NULL) {
        CFDictionarySetValue(result, CFSTR(kIOHIDPrimaryUsagePageKey), usagePageCFNumberRef);
        CFRelease(usagePageCFNumberRef);
    }
    
    CFNumberRef versionNumberCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &versionNumber);
    if (versionNumberCFNumberRef != NULL) {
        CFDictionarySetValue(result, CFSTR(kIOHIDVersionNumberKey), versionNumberCFNumberRef);
        CFRelease(versionNumberCFNumberRef);
    }
    
    return result;
}

SSKeyboard::SSKeyboard() {
    
    // Initializes hidManagerRef
    hidManagerRef = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    
    // Checks if keyboard is a GS65
    CFDictionaryRef matchingGS65 = createMatchingDictionaryForKeyboard(kVendorId, kProductId, kMaxFeatureReportSize, kPrimaryUsagePage, kGS65VersionNumber);
    
    IOReturn isKeyboardPerKeyGS65 = checkForDevice(matchingGS65);
    if (isKeyboardPerKeyGS65 == kIOReturnSuccess) {
        model = PerKeyGS65;
        return;
    }
   
    // Checks if keyboard is a Per Key Model other than GS65. GS65 uses a different keyboard version
    CFDictionaryRef matchingPerKey = createMatchingDictionaryForKeyboard(kVendorId, kProductId, kMaxFeatureReportSize, kPrimaryUsagePage, kPerKeyVersionNumber);
    IOReturn isKeyboardPerKey = checkForDevice(matchingPerKey);
    if (isKeyboardPerKey == kIOReturnSuccess) {
        model = PerKey;
        
        return;
    }
    
    // This is not implemented yet
    IOReturn isKeyboardThreeRegion = checkThreeRegion();
    if (isKeyboardThreeRegion == kIOReturnSuccess) {
        model = ThreeRegion;
        return;
    }
}

IOReturn SSKeyboard::checkForDevice(CFDictionaryRef matchingCFDictRef) { 
    IOHIDManagerSetDeviceMatching(hidManagerRef, matchingCFDictRef);
    if (matchingCFDictRef) {
        CFRelease(matchingCFDictRef);
    }
    
    IOHIDManagerOpen(hidManagerRef, kIOHIDOptionsTypeNone);
    
    CFSetRef deviceSetRef = IOHIDManagerCopyDevices(hidManagerRef);
    
    //If returns null, then couldn't find device with the matching dictionary
    if (deviceSetRef == nullptr) {
        return kIOReturnNotFound;
    }
    
    CFIndex deviceCount = CFSetGetCount(deviceSetRef);
    
    IOHIDDeviceRef *deviceRef = new IOHIDDeviceRef[deviceCount];
    
    if (!deviceRef) {
        CFRelease(deviceSetRef);
        deviceSetRef = NULL;
        if ( deviceRef ) {
            free(deviceRef);
        }
        
        if ( hidManagerRef ) {
            CFRelease( hidManagerRef );
        }
        return kIOReturnError;
    }
    
    CFSetGetValues(deviceSetRef, (const void **) deviceRef);
    CFRelease(deviceSetRef);
    deviceSetRef = NULL;
    
    //Should have only one device matched
    for (uint8_t i = 0; i < deviceCount; i++) {
        keyboardDevice = deviceRef[i];
    }
    
    // We don't need deviceRef anymore since keyboardDevices has the reference.
    delete[] deviceRef;
    return kIOReturnSuccess;
}

// not implemented since I still haven't had time to read values of three region based keyboard
IOReturn SSKeyboard::checkThreeRegion() {
    return kIOReturnError;
}

IOReturn SSKeyboard::sendFeatureReportPackage(uint8_t *featurePackage) {
    IOReturn canOpenUSB = IOHIDDeviceOpen(keyboardDevice, kIOHIDOptionsTypeNone);
    if (canOpenUSB != kIOReturnSuccess) {
        printf("Could not open usb port\n");
        return canOpenUSB;
    }
    
    IOReturn reportOutput = IOHIDDeviceSetReport(keyboardDevice, kIOHIDReportTypeFeature, featurePackage[0], featurePackage, kPackageSize);
    std::this_thread::sleep_for(chrono::milliseconds(times));
    if (reportOutput != kIOReturnSuccess) {
        printf("Could not send feature report\n");
        return reportOutput;
    }
    
    IOReturn canCloseUSB = IOHIDDeviceClose(keyboardDevice, kIOHIDOptionsTypeNone);
    if (reportOutput != kIOReturnSuccess) {
        printf("Could not close usb port\n");
        return canCloseUSB;
    }
    
    return kIOReturnSuccess;
}

IOReturn SSKeyboard::sendOutputReportPackage(uint8_t *outputPackage) {
    IOReturn canOpenUSB = IOHIDDeviceOpen(keyboardDevice, kIOHIDOptionsTypeNone);
    if (canOpenUSB != kIOReturnSuccess) {
        printf("Could not open usb port");
        return canOpenUSB;
    }
    
    IOReturn reportOutput = IOHIDDeviceSetReport(keyboardDevice, kIOHIDReportTypeOutput, outputPackage[0], outputPackage, kOutputPackageSize);
    std::this_thread::sleep_for(chrono::milliseconds(times));
    if (reportOutput != kIOReturnSuccess) {
        printf("Could not send output report\n");
        return reportOutput;
    }
    
    IOReturn canCloseUSB = IOHIDDeviceClose(keyboardDevice, kIOHIDOptionsTypeNone);
    if (reportOutput != kIOReturnSuccess) {
        printf("Could not close usb port\n");
        return canCloseUSB;
    }

    return kIOReturnSuccess;
}

void SSKeyboard::makeOutputPackage(uint8_t region, uint8_t *packet) {
    uint8_t lastByteRegion = 0;
    uint8_t index = 0;
    if (region != 0x0) {
        while (lastByteRegion == 0 && index < kRegions) {
            if (regions[index] == region) {
                lastByteRegion = last_byte_region[index];
            } else {
                index++;
            }
        }
    } else {
        lastByteRegion = region;
    }
    
    packet[0] = 0x0d;
    packet[1] = 0x00;
    packet[2] = 0x02;
    packet[kOutputPackageSize - 1] = lastByteRegion;
}

IOReturn SSKeyboard::sendColorKeys(Keys *keysArray, bool updateKeys) {
    if (keysArray == nullptr) {
        return kIOReturnError;
    }
    if (model == UnknownModel) {
        return kIOReturnError;
    }
    
    uint8_t packet[kPackageSize]{};
    makeColorPackage(keysArray, packet);
    IOReturn setFeatureReturn = sendFeatureReportPackage(packet);
        
    if (setFeatureReturn != kIOReturnSuccess) {
        return setFeatureReturn;
    }
    
    if (updateKeys) {
        Keys region = (*(keysArray));
        uint8_t packageOutput[kOutputPackageSize]{};
        makeOutputPackage(region.region, packageOutput);
        IOReturn setOutputReport = sendOutputReportPackage(packageOutput);
        
        // delete[] packet;
        
        if (setOutputReport != kIOReturnSuccess) {
            return setOutputReport;
        }
    }
    
    return kIOReturnSuccess;

}

IOReturn SSKeyboard::sendEffectKeys(KeyEffect *keyEffect, bool updateKeys) {
    if (keyEffect == nullptr) {
        return kIOReturnError;
    }
    
    if (model == UnknownModel) {
        return kIOReturnError;
    }

    uint8_t packet[kPackageSize]{};
    makeEffectPackage(keyEffect, packet);
    IOReturn setFeatureReturn = sendFeatureReportPackage(packet);
        
    if (setFeatureReturn != kIOReturnSuccess) {
        return setFeatureReturn;
    }
    
    if (updateKeys) {
        uint8_t packetOutput[kOutputPackageSize]{};
        makeOutputPackage(0x00, packetOutput);
        IOReturn setOutputReport = sendOutputReportPackage(packetOutput);
                
        if (setOutputReport != kIOReturnSuccess) {
            return setOutputReport;
        }
    }
    
    return kIOReturnSuccess;
}


void SSKeyboard::makeColorPackage(Keys *colorArray, uint8_t *packet) {
    uint8_t keycodes_size;
    // Region key should be the first key in the array
    Keys regionKey = (*(colorArray));
    uint8_t mode;

    if (regionKey.region == regions[0]) {
        keycodes_size = kModifiersSize;
    } else if (regionKey.region == regions[1]) {
        keycodes_size = kAlphanumsSize;
    } else if (regionKey.region == regions[2]) {
        keycodes_size = kEnterSize;
    } else if (model == PerKeyGS65) {
        keycodes_size = kSpecialSize;
    } else {
        keycodes_size = kSpecialPerKeySize;
    }
    
    // This array includes the region key so we need to append one
    keycodes_size += 1;
    
    for (uint8_t i = 0; i < keycodes_size; i++)
    {
        Keys *currentKey = (colorArray + i);
        uint16_t keys = 2 + (12 * i);

        if (currentKey->getMode() == PerKeyModes::Steady) {
            mode = 0x01;
        } else if (currentKey->getMode() == PerKeyModes::Reactive) {
            mode = 0x08;
        } else if (currentKey->getMode() == PerKeyModes::Disabled){
            mode = 0x03;
        } else {
            mode = 0x0;
        }
                
        // The first key should be the the region key.
        if (i == 0) {
            packet[0]       = 0x0e;
            packet[1]       = 0x00;
            packet[keys]    = currentKey->keycode;
            packet[keys+1]  = 0x00;
        } else {
            packet[keys]    = 0x00;
            packet[keys+1]  = currentKey->keycode;
        }
        
        packet[keys + 2]    = currentKey->getMainColor().r;
        packet[keys + 3]    = currentKey->getMainColor().g;
        packet[keys + 4]    = currentKey->getMainColor().b;
        packet[keys + 5]    = currentKey->getActiveColor().r;
        packet[keys + 6]    = currentKey->getActiveColor().g;
        packet[keys + 7]    = currentKey->getActiveColor().b;
        // Separates the duration into two bytes
        packet[keys + 8]    = currentKey->duration & 0x00ff;
        packet[keys + 9]    = (currentKey->duration & 0xff00) >> 8;
        packet[keys + 10]   = currentKey->effect_id;
        packet[keys + 11]   = mode;
    }
    
}

void SSKeyboard::makeEffectPackage(KeyEffect *keyEffect, uint8_t *packet) {
    uint16_t totalDuration = 0;
    uint8_t index = 0;
    packet[index + 0] = 0x0b;
    // packet[index + 1] = 0x0;
    
    /// Transitions - each transition will take 8 bytes
    if (keyEffect->getTransitionSize() == 0) {
        KeyTransition currentTransition;
        currentTransition.color = keyEffect->getStartColor();
        currentTransition.duration = 0x0;
        index = 2;
        
        packet[index] = keyEffect->getEffectId();
        
        /// Calculate color difference
        RGB colorDifference = calculateColorDelta(currentTransition.color, currentTransition.color, 0);

        // packet[index + 1] = 0x0;
        packet[index + 2] = colorDifference.r;
        packet[index + 3] = colorDifference.g;
        packet[index + 4] = colorDifference.b;
        // packet[index + 5] = 0;
        
        /// Separates the duration into two bytes
        packet[index + 6] = currentTransition.duration & 0x00ff;
        packet[index + 7] = (currentTransition.duration & 0xff00) >> 8;
        
        totalDuration += currentTransition.duration;
    } else {
        uint8_t size = keyEffect->getTransitionSize();
        for (uint8_t i = 0; i < size; i++) {
            KeyTransition *currentTransition = (keyEffect->getTransitions() + i);
            index = (8 * i) + 2;
            
            if (i == 0) {
                packet[index] = keyEffect->getEffectId();
            } else {
                packet[index] = i;
            }
            
            /// Calculate color difference
            RGB nextColor  = (i + 1 < size) ? (*(currentTransition + 1)).color : keyEffect->getStartColor();
            RGB colorDifference = calculateColorDelta(currentTransition->color, nextColor, currentTransition->duration);

            // packet[index + 1] = 0x0;
            packet[index + 2] = colorDifference.r;
            packet[index + 3] = colorDifference.g;
            packet[index + 4] = colorDifference.b;
            // packet[index + 5] = 0;
            
            /// Separates the duration into two bytes
            packet[index + 6] = currentTransition->duration & 0x00ff;
            packet[index + 7] = (currentTransition->duration & 0xff00) >> 8;
            
            totalDuration += currentTransition->duration;
        }
    }
    
    /// Set starting color, each value will have 2 bytes
    index = 0x84;
    
    RGB startColor = keyEffect->getStartColor();
    packet[index + 0] = (startColor.r & 0x0f) << 4;
    packet[index + 1] = (startColor.r & 0xf0) >> 4;
    packet[index + 2] = (startColor.g & 0x0f) << 4;
    packet[index + 3] = (startColor.g & 0xf0) >> 4;
    packet[index + 4] = (startColor.b & 0x0f) << 4;
    packet[index + 5] = (startColor.b & 0xf0) >> 4;
    
    /// Separator
    index = 0x8a;
    packet[index + 0] = 0xff;
    // packet[index + 1] = 0x0;
    
    /// WaveMode
    if (keyEffect->isWaveModeActive()) {
        index = 0x8c;
        KeyPoint point = keyEffect->getWaveOrigin();
        /// Split the points into two separate bytes
        packet[index + 0] = point.x & 0x00ff;
        packet[index + 1] = (point.x & 0xff00) >> 8;
        packet[index + 2] = point.y & 0x00ff;
        packet[index + 3] = (point.y & 0xff00) >> 8;
        
        /// Radiation Control
        index = 0x90;
        WaveRadControl control = keyEffect->getWaveRadControl();
        switch (control) {
            case XY:
                packet[index + 0] = 0x01;
                packet[index + 1] = 0x0;
                packet[index + 2] = 0x01;
                packet[index + 3] = 0x0;
                
            case X:
                packet[index + 0] = 0x01;
                packet[index + 1] = 0x0;
                // packet[index + 2] = 0x0;
                // packet[index + 3] = 0x0;
                break;
            case Y:
                // packet[index + 0] = 0x0;
                // packet[index + 1] = 0x0;
                packet[index + 2] = 0x01;
                packet[index + 3] = 0x0;
                
        }
        
        /// Wave Length
        index = 0x94;
        packet[index + 0] = keyEffect->getWaveLength() & 0x00ff;
        packet[index + 1] = (keyEffect->getWaveLength() & 0xff00) >> 8;
    }
    
    /// Transition count
    index = 0x96;
    packet[index + 0] = keyEffect->getTransitionSize();
    
    /// Total effect length
    index = 0x98;
    packet[index + 0] = totalDuration & 0x00ff;
    packet[index + 1] = (totalDuration & 0xff00) >> 8;

    /// Wave Direction
    index = 0x9a;
    packet[index] = keyEffect->getWaveDirection();
}

IOReturn SSKeyboard::exit() {
    if (hidManagerRef) {
        IOHIDManagerClose(hidManagerRef, kIOHIDOptionsTypeNone);
        CFRelease(hidManagerRef);
        hidManagerRef = NULL;
    }
    
    return kIOReturnSuccess;
}
void SSKeyboard::setSleepInMillis(uint16_t millis) {
    times = millis;
}

KeyboardModels SSKeyboard::getKeyboardModel(){
    return model;
}

uint8_t SSKeyboard::findRegionOfKey(uint8_t findThisKey) {
    if (model != ThreeRegion) {
        for (uint8_t key = 0; key < kModifiersSize; key++) {
            if (modifiers[key] == findThisKey ) {
                return regions[0];
            }
        }
        
        for (uint8_t key = 0; key < kAlphanumsSize; key++) {
            if (alphanums[key] == findThisKey ) {
                return regions[1];
            }
        }
        
        for (uint8_t key = 0; key < kEnterSize; key++) {
            if (enter[key] == findThisKey ) {
                return regions[2];
            }
        }
        
        for (uint8_t key = 0; key < kSpecialPerKeySize; key++) {
            if (specialPerKey[key] == findThisKey ) {
                return regions[3];
            }
        }

    } else {
        // TODO - need to implement ThreeRegion
    }
    return 0;
}

RGB SSKeyboard::calculateColorDelta(RGB start, RGB target, uint16_t duration) {
    /// Depending on the duration of the transition it will calculate the delta of the color
    if (duration  < 32) {
        duration = 32;
    }
    
    uint8_t divisible = duration / 16;
    uint8_t deltaR = (target.r - start.r) / divisible;
    uint8_t deltaG = (target.g - start.g) / divisible;
    uint8_t deltaB = (target.b - start.b) / divisible;

    return {deltaR, deltaG, deltaB};
}
