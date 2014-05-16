#ifndef ADC_H
#define ADC_H

#include <adapter.h>

#define ADC_VREF_TYPE 0xC0
#define ION 1298 //Напряжение внутреннего ИОН (1,23) в мВ 
#define RIZM 200  //Сопротивление измер. резистора в Омах (для тока)

volatile uint16_t adc_result = 0;
uint16_t vref=0,volt,watt,delta,adc_tmp,d=200,avcc;    
unsigned int adc_calib_cnt;                //Вспомогательный счетчик 

void ADC_init(void);
void ADC_start(char channel);



#endif