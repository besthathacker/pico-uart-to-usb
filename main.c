#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "tusb.h"

#define UART_ID    uart0
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define BAUD_DEFAULT 115200

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
    }
    tud_cdc_write_flush();
}

int main() {
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_init(UART_ID, BAUD_DEFAULT);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);

    tusb_init();

    while (true) {
        tud_task();
        cdc_task();
        tight_loop_contents();
    }
}

// Handle baud/format change from host
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* coding) {
    (void)itf;
    uart_set_baudrate(UART_ID, coding->bit_rate);
    uart_set_format(
        UART_ID,
        coding->data_bits,
        (coding->stop_bits == 2) ? 2 : 1,
        (coding->parity == 1) ? UART_PARITY_ODD :
        (coding->parity == 2) ? UART_PARITY_EVEN : UART_PARITY_NONE
    );
}
