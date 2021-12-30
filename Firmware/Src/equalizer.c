// ****************************************************************************
/// \file      equalizer.c
///
/// \brief     equalizer C Source File
///
/// \details   Driver Module for equalizer leds.
///
/// \author    Nico Korn
///
/// \version   1.0.0.0
///
/// \date      27122021
/// 
/// \copyright Copyright (c) 2021 Nico Korn
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// \pre       
///
/// \bug       
///
/// \warning   
///
/// \todo      
///
// ****************************************************************************

// Include ********************************************************************
#include "equalizer.h"
#include "ws2812b.h"

// Private define *************************************************************
#define NR_OF_EFFECTS     ( 4u )
#define EFFECT_OPT_FADE   ( 0x01 )

// Private types     **********************************************************
typedef __packed struct equalizer_s{
   const uint8_t*    effect;
   uint8_t           effectIndex;
   FlagStatus        effectChanged;
   uint8_t           level;
   uint8_t           levelTop;
   uint8_t           levelFil;
   uint16_t          levelTopTime;
   uint16_t          levelFilTime;
   
}euqalizer_t;

// Private variables **********************************************************
static const uint8_t effect_1[COL+1][3] =        { { 0x00, 0xff, 0x00 },     // green
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x80, 0x00 },
                                                   { 0xff, 0x00, 0x00 },      // red
                                                   { 0x00, 0x00, 0x00 } };    // effect option flags

