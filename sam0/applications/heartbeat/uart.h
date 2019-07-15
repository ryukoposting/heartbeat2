#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#ifdef _cplusplus
extern"C"{
#endif

#include<compiler.h>
#include<system.h>


void configure_console(void);
void write_to_terminal(char *bpm);
void int_to_string(int x);

#ifdef _cplusplus
}
#endif 

#endif
