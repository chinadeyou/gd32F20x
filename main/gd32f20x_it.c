/*!
    \file  gd32f20x_it.c
    \brief interrupt service routines

    \version 2015-07-15, V1.0.0, firmware for GD32F20x
    \version 2017-06-05, V2.0.0, firmware for GD32F20x
    \version 2018-10-31, V2.1.0, firmware for GD32F20x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f20x_it.h"
#include "systick.h"
#include "i2c0-slave.h"
#include "i2c1-slave.h"
#include "i2c2-slave.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}

/*!
    \brief      this function handles I2C0 event interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C0_EV_IRQHandler(void)
{
    I2C0_EventIRQ_Handler();
}

/*!
    \brief      this function handles I2C0 error interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C0_ER_IRQHandler(void)
{
    I2C0_ErrorIRQ_Handler();
}

/*!
    \brief      this function handles I2C1 event interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_EV_IRQHandler(void)
{
    I2C1_EventIRQ_Handler();
}

/*!
    \brief      this function handles I2C1 error interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_ER_IRQHandler(void)
{
    I2C1_ErrorIRQ_Handler();
}

/*!
    \brief      this function handles I2C1 event interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C2_EV_IRQHandler(void)
{
    I2C2_EventIRQ_Handler();
}

/*!
    \brief      this function handles I2C1 error interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C2_ER_IRQHandler(void)
{
    I2C2_ErrorIRQ_Handler();
}

/*!
    \brief		DMA0 Channel6 interrupt handler，i2c0 RX
*/
void DMA0_Channel6_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF)){     
   		dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_G);
    	if(i2c0_offset < 32)
    		i2c0_data_transmit = i2c_reg[i2c0_offset];
    	else
    		i2c0_data_transmit = 0xff;
    }
}

/*!
    \brief		DMA0 Channel4 interrupt handler，i2c1 RX
*/
void DMA0_Channel4_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0, DMA_CH4, DMA_INT_FLAG_FTF)){     
   		dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_G);
    	if(i2c1_offset < 32)
    		i2c1_data_transmit = i2c_reg[i2c1_offset];
    	else
    		i2c1_data_transmit = 0xff;
    }
}

/*!
    \brief		DMA1 Channel6 interrupt handler，i2c2 RX
*/
void DMA1_Channel6_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1, DMA_CH6, DMA_INT_FLAG_FTF)){     
   		dma_interrupt_flag_clear(DMA1, DMA_CH6, DMA_INT_FLAG_G);
    	if(i2c2_offset < 32)
    		i2c2_data_transmit = i2c_reg[i2c2_offset];
    	else
    		i2c2_data_transmit = 0xff;
    }
}


