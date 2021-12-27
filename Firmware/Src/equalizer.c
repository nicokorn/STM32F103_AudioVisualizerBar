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

// Private types     **********************************************************
typedef struct equalizer_s{
   const uint8_t*    effect;
   uint8_t           level;
}euqalizer_t;

// Private variables **********************************************************
static const uint8_t effect_default[COL][3] =  { { 0x00, 0xff, 0x00 },     // green
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
                                                 { 0xff, 0x00, 0x00 } };    // red

static const uint8_t effect_test[COL][3] =     { { 0xff, 0x00, 0x00 },     
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
                                                 { 0xff, 0x00, 0x00 } };   
                                                           
// Private function prototypes ************************************************

// Private variables **********************************************************
static euqalizer_t equalizer;

// Functions ******************************************************************
// ----------------------------------------------------------------------------
/// \brief     Initialisation of the equalizer.
///
/// \param     none
///
/// \return    none
void equalizer_init( void )
{   
   equalizer.effect = &effect_test[0][0];
   equalizer.level  = 0;
}

// ----------------------------------------------------------------------------
/// \brief     Set equalizer bar level.
///
/// \param     none
///
/// \return    none
void equalizer_set( uint8_t level )
{   
   if( level > COL )
   {
      return;
   }
   
   equalizer.level = level;
   
   WS2812B_clearBuffer();
   for( uint8_t i=0; i<level; i++ )
   {
      WS2812B_setPixel( 0, i, *(equalizer.effect+i*3+0), *(equalizer.effect+i*3+1), *(equalizer.effect+i*3+2) ); 
   }
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