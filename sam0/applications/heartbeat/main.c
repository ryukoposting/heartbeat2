#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_spi.h"
#include "uart.h" 

// sitting: <7,700,000
//

unsigned int bit_reverse(unsigned int b) {
   b = (b & 0xFFFF0000) >> 16 | (b & 0x0000FFFF) << 16;
   b = (b & 0xFF00FF00) >> 8 | (b & 0x00FF00FF) << 8;
   b = (b & 0xF0F0F0F0) >> 4 | (b & 0x0F0F0F0F) << 4;
   b = (b & 0xCCCCCCCC) >> 2 | (b & 0x33333333) << 2;
   b = (b & 0xAAAAAAAA) >> 1 | (b & 0x55555555) << 1;
   return b;
}


#define BEAT_RISING_THRESHOLD  5700000
#define BEAT_FALLING_THRESHOLD 5400000
#define READS_PER_SAMPLE 2


#ifdef __cplusplus
extern "C" {
#endif
  
typedef enum BeatState {
  BEATSTATE_LOW,
  BEATSTATE_HIGH
} BeatState;

#define TC_COUNT_VALUE 55535
volatile unsigned int adcval = 0;
volatile unsigned int adcreverse = 0;
volatile BeatState bs = BEATSTATE_LOW;
int main(void)
{
  system_init();
  spi_setup();
  /*Configure UART console.*/
  configure_console();
  
  adc_reset();
  
  asm("nop");
  
  /* THIS WORKS */
  unsigned int nread = 0;
  unsigned int reads[READS_PER_SAMPLE] = {};
  while (1) {
    reads[nread] = adc_read();
    ++nread;
    if (nread == READS_PER_SAMPLE) {
      /* get average of ADC reads */
      unsigned int sum = 0;
      for (int i = 0; i < READS_PER_SAMPLE; ++i)
        sum += reads[i];
      adcval = sum / READS_PER_SAMPLE;
      
      /* beat detection logic */
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
      nread = 0;
    }
    adc_reset();
  }
  
  while(1);
}

#ifdef __cplusplus
}
#endif
