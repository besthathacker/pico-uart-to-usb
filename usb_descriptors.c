#include "tusb.h"

//--------------------------------------------------------------------+
// Device Descriptor
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,

    .bDeviceClass       = 0x00, // each interface specifies its own class
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCafe,   // pick your VID
    .idProduct          = 0x4001,   // and PID
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

// Called by tinyusb to get device descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
enum {
  ITF_NUM_CDC = 0,
  ITF_NUM_CDC_DATA,
  ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)

uint8_t const desc_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power (mA)
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

  // CDC: Interface number, string index, EP notification, EP data out & in
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, 0x81, 8, 0x02, 0x82, 64)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // only one configuration
  return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+
char const* string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 }, // 0: English (0x0409)
    "Gremlin Corp",                // 1: Manufacturer
    "Pico UART-USB",               // 2: Product
    "123456",                      // 3: Serial (dummy)
    "UART Bridge"                  // 4: CDC interface
};

// UTF-16LE string
static uint16_t _desc_str[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if (index == 0) {
    _desc_str[1] = 0x0409;
    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2+2);
    return _desc_str;
  }

  if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0]))) return NULL;

  const char* str = string_desc_arr[index];

  // Convert ASCII to UTF-16
  chr_count = strlen(str);
  if ( chr_count > 31 ) chr_count = 31;
  for(uint8_t i=0; i<chr_count; i++) {
    _desc_str[1+i] = str[i];
  }

  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}
