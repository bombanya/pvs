/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "LED.h"
#include "queue.h"
#include "speaker.h"
#include "uart_io.h"
#include "kb.h"
#include "pca9538.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIMEOUT 5000
#define GLOW_TIME 3000
#define CODE 0xD4
#define TIMES_BLINK 1
#define BLINK_PERIOD 250
#define MAX_TIMES_PRESSED_WRONG 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct queue input_queue;
struct queue output_queue;
struct queue kb_queue;

char output_buffer[256];

uint8_t row_n = 0;
uint8_t btn_res;

uint8_t input_byte;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
bool check_input();
enum keys char_to_key(char c);
struct melody init_melody();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	struct melody melody;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  input_queue = queue_init(256);
  output_queue = queue_init(256);
  kb_queue = queue_init(256);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  uart_io_init(&input_queue, &output_queue);
  kb_init(&kb_queue, TEST);
  melody = init_melody();
  game_register_melody(&melody);

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim6);
  //HAL_TIM_Base_Start_IT(&htim7);

  print_string("Init complete\n\r");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		btn_res = kb_check_row(btn_res);
		btn_res = (btn_res + 1) % 4;
		sprintf(output_buffer, "%d\n\r", btn_res);
		print_string(output_buffer);

		if (!queue_is_empty(&input_queue)) {
			input_byte = queue_pop(&input_queue);
			sprintf(output_buffer, "%c\n\r", input_byte);
			print_string(output_buffer);
			if (check_input()) {
				game_key_pressed(char_to_key(input_byte));
			} else {
				sprintf(output_buffer, "Wrong symbol: %c\n\r", input_byte);
				print_string(output_buffer);
			}
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 108;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void print_string(char* string) {
	uart_io_write_from_buffer((uint8_t *) string, strlen(string));
}

bool check_input() {
	return ((input_byte >= '1' && input_byte <= '9')
			|| input_byte == '+'
			|| input_byte == 'a'
			|| input_byte == '\r');
}

struct melody init_melody(){
	struct melody melody = melody_init(28);
	melody_add_play(&melody, re);
	melody_add_play(&melody, fa);
	melody_add_play(&melody, ut);
	melody_add_wait(&melody, 200);
	melody_add_play(&melody, fa);
	melody_add_play(&melody, mi);
	melody_add_play(&melody, sol);
	melody_add_wait(&melody, 500);
	melody_add_play(&melody, mi);
	melody_add_play(&melody, la);
	melody_add_play(&melody, si);
	melody_add_play(&melody, mi);
	melody_add_wait(&melody, 300);
	melody_add_play(&melody, re);
	melody_add_play(&melody, fa);
	melody_add_play(&melody, ut);
	melody_add_wait(&melody, 200);
	melody_add_play(&melody, re_2);
	melody_add_play(&melody, ut_2);
	melody_add_play(&melody, si);
	melody_add_play(&melody, la);
	melody_add_play(&melody, sol);
	melody_add_play(&melody, re_2);
	melody_add_play(&melody, re);
	melody_add_wait(&melody, 500);
	melody_add_play(&melody, fa);
	melody_add_play(&melody, si);
	melody_add_play(&melody, mi);
	return melody;
}

enum keys char_to_key(char c){
	switch(c){
	case '1':
		return KEY_1;
	case '2':
		return KEY_2;
	case '3':
		return KEY_3;
	case '4':
		return KEY_4;
	case '5':
		return KEY_5;
	case '6':
		return KEY_6;
	case '7':
		return KEY_7;
	case '8':
		return KEY_8;
	case '9':
		return KEY_9;
	case 'a':
		return KEY_A;
	case '+':
		return KEY_PLUS;
	case '\r':
		return KEY_ENTER;
	default:
		return NO_KEY;
	}
}

void main_notify_game_started(){
	print_string("Game is starting!\n\r");
}
void main_notify_game_finished(){
	sprintf(output_buffer, "Game finished! Your score is: %"PRIu32"!\n\r", game_get_score());
	print_string(output_buffer);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		game_timeout_callback();
	}
	else if (htim->Instance == TIM7) {
		kb_tim_callback();
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
