/**
  ******************************************************************************
  * @file    Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"
#include "delay.h"
#include "main.h"

#define DS1307_I2C_ADDRESS 0x68 << 1 // DS1307  I2C 地址
#define MESSAGE1   "Calendar"
#define LINENUM            0x15
#define MESSAGE2   "    Calendar      "
#define MESSAGE3   "Year/Month/Day:"
#define MESSAGE4   "Week/Hour/Min/Sec: "
#define MESSAGE5   " %04d/%02d/%02d"
#define MESSAGE6  " %02d, %02d:%02d:%02d"

#define FONTSIZE         Font12x12
#define MESSAGE_Clock   " Data = %d/%d "
char buf[50];
TM_RTC_Time_t datatime;
struct ReturnDate
{
	int date;
	int month;
	int year;
	int hours;
	int min;
	int sec;
}stu;

#ifdef USE_LCD
static void Display_Init(void);
static void Display(void);
#endif /* USE_LCD */



#ifdef USE_LCD
/**
  * @brief  Display ADCs converted values on LCD
  * @param  None
  * @retval None
  */
static void Display(void)
{	
	uint8_t aTextBuffer1[60];
	uint8_t aTextBuffer2[60];
	sprintf ((char*)aTextBuffer1, MESSAGE5, datatime.year+2000,datatime.month,datatime.date); // 將數字(年)轉成字串 (String)
	LCD_DisplayStringLine(LCD_LINE_4, (uint8_t*)aTextBuffer1);
	sprintf ((char*)aTextBuffer1, MESSAGE3); // 將數字(年)轉成字串 (String)
	LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*)aTextBuffer1);
	
	sprintf ((char*)aTextBuffer2, MESSAGE4); // 將數字(星期)轉成字串 (String)
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)aTextBuffer2);
	sprintf ((char*)aTextBuffer2, MESSAGE6, datatime.day, datatime.hours, datatime.minutes, datatime.seconds); // 將數字(星期)轉成字串 (String)
	LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)aTextBuffer2);
}

/**
  * @brief  Display Init (LCD)
  * @param  None
  * @retval None
  */
static void Display_Init(void)
{
  /* Initialize the LCD */
  LCD_Init();
  LCD_LayerInit();
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD Background Layer  */
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  
  /* Clear the Background Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Configure the transparency for background */
  LCD_SetTransparency(0);
  
  /* Set LCD Foreground Layer  */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Configure the transparency for foreground */
  LCD_SetTransparency(200);
  
  /* Clear the Foreground Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
    /* Set the LCD Text size */
  LCD_SetFont(&FONTSIZE);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
	//Bottom text
  LCD_DisplayStringLine(LINE(0x15), (uint8_t*)MESSAGE1);
  LCD_DisplayStringLine(LINE(0x16), (uint8_t*)"                               ");
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
  
	//Top text
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)MESSAGE2);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_WHITE);
  LCD_SetTextColor(LCD_COLOR_BLUE); 
}
#endif /* USE_LCD */
 
int main(){
	delay_init();
	/* LCD Display init  */
	Display_Init();
	/* Initialize system */
	SystemInit();
  /* Set wakeup interrupt every 1 second */
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	//Initialize RTC with internal 32768Hz clock
    //It's not very accurate
    if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
        //RTC was first time initialized
        //Do your stuf here
        //eg. set default time
    }
	/* Set new time */
	TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
	while (1){
		if (TM_DISCO_ButtonPressed()) {
			datatime.hours = 14;
			datatime.minutes = 59;
			datatime.seconds = 50;
			datatime.year = 19;
			datatime.month = 6;
			datatime.date = 13;
			datatime.day = 4;
			/////////////////////////
			/*測試進位用初始設定
			datatime.hours = 23;
			datatime.minutes = 59;
			datatime.seconds = 50;
			datatime.year = 19;
			datatime.month = 6;
			datatime.date = 16;
			datatime.day = 7;
			*/
			/////////////////////////
			/* Set new time */
			TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
		}
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
		Display();
			
  }
}
//void TM_RTC_RequestHandler() {
//	/* Get time */
//	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
//}
