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
#include "oled-display.h"
#include "stdint.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  OLEDDIAPLY_MODE_COMMAND,
  OLEDDISAPLY_MODE_RAMDATA
}oledDisplayMode_et;
/* Private define ------------------------------------------------------------*/
#define OLED_12C_ADDRESS    0x7A 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void
OLEDDISPLAY_TransferMode(oledDisplayMode_et cmd)
{
  I2C_Start();
  I2C_Write(OLED_12C_ADDRESS);
  if (OLEDDIAPLY_MODE_COMMAND == cmd)
  {
    I2C_Write(0x00);  /* Co = 0, D/C = 0 */
  }
  else
  {
    I2C_Write(0x40);  /* Co = 0, D/C = 1 */
  }
}

static inline void
OLEDDIPLAY_Transfer(uint8_t data)
{
  I2C_Write(data);
}

static inline void
OLEDDIPLAY_TransferEnd()
{
  I2C_Stop();
}


void
OLEDDISPLAY_Init()
{
  OLEDDISPLAY_TransferMode(OLEDDIAPLY_MODE_COMMAND);
  
  OLEDDIPLAY_Transfer(0xAE);                      // Set Display OFF
  OLEDDIPLAY_Transfer(0x81);OLEDDIPLAY_Transfer(0xCF); // Set Contrast Control
  OLEDDIPLAY_Transfer(0xA4);                     // Entire Display ON
  OLEDDIPLAY_Transfer(0xA6);                     // Set Normal

  OLEDDIPLAY_Transfer(0x20); OLEDDIPLAY_Transfer(0x02); // Set Memory Addressing Mode
  OLEDDIPLAY_Transfer(0x00);                     // Set Lower Column
  OLEDDIPLAY_Transfer(0x10);                     // Set Higher Column
  OLEDDIPLAY_Transfer(0xB0);                     // Set Page Start

  OLEDDIPLAY_Transfer(0x40);                     // Set Display Start Line
  OLEDDIPLAY_Transfer(0xA1);                     // Set Segment Re-map
  OLEDDIPLAY_Transfer(0xA8); OLEDDIPLAY_Transfer(0x3F); // Set Multiplex Ratio
  OLEDDIPLAY_Transfer(0xC8);                     // Set COM Output
  OLEDDIPLAY_Transfer(0xD3); OLEDDIPLAY_Transfer(0x00); // Set Display Offset
  OLEDDIPLAY_Transfer(0xDA); OLEDDIPLAY_Transfer(0x12); // Set COM Pins Hardware Configuration

  OLEDDIPLAY_Transfer(0xD5); OLEDDIPLAY_Transfer(0x80); // Set Display Clock
  OLEDDIPLAY_Transfer(0xD9); OLEDDIPLAY_Transfer(0xF1); // Set Pre-charge Period
  OLEDDIPLAY_Transfer(0xDB); OLEDDIPLAY_Transfer(0x40); // Set VCOMH Deselect Level
  OLEDDIPLAY_Transfer(0x8D); OLEDDIPLAY_Transfer(0x14); // Charge Pump Setting

  OLEDDIPLAY_Transfer(0xAF);                     // Set Display ON
  
  OLEDDIPLAY_TransferEnd();
}

void 
OLEDDISPLAY_SetPointer(uint8_t seg, uint8_t pag)
{
  uint8_t low_column;
  uint8_t hig_column;
  
  low_column = (seg & 0b00001111);
  hig_column = (seg & 0b11110000) >> 4;
  hig_column = hig_column | 0b00010000;
  pag = (pag & 0b00000111);
  pag = (pag | 0b10110000);
  
  OLEDDISPLAY_TransferMode(OLEDDIAPLY_MODE_COMMAND);
  OLEDDIPLAY_Transfer(low_column); /* Set Lower Column  */
  OLEDDIPLAY_Transfer(hig_column); /* Set Higher Column */
  OLEDDIPLAY_Transfer(pag);        /* Set Page Start    */
  OLEDDIPLAY_TransferEnd();
  
}

void 
OLEDDISAPLY_FillScreen(uint8_t pattern)
{
  uint8_t pag;
  uint8_t seg;
  
  for(pag = 0; pag < 8; pag++)
  {
    OLEDDISPLAY_SetPointer(0, pag);

    OLEDDISPLAY_TransferMode(OLEDDISAPLY_MODE_RAMDATA);
    for(seg = 0; seg < 128; seg++)
    {
      OLEDDIPLAY_Transfer(pattern);
    }
    OLEDDIPLAY_TransferEnd();
  }
}