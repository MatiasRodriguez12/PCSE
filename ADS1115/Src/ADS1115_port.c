/*
 * ADS1115_port.c
 *
 *  Created on: 3 oct. 2022
 *      Author: MATIAS
 */
#include "ADS1115_port.h"
#include <stdbool.h>


I2C_HandleTypeDef I2C1_handle;

/*I2CGpio_init:
 *Función que inicializa la configuración de los puertos SDA y SCL.*/
void I2CGpio_init(void){
	GPIO_InitTypeDef gpio_I2C={0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_I2C.Pin 		= i2c_SDA_pin | i2c_SCL_pin;
	gpio_I2C.Mode 		= GPIO_MODE_AF_OD;
	gpio_I2C.Pull 		= GPIO_NOPULL;
	gpio_I2C.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_I2C.Alternate	= GPIO_AF4_I2C1;

	HAL_GPIO_Init(i2c_SDA_port,&gpio_I2C);

	__HAL_RCC_I2C1_CLK_ENABLE();
	I2C1_handle.Instance = I2C1;
	I2C1_handle.Init.ClockSpeed = 100000;
	I2C1_handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C1_handle.Init.OwnAddress1 = 0;
	I2C1_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C1_handle.Init.OwnAddress2 = 0;
	I2C1_handle.Init.DualAddressMode= I2C_DUALADDRESS_DISABLE;
	I2C1_handle.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	I2C1_handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&I2C1_handle) != HAL_OK)
	  {
		 BSP_LED_On(LED2);
		  while (1)
		  {
		  }
	  }
}

/*gpioADS1115Ready_init:
 *Función que inicializa la configuración del puerto de entrada por donde se leerá la señal READY.*/
void gpioADS1115Ready_init(void){

	GPIO_InitTypeDef gpioButtonInit={0};

	__HAL_RCC_GPIOF_CLK_ENABLE();

	gpioButtonInit.Pin = (D7_pin);
	gpioButtonInit.Mode= GPIO_MODE_INPUT;
	gpioButtonInit.Pull = GPIO_PULLDOWN;
	gpioButtonInit.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(D7_port, &gpioButtonInit);
}

/*gpioADS1115Ready_init:
 *Función lee el estado del puerto de entrada.*/
bool gpioADS1115Ready_Read(void){
	bool stateButton=false;
	if(HAL_GPIO_ReadPin(D7_port, D7_pin)){
		stateButton=true;

	}
	else{
		stateButton=false;
	}
	return stateButton;
}

/*ADS1115_Transmit:
 *Función que envia datos desde el Master al Esclavo.
 *Parametros: slaveAddres - Dirección del esclavo.
 *			  word - datos a enviar.
 *			  nroBytes - Cantidad de bytes a enviar*/
void ADS1115_Transmit(uint8_t slaveAddres, uint8_t *word,uint16_t nroBytes){
	HAL_I2C_Master_Transmit(&I2C1_handle,slaveAddres<<1,(uint8_t *)word,nroBytes,100);
}

/*ADS1115_Receive:
 *Función que lee los datos del esclavo.
 *Parametros: slaveAddres - Dirección del esclavo.
 *			  word - Lugar donde se va a almacenar la información.
 *			  nroBytes - Cantidad de bytes a recibir*/
void ADS1115_Receive(uint8_t slaveAddres,uint8_t *word,uint16_t nroBytes){
		HAL_I2C_Master_Receive(&I2C1_handle,slaveAddres<<1,(uint8_t *)word,nroBytes,100);
}

/*ADS1115_delay:
 *Función que realiza un delay bloqueante.
 *Parametro: delay - Tiempo expresado en ms.*/
void ADS1115_delay(uint8_t delay){
	HAL_Delay(delay);
}


