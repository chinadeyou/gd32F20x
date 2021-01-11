/*!
    \file  gt24c256.c
    \brief GT24C256接口函数，使用硬件I2C
*/

#include "gt24c256.h"
#include "systick.h"
#include "i2c2.h"

uint8_t gt24c256_addr = 0;

/*!
    \brief      configure the I2C2 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c2_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_AF);
    /* connect PA8 to I2C2_SCL */
    /* connect PC9 to I2C2_SDA */
    gpio_init(GPIOA, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_init(GPIOC, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_pin_remap1_config(GPIO_PCF5, GPIO_PCF5_I2C2_REMAP0, ENABLE);
		
    /* enable I2C clock */
    rcu_periph_clock_enable(RCU_I2C2);
    /* configure I2C clock */
    i2c_clock_config(I2C2,I2C2_SPEED,I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C2, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C2_OWN_ADDR7);
    /* enable I2C2 */
    i2c_enable(I2C2);
    /* enable acknowledge */
    i2c_ack_config(I2C2,I2C_ACK_ENABLE);
}

/*!
    \brief      write buffer of data to GT24C256
    \param[in]  p_buffer: pointer to the buffer  containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void gt24_eeprom_buffer_write(uint8_t* p_buffer, uint16_t write_address, uint16_t number_of_byte)
{
    uint16_t number_of_page = 0, number_of_single = 0, address = 0, count = 0;
    
    address = write_address % GT24C256_PAGE_SIZE;
    count = GT24C256_PAGE_SIZE - address;
    number_of_page =  number_of_byte / GT24C256_PAGE_SIZE;
    number_of_single = number_of_byte % GT24C256_PAGE_SIZE;
    
    /* if write_address is GT24C256_PAGE_SIZE aligned  */
    if(0 == address){
        while(number_of_page--){
            gt24_eeprom_page_write(p_buffer, write_address, GT24C256_PAGE_SIZE); 
            gt24_eeprom_wait_standby_state();
            write_address +=  GT24C256_PAGE_SIZE;
            p_buffer += GT24C256_PAGE_SIZE;
        }
        if(0 != number_of_single){
            gt24_eeprom_page_write(p_buffer, write_address, number_of_single);
            gt24_eeprom_wait_standby_state();
        }      
    }else{
        /* if write_address is not GT24C256_PAGE_SIZE aligned */
        if(number_of_byte < count){ 
            gt24_eeprom_page_write(p_buffer, write_address, number_of_byte);
            gt24_eeprom_wait_standby_state();
        }else{
            number_of_byte -= count;
            number_of_page =  number_of_byte / GT24C256_PAGE_SIZE;
            number_of_single = number_of_byte % GT24C256_PAGE_SIZE;
            
            if(0 != count){
                gt24_eeprom_page_write(p_buffer, write_address, count);
                gt24_eeprom_wait_standby_state();
                write_address += count;
                p_buffer += count;
            } 
            /* write page */
            while(number_of_page--){
                gt24_eeprom_page_write(p_buffer, write_address, GT24C256_PAGE_SIZE);
                gt24_eeprom_wait_standby_state();
                write_address +=  GT24C256_PAGE_SIZE;
                p_buffer += GT24C256_PAGE_SIZE;
            }
            /* write single */
            if(0 != number_of_single){
                gt24_eeprom_page_write(p_buffer, write_address, number_of_single); 
                gt24_eeprom_wait_standby_state();
            }
        }
    }  
}

/*!
    \brief      write one byte to GT24C256 
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[out] none
    \retval     none
*/
void gt24_eeprom_byte_write(uint8_t* p_buffer, uint16_t write_address)
{
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C2, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C2);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C2, gt24c256_addr, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C2,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get(I2C2, I2C_FLAG_TBE));
    
    /* send the EEPROM's internal address to write to : 16位片内地址,高字节 */
    i2c_data_transmit(I2C2, write_address >> 8);
    
	/* wait until BTC bit is set */	
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
	 
	/* send the EEPROM's internal address to write to : 16位片内地址,低字节 */
    i2c_data_transmit(I2C2, write_address);
	
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
    
    /* send the byte to be written */
    i2c_data_transmit(I2C2, *p_buffer); 
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C2);
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C2)&0x0200);
}

/*!
    \brief      write more than one byte to GT24C256 with a single write cycle
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void gt24_eeprom_page_write(uint8_t* p_buffer, uint16_t write_address, uint8_t number_of_byte)
{
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C2, I2C_FLAG_I2CBSY));
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C2);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C2, gt24c256_addr, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C2,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while( SET != i2c_flag_get(I2C2, I2C_FLAG_TBE));
    
    /* send the EEPROM's internal address to write to : 16位片内地址,高字节 */
    i2c_data_transmit(I2C2, write_address >> 8);
	
	/* wait until BTC bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
	
	/* send the EEPROM's internal address to write to : 16位片内地址,低字节 */
    i2c_data_transmit(I2C2, write_address);
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
	

    /* while there is data to be written */
    while(number_of_byte--){  
        i2c_data_transmit(I2C2, *p_buffer);
        
        /* point to the next byte to be written */
        p_buffer++; 
        
        /* wait until BTC bit is set */
        while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C2);
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C2)&0x0200);
}

