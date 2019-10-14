//
//  sskeyboard.h
//  SSKeyboardTest'
//
//  Created by Erik Bautista on 9/28/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <stdlib.h>

// Package size for Feature and Output Request
#define kFeaturePackageSize     0x20c   // 524
#define kOutputPackageSize      0x40    // 64

#define kRegions                0x04    // 4

// Size of keys - GS65
#define kModifiersSize          0x18    // 24
#define kAlphanumsSize          0x2a    // 42
#define kEnterSize              0xb     // 11
#define kSpecialSize            0x13    // 19

// Size of keys - PerKey
#define kSpecialPerKeySize      0x24    // 36

// Per-Key & GS65 Matching
#define kVendorId               0x1038  // 4152
#define kProductId              0x1122  // 4386
#define kMaxFeatureReportSize   0x20c   // 524
#define kPrimaryUsagePage       0xffc0  // 65472

// GS65 Matching
#define kGS65VersionNumber      0x229   // 553

// Per-Key Matching
#define kPerKeyVersionNumber    0x230   // 560

// TODO Three Region Matching

#ifdef __cplusplus
extern "C" {
#endif
    
struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
    
struct Keys {
    uint8_t key;
    char * keyLetter;
    uint8_t region;
    struct RGB color;
    uint8_t mode;
};
    
#ifdef __cplusplus
}
#endif

// Model of keyboard
enum KeyboardModels {
    PerKey,
    PerKeyGS65,
    ThreeRegion,
    UnknownModel
};

// Modes for PerKey/GS65
enum PerKeyModes {
    Steady,
    ColorShift,
    Breathing,
    Reactive,
    Disabled
};

// GS65 & PerKey Region keys
static uint8_t regions[kRegions] = {
    0x18,   // esc (modifiers)
    0x2a,   // a (alphanums)
    0x0b,   // enter (enter)
    0x24    // f7 (special/numpad)
};

static uint8_t last_byte_region[kRegions] = {
    0x2d,   // last byte modifier
    0x08,   // last byte alphanums
    0x87,   // last byte enter
    0x44    // last byte special
};

// GS65 & PerKey modifier keys
static uint8_t modifiers[kModifiersSize] = {
    0x29,   // backspace
    0x2a,   // tab
    0x2b,   //  spacebar
    0x2c,   // -
    0x2d,   // =
    0x2e,   // [
    0x2f,   // ]
    0x30,   // ;
    0x33,   // '
    0x34,   // ~
    0x35,   // ,
    0x36,   // .
    0x37,   // /
    0x38,   // capslock
    0x39,   // NULL
    0x65,   // lctrl
    0xe0,   // lshift
    0xe1,   // lalt
    0xe2,   // win key
    0xe3,   // rctrl
    0xe4,   // rshift
    0xe5,   // ralt
    0xe6,   // fn
    0xf0};  // NULL

// GS65 and PerKey alphanum keys
static uint8_t alphanums[kAlphanumsSize] = {
    0x04,   // b
    0x05,   // c
    0x06,   // d
    0x07,   // e
    0x08,   // f
    0x09,   // g
    0x0a,   // h
    0x0b,   // i
    0x0c,   // j
    0x0d,   // k
    0x0e,   // l
    0x0f,   // m
    0x10,   // n
    0x11,   // o
    0x12,   // p
    0x13,   // q
    0x14,   // r
    0x15,   // s
    0x16,   // t
    0x17,   // u
    0x18,   // v
    0x19,   // w
    0x1a,   // x
    0x1b,   // y
    0x1c,   // z
    0x1d,   // 1
    0x1e,   // 2
    0x1f,   // 3
    0x20,   // 4
    0x21,   // 5
    0x22,   // 6
    0x23,   // 7
    0x24,   // 8
    0x25,   // 9
    0x26,   // 0
    0x27,   // f1
    0x3a,   // f2
    0x3b,   // f3
    0x3c,   // f4
    0x3d,   // f5
    0x3e,   // f6
    0x3f};  // NULL

// GS65 and PerKey - Enter Keys
static uint8_t enter[kEnterSize] = {
    0x28,   // backslash
    0x31,   // NULL
    0x32,   // backslash (next to spacebar)
    0x64,   // NULL
    0x87,   // NULL
    0x88,   // NULL
    0x89,   // NULL
    0x8A,   // NULL
    0x8B,   // NULL
    0x90,   // NULL
    0x91};  // NULL

// GS65 Special keys
static uint8_t special[kSpecialSize] = {
    0x40,   // F8
    0x41,   // F9
    0x42,   // F10
    0x43,   // F11
    0x44,   // F12
    0x45,   // prtscr
    0x46,   // NULL
    0x47,   // NULL
    0x48,   // NULL
    0x49,   // home
    0x4a,   // pgup
    0x4b,   // del (insert)
    0x4c,   // end
    0x4d,   // pgdn
    0x4e,   // right arrow
    0x4f,   // left arrow
    0x50,   // down arrow
    0x51,   // up arrow
    0x52};  // NULL

// Similar to GS65 special keys, except that it has 17 more keys (since it has numpad)
static uint8_t specialPerKey[kSpecialPerKeySize] = {
    0x40,   // F8
    0x41,   // F9
    0x42,   // F10
    0x43,   // F11
    0x44,   // F12
    0x45,   // prtscr
    0x46,   // scroll lock
    0x47,   // pause/break
    0x48,   // insert
    0x49,   // NULL
    0x4a,   // pgup
    0x4b,   // delete
    0x4c,   // NLL
    0x4d,   // pgdn
    0x4e,   // right arrow
    0x4f,   // left arrow
    0x50,   // down arrow
    0x51,   // up arrow
    0x52,   // num lock
    0x53,   // / - numpad
    0x54,   // * - numpad
    0x55,   // minus - numpad
    0x56,   // plus - numpad
    0x57,   // enter - numpad
    0x58,   // 1 - numpad
    0x59,   // 2 - numpad
    0x5a,   // 3 - numpad
    0x5b,   // 4 - numpad
    0x5c,   // 5 - numpad
    0x5d,   // 6 - numpad
    0x5e,   // 7 - numpad
    0x5f,   // 8 - numpad
    0x60,   // 9 - numpad
    0x61,   // 0 - numpad
    0x62,   // . - numpad
    0x63    // NULL
};

#ifdef __cplusplus

class SSKeyboard {

private:
    IOHIDManagerRef hidManagerRef;
    IOHIDDeviceRef keyboardDevice;
    enum KeyboardModels model = UnknownModel;
    uint8_t new_packet[kFeaturePackageSize];
    bool usbOpen = false;
    IOReturn sendFeatureReportPackage(uint8_t *featurePackage);
    IOReturn sendOutputReportPackage(uint8_t *outputPackage);
    uint8_t *makeColorPackage(uint8_t region, RGB color, RGB *colorArray);
    uint8_t *makeOutputPackage(uint8_t region);
    IOReturn checkForDevice(CFDictionaryRef matchingCFDictRef);
    IOReturn checkThreeRegion();
public:
    SSKeyboard();
    bool isUsbOpen();
    KeyboardModels getKeyboardModel();
    IOReturn setSteadyMode(uint8_t region, RGB regionColor , RGB *colorArray, bool createOutputPacket);
    IOReturn closeKeyboardPort();
    void setSleepInMillis(uint16_t millis);
    IOReturn exit();
    uint8_t findKeyInRegion(uint8_t findThisKey);
};
#endif
