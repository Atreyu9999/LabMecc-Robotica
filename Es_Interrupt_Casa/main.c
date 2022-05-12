/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void setPow(int i);
void chooseLed(int k);
void LedOff();
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t msg_in[7];
uint8_t msg_debug[3] = "log";
uint32_t earthquake = 0;

uint32_t message_received = 0;
uint64_t k = 0;
uint32_t j = 0;
uint32_t i = 0;
uint8_t p[3] = "";
uint8_t f[3] = "";
uint32_t led_time = 0;
uint32_t led_time2 = 0;
uint32_t my_time = 0;
uint32_t my_time2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, msg_in, 7);
	HAL_TIM_Base_Start_IT(&htim2);
	my_time = HAL_GetTick();
	my_time2 = HAL_GetTick();

	led_time = my_time;
	led_time2 = my_time2;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		if (message_received) {
			message_received = 0;
			k = 0;
			j = 0;
			i = 0;

			k = (int) (msg_in[0] - '0');
			for (int w = 1; w <= 3; w++) {
				p[w - 1] = (int) (msg_in[w] - '0');
			}

			uint8_t counter = 0;
			for (int m = 2; m >= 0; m--) {
				//	i = 10 * i + p[m];
				i += p[m] * (int) pow(10.0, counter);
				counter++;
			}

			for (int w = 4; w <= 6; w++) {
				f[w - 4] = (int) (msg_in[w] - '0');
			}
			counter = 0;
			for (int m = 2; m >= 0; m--) {
				j += f[m] * (int) pow(10.0, counter);
				counter++;
			}
			HAL_UART_Receive_IT(&huart1, msg_in, 7);

		}
		if(message_received == 0) {
			chooseLed(k);
			setPow(i);
			LedOff();
		}


//			if(j == 999) {
//				HAL_UART_Transmit(&huart1, msg_debug, 3, 20);
//			}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void chooseLed(int k) {
	if (my_time - led_time > j / 2) {
		led_time = my_time;
		switch (k) {
		case 1:
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

			break;

		case 2:
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

			break;
		case 0:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			break;
		case 3:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			break;
		case 4:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			break;

		}
		earthquake = 1;
	}

}

void setPow(int i) {
	switch (k) {
	case 0:
		htim1.Instance->CCR1 = 0;
		htim1.Instance->CCR2 = 0;

		break;
	case 1:
		htim1.Instance->CCR1 = i;
		break;
	case 2:

		htim1.Instance->CCR2 = i;
	case 3:
		htim1.Instance->CCR1 = 0;
		break;
	case 4:
		htim1.Instance->CCR2 = 0;
		break;

	}

}

void LedOff() {
	if (my_time2 - led_time2 > j / 2) {
		led_time2 = my_time2;
		switch (k) {
		case 0:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

			break;
		case 1:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			break;
		case 2:

			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			break;
		case 3:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			break;
		case 4:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			break;

		}
		earthquake = 0;

	}

}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
