#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------+
// Common Configuration
//--------------------------------------------------------------------+
#define CFG_TUSB_MCU OPT_MCU_RP2040
#define CFG_TUSB_OS  OPT_OS_PICO
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)
#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN   __attribute__ ((aligned(4)))

//--------------------------------------------------------------------+
// Device Configuration
//--------------------------------------------------------------------+
#define CFG_TUD_ENDPOINT0_SIZE    64

//------------- CDC -------------//
#define CFG_TUD_CDC              1
#define CFG_TUD_CDC_RX_BUFSIZE  256
#define CFG_TUD_CDC_TX_BUFSIZE  256
#define CFG_TUD_CDC_EP_BUFSIZE   64

//--------------------------------------------------------------------+
// Disable unused classes
//--------------------------------------------------------------------+
#define CFG_TUD_MSC              0
#define CFG_TUD_HID              0
#define CFG_TUD_MIDI             0
#define CFG_TUD_VENDOR           0

#ifdef __cplusplus
}
#endif
