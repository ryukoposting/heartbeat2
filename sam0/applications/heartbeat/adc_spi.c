/* SPI MODE = 1*/

/*
 pins:
    SCK  out
    SDI  in
    SDO  out
    DRDY in
 pinmux must be manually set?
 */

#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

struct spi_module g_spimod;
struct tc_module g_tcmod;

typedef enum CallbackState {
  CB_READY,
  CB_BYTE0,
  CB_BYTE1,
  CB_BYTE2
} CallbackState;

static CallbackState cbstate = CB_BYTE2;
static int adcdata = 0;

void spi_setup()
{
  /* configure SPI */
  struct spi_config modcfg;
  spi_get_config_defaults(&modcfg);
  modcfg.mode = SPI_MODE_MASTER;
  modcfg.mode_specific.master.baudrate = 40000;
  
  modcfg.character_size = SPI_CHARACTER_SIZE_8BIT;
  modcfg.data_order = SPI_DATA_ORDER_MSB;
  modcfg.transfer_mode = SPI_TRANSFER_MODE_1;
  
  modcfg.mux_setting = SPI_SIGNAL_MUX_SETTING_C;
  modcfg.pinmux_pad0 = PINMUX_PA22C_SERCOM1_PAD0;
  modcfg.pinmux_pad1 = PINMUX_PA23C_SERCOM1_PAD1;
  modcfg.pinmux_pad2 = PINMUX_PA16C_SERCOM1_PAD2;
  modcfg.pinmux_pad3 = PINMUX_UNUSED;
  
  /* initialize the SPI module */
  switch(spi_init(&g_spimod, SERCOM1, &modcfg)) {
  case STATUS_ERR_DENIED:
  case STATUS_ERR_INVALID_ARG:
  case STATUS_BUSY:
    while(1);
  default:
    break;
  }
  
  /* set up PA14 as a strobe, up to 1MHz (good: 500kHz, rated SNR=108dB) */
//   struct tc_config tccfg;
//   tc_get_config_defaults(&tccfg);
// 
//   tccfg.counter_size    = TC_COUNTER_SIZE_16BIT;
//   tccfg.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
//   tccfg.counter_16_bit.compare_capture_channel[0] = 0x8;
// 
//   tccfg.pwm_channel[0].enabled = true;
//   tccfg.pwm_channel[0].pin_out = STROBE_PIN;
//   tccfg.pwm_channel[0].pin_mux = STROBE_PINMUX;
//   tccfg.waveform_invert_output = TC_WAVEFORM_INVERT_OUTPUT_CHANNEL_0;
// 
//   tc_init(&g_tcmod, STROBE_TIMER, &tccfg);
// 
//   tc_enable(&g_tcmod);
  
  struct port_config pcfg2;
  port_get_config_defaults(&pcfg2);
  pcfg2.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(STROBE_PIN, &pcfg2);
  port_pin_set_output_level(STROBE_PIN, 1);
  
  /* set up PA15 as digital input. read when PA15 goes low. */
  struct port_config pcfg;
  port_get_config_defaults(&pcfg);
  pcfg.direction = PORT_PIN_DIR_INPUT;
  pcfg.input_pull = PORT_PIN_PULL_NONE;
  port_pin_set_config(PIN_DRDY, &pcfg);
  
  /* set up PA24 as digital output. drive low to do SYNC. */
  struct port_config pcfg3;
  port_get_config_defaults(&pcfg3);
  pcfg.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(PIN_SYNC, &pcfg3);
  port_pin_set_output_level(PIN_SYNC, 1);
  
  for (int i = 0; i < 0x7FFFF; ++i) asm("nop");
  
  /* enable SPI and add callback */
  spi_enable(&g_spimod);
}

void strobe_pulse()
{
  port_pin_set_output_level(STROBE_PIN, 0);
  port_pin_set_output_level(STROBE_PIN, 1);
}

void adc_reset()
{
  for (int i = 0; i < 64; ++i)
    strobe_pulse();
  
  port_pin_set_output_level(PIN_SYNC, 0);
  for (int i = 0; i < 32; ++i)
    strobe_pulse();
  port_pin_set_output_level(PIN_SYNC, 1);
  
  while (!adc_ready())
    strobe_pulse();
}

int adc_ready()
{
  return !port_pin_get_input_level(PIN_DRDY);
}

int adc_read()
{
  int result = 0;
  uint16_t temp = 0;
  
  spi_transceive_wait(&g_spimod, 0, &temp);
  result = temp & 0xFF;
  
  spi_transceive_wait(&g_spimod, 0, &temp);
  result = (result << 8) | (temp & 0xFF);
  
  spi_transceive_wait(&g_spimod, 0, &temp);
  result = (result << 8) | (temp & 0xFF);
  
  return result;
}

#ifdef __cplusplus
}
#endif
