#include "asf.h"
#include "stdio_serial.h"
#include "conf_uart_serial.h"
#include "conf_spi.h"
  


#ifdef __cplusplus
extern "C" {
#endif


#define TC_COUNT_VALUE 55535
int main(void)
{
	struct port_config pin;
	system_init();
  	spi_setup();
	/*Configure UART console.*/
	configure_console();
	while(1);
}

#ifdef __cplusplus
}
#endif
