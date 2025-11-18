// ===================================================================================
// USB Descriptors - Flight Controller (Game Pad Only)
// 简化版：只保留Game Pad接口，删除键盘、鼠标、Consumer功能
// ===================================================================================

#include "usb_descr.h"

// ===================================================================================
// Device Descriptor
// ===================================================================================
__code USB_DEV_DESCR DevDescr = {
  .bLength            = sizeof(DevDescr),
  .bDescriptorType    = USB_DESCR_TYP_DEVICE,
  .bcdUSB             = 0x0110,                 // USB 1.1
  .bDeviceClass       = 0,
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
// Configuration Descriptor
// ===================================================================================
__code USB_CFG_DESCR_HID CfgDescr = {

  // Configuration Descriptor
  .config = {
    .bLength            = sizeof(USB_CFG_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_CONFIG,
    .wTotalLength       = sizeof(CfgDescr),
    .bNumInterfaces     = 1,                    // 只有1个接口（Game Pad）
    .bConfigurationValue= 1,
    .iConfiguration     = 0,
    .bmAttributes       = 0x80,
    .MaxPower           = USB_MAX_POWER_mA / 2
  },

  // Interface Descriptor
  .interface0 = {
    .bLength            = sizeof(USB_ITF_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber   = 0,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 1,                    // 只需要1个IN端点
    .bInterfaceClass    = USB_DEV_CLASS_HID,
    .bInterfaceSubClass = 0,                    // 非Boot设备
    .bInterfaceProtocol = 0,                    // 非键盘/鼠标
    .iInterface         = 4
  },

  // HID Descriptor
  .hid0 = {
    .bLength            = sizeof(USB_HID_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_HID,
    .bcdHID             = 0x0111,               // HID 1.11
    .bCountryCode       = 0,                    // 不指定国家
    .bNumDescriptors    = 1,
    .bDescriptorTypeX   = USB_DESCR_TYP_REPORT,
    .wDescriptorLength  = sizeof(ReportDescr)
  },

  // Endpoint Descriptor: Endpoint 1 (IN)
  .ep1IN = {
    .bLength            = sizeof(USB_ENDP_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_ENDP,
    .bEndpointAddress   = USB_ENDP_ADDR_EP1_IN,
    .bmAttributes       = USB_ENDP_TYPE_INTER,
    .wMaxPacketSize     = EP1_SIZE,
    .bInterval          = 10                    // 10ms轮询间隔
  },

  // 删除EP2 OUT（不需要LED状态反馈）
  .ep2OUT = {
    .bLength            = 0,
    .bDescriptorType    = 0,
    .bEndpointAddress   = 0,
    .bmAttributes       = 0,
    .wMaxPacketSize     = 0,
    .bInterval          = 0
  }
};

// ===================================================================================
// HID Report Descriptor - 只包含Game Pad
// ===================================================================================
__code uint8_t ReportDescr[] = {
  // Joystick/Game Pad with 2 independent axes (Z, Slider) and 4 buttons
  0x05, 0x01,           // USAGE_PAGE (Generic Desktop)
  0x09, 0x05,           // USAGE (Game Pad)
  0xa1, 0x01,           // COLLECTION (Application)
  0xa1, 0x00,           //   COLLECTION (Physical)
  
  // 按钮定义（4个按钮）
  0x05, 0x09,           //     USAGE_PAGE (Button)
  0x19, 0x01,           //     USAGE_MINIMUM (Button 1)
  0x29, 0x04,           //     USAGE_MAXIMUM (Button 4)
  0x15, 0x00,           //     LOGICAL_MINIMUM (0)
  0x25, 0x01,           //     LOGICAL_MAXIMUM (1)
  0x75, 0x01,           //     REPORT_SIZE (1)
  0x95, 0x04,           //     REPORT_COUNT (4)
  0x81, 0x02,           //     INPUT (Data,Var,Abs)
  
  // 填充位（4位，凑齐1字节）
  0x75, 0x04,           //     REPORT_SIZE (4)
  0x95, 0x01,           //     REPORT_COUNT (1)
  0x81, 0x03,           //     INPUT (Cnst,Var,Abs)
  
  // 轴定义（Z轴和Slider，独立校准）
  0x05, 0x01,           //     USAGE_PAGE (Generic Desktop)
  0x09, 0x32,           //     USAGE (Z) - 油门轴
  0x09, 0x36,           //     USAGE (Slider) - 滑块轴（反推）
  0x15, 0x81,           //     LOGICAL_MINIMUM (-127)
  0x25, 0x7f,           //     LOGICAL_MAXIMUM (127)
  0x75, 0x08,           //     REPORT_SIZE (8)
  0x95, 0x02,           //     REPORT_COUNT (2)
  0x81, 0x02,           //     INPUT (Data,Var,Abs)
  
  0xc0,                 //   END_COLLECTION
  0xc0                  // END_COLLECTION
};

__code uint8_t ReportDescrLen = sizeof(ReportDescr);

// ===================================================================================
// String Descriptors
// ===================================================================================

// Language Descriptor (Index 0)
__code uint16_t LangDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(LangDescr), 0x0409 };

// Manufacturer String Descriptor (Index 1)
__code uint16_t ManufDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ManufDescr), MANUFACTURER_STR };

// Product String Descriptor (Index 2)
__code uint16_t ProdDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ProdDescr), PRODUCT_STR };

// Serial String Descriptor (Index 3)
__code uint16_t SerDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(SerDescr), SERIAL_STR };

// Interface String Descriptor (Index 4)
__code uint16_t InterfDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(InterfDescr), INTERFACE_STR };


