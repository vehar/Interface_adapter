//#include "global_variables.h"

#ifndef global_funktions_H
#define global_funktions_H

#include "global_defines.h"
#include "adapter"

//#include "parser.h"
//#include "usart.h"

uint8_t check_after_pow_on(void);
void all_init (void);
void red_blink(void);
void TIM2_ON(void);
void TIM2_OFF(void);
void flags_init(void);
void sys_timer_init(void);

#endif