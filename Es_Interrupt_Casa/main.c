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
#include "stdbool.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//void setPow(int i);
void chooseLed(int k);
void LedOff();
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t msg_in[7];


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
uint32_t z = 0;
uint32_t x = 0;
uint32_t l1 = 0;
uint32_t l2 = 0;
uint8_t msg_err[12] = "error input";
uint8_t msg_def[13] = "correct input";
uint32_t vectSize = 0;

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
	MX_TIM16_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, &msg_in[0], sizeof msg_in / sizeof *msg_in);
	HAL_TIM_Base_Start_IT(&htim16);



	led_time = my_time;
	led_time2 = my_time2;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		if (message_received) {
			message_received = 0;
			k = 0;
			j = 0;
			i = 0;
			for (int a = 0; a < sizeof msg_in / sizeof *msg_in; a++) {
				if (msg_in[a] != '\000') {
					vectSize++;
				}
			}
			if (vectSize != 7) {
				HAL_UART_Transmit(&huart1, &msg_err[0], 12, 20);
				for (int b = 0; b < sizeof msg_in / sizeof *msg_in; b++) {
					msg_in[b] = '\000';
				}

			} else if (msg_in[0] - '0' != 1 && msg_in[0] - '0' != 2
					&& msg_in[0] - '0' != 3 && msg_in[0] - '0' != 4) {

				HAL_UART_Transmit(&huart1, &msg_err[0], 12, 20);
				for (int b = 0; b < sizeof msg_in / sizeof *msg_in; b++) {
					msg_in[b] = '\000';
				}
			} else if (isdigit(msg_in[1]) == false
					|| isdigit(msg_in[2]) == false
					|| isdigit(msg_in[3]) == false
					|| isdigit(msg_in[4]) == false
					|| isdigit(msg_in[5]) == false
					|| isdigit(msg_in[6]) == false) {
				HAL_UART_Transmit(&huart1, &msg_err[0], 12, 20);
				for (int b = 0; b < sizeof msg_in / sizeof *msg_in; b++) {
					msg_in[b] = '\000';
				}
			} else {
				HAL_UART_Transmit(&huart1, &msg_def[0], 13, 20);
			}

			k = (int) (msg_in[0] - '0');
			for (int w = 1; w <= 3; w++) {
				p[w - 1] = (int) (msg_in[w] - '0');
			}

			uint8_t counter = 0;
			for (int m = 2; m >= 0; m--) {

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

			vectSize = 0;
			HAL_UART_Receive_IT(&huart1, &msg_in[0],sizeof msg_in / sizeof *msg_in);

		}
		chooseLed(k);
		LedOff();


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
	if (my_time - led_time > j / 50) {
		led_time = my_time;
		my_time2 -= j / 50;

		switch (k) {
		case 1:
			htim1.Instance->CCR1 = i;
			z = i;
			if (l2 != 0) {
				htim1.Instance->CCR2 = x;
			}
			l1 = 1;

			break;

		case 2:
			htim1.Instance->CCR2 = i;
			x = i;
			if (l1 != 0) {
				htim1.Instance->CCR1 = z;
			}
			l2 = 1;

			break;
		case 0:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			l1 = 0;
			l2 = 0;
			break;
		case 3:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			if (l2 == 1) {
				htim1.Instance->CCR2 = i;
			}
			l1 = 0;

			break;
		case 4:
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			if (l1 == 1) {
				htim1.Instance->CCR1 = i;
			}
			l2 = 0;
			break;

		}

	}

}

//void setPow(int i) {
//	switch (k) {
//	case 0:
//		htim1.Instance->CCR1 = 0;
//		htim1.Instance->CCR2 = 0;
//
//		break;
//	case 1:
//		htim1.Instance->CCR1 = i;
//		break;
//	case 2:
//
//		htim1.Instance->CCR2 = i;
//		break;
//	case 3:
//		htim1.Instance->CCR1 = 0;
//		break;
//	case 4:
//		htim1.Instance->CCR2 = 0;
//		break;
//
//	}
//
//}

void LedOff() {
	if (my_time2 - led_time2 > j / 50) {
		led_time2 = my_time2;
		if (l1 == 1) {
			htim1.Instance->CCR1 = 0;
		}
		if (l2 == 1) {
			htim1.Instance->CCR2 = 0;
		}

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



