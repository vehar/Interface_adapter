#ifndef RTC_H
#define RTC_H

//#include "compilers.h"

inline void RTC_init(void); //static
static char not_leap(void);

//orig struct  = 6 bytes

/*
typedef struct TIME {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char date;
    unsigned char month;
    unsigned char year;
    }time;
 */


typedef struct DATETIME  {
    uint32_t second:6; //7
    uint32_t minute:6; //7
    uint32_t hour:5;   //6
    uint32_t date:5;   //6
    uint32_t month:4;  //4
    uint32_t year:6;   //2 //по остатку
    }time_t;
            
    



#endif
