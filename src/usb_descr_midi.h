// ===================================================================================
// USB MIDI Descriptors - Header File
// ===================================================================================

#pragma once
#include <stdint.h>
#include "usb.h"
#include "config.h"

// ===================================================================================
// USB Endpoint Configuration
// ===================================================================================
#define EP0_SIZE        8

// 如果EP1_SIZE已被定义（HID模式），先取消定义
#ifdef EP1_SIZE
#undef EP1_SIZE
#endif
#define EP1_SIZE        64   // MIDI使用64字节Bulk端点

// ===================================================================================
// MIDI Specific Descriptor Types
// ===================================================================================
#define USB_DESCR_TYP_CS_INTF   0x24  // Class-Specific Interface
#define USB_DESCR_TYP_CS_ENDP   0x25  // Class-Specific Endpoint

// ===================================================================================
// Class-Specific AC Interface Descriptor Header
// ===================================================================================
typedef struct _USB_AC_CS_INTERFACE_DESCR {
  uint8_t  bLength;
  uint8_t  bDescriptorType;      // 0x24
  uint8_t  bDescriptorSubtype;   // 0x01 HEADER
  uint16_t bcdADC;
  uint16_t wTotalLength;
  uint8_t  bInCollection;
  uint8_t  baInterfaceNr;
} USB_AC_CS_INTERFACE_DESCR;

// ===================================================================================
// Class-Specific MS Interface Descriptor Header
// ===================================================================================
typedef struct _USB_MS_CS_INTERFACE_DESCR {
  uint8_t  bLength;
  uint8_t  bDescriptorType;      // 0x24
  uint8_t  bDescriptorSubtype;   // 0x01 MS_HEADER
  uint16_t bcdMSC;
  uint16_t wTotalLength;
} USB_MS_CS_INTERFACE_DESCR;

// ===================================================================================
// MIDI IN Jack Descriptor
// ===================================================================================
typedef struct _USB_MIDI_IN_JACK_DESCR {
  uint8_t  bLength;
  uint8_t  bDescriptorType;      // 0x24
  uint8_t  bDescriptorSubtype;   // 0x02 MIDI_IN_JACK
  uint8_t  bJackType;            // 0x01=EMBEDDED, 0x02=EXTERNAL
  uint8_t  bJackID;
  uint8_t  iJack;
} USB_MIDI_IN_JACK_DESCR;

// ===================================================================================
// MIDI OUT Jack Descriptor
// ===================================================================================
typedef struct _USB_MIDI_OUT_JACK_DESCR {
  uint8_t  bLength;
  uint8_t  bDescriptorType;      // 0x24
  uint8_t  bDescriptorSubtype;   // 0x03 MIDI_OUT_JACK
  uint8_t  bJackType;            // 0x01=EMBEDDED, 0x02=EXTERNAL
  uint8_t  bJackID;
  uint8_t  bNrInputPins;
  uint8_t  baSourceID;
  uint8_t  baSourcePin;
  uint8_t  iJack;
} USB_MIDI_OUT_JACK_DESCR;

// ===================================================================================
// Class-Specific MS Bulk Endpoint Descriptor
// ===================================================================================
typedef struct _USB_MS_ENDP_DESCR {
  uint8_t  bLength;
  uint8_t  bDescriptorType;      // 0x25
  uint8_t  bDescriptorSubtype;   // 0x01 MS_GENERAL
  uint8_t  bNumEmbMIDIJack;
  uint8_t  baAssocJackID;
} USB_MS_ENDP_DESCR;

// ===================================================================================
// Complete MIDI Configuration Descriptor Structure
// ===================================================================================
typedef struct _USB_CFG_DESCR_MIDI {
  USB_CFG_DESCR                config;              // Configuration Descriptor
  USB_ITF_DESCR                ac_interface;        // Audio Control Interface
  USB_AC_CS_INTERFACE_DESCR    ac_cs_interface;    // AC Class-Specific Interface
  USB_ITF_DESCR                ms_interface;        // MIDI Streaming Interface
  USB_MS_CS_INTERFACE_DESCR    ms_cs_interface;    // MS Class-Specific Interface
  USB_MIDI_IN_JACK_DESCR       midi_in_jack_emb;   // MIDI IN Jack Embedded
  USB_MIDI_OUT_JACK_DESCR      midi_out_jack_emb;  // MIDI OUT Jack Embedded
  USB_MIDI_IN_JACK_DESCR       midi_in_jack_ext;   // MIDI IN Jack External
  USB_MIDI_OUT_JACK_DESCR      midi_out_jack_ext;  // MIDI OUT Jack External
  USB_ENDP_DESCR               ms_ep_out;          // Bulk OUT Endpoint
  USB_MS_ENDP_DESCR            ms_ep_out_cs;       // Bulk OUT Class-Specific
  USB_ENDP_DESCR               ms_ep_in;           // Bulk IN Endpoint
  USB_MS_ENDP_DESCR            ms_ep_in_cs;        // Bulk IN Class-Specific
} USB_CFG_DESCR_MIDI;

// ===================================================================================
// External Declarations
// ===================================================================================
extern __code USB_DEV_DESCR DevDescr_MIDI;
extern __code USB_CFG_DESCR_MIDI CfgDescr_MIDI;
extern __code uint16_t LangDescr_MIDI[];
extern __code uint16_t ManufDescr_MIDI[];
extern __code uint16_t ProdDescr_MIDI[];
extern __code uint16_t SerDescr_MIDI[];

