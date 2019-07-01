/* Falling edge = Trailing edge = sample
   Rising edge  = Leading edge  = setup
   rising = 
   DATA MODE 0 */

/*
 pins:
    SCK  out
    SDI  in
    SDO  out
    DRDY in
 pinmux must be manually set
 */

#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_SS PIN_PA15

struct spi_module g_spimod;
struct spi_slave_inst g_adc_slave;

void spi_callback(struct spi_module *const module)
{
  
}

void spi_setup()
{
  /* configure SPI */
  struct spi_config modcfg;
  spi_get_config_defaults(&modcfg);
  modcfg.mode = SPI_MODE_MASTER;
  modcfg.mode_specific.master.baudrate = 400000;
  
  modcfg.character_size = SPI_CHARACTER_SIZE_8BIT;
  modcfg.data_order = SPI_DATA_ORDER_MSB;
  modcfg.transfer_mode = SPI_TRANSFER_MODE_1;
  
  modcfg.mux_setting = SPI_SIGNAL_MUX_SETTING_C;
  modcfg.pinmux_pad0 = PINMUX_PA22C_SERCOM1_PAD0;
  modcfg.pinmux_pad1 = PINMUX_PA23C_SERCOM1_PAD1;
  modcfg.pinmux_pad2 = PINMUX_PA16C_SERCOM1_PAD2;
  modcfg.pinmux_pad3 = PINMUX_UNUSED;
  
  struct spi_slave_inst_config slvcfg;
  spi_slave_inst_get_config_defaults(&slvcfg);
  slvcfg.ss_pin = PIN_SS;
  
  
  switch(spi_init(&g_spimod, SERCOM1, &modcfg)) {
  case STATUS_ERR_DENIED:
  case STATUS_ERR_INVALID_ARG:
  case STATUS_BUSY:
    while(1);
  default:
    break;
  }
  
  
  /* set up PA14 as a strobe, up to 1MHz (good: 500kHz, rated SNR=108dB) */
  
  /* set up PA15 as digital input. read when PA15 goes low. */
  
  
  /* enable SPI and add callback */
  spi_enable(&g_spimod);
  spi_register_callback(&g_spimod, spi_callback, SPI_CALLBACK_BUFFER_TRANSCEIVED);
  spi_enable_callback(&g_spimod, SPI_CALLBACK_BUFFER_TRANSCEIVED);
}

#ifdef __cplusplus
}
#endif
