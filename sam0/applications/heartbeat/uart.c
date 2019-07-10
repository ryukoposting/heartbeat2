#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
  


#ifdef __cplusplus
extern "C" {
#endif

static struct usart_module cdc_uart_module;

#define TC_COUNT_VALUE 55535
//Initializes UART
static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = CONF_STDIO_MUX_SETTING;
	usart_conf.pinmux_pad0 = PINMUX_PA04D_SERCOM0_PAD0;
	usart_conf.pinmux_pad1 = PINMUX_PA05D_SERCOM0_PAD1;
	usart_conf.pinmux_pad2 = PINMUX_UNUSED;
	usart_conf.pinmux_pad3 = PINMUX_UNUSED;
	usart_conf.baudrate    = CONF_STDIO_BAUDRATE;

	stdio_serial_init(&cdc_uart_module, CONF_STDIO_USART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}

//Function that is called to write to the Termimnal with the current bpm.
void write_to_terminal(char *bpm)
{
	usart_write_buffer_wait(&cdc_uart_module, bpm, strlen(bpm));
}