static const uint8_t effect_2[COL+1][3] =        { { 0x00, 0x00, 0xff },     
                                                   { 0x11, 0x00, 0xee },
                                                   { 0x22, 0x00, 0xdd },
                                                   { 0x33, 0x00, 0xcc },
                                                   { 0x44, 0x00, 0xbb },
                                                   { 0x55, 0x00, 0xaa },
                                                   { 0x66, 0x00, 0x99 },
                                                   { 0x77, 0x00, 0x88 },
                                                   { 0x99, 0x00, 0x66 },
                                                   { 0xaa, 0x00, 0x55 },
                                                   { 0xbb, 0x00, 0x44 },
                                                   { 0xcc, 0x00, 0x33 },
                                                   { 0xdd, 0x00, 0x22 },
                                                   { 0xee, 0x00, 0x11 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // effect option flags

static const uint8_t effect_3[COL+1][3] =        { { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xee, 0x11 },
                                                   { 0x00, 0xdd, 0x22 },
                                                   { 0x00, 0xcc, 0x33 },
                                                   { 0x00, 0xbb, 0x44 },
                                                   { 0x00, 0xaa, 0x55 },
                                                   { 0x00, 0x99, 0x66 },
                                                   { 0x00, 0x88, 0x77 },
                                                   { 0x00, 0x77, 0x88 },
                                                   { 0x00, 0x66, 0x99 },
                                                   { 0x00, 0x55, 0xaa },
                                                   { 0x00, 0x44, 0xbb },
                                                   { 0x00, 0x33, 0xcc },
                                                   { 0x00, 0x22, 0xdd },
                                                   { 0x00, 0x11, 0xee },
                                                   { 0x00, 0x00, 0x00 } };    // effect option flags

static const uint8_t effect_4[COL+1][3] =        { { 0xff, 0xff, 0x00 },     
                                                   { 0xee, 0xff, 0x11 },
                                                   { 0xdd, 0xff, 0x22 },
                                                   { 0xcc, 0xff, 0x33 },
                                                   { 0xbb, 0xff, 0x44 },
                                                   { 0xaa, 0xff, 0x55 },
                                                   { 0x99, 0xff, 0x66 },
                                                   { 0x88, 0xff, 0x77 },
                                                   { 0x77, 0xff, 0x88 },
                                                   { 0x66, 0xff, 0x99 },
                                                   { 0x55, 0xff, 0xaa },
                                                   { 0x44, 0xff, 0xbb },
                                                   { 0x33, 0xff, 0xcc },
                                                   { 0x22, 0xff, 0xdd },
                                                   { 0x11, 0xff, 0xee },
                                                   { 0x00, 0x00, 0x00 } };    // effect option flags

static const uint8_t effect_funny[COL+1][3] =    { { 0x00, 0x00, 0x11 },     
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x00, 0x00, 0x11 },
                                                   { 0x01, 0x00, 0x00 } };    // effect option flags

static const uint8_t *effects[NR_OF_EFFECTS] = { &effect_1[0][0], &effect_2[0][0], &effect_3[0][0], &effect_4[0][0] };
                                                           
// Private function prototypes ************************************************

// Private variables **********************************************************
static euqalizer_t equalizer;
//static const float m = ((float)15/(float)2395);
static const float m = ((float)15/(float)2048);

// Functions ******************************************************************
// ----------------------------------------------------------------------------
/// \brief     Initialisation of the graphic equalizer.
///
/// \param     none
///
/// \return    none
EQUALIZER_StatusTypeDef equalizer_init( void )
{   
   equalizer.levelFil      = 0;
   equalizer.levelFilTime  = 0;
   equalizer.levelTop      = 0;
   equalizer.levelTopTime  = 0;
   equalizer.level         = 0;
   equalizer.effectIndex   = 0;
   equalizer.effect        = effects[equalizer.effectIndex%NR_OF_EFFECTS];
   equalizer.effectChanged = SET;
   
   return EQUALIZER_OK;
}

// ----------------------------------------------------------------------------
/// \brief     Set equalizer bar level.
///
/// \param     none
///
/// \return    none
void equalizer_setLevel( uint8_t level )
{   
   static uint8_t levelFilCounter;
   levelFilCounter++;
   static uint8_t levelTopCounter;
   levelTopCounter++;
   
   if( level >= COL )
   {
      return;
   }
   
   // level of the bar
   if( equalizer.effectChanged != RESET )
   {
      equalizer.level         = COL-1;
      equalizer.effectChanged = RESET;
   }
   else
   {
      equalizer.level = level;
   }
   
   // set fil level
   if( equalizer.levelFil < equalizer.level )
   {
      equalizer.levelFil      = equalizer.level;
      equalizer.levelFilTime  = 25;
   }
   else if( equalizer.levelFilTime )
   {
      equalizer.levelFilTime--;
   }
   else if( equalizer.levelFil > 0 && !(levelFilCounter%15) )
   {
      equalizer.levelFil--;
      levelFilCounter = 0;
   }

   // set top level
   if( equalizer.levelTop < equalizer.levelFil )
   {
      equalizer.levelTop      = equalizer.level;
      equalizer.levelTopTime  = 125;
   }
   else if( equalizer.levelTopTime )
   {
      equalizer.levelTopTime--;
   }
   else if( equalizer.levelTop > 0 && !(levelTopCounter%40) )
   {
      equalizer.levelTop--;
      levelTopCounter = 0;
   }
   
   // draw the bar
   WS2812B_clearBuffer();
   for( uint8_t i=0; i<equalizer.levelFil; i++ )
   {
      if( EFFECT_OPT_FADE == *(equalizer.effect+COL*3+0))
      {
         WS2812B_setPixel( 0, i, *(equalizer.effect+i*3+0)*level, *(equalizer.effect+i*3+1)*level, *(equalizer.effect+i*3+2)*level ); 
      }
      else
      {
         WS2812B_setPixel( 0, i, *(equalizer.effect+i*3+0), *(equalizer.effect+i*3+1), *(equalizer.effect+i*3+2) ); 
      }
   }
   WS2812B_setPixel( 0, equalizer.levelTop, 0xFF, 0x00, 0x00 ); 
   WS2812B_sendBuffer();
}

// ----------------------------------------------------------------------------
/// \brief     Clear equalizer bar level.
///
/// \param     none
///
/// \return    none
void equalizer_clear( void )
{   

}

// ----------------------------------------------------------------------------
/// \brief     Choose next effect
///
/// \param     none
///
/// \return    none
void equalizer_nextEffect( void )
{   
   equalizer.effectIndex++;
   equalizer.effect        = effects[equalizer.effectIndex%NR_OF_EFFECTS];
   equalizer.effectChanged = SET;
}

// ----------------------------------------------------------------------------
/// \brief     Convert adc value to 15 leds.
///
/// \param     none
///
/// \return    none
uint8_t equalizer_convert( uint32_t adcValue )
{  
   return (uint8_t)(m*(float)adcValue);
}