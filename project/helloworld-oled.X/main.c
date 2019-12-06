/**\cond
  ******************************************************************************
  * ______  _                             ___  ___        _               
  * | ___ \(_)                            |  \/  |       | |              
  * | |_/ / _  _ __    __ _  _ __  _   _  | .  . |  __ _ | | __ ___  _ __ 
  * | ___ \| || '_ \  / _` || '__|| | | | | |\/| | / _` || |/ // _ \| '__|
  * | |_/ /| || | | || (_| || |   | |_| | | |  | || (_| ||   <|  __/| |   
  * \____/ |_||_| |_| \__,_||_|    \__, | \_|  |_/ \__,_||_|\_\\___||_|   
  *                                 __/ |                                 
  *                                |___/                                  
  *                                                                       
  * Copyright (C) 2019 Binary Maker - All Rights Reserved
  *
  * This program and the accompanying materials are made available
  * under the terms described in the LICENSE file which accompanies
  * this distribution.
  * Written by Binary Maker <https://github.com/binarymaker>
  ******************************************************************************
  \endcond*/

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"
#include "oled-display.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int
main(void)
{
  MCU_Init();
  OLED_DISPLAY_Init();
  uint8_t count = 0;
  char buf[10];
 // OLEDDISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
  OLED_DISPLAY_FontSelect(Segment_25x40, 25, 40, 46, 58);
  while(1)
  {
    OLED_DISPLAY_SetPointer(0, 0);
    PRINT_IntegerToAscii(count, buf, 10, 3, 0);
    USART_Printf("%s",buf);
    OLED_DISPLAY_WriteString(buf); 
    DELAY_ms(100);
    count++;
  }
  return 0;
}