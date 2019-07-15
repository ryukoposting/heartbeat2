#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_spi.h"
#include "uart.h" 


#ifdef __cplusplus
extern "C" {
#endif


#define TC_COUNT_VALUE 55535
volatile int adcval = 0;
int main(void)
{
  system_init();
  spi_setup();
  /*Configure UART console.*/
   configure_console();
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
    int_to_string(adcval);
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
