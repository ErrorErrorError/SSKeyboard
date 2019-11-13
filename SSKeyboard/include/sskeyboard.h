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
#define kPackageSize            0x20c   /// 524
#define kOutputPackageSize      0x40    /// 64

/// Per-Key & GS65 Matching
#define kVendorId               0x1038  /// 4152
#define kProductId              0x1122  /// 4386
#define kMaxFeatureReportSize   0x20c   /// 524
#define kPrimaryUsagePage       0xffc0  /// 65472

/// GS65 Matching
#define kGS65VersionNumber      0x229   /// 553

/// Per-Key Matching
#define kPerKeyVersionNumber    0x230   /// 560

/// Three Region Matching
#define kVendorIdThreeRegion    0x1770  /// 6000
#define kProductIdThreeRegion   0xff00  /// 65280
#define kMaxFeatureThreeRegion  0x101   /// 257
#define kPrimaryUsageThree      0xffa0  /// 65440
#define kThreeRegionVersion     0x110   /// 272

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
    /// @discussion Each key contains 12 bytes of information and every byte can go from 0-255.
    /// @warning This method only works on PerKey and PerKeyGS65 models. Also, your array must include the region key in the beginning, then the keys must be in the same order as how the "modifiers, alphanums, enter, or special" are placed.
    /// @param keysArray an array of keys that are in order according to each region. It must include the region key before the other keys
    /// @param updateKeys a bool which decides whether or not to update the keyboard.
    /// @return kIOReturnSuccess if it's successful.
    IOReturn sendColorKeys(Keys *keysArray, bool updateKeys);
    
    /// This method receives an effect and converts it into a packet to be sent to the usb device.
    /// @discussion Each effect should have at least one transition. Depending on each transition duration, it will calculate how fast should the colors change.
    /// @warning This method only works on PerKey and PerKeyGS65 models. Make sure you have at least one transition key. If you have less than one transition, it can cause abnormal behavior in the keyboard.
    /// @param keyEffect the effect to send to the keyboard.
    /// @param updateKeys whether it should restart the keyboard
    IOReturn sendEffect(KeyEffect *keyEffect, bool updateKeys);
    
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
