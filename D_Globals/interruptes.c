#warning This file`ll be destroyed!

#include <adapter.h>

#include "D_usart/usart.h"
//#include "D_usart/usart.c"

#include "global_variables.h"





/*   //теперь исспользуется прерывание ртос!
// Timer2 output compare interrupt service routine
interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
v_u32_SYS_TICK++;
   //  if (USART_Get_rxCount(USART_0) > 0) //если в приёмном буфере что-то есть
   //    {
   //     symbol = USART_Get_Char(USART_0);
   //     --Parser_req_state_cnt; //  Декримент счётчика вызова парсера

            #warning not_optimized
   //      if(Parser_req_state_cnt % 5 != 0) //Обычно просто ставится флаг..
   //      {
   //        _set(fl.Parser_Req);  //- опросить парсер в главном цикле,..
   //      }
   //      else //..но 1 раз в 5 прерываний он обрабатывается прямо сдесь..
   //      {
   //         PARS_Parser(symbol);//..если вдруг главный цикл завис
   //      }
   //    }
}
     */
