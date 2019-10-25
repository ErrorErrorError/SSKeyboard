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
    if (vendorCFNumberRef) {
        CFDictionarySetValue(result, CFSTR(kIOHIDVendorIDKey), vendorCFNumberRef);
        CFRelease(vendorCFNumberRef);
    }
    
    CFNumberRef productCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &productID);
    if (productCFNumberRef) {
        CFDictionarySetValue(result, CFSTR(kIOHIDProductIDKey), productCFNumberRef);
        CFRelease(productCFNumberRef);
    }

    CFNumberRef reportCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &maxFeatureReport);
    if (reportCFNumberRef) {
        CFDictionarySetValue(result, CFSTR(kIOHIDMaxFeatureReportSizeKey), reportCFNumberRef);
        CFRelease(reportCFNumberRef);
    }
    
    CFNumberRef usagePageCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &usagePage);
    if (usagePageCFNumberRef) {
        CFDictionarySetValue(result, CFSTR(kIOHIDPrimaryUsagePageKey), usagePageCFNumberRef);
        CFRelease(usagePageCFNumberRef);
    }
    
    CFNumberRef versionNumberCFNumberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &versionNumber);
    if (versionNumberCFNumberRef) {
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
    
    IOHIDManagerScheduleWithRunLoop(hidManagerRef, CFRunLoopGetMain(), kCFRunLoopDefaultMode);

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
    while (lastByteRegion == 0 && index < kRegions) {
        if (regions[index] == region) {
            lastByteRegion = last_byte_region[index];
        } else {
            index++;
        }
    }
    
    packet[0] = 0x0d;
    packet[1] = 0x00;
    packet[2] = 0x02;
    packet[kOutputPackageSize - 1] = lastByteRegion;
}

IOReturn SSKeyboard::sendColorKeys(Keys **keysArray, bool updateKeys) {
    uint8_t *packet = new uint8_t[kPackageSize]{0};
    makeColorPackage(keysArray, packet);
    IOReturn setFeatureReturn = sendFeatureReportPackage(packet);
    
    delete[] packet;
    
    if (setFeatureReturn != kIOReturnSuccess) {
        return setFeatureReturn;
    }
    
    if (updateKeys) {
        Keys *region = (*(keysArray));
        packet = new uint8_t[kOutputPackageSize]{0};
        makeOutputPackage(region->region, packet);
        IOReturn setOutputReport = sendOutputReportPackage(packet);
        
        delete[] packet;
        
        if (setOutputReport != kIOReturnSuccess) {
            return setOutputReport;
        }
    }
    
    return kIOReturnSuccess;
}

void SSKeyboard::makeColorPackage(Keys **colorArray, uint8_t *new_packet) {
    uint8_t keycodes_size;
    // Region key should be the first key in the array
    Keys *regionKey = (*(colorArray));
    uint8_t mode;

    if (regionKey->region == regions[0]) {
        keycodes_size = kModifiersSize;
    } else if (regionKey->region == regions[1]) {
        keycodes_size = kAlphanumsSize;
    } else if (regionKey->region == regions[2]) {
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
        Keys *currentKey = (*(colorArray + i));
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
            new_packet[0]       = 0x0e;
            new_packet[1]       = 0x00;
            new_packet[keys]    = currentKey->keycode;
            new_packet[keys+1]  = 0x00;
        } else {
            new_packet[keys]    = 0x00;
            new_packet[keys+1]  = currentKey->keycode;
        }
        
        new_packet[keys + 2]    = currentKey->getMainColor().r;
        new_packet[keys + 3]    = currentKey->getMainColor().g;
        new_packet[keys + 4]    = currentKey->getMainColor().b;
        new_packet[keys + 5]    = currentKey->getActiveColor().r;
        new_packet[keys + 6]    = currentKey->getActiveColor().g;
        new_packet[keys + 7]    = currentKey->getActiveColor().b;
        // Separates the duration into two bytes
        new_packet[keys + 8]    = currentKey->duration & 0x00ff;
        new_packet[keys + 9]    = (currentKey->duration & 0xff00) >> 8;
        new_packet[keys + 10]   = currentKey->effect_id;
        new_packet[keys + 11]   = mode;
    }
    
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
