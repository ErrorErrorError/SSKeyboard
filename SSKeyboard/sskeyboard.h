//
//  sskeyboard.h
//  SSKeyboardTest'
//
//  Created by Erik Bautista on 9/28/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

// #ifndef sskeyboard_h
// #define sskeyboard_h

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <stdlib.h>

#define kPackageSize    0x20c
#define kOutputPackageSize 0x40
#define kInputPackageSize 0x08

#define kRegions        0x04

// Size of keys
#define kModifiersSize  0x18
#define kAlphanumsSize  0x2a
#define kEnterSize      0xb
#define kSpecialSize    0x13

// Null Values
#define kNullModifiers  0x02
#define kNullAlphanums  0x01
#define kNullEnter      0x09
#define kNullSpecial    0x04

// GS65 Matching
#define kVendorId       0x1038
#define kProductId      0x1122
#define kMaxFeatureReportSize 0x20c
#define kPrimaryUsagePage 0xffc0

#ifdef __cplusplus
extern "C" {
#endif

uint8_t findKeyInRegion(uint8_t findThisKey);

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#ifdef __cplusplus
}
#endif

enum KeyboardModels {
    PerKey,
    PerKeyGS65,
    ThreeRegion,
    UnknownModel
};

static uint8_t regions[kRegions] = {
    0x18,   // esc (modifiers)
    0x2a,   // a (alphanums)
    0x0b,   // enter (enter)
    0x13};  // f7 (special/numpads)

static uint8_t last_byte_region[kRegions] = {
    0x2d,   // last byte modifier
    0x08,   // last byte alphanums
    0x87,   // last byte enter
    0x44    // last byte special
};

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
/*
static uint8_t null_modifiers[kNullModifiers] = {
    0x39,   // NULL
    0xf0};  // NULL

static uint8_t null_alphanums[kNullAlphanums] = {
    0x3f};  // NULL

static uint8_t null_enter[kNullEnter] = {
    0x31,   // NULL
    0x64,   // NULL
    0x87,   // NULL
    0x88,   // NULL
    0x89,   // NULL
    0x8A,   // NULL
    0x8B,   // NULL
    0x90,   // NULL
    0x91};  // NULL
static uint8_t null_special[kNullSpecial] = {
    0x46,   // NULL
    0x47,   // NULL
    0x48,   // NULL
    0x52};  // NULL
 */

// #ifdef  __cplusplus

#pragma mark - function
#ifdef __cplusplus

class SSKeyboard {

private:
    IOHIDManagerRef hidManagerRef;
    IOHIDDeviceRef keyboardDevice;
    enum KeyboardModels model = UnknownModel;
    uint8_t new_packet[kPackageSize];
    bool usbOpen = false;
    IOReturn sendFeatureReportPackage(uint8_t *featurePackage);
    IOReturn sendOutputReportPackage(uint8_t *outputPackage);
    uint8_t *makeColorPackage(uint8_t region, RGB color, RGB *colorArray);
    uint8_t *makeOutputPackage(uint8_t region);
    IOReturn checkPerKeyGS65();
    IOReturn checkPerKey();
    IOReturn checkThreeRegion();
public:
    SSKeyboard();
    bool isUsbOpen();
    KeyboardModels getKeyboardModel();
    IOReturn setSteadyMode(uint8_t region, RGB regionColor , RGB *colorArray, bool createOutputPacket);
    IOReturn closeKeyboardPort();
    IOReturn exit();
};
#endif


#pragma mark - c wrapper
// #endif /* sskeyboard_h */
