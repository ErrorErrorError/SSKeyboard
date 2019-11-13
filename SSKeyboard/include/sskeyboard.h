//
//  sskeyboard.h
//  SSKeyboardTest'
//
//  Created by Erik Bautista on 9/28/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//
#ifndef sskeyboard_h
#define sskeyboard_h
#include "sskeys.h"
#include "sseffect.h"

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <stdlib.h>

/// Package size for Feature and Output Request
constexpr uint32_t kPackageSize = 0x20c;   /// 524
constexpr uint8_t kOutputPackageSize = 0x40;    /// 64

/// Per-Key & GS65 Matching
constexpr uint32_t kVendorId = 0x1038;  /// 4152
constexpr uint32_t kProductId = 0x1122;  /// 4386
constexpr uint32_t kMaxFeatureReportSize = 0x20c;   /// 524
constexpr uint32_t kPrimaryUsagePage = 0xffc0;  /// 65472

/// GS65 Matching
constexpr uint32_t kGS65VersionNumber = 0x229;  /// 553

/// Per-Key Matching
constexpr uint32_t kPerKeyVersionNumber = 0x230;   /// 560

/// TODO Three Region Matching

/// Model of keyboard
enum KeyboardModels {
    PerKey,
    PerKeyGS65,
    ThreeRegion,
    UnknownModel
};

#ifdef __cplusplus

class SSKeyboard {

private:
    IOHIDManagerRef hidManagerRef;
    IOHIDDeviceRef keyboardDevice;
    enum KeyboardModels model = UnknownModel;
    IOReturn sendFeatureReportPackage(uint8_t *featurePackage);
    IOReturn sendOutputReportPackage(uint8_t *outputPackage);
    void makeColorPackage(Keys *colorArray, uint8_t *packet);
    void makeOutputPackage(uint8_t region, uint8_t *packet);
    void makeEffectPackage(KeyEffect *keyEffect, uint8_t *packet);
    IOReturn checkForDevice(CFDictionaryRef matchingCFDictRef);
    IOReturn checkThreeRegion();
    RGB calculateColorDelta(RGB start, RGB target, uint16_t duration);
public:
    /// This constructor initializes the IOHIDManager and attempts to find the MSI RGB Keyboard's usb hid port. If it's not found, then the keyboard model is unknown.
    SSKeyboard();
    /// This method returns the keyboard model of the device.
    /// @return The model of the keyboard. If the constructor couldn't find a model, it returns UnknownModel.
    KeyboardModels getKeyboardModel();
    /// This method creates a feature package to send to the usb hid keyboard device.
    /// @discussion each key contains 12 bytes of information (except for the region key since it contains 14) and every byte can go from 0-255.
    /// @warning Your array must include the region key in the beginning, then the keys must be in the same order as how the "modifiers, alphanums, enter, or special" are placed.
    /// @param keysArray an array of keys that are in order according to each region. It must include the region key before the other keys
    /// @param updateKeys a bool which decides whether or not to update the keyboard.
    /// @return kIOReturnSuccess if it's successful.
    IOReturn sendColorKeys(Keys *keysArray, bool updateKeys);
    
    /// This method receives an effect and converts it into a packet to be sent to the usb device.
    /// @param keyEffect the effect to send
    /// @param updateKeys restart the keyboard
    IOReturn sendEffectKeys(KeyEffect *keyEffect, bool updateKeys);
    
    /// Sets the time of each package in millis.
    /// @param millis time in millis
    void setSleepInMillis(uint16_t millis);
    /// Closes hidmanager and keyboardDevice
    IOReturn exit();
    /// This method finds the region of a key.
    /// @param findThisKey the key to search which region it's located.
    /// @return The region of the key.
    uint8_t findRegionOfKey(uint8_t findThisKey);
};
#endif
#endif
