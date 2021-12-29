// ****************************************************************************
/// \file      microphone.c
///
/// \brief     microphone C Source File
///
/// \details   Driver Module for microphone.
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
#include "microphone.h"

// Private define *************************************************************

// Private types     **********************************************************

// Private variables **********************************************************

// Private function prototypes ************************************************

// Global variables ***********************************************************
ADC_HandleTypeDef hadc1;

// Private variables **********************************************************

// Functions ******************************************************************
// ----------------------------------------------------------------------------
/// \brief     Initialisation of the peripherals for the microphone.
///
/// \param     none
///
/// \return    none
MICROPHONE_StatusTypeDef microphone_init( void )
{   
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   ADC_ChannelConfTypeDef sConfig   = {0};

   __HAL_RCC_ADC1_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();

   // Microphone analog GPIO init
   GPIO_InitStruct.Pin              = MICROPHONE_ANALOG_PIN;
   GPIO_InitStruct.Mode             = GPIO_MODE_ANALOG;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   
	// Microphone digital GPIO init
	//__HAL_RCC_GPIOB_CLK_ENABLE();                   
	//GPIO_InitTypeDef GPIO_InitStruct_Digital;
	//GPIO_InitStruct_Digital.Pin 		= MICROPHONE_DIGITAL_PIN; 	
	//GPIO_InitStruct_Digital.Mode 		= GPIO_MODE_IT_FALLING; 			
	//GPIO_InitStruct_Digital.Pull 		= GPIO_NOPULL;							
	//HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_Digital);
   //
	//HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI1_IRQn);

   // ADC init
   hadc1.Instance                   = ADC1;
   hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;
   hadc1.Init.ContinuousConvMode    = DISABLE;
   hadc1.Init.DiscontinuousConvMode = DISABLE;
   hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
   hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
   hadc1.Init.NbrOfConversion       = 1;
   if( HAL_ADC_Init(&hadc1) != HAL_OK )
   {
      return MICROPHONE_ERROR;
   }
   
   sConfig.Channel                  = ADC_CHANNEL_8;
   sConfig.Rank                     = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime             = ADC_SAMPLETIME_1CYCLE_5;
   if( HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK )
   {
      return MICROPHONE_ERROR;
   }
   
   return MICROPHONE_OK;
}
// ----------------------------------------------------------------------------
/// \brief     Make an adc measurement.
///
/// \param     none
///
/// \return    uint32_t adc value
uint32_t microphone_getAdc( void )
{
   HAL_ADC_Start(&hadc1);
   
   if( HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK )
   {
      return HAL_ADC_GetValue(&hadc1);
   }
   
   return 0;
}