/*!
    \brief      read data from GT24C256
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the EEPROM
    \param[in]  read_address: EEPROM's internal address to start reading from
    \param[in]  number_of_byte: number of bytes to reads from the EEPROM
    \param[out] none
    \retval     none
*/
void gt24_eeprom_buffer_read(uint8_t* p_buffer, uint16_t read_address, uint16_t number_of_byte)
{  
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C2, I2C_FLAG_I2CBSY));

    if(2 == number_of_byte){
        i2c_ackpos_config(I2C2,I2C_ACKPOS_NEXT);
    }
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C2);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C2, gt24c256_addr, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C2,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get( I2C2 , I2C_FLAG_TBE));

    /* enable I2C2*/
    i2c_enable(I2C2);
    
    /* send the EEPROM's internal address to write to : 16位片内地址,高字节  */
    i2c_data_transmit(I2C2, read_address >> 8);  
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
	
    /* send the EEPROM's internal address to write to : 16位片内地址,低字节  */	
	i2c_data_transmit(I2C2, read_address);
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_BTC)); 

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C2);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C2, gt24c256_addr, I2C_RECEIVER);

    if(number_of_byte < 3){
        /* disable acknowledge */
        i2c_ack_config(I2C2, I2C_ACK_DISABLE);
    }
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C2, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C2, I2C_FLAG_ADDSEND);
    
    if(1 == number_of_byte){
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C2);
    }
    
    /* while there is data to be read */
    while(number_of_byte){
        if(3 == number_of_byte){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C2,I2C_ACK_DISABLE);
        }
        if(2 == number_of_byte){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C2, I2C_FLAG_BTC));
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C2);
        }
        
        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C2, I2C_FLAG_RBNE)){
            /* read a byte from the EEPROM */
            *p_buffer = i2c_data_receive(I2C2);
            
            /* point to the next location where the byte read will be saved */
            p_buffer++; 
            
            /* decrement the read bytes counter */
            number_of_byte--;
        } 
    }
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C2)&0x0200);
    
    /* enable acknowledge */
    i2c_ack_config(I2C2,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C2,I2C_ACKPOS_CURRENT);
}

/*!
    \brief      wait for EEPROM standby state
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gt24_eeprom_wait_standby_state(void)
{
    __IO uint32_t val = 0;
    
    while(1){
        /* wait until I2C bus is idle */
        while(i2c_flag_get(I2C2, I2C_FLAG_I2CBSY));
        
        /* send a start condition to I2C bus */
        i2c_start_on_bus(I2C2);
        
        /* wait until SBSEND bit is set */
        while(!i2c_flag_get(I2C2, I2C_FLAG_SBSEND));
        
        /* send slave address to I2C bus */
        i2c_master_addressing(I2C2, gt24c256_addr, I2C_TRANSMITTER);
        
        /* keep looping till the Address is acknowledged or the AE flag is set (address not acknowledged at time) */
        do{
            /* get the current value of the I2C_STAT0 register */
            val = I2C_STAT0(I2C2);
            
        }while(0 == (val & (I2C_STAT0_ADDSEND | I2C_STAT0_AERR)));
         
        /* check if the ADDSEND flag has been set */
        if(val & I2C_STAT0_ADDSEND){
            
            /* clear ADDSEND flag */
            i2c_flag_clear(I2C2,I2C_FLAG_ADDSEND);
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C2);
            
            /* exit the function */
            return ;
            
        }else{
            /* clear the bit of AE */
            i2c_flag_clear(I2C2,I2C_FLAG_AERR);
        }
        
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C2);
        /* wait until the stop condition is finished */
        while(I2C_CTL0(I2C2)&0x0200);
    }
}


/*!
    \brief      write one byte to gt24c02
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the slave device
    \param[in]  write_address: salve's internal address to write to
    \param[out] none
    \retval     none
*/
void gt24c256_byte_write(uint16_t write_address, uint8_t* p_buffer)
{
	i2c2_start();
	i2c2_send_byte(gt24c256_addr);		//发送从机地址
	i2c2_wait_ack();
	i2c2_send_byte(write_address >> 8);	//发送片内地址 
	i2c2_wait_ack();
	i2c2_send_byte(write_address);		//发送片内地址 
	i2c2_wait_ack();
	i2c2_send_byte(*p_buffer);			//发送数据
	i2c2_wait_ack();
    i2c2_stop();						//产生一个停止条件 
}

/*!
    \brief      gt24c256 random address read
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the slave device
    \param[in]  read_address:  slave device's internal address to start reading from    
    \param[out] none
    \retval     none
*/
void gt24c256_random_read(uint16_t read_address, uint8_t* p_buffer)
{
	i2c2_start();
	i2c2_send_byte(gt24c256_addr);		//发送从机地址 + Write
	i2c2_wait_ack();
	i2c2_send_byte(read_address >> 8);	//发送片内地址	    
	i2c2_wait_ack();
	i2c2_send_byte(read_address);		//发送片内地址 
	i2c2_wait_ack();
	i2c2_start();
	i2c2_send_byte(gt24c256_addr + 1);	//发送从机地址 + Read
	i2c2_wait_ack();
	*p_buffer=i2c2_read_byte(0);
	i2c2_stop();						//产生一个停止条件    
}


/* slect gt24c256 device */
void gt24c256_init(uint8_t slave_addr)
{
	gt24c256_addr = slave_addr;
}
