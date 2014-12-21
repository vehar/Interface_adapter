#include "parser.h"

char buf[SIZE_RECEIVE_BUF];
char *argv[AMOUNT_PAR];
uint8_t argc;

uint8_t i = 0;
uint8_t devider_f = 0;

void PARSER_Init(void)
{
  argc = 0;
  argv[0] = buf;
  devider_f = FALSE;
  i = 0;
}

void PARS_Parser(char symbol)
{
   if (symbol !='\r')    //'\r' //end of string 
   {               
     if (i < SIZE_RECEIVE_BUF - 1)
     {
        if (symbol != ' ')
         {
           if (!argc)
           {
              argv[0] = buf;
              argc++;
           }

           if (devider_f)
           {
              if (argc < AMOUNT_PAR)
              {
                 argv[argc] = &buf[i];
                 argc++;
              }
              devider_f = FALSE;
            }

            buf[i] = symbol;
            i++;
         }
        else
         {                 // "space" - is divider
           if (!devider_f)
           {
              buf[i] = 0;
              i++;
              devider_f = TRUE;
           }
         }
     }
     buf[i] = 0;
     return;
   }
   else
   {     
      buf[i] = 0;
        if (argc)
           {
                PARS_Handler(argc, argv);
           }
      argc = 0;
      devider_f = FALSE;
      i = 0;
   }
}

#ifdef  __GNUC__

uint8_t PARS_EqualStrFl(char *s1, char const *s2)
{
  uint8_t i = 0;

  while(s1[i] == pgm_read_byte(&s2[i]) && s1[i] != '\0' && pgm_read_byte(&s2[i]) != '\0')
  {
     i++;
  }
  if (s1[i] =='\0' && pgm_read_byte(&s2[i]) == '\0')
  {
     return TRUE;
  }
  else
  {
     return FALSE;
  }
}

#else

#warning standart strcmpf req less memory
uint8_t PARS_EqualStrFl(char *s1, char __flash *s2)
{
  uint8_t i = 0;

  while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
  {
     i++;
  }
  if (s1[i] =='\0' && s2[i] == '\0')
  {
     return TRUE;
  }
  else
  {
     return FALSE;
  }
}

#endif

#warning standart strcmp req less memory
uint8_t PARS_EqualStr(char *s1, char *s2)
{
  uint8_t i = 0;

  while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
  {
     i++;
  }
  if (s1[i] =='\0' && s2[i] == '\0')
  {
     return TRUE;
  }
  else
  {
     return FALSE;
  }
}

uint8_t PARS_StrToUchar(char *s)
{
   uint8_t value = 0;
  // while(*s == '0'){s++;} // For what?
   while(*s)
   {
      value += (*s - 0x30);
      s++;
      if (*s){
         value *= 10;
      }
   };

  return value;
}

//atoi 
uint16_t PARS_StrToUint(char *s)
{
   uint16_t value = 0;
   //while(*s == '0'){s++;}
   while(*s)
   {
      value += (*s - 0x30);
      s++;
      if (*s){
         value *= 10;
      }
   };

  return value;
}