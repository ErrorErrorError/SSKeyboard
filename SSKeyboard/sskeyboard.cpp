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
    
    IOHIDManagerOpen(hidManagerRef, kIOHIDOptionsTypeNone);
    
    CFSetRef deviceSetRef = IOHIDManagerCopyDevices(hidManagerRef);
    
    //If returns null, then couldn't find device with the matching dictionary
    if (deviceSetRef == nullptr) {
        return kIOReturnNotFound;
    }
    
    CFIndex deviceCount = CFSetGetCount(deviceSetRef);
    
    
    IOHIDDeviceRef *deviceRef = (IOHIDDeviceRef *) malloc( sizeof( IOHIDDeviceRef ) * deviceCount);
     
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
    free(deviceRef);
    
    IOReturn openSuccessful = IOHIDDeviceOpen(keyboardDevice, kIOHIDOptionsTypeNone);
    if (openSuccessful != kIOReturnSuccess) {
        printf("Could not open usb port");
        usbOpen = false;
        return kIOReturnNotOpen;
    }
    
    usbOpen = true;
    return kIOReturnSuccess;
}

// not implemented since I still haven't had time to read values of three region based keyboard
IOReturn SSKeyboard::checkThreeRegion() {
    return kIOReturnError;
}

IOReturn SSKeyboard::sendFeatureReportPackage(uint8_t *featurePackage) {
    return (usbOpen)? IOHIDDeviceSetReport(keyboardDevice, kIOHIDReportTypeFeature, featurePackage[0], featurePackage, kPackageSize) : kIOReturnNotOpen;
}

IOReturn SSKeyboard::sendOutputReportPackage(uint8_t *outputPackage) {
    return (usbOpen)? IOHIDDeviceSetReport(keyboardDevice, kIOHIDReportTypeOutput, outputPackage[0], outputPackage, kOutputPackageSize) : kIOReturnNotOpen;
}

uint8_t *SSKeyboard::makeOutputPackage(uint8_t region) {
    uint8_t lastByteRegion = 0;
    uint8_t index = 0;
    while (lastByteRegion == 0 && index < kRegions) {
        if (regions[index] == region) {
            lastByteRegion = last_byte_region[index];
        } else {
            index++;
        }
    }
    
    static uint8_t newOutputPackage[kOutputPackageSize];
    newOutputPackage[0] = 0x0d;
    newOutputPackage[1] = 0x00;
    newOutputPackage[2] = 0x02;
    newOutputPackage[kOutputPackageSize - 1] = lastByteRegion;
    return newOutputPackage;
}

bool SSKeyboard::isUsbOpen() {
    return usbOpen;
}

IOReturn SSKeyboard::sendColorKeys(Keys **keysArray, bool createOutputPacket) {
    if (!usbOpen) {
        return kIOReturnNotOpen;
    }
    // printf("newPackage\n");
    uint8_t *package = makeColorPackage(keysArray);
    IOReturn setFeatureReturn = sendFeatureReportPackage(package);
    std::this_thread::sleep_for(chrono::milliseconds(times));
    
    if (setFeatureReturn != kIOReturnSuccess) {
        return kIOReturnError;
    }
    
    if (createOutputPacket) {
        Keys *region = (*(keysArray + 0));
        IOReturn setOutputReport = sendOutputReportPackage(makeOutputPackage(region->region));
        std::this_thread::sleep_for(chrono::milliseconds(times));

        if (setOutputReport != kIOReturnSuccess) {
            return kIOReturnError;
        }
    }
    
    return kIOReturnSuccess;
}

uint8_t *SSKeyboard::makeColorPackage(Keys **colorArray) {
    uint8_t keycodes_size;
    Keys *region = (*(colorArray + 0));

    if (region->region == regions[0])
    {
        keycodes_size = kModifiersSize;
    }
    
    else if (region->region == regions[1])
    {
        keycodes_size = kAlphanumsSize;
    }
    
    else if (region->region == regions[2])
    {
        keycodes_size = kEnterSize;
    }
    
    else if (model == PerKeyGS65)
    {
        keycodes_size = kSpecialSize;
    } else {
        keycodes_size = kSpecialPerKeySize;
    }
    // This array includes the region key
    keycodes_size += 1;

    uint8_t mode_id;
    if (region->getMode() == Steady) {
        mode_id = 1;
    } else if (region->getMode() == Reactive) {
        mode_id = 8;
    } else {
        mode_id = 0;
    }
    
    // This calculates the duration into two bytes
    uint8_t *duration = new uint8_t[2];
    toByte(region->getDuration(), duration);
    
    memset(new_packet, 0, kPackageSize);
    
    RGB regionColor = region->getMainColor();
    RGB regionActiveColor = region->getActiveColor();
    new_packet[0] = 0x0e;
    new_packet[1] = 0x00;
    new_packet[2] = region->region;
    //new_packet[3] = 0x00;
    new_packet[4] = regionColor.r;
    new_packet[5] = regionColor.g;
    new_packet[6] = regionColor.b;
    new_packet[7] = regionActiveColor.r;
    new_packet[8] = regionActiveColor.g;
    new_packet[9] = regionActiveColor.b;
    new_packet[10] = duration[0];
    new_packet[11] = duration[1];
    new_packet[12] = mode_id;
    new_packet[13] = region->getMode();

    for (uint8_t i = 1; i < keycodes_size; i++)
    {
    
        Keys *currentKey = (*(colorArray + i));
        uint16_t keys = 2 + (12 * i);
        if (currentKey->getMode() == Steady) {
            mode_id = 1;
        } else if (currentKey->getMode() == Reactive) {
            mode_id = 8;
        } else {
            mode_id = 0;
        }
        toByte(currentKey->getDuration(), duration);
        
        //new_packet[keys] = 0x00;
        new_packet[keys + 1] = currentKey->keycode;
        new_packet[keys + 2] = currentKey->getMainColor().r;
        new_packet[keys + 3] = currentKey->getMainColor().g;
        new_packet[keys + 4] = currentKey->getMainColor().b;
        new_packet[keys + 5] = currentKey->getActiveColor().r;
        new_packet[keys + 6] = currentKey->getActiveColor().g;
        new_packet[keys + 7] = currentKey->getActiveColor().b;
        new_packet[keys + 8] = duration[0];
        new_packet[keys + 9] = duration[1];
        new_packet[keys + 10] = mode_id;
        new_packet[keys + 11] = currentKey->getMode();
    }
    
    free(duration);
    return new_packet;
}

uint8_t twos_complement(uint8_t b)
{
    Byte TwosComplement = ~b + 2;
    return TwosComplement;
}


IOReturn SSKeyboard::closeKeyboardPort() {
    if (!usbOpen) {
        return kIOReturnNotOpen;
    }
    
    IOHIDDeviceClose(keyboardDevice, kIOHIDOptionsTypeNone);
    CFRelease(keyboardDevice);
    
    return kIOReturnSuccess;
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

uint8_t SSKeyboard::findKeyInRegion(uint8_t findThisKey) {
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


void SSKeyboard::toByte(uint16_t speed, uint8_t * array) {
    uint8_t first = 0;
    uint8_t second = 0;
    for (int i = 0; i < 4; i++) {
        if (i < 2) {
            second += (speed >> (8 + (8 * i)) & 0xff);
        } else {
            first += (speed >> (8 + (8 * i)) & 0xff);
        }
    }
    
    (*(array)) = first;
    (*(array + 1)) = second;
}
