/*
    \file  i2c1-slave.c
    \brief slave receiver
*/

#include "i2c1-slave.h"

#define I2C1_DATA_ADDRESS	0x40005810

uint8_t i2c1_own_addr = 0x82;
volatile uint8_t i2c1_offset;
uint8_t i2c1_data_transmit = 0;

/*!
    \brief      handle I2C1 event interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_EventIRQ_Handler(void)
{
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND)){
        /* clear the ADDSEND bit */
        i2c_interrupt_flag_clear(I2C1,I2C_INT_FLAG_ADDSEND);			
    }
    else if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_STPDET)){
    	/* clear the STPDET bit */
    	i2c_enable(I2C1);
    }
}

/*!
    \brief      handle I2C1 error interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_ErrorIRQ_Handler(void)
{
    /* no acknowledge received */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_AERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_AERR);
    }

    /* SMBus alert */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SMBALT)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_SMBALT);
    }

    /* bus timeout in SMBus mode */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SMBTO)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_SMBTO);
    }

    /* over-run or under-run when SCL stretch is disabled */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_OUERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_OUERR);
    }

    /* arbitration lost */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_LOSTARB)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_LOSTARB);
    }

    /* bus error */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_BERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_BERR);
    }

    /* CRC value doesn't match */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_PECERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_PECERR);
    }

    i2c_enable(I2C1);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_DMA_Config(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC_DMA_channel deinit */
    dma_deinit(DMA0, DMA_CH3);
    											
    /* initialize DMA single data mode */
    dma_data_parameter.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_data_parameter.memory_addr  = (uint32_t)(&i2c1_data_transmit);
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_addr  = I2C1_DATA_ADDRESS;    
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.number       = 1;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;  
    dma_init(DMA0, DMA_CH3, &dma_data_parameter);
  	dma_circulation_enable(DMA0, DMA_CH3);

    /* ADC_DMA_channel deinit */
    dma_deinit(DMA0, DMA_CH4);
    /* initialize DMA single data mode */
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.memory_addr  = (uint32_t)(&i2c1_offset);
    dma_data_parameter.periph_addr  = I2C1_DATA_ADDRESS;    
    dma_data_parameter.number       = 1;
    dma_data_parameter.priority     = DMA_PRIORITY_ULTRA_HIGH;  
    dma_init(DMA0, DMA_CH4, &dma_data_parameter);
    dma_circulation_enable(DMA0, DMA_CH4);

    nvic_irq_enable(DMA0_Channel4_IRQn, 1, 0);
    dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
}

/*!
    \brief      cofigure the NVIC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_Nvic_Config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(I2C1_EV_IRQn, 0, 4);
    nvic_irq_enable(I2C1_ER_IRQn, 0, 1);
    /* enable the I2C1 interrupt */
    i2c_interrupt_enable(I2C1, I2C_INT_ERR);
    i2c_interrupt_enable(I2C1, I2C_INT_EV);
    i2c_interrupt_enable(I2C1, I2C_INT_BUF);
}

 /*!
    \brief      I2C1 从机配置
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_Slave_Config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);
    /* i2c0 pin_remap */
    rcu_periph_clock_enable(RCU_AF);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);

    /* connect PB10 to I2C1_SCL */
    /* connect PB11 to I2C1_SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);


    /* I2C clock configure */
    i2c_clock_config(I2C1, 400000, I2C_DTCY_16_9);
    /* I2C address configure */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, i2c1_own_addr);

    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
    i2c_stretch_scl_low_config(I2C1,I2C_SCLSTRETCH_DISABLE);

    I2C1_DMA_Config();
    i2c_dma_enable(I2C1,I2C_DMA_ON);
    /* enable DMA channel */
  	dma_channel_enable(DMA0, DMA_CH3);
    dma_channel_enable(DMA0, DMA_CH4);

    I2C1_Nvic_Config();
}

/*!
    \brief  	设置I2C1从机地址
    \param[in] 	addr:七位i2c地址
 */
void i2c1_set_own_addr(uint8_t addr)
{
    i2c1_own_addr = addr << 1;
}
