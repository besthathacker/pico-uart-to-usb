#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "tusb.h"

#define UART_ID    uart0
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static uint32_t current_baud = 1;
static absolute_time_t next_change;
static bool locked = false;

static void set_baud(uint32_t baud) {
    uart_init(UART_ID, baud);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

// USB <-> UART bridge
static void cdc_task(void) {
    // USB -> UART
    if (tud_cdc_available()) {
        uint8_t buf[64];
        uint32_t n = tud_cdc_read(buf, sizeof(buf));
        for (uint32_t i = 0; i < n; i++) {
            uart_putc_raw(UART_ID, buf[i]);
        }
    }

    // UART -> USB
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);

        tud_cdc_write_char(ch);

        // Auto-detect: if we see printable ASCII, assume good baud
        if (!locked && ch >= 32 && ch <= 126) {
            locked = true;
            char msg[64];
            int n = snprintf(msg, sizeof(msg),
                             "\r\n[LOCKED baud=%u]\r\n", current_baud);
            tud_cdc_write(msg, n);
            tud_cdc_write_flush();
        }
    }
    tud_cdc_write_flush();
}

int main() {
    tusb_init();
    set_baud(current_baud);
    next_change = make_timeout_time_ms(50);

    while (true) {
        tud_task();
        cdc_task();

        if (!locked && absolute_time_diff_us(get_absolute_time(), next_change) < 0) {
            // Try next baud
            current_baud++;
            if (current_baud > 999999) current_baud = 1;
            set_baud(current_baud);

            // Send status marker up USB so you know where we are
            char msg[32];
            int n = snprintf(msg, sizeof(msg), "[baud=%u]\r\n", current_baud);
            tud_cdc_write(msg, n);
            tud_cdc_write_flush();

            next_change = make_timeout_time_ms(50);
        }
    }
}
