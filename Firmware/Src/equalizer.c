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
#define NR_OF_EFFECTS     ( 5u )
#define EFFECT_OPT_FADE   ( 0x01 )

// Private types     **********************************************************
typedef __packed struct equalizer_s{
   const uint8_t*    effect;
   uint8_t           effectIndex;
   uint8_t           level;
   uint8_t           levelTop;
   uint16_t          levelTopTime;
   
}euqalizer_t;

// Private variables **********************************************************
static const uint8_t effect_default[COL+1][3] =  { { 0x00, 0xff, 0x00 },     // green
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

static const uint8_t effect_red[COL+1][3] =      { { 0xff, 0x00, 0x00 },     
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0xff, 0x00, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // effect option flags

static const uint8_t effect_green[COL+1][3] =    { { 0x00, 0xff, 0x00 },     
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0xff, 0x00 },
                                                   { 0x00, 0x00, 0x00 } };    // effect option flags

static const uint8_t effect_blue[COL+1][3] =     { { 0x00, 0x00, 0xff },     
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
                                                   { 0x00, 0x00, 0xff },
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

static const uint8_t *effects[NR_OF_EFFECTS] = { &effect_default[0][0], &effect_red[0][0], &effect_green[0][0], &effect_blue[0][0], &effect_funny[0][0] };
                                                           
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
   equalizer.levelTop      = 0;
   equalizer.levelTopTime  = 0;
   equalizer.level         = 0;
   equalizer.effectIndex   = 0;
   equalizer.effect        = effects[equalizer.effectIndex%NR_OF_EFFECTS];
   
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
   static uint8_t topLevelBarCounter;
   topLevelBarCounter++;
   
   if( level >= COL )
   {
      return;
   }
   
   // level of the bar
   equalizer.level = level;
   
   // set top level
   if( equalizer.levelTop < equalizer.level )
   {
      equalizer.levelTop      = equalizer.level;
      equalizer.levelTopTime  = 250;
   }
   else if( equalizer.levelTopTime )
   {
      equalizer.levelTopTime--;
   }
   else if( equalizer.levelTop > 0 && topLevelBarCounter%2 )
   {
      equalizer.levelTop--;
   }
   
   // draw the bar
   WS2812B_clearBuffer();
   for( uint8_t i=0; i<level; i++ )
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