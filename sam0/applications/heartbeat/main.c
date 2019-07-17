#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_spi.h"
#include "uart.h" 

#define BEAT_RISING_THRESHOLD 0x4000
#define BEAT_FALLING_THRESHOLD 0x2000


#ifdef __cplusplus
extern "C" {
#endif
  
typedef enum BeatState {
  BEATSTATE_LOW,
  BEATSTATE_HIGH
};

#define TC_COUNT_VALUE 55535
volatile int adcval = 0;
volatile BeatState bs = BEATSTATE_LOW;
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
  
  adc_reset();
  
  asm("nop");
  
  /* THIS WORKS */
  while (1) {
    adcval = adc_read();
    switch (bs) {
    case BEATSTATE_HIGH:
      if (adcval < BEAT_FALLING_THRESHOLD)
        bs = BEATSTATE_LOW;
      break;
    case BEATSTATE_LOW:
      if (adcval > BEAT_RISING_THRESHOLD) {
        bs = BEATSTATE_HIGH;
        write_to_terminal("a");
      }
      break;
    }
//     int_to_string(123456);
    adc_reset();
  }
  
  while(1);
}

#ifdef __cplusplus
}
#endif
