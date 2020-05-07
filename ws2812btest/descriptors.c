//#include "pic24fj.h"
#include "usb.h"
#include <stdint.h>

uint8_t __attribute__ ((space(auto_psv))) Device[] = {
    0x12,       // bLength
    DEVICE,     // bDescriptorType
    0x00,       // bcdUSB (low byte)
    0x02,       // bcdUSB (high byte)
    0x00,       // bDeviceClass
    0x00,       // bDeviceSubClass
    0x00,       // bDeviceProtocol
    MAX_PACKET_SIZE,    // bMaxPacketSize
    0x66,       // idVendor (low byte)
    0x66,       // idVendor (high byte)
    0x03,       // idProduct (low byte)
    0x00,       // idProduct (high byte)
    0x00,       // bcdDevice (low byte)
    0x00,       // bcdDevice (high byte)
    0x01,       // iManufacturer
    0x02,       // iProduct
    0x00,       // iSerialNumber (none)
    NUM_CONFIGURATIONS    // bNumConfigurations
};

uint8_t __attribute__ ((space(auto_psv))) Configuration1[] = {
    0x09,       // bLength
    CONFIGURATION,    // bDescriptorType
    0x12,       // wTotalLength (low byte)
    0x00,       // wTotalLength (high byte)
    NUM_INTERFACES,   // bNumInterfaces
    0x01,       // bConfigurationValue
    0x00,       // iConfiguration (none)
    0xA0,       // bmAttributes
    0x32,       // bMaxPower (100 mA)
    0x09,       // bLength (Interface1 descriptor starts here)
    INTERFACE,  // bDescriptorType
    0x00,       // bInterfaceNumber
    0x00,       // bAlternateSetting
    0x00,       // bNumEndpoints (excluding EP0)
    0xFF,       // bInterfaceClass (vendor specific class code)
    0x00,       // bInterfaceSubClass
    0xFF,       // bInterfaceProtocol (vendor specific protocol used)
    0x00        // iInterface (none)
};

uint8_t __attribute__ ((space(auto_psv))) *Configurations[] = {
    Configuration1
};

uint8_t __attribute__ ((space(auto_psv))) String0[] = {
    4,          // bLength
    STRING,     // bDescriptorType
    0x09,       // wLANGID[0] (low byte)
    0x04        // wLANGID[0] (high byte)
};

uint8_t __attribute__ ((space(auto_psv))) String1[] = {
    26,         // bLength
    STRING,     // bDescriptorType
    'O', 0, 'l', 0, 'i', 0, 'n', 0, ' ', 0, 
    'C', 0, 'o', 0, 'l', 0, 'l', 0, 'e', 0, 'g', 0, 'e', 0
};

uint8_t __attribute__ ((space(auto_psv))) String2[] = {
    32,         // bLength
    STRING,     // bDescriptorType
    'P', 0, 'I', 0, 'C', 0,
    '2', 0, '4', 0, 'F', 0, 'J', 0, 
    '1', 0, '2', 0, '8', 0, 'G', 0, 'B', 0, '2', 0, '0', 0, '6', 0
};

uint8_t __attribute__ ((space(auto_psv))) *Strings[] = {
    String0, 
    String1, 
    String2
};

