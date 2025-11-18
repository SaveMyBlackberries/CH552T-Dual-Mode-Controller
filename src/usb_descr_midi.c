// ===================================================================================
// USB MIDI Descriptors - Audio Class Implementation
// Based on USB Audio Device Class v1.0 and USB MIDI v1.0
// ===================================================================================

#include "usb_descr_midi.h"

// ===================================================================================
// Device Descriptor (共用，HID和MIDI都一样)
// ===================================================================================
__code USB_DEV_DESCR DevDescr_MIDI = {
  .bLength            = sizeof(USB_DEV_DESCR),
  .bDescriptorType    = USB_DESCR_TYP_DEVICE,
  .bcdUSB             = 0x0110,                 // USB 1.1
  .bDeviceClass       = 0,                      // 在接口描述符中定义
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = EP0_SIZE,
  .idVendor           = USB_VENDOR_ID,
  .idProduct          = USB_PRODUCT_ID,
  .bcdDevice          = USB_DEVICE_VERSION,
  .iManufacturer      = 1,
  .iProduct           = 2,
  .iSerialNumber      = 3,
  .bNumConfigurations = 1
};

// ===================================================================================
// Configuration Descriptor + Audio Control + MIDI Streaming
// ===================================================================================
__code USB_CFG_DESCR_MIDI CfgDescr_MIDI = {

  // ===== Configuration Descriptor =====
  .config = {
    .bLength            = sizeof(USB_CFG_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_CONFIG,
    .wTotalLength       = sizeof(CfgDescr_MIDI),
    .bNumInterfaces     = 2,                    // 2个接口：Audio Control + MIDI Streaming
    .bConfigurationValue= 1,
    .iConfiguration     = 0,
    .bmAttributes       = 0x80,                 // Bus-powered
    .MaxPower           = USB_MAX_POWER_mA / 2
  },

  // ===== Interface 0: Audio Control (AC) =====
  .ac_interface = {
    .bLength            = sizeof(USB_ITF_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber   = 0,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 0,                    // AC接口没有端点
    .bInterfaceClass    = USB_DEV_CLASS_AUDIO,  // Audio Class
    .bInterfaceSubClass = 0x01,                 // Audio Control
    .bInterfaceProtocol = 0,
    .iInterface         = 0
  },

  // ===== Class-Specific AC Interface Descriptor =====
  .ac_cs_interface = {
    .bLength            = 9,
    .bDescriptorType    = USB_DESCR_TYP_CS_INTF, // 0x24
    .bDescriptorSubtype = 0x01,                   // HEADER subtype
    .bcdADC             = 0x0100,                 // Audio Device Class 1.0
    .wTotalLength       = 9,                      // AC接口描述符总长度
    .bInCollection      = 1,                      // 1个MIDI Streaming接口
    .baInterfaceNr      = 1                       // Interface 1
  },

  // ===== Interface 1: MIDI Streaming (MS) =====
  .ms_interface = {
    .bLength            = sizeof(USB_ITF_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber   = 1,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 2,                    // 1 IN + 1 OUT
    .bInterfaceClass    = USB_DEV_CLASS_AUDIO,  // Audio Class
    .bInterfaceSubClass = 0x03,                 // MIDI Streaming
    .bInterfaceProtocol = 0,
    .iInterface         = 0
  },

  // ===== Class-Specific MS Interface Descriptor =====
  .ms_cs_interface = {
    .bLength            = 7,
    .bDescriptorType    = USB_DESCR_TYP_CS_INTF, // 0x24
    .bDescriptorSubtype = 0x01,                   // MS_HEADER subtype
    .bcdMSC             = 0x0100,                 // MIDI Streaming Class 1.0
    .wTotalLength       = 7 + 6 + 9 + 9 + 5 + 5   // 总长度41字节
  },

  // ===== MIDI IN Jack - Embedded =====
  .midi_in_jack_emb = {
    .bLength            = 6,
    .bDescriptorType    = USB_DESCR_TYP_CS_INTF, // 0x24
    .bDescriptorSubtype = 0x02,                   // MIDI_IN_JACK
    .bJackType          = 0x01,                   // EMBEDDED
    .bJackID            = 1,
    .iJack              = 0
  },

  // ===== MIDI OUT Jack - Embedded =====
  .midi_out_jack_emb = {
    .bLength            = 9,
    .bDescriptorType    = USB_DESCR_TYP_CS_INTF, // 0x24
    .bDescriptorSubtype = 0x03,                   // MIDI_OUT_JACK
    .bJackType          = 0x01,                   // EMBEDDED
    .bJackID            = 2,
    .bNrInputPins       = 1,
    .baSourceID         = 1,                      // 连接到IN Jack 1
    .baSourcePin        = 1,
    .iJack              = 0
  },

  // ===== MIDI IN Jack - External =====
  .midi_in_jack_ext = {
    .bLength            = 6,
    .bDescriptorType    = USB_DESCR_TYP_CS_INTF,
    .bDescriptorSubtype = 0x02,                   // MIDI_IN_JACK
    .bJackType          = 0x02,                   // EXTERNAL
    .bJackID            = 3,
    .iJack              = 0
  },

  // ===== MIDI OUT Jack - External =====
  .midi_out_jack_ext = {
    .bLength            = 9,
    .bDescriptorType    = USB_DESCR_TYP_CS_INTF,
    .bDescriptorSubtype = 0x03,                   // MIDI_OUT_JACK
    .bJackType          = 0x02,                   // EXTERNAL
    .bJackID            = 4,
    .bNrInputPins       = 1,
    .baSourceID         = 3,
    .baSourcePin        = 1,
    .iJack              = 0
  },

  // ===== Standard Bulk OUT Endpoint Descriptor =====
  .ms_ep_out = {
    .bLength            = sizeof(USB_ENDP_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_ENDP,
    .bEndpointAddress   = 0x01,                   // OUT endpoint 1
    .bmAttributes       = USB_ENDP_TYPE_BULK,     // Bulk传输
    .wMaxPacketSize     = 64,                     // 64字节
    .bInterval          = 0                       // Bulk端点忽略
  },

  // ===== Class-Specific MS Bulk OUT Endpoint Descriptor =====
  .ms_ep_out_cs = {
    .bLength            = 5,
    .bDescriptorType    = USB_DESCR_TYP_CS_ENDP,  // 0x25
    .bDescriptorSubtype = 0x01,                   // MS_GENERAL
    .bNumEmbMIDIJack    = 1,
    .baAssocJackID      = 1                       // 关联到Embedded IN Jack 1
  },

  // ===== Standard Bulk IN Endpoint Descriptor =====
  .ms_ep_in = {
    .bLength            = sizeof(USB_ENDP_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_ENDP,
    .bEndpointAddress   = 0x81,                   // IN endpoint 1
    .bmAttributes       = USB_ENDP_TYPE_BULK,     // Bulk传输
    .wMaxPacketSize     = 64,                     // 64字节
    .bInterval          = 0                       // Bulk端点忽略
  },

  // ===== Class-Specific MS Bulk IN Endpoint Descriptor =====
  .ms_ep_in_cs = {
    .bLength            = 5,
    .bDescriptorType    = USB_DESCR_TYP_CS_ENDP,  // 0x25
    .bDescriptorSubtype = 0x01,                   // MS_GENERAL
    .bNumEmbMIDIJack    = 1,
    .baAssocJackID      = 2                       // 关联到Embedded OUT Jack 2
  }
};

// ===================================================================================
// String Descriptors (与HID共用)
// ===================================================================================

// Language Descriptor (Index 0)
__code uint16_t LangDescr_MIDI[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(LangDescr_MIDI), 0x0409 };

// Manufacturer String Descriptor (Index 1)
__code uint16_t ManufDescr_MIDI[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ManufDescr_MIDI), MANUFACTURER_STR };

// Product String Descriptor (Index 2) - MIDI版本
__code uint16_t ProdDescr_MIDI[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ProdDescr_MIDI), PRODUCT_STR_MIDI };

// Serial String Descriptor (Index 3)
__code uint16_t SerDescr_MIDI[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(SerDescr_MIDI), SERIAL_STR };

