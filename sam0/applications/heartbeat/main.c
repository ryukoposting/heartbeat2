#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_spi.h"
  


#ifdef __cplusplus
extern "C" {
#endif


#define TC_COUNT_VALUE 55535
<<<<<<< HEAD
int main(void)
{
	struct port_config pin;
	system_init();
  	spi_setup();
	/*Configure UART console.*/
	configure_console();
	while(1);
=======

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

volatile int adcval = 0;
int main(void)
{
  system_init();
  spi_setup();

  /*Configure UART console.*/
//   configure_console();
//   char count = 0;
//   while(count != 15)
//   {
//     write_to_terminal(count);
//     count++;
//   }
  
  volatile int x = 0;
  
  adc_reset();
  
  asm("nop");
  
  /* THIS WORKS */
  while (1) {
    adcval = adc_read();
    adc_reset();
  }
  
//   while (adc_ready()) {
//     adc_strobe();
//     for (int i = 0; i < 100; ++i) asm("nop");
//   }
//   
//   while (1) {
//     while (!adc_ready()) {
//       adc_strobe();
//     }
//     adcval = adc_read();
//     for (int i = 0; i < 0xFFFF; ++i) asm("nop");
//   }
  
  while(1);
}

#ifdef __cplusplus
}
#endif
