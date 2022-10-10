/**
  ******************************************************************************
  * @file    ../Ejemplo ADS1115/Src/main.c
  * @author  Matías Nahuel Rodriguez
  * @brief   Un ejemplo que muestra como utilizar el driver ADS1115 para controlar
  *          el ADC del mismo nombre.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include "API_uart.h"
#include "ADS1115.h"
#include "ADS1115_port.h"



/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_Printf
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


///* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */



int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Initialize BSP Led for LED1, LED2 and LED3 */
  	BSP_LED_Init(LED1);
  	BSP_LED_Init(LED2);
  	BSP_LED_Init(LED3);


  /* Initialize BSP PB for BUTTON_USER */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /*Se declaran las estructuras para los canales de conversión*/
  signalADS1115 channelCero;
  signalADS1115 channelUno;
  signalADS1115 channelDos;

  /*Se declara una estructura auxiliar para recorrer un bucle for*/
  signalADS1115 channels;

  /*Se inicializa los puertos I2C, UART y puerto de lectura para señal READY.*/
  I2CGpio_init();
  uartInit();
  ADS1115_gpioReadyInit();

  /*Palabra donde se almacenará el resultado de la conversión.*/
  uint16_t wordReading;

  /*Array de 3 elementos utilizados para conversión de cada canal en Nro de cuenta a Volts.*/
  float voltage[3]={0.1,0.1,0.1};

  //-------------------------------------------
  //EJEMPLO FUNCIONAMIENTO SIN PIN READY

  //ADS1115_channelInit(&channelCero,SINGLE_MODE_A0);
  //ADS1115_channelInit(&channelUno,SINGLE_MODE_A1);
  //ADS1115_channelInit(&channelDos,DIFERENTIAL_MODE_A2_A3);
  //-------------------------------------------

  //-------------------------------------------
  //EJEMPLO FUNCIONAMIENTO CON PIN READY

  ADS1115_channelInitPolled(&channelCero,SINGLE_MODE_A0,SLAVE_ADRRES_GND);
  ADS1115_channelInitPolled(&channelUno,SINGLE_MODE_A1,SLAVE_ADRRES_GND);
  ADS1115_channelInitPolled(&channelDos,DIFERENTIAL_MODE_A2_A3,SLAVE_ADRRES_GND);
  //-------------------------------------------

  //----------------------------------------------
  //-----------UPDATE FUNTIONS--------------------
  //ADS1115_updateOperationMode(&channelCero,MODE_CONTINUOUS_CONVERSION);
  //----------------------------------------------

  //------------PARA MODO CONTINUOUS----------------
  //ADS1115_startConversionPolled(&channelCero,SLAVE_ADRRES_GND);
  //----------------------------------------------
  /* Infinite loop */
  while (1)
  {
	  	BSP_LED_On(LED1);
	  	BSP_LED_Off(LED2);
	  	BSP_LED_On(LED3);
	  //-------------------------------------------
	  //EJEMPLO FUNCIONAMIENTO CON PIN READY (POLLED DRIVEN)

	  for(int i=0;i<3;i++){
	  if (i==0){
		  channels=channelCero;
	  }
	  else if(i==1){
		  channels=channelUno;
	  }
	  else if(i==2){
		  channels=channelDos;
	  }

	  ADS1115_startConversionPolled(&channels,SLAVE_ADRRES_GND);
	  wordReading=ADS1115_getConversionPolled(&channels,SLAVE_ADRRES_GND);

	  BSP_LED_On(LED2);

	  voltage[i]=ADS1115_getValueVoltage(&channels);
	  }

	  sendADS1115ByUart(voltage);
	  //-------------------------------------------


	  //-------------------------------------------
	  //EJEMPLO FUNCIONAMIENTO EN MODO CONTINUO
	  /*
	  wordReading=ADS1115_getConversionPolled(&channelCero,SLAVE_ADRRES_GND);

	  voltage[0]=ADS1115_getValueVoltage(&channelCero);

	  sendADS1115ByUart(voltage);
	  BSP_LED_On(LED2);*/
	  //-------------------------------------------


	  //-------------------------------------------
	  //EJEMPLO FUNCIONAMIENTO SIN PIN READY
/*
	  for(int i=0;i<3;i++){
	  if (i==0){
		  channels=channelCero;
	  }
	  else if(i==1){
		  channels=channelUno;
	  }
	  else if(i==2){
		  channels=channelDos;
	  }
	  wordReading=ADS1115_signalConversion(&channels,SLAVE_ADRRES_GND);

	  voltage[i]=ADS1115_getValueVoltage(&channels);
	  }

	  sendADS1115ByUart(voltage);*/
	  //-------------------------------------------



  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
