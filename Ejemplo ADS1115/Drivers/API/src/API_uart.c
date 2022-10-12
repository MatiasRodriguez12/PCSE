/*
 * API_uart.c
 *
 *  Created on: 21 sep. 2022
 *      Author: MATIAS
 */

#include "API_uart.h"
#include <string.h>
#include "main.h"

UART_HandleTypeDef UartHandle;


/* Inicia la comunicación UART.
 * Envía mensaje indicando los parametros de su configuración.
 * Devuelve valor booleano TRUE si la iniciación fue exitosa.*/
bool uartInit(void){
	 bool successfullInitialization=true;
	 char baudRateValue[nroCaracteresConversion]={};
	 char wordLengthValue[nroCaracteresConversion]={};
	 char StopBitsValue[nroCaracteresConversion]={};
	 char parityValue[nroCaracteresConversion]={};

	  /*##-1- Configure the UART peripheral ######################################*/
	  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	  /* UART configured as follows:
	      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
		                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	      - Stop Bit    = One Stop bit
	      - Parity      = ODD parity
	      - BaudRate    = 9600 baud
	      - Hardware flow control disabled (RTS and CTS signals) */
	  UartHandle.Instance        = USARTx;

	  UartHandle.Init.BaudRate   = 9600;
	  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits   = UART_STOPBITS_1;
	  UartHandle.Init.Parity     = UART_PARITY_ODD;
	  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode       = UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&UartHandle) != HAL_OK)
	  {
	    /* Initialization Error */
		successfullInitialization=false;
	    while(1){

	    }

	  }

	  //Funcion que convierte valor numerico a char
	  sprintf((char*) baudRateValue, "%d",(int) UartHandle.Init.BaudRate);
	  sprintf((char*) wordLengthValue, "%d",(int) UartHandle.Init.WordLength);
	  sprintf((char*) StopBitsValue, "%d",(int) UartHandle.Init.StopBits);
	  sprintf((char*) parityValue, "%d",(int) UartHandle.Init.Parity);

	  uartSendString((uint8_t*)"Uart Iniciada\n\r");
	  uartSendString((uint8_t*)"-----Parametros UART-----\n\r");
	  uartSendString((uint8_t*)"\n\rBaud rate: ");
	  uartSendString((uint8_t*)baudRateValue);
	  uartSendString((uint8_t*)"\n\rWord Length: ");
	  uartSendString((uint8_t*)wordLengthValue);
	  uartSendString((uint8_t*)"\n\rStop Bits: ");
	  uartSendString((uint8_t*)StopBitsValue);
	  uartSendString((uint8_t*)"\n\rParity: ");
	  uartSendString((uint8_t*)parityValue);
	  uartSendString((uint8_t*)"\n\r");

	  return successfullInitialization;
}

/* Función que envia un mensaje por UART.
 * Recibe un punto a array como parametro, el cual contiene el mensaje a enviar.*/
void uartSendString(uint8_t * pstring){
	HAL_UART_Transmit(&UartHandle,(uint8_t*) pstring,strlen((const char*) pstring), timeOutUart);
}

/* Función que envia un mensaje por UART.
 * Recibe un punto a array como parametro, el cual contiene el mensaje a enviar.
 * Tambien se debe indicar la cantidad de caracteres que deben ser enviados.*/
void uartSendStringSize(uint8_t * pstring, uint16_t size){
	HAL_UART_Transmit(&UartHandle,(uint8_t*) pstring,size, timeOutUart);
}

void sendADS1115ByUart(float voltage[3]){
	  float decimal=0.0;
	  char voltajeChar[6];
	  char dec[4];
	  char channel[2];
	  uartSendString((uint8_t*)"\n\r");
	  for(int i=0;i<3;i++){
		  uartSendString((uint8_t*)"Canal: ");
		  sprintf((char*) channel, "%d",(int) i);
		  uartSendString((uint8_t*)channel);
		  uartSendString((uint8_t*)"  ");
		  sprintf((char*) voltajeChar, "%d",(int) voltage[i]);
		  uartSendString((uint8_t*)voltajeChar);
	 	  if (voltage[i]<1) {
	 		 voltage[i]=voltage[i];
	 	  }
	 	  else if ((voltage[i]<2)){
	 		 voltage[i]=voltage[i]-1.0;
	 	  }
	 	  else if ((voltage[i]<3)){
	 		 voltage[i]=voltage[i]-2.0;
	 	  }
	 	  else if ((voltage[i]<4)){
	 		 voltage[i]=voltage[i]-3.0;
	 	  }
	 	  else{
	 		 voltage[i]=voltage[i]-4.0;
	 	  }


     	  decimal=voltage[i]*1000;

	      sprintf((char*) dec, "%d",(int) decimal);
		  uartSendString((uint8_t*)",");
		  if (decimal<10){
		  uartSendString((uint8_t*)"00");
		  }
    	  else if(decimal<100){
	    	uartSendString((uint8_t*)"0");
		  }

		  uartSendString((uint8_t*)dec);
		  uartSendString((uint8_t*)"  ");
		  uartSendString((uint8_t*)"  ");
		  uartSendString((uint8_t*)"  ");

	  }

}
