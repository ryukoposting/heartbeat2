#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_spi.h"

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Getting Started Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --\r\n" \
		"-- Pressing and release button SW0 should make LED0 on and off --"STRING_EOL

#ifdef __cplusplus
extern "C" {
#endif

static struct usart_module cdc_uart_module;

#define TC_COUNT_VALUE 55535

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = CONF_STDIO_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = CONF_STDIO_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = CONF_STDIO_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = CONF_STDIO_PINMUX_PAD3;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}

//This function transmits a single character via UART.
//"module" is a pointer to the software instance struct.
//tx_data is the data that will be transfered.
//enum status_code usart_write_wait(struct usart_module *const module, const uint16_t tx_data);


//Function that is called to write to the Termimnal with the current bpm.
char write_to_terminal(char bpm)
{
	while(usart_write_wait(&cdc_uart_module,bpm) != STATUS_OK);
	//While loop that will transmit a character via UART until opertaion is completed.
	return;
}


int main(void)
{
	struct port_config pin;

	system_init();

	/*Configure UART console.*/
	configure_console();
	char count = 0;
	while(count != 15)
	{
		write_to_terminal(count);
		count++;
	}
}

#ifdef __cplusplus
}
#endif
