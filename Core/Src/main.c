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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LED.h"
#include "side_button.h"
#include "queue.h"
#include "uart_io.h"
#include <stdio.h>
#include <string.h>
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

uint8_t code[8] = "abuba123";
uint8_t code_len = 8;
uint8_t new_code[8];

uint8_t pos = 0;
uint8_t times_pressed_wrong = 0;
uint32_t time_last_input = 0;
uint32_t time_last_auth = 0;

uint8_t code_changing_mode = 0;
uint8_t code_changing_waiting_for_confirmation = 0;

struct queue input_queue;
struct queue output_queue;

char output_buffer[256];

uint8_t input_byte;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void set_code(uint8_t proto);
uint8_t check_input();
void auth(uint8_t new_input);
void change_code(uint8_t new_input);

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  input_queue = queue_init(256);
  output_queue = queue_init(256);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  LED_turn_off(RED);
  LED_turn_off(GREEN);
  LED_turn_off(YELLOW);
  uart_io_init(&input_queue, &output_queue, NONBLOCKING);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  side_button_pressed_callback();
	  uart_io_read_data();

	  if (!queue_is_empty(&input_queue)) {
		  input_byte = queue_pop(&input_queue);
		  time_last_input = HAL_GetTick();
		  sprintf(output_buffer, "%c", input_byte);
		  print_string(output_buffer);

		  if (code_changing_mode) change_code(1);
		  else if (pos == 0 && input_byte == '+') {
			  if (HAL_GetTick() - time_last_auth > 2 * TIMEOUT || time_last_auth == 0)
				  print_string("\n\renter your code first to change it\n\r");
			  else {
				  pos = 0;
				  times_pressed_wrong = 0;
				  code_changing_mode = 1;
				  print_string("\n\renter new code\n\r");
			  }
		  }
		  else auth(1);
	  }

	  if (code_changing_mode) change_code(0);
	  else auth(0);

	  if (side_button_get_pressed()) uart_io_change_mode();

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void print_string(char* string) {
	uart_io_write_from_buffer((uint8_t *) string, strlen(string));
}

void set_code(uint8_t proto){
	for(size_t i = 0; i < sizeof(uint8_t) * 8; i++){
		code[i] = (proto >> (sizeof(uint8_t) * 8 - i - 1)) & 1 ? LONG : SHORT;
	}
}

int _write(int file, char *ptr, int len) {
	uart_io_write_from_buffer((uint8_t *) ptr, len);
	return 0;
}

uint8_t check_input() {
	if (input_byte >= 65 && input_byte <= 90) input_byte += 32;
	return ((input_byte >= '0' && input_byte <= '9')
			|| (input_byte >= 'a' && input_byte <= 'z'));
}

void auth(uint8_t new_input) {
	if (new_input) {
		check_input();
		if (pos < code_len && input_byte == code[pos]) {
				  uint8_t i;
				  pos++;
				  for(i = 0; i < TIMES_BLINK; i++)
					  LED_blink(YELLOW, BLINK_PERIOD);
			  } else if (pos < code_len && input_byte != code[pos]) {
				  times_pressed_wrong++;
				  pos = 0;
				  print_string("\n\rgo away, hacker\n\r");

				  if(times_pressed_wrong <= MAX_TIMES_PRESSED_WRONG) {
					  uint8_t i;
					  for(i = 0; i < TIMES_BLINK; i++){
						  LED_blink(RED, BLINK_PERIOD);
					  }
				  }
				  else {
					  LED_turn_on(RED);
					  HAL_Delay(GLOW_TIME);
					  LED_turn_off(RED);
					  times_pressed_wrong = 0;
				  }
			  }
		  } else if (HAL_GetTick() - time_last_input > TIMEOUT &&
					(pos != 0 || times_pressed_wrong != 0)){
			  pos = 0;
			  print_string("\n\rtoo slow\n\r");
			  LED_turn_on(RED);
			  HAL_Delay(GLOW_TIME);
			  LED_turn_off(RED);
			  times_pressed_wrong = 0;
		  }

		  if (pos == code_len) {
			  print_string("\n\rwelcome\n\r");
			  LED_turn_on(GREEN);
			  HAL_Delay(GLOW_TIME);
			  LED_turn_off(GREEN);
			  pos = 0;
			  times_pressed_wrong = 0;
			  time_last_auth = HAL_GetTick();
		  }
}


void change_code(uint8_t new_input) {
	if (new_input) {
		if (code_changing_waiting_for_confirmation) {
			code_changing_waiting_for_confirmation = 0;
			code_changing_mode = 0;
			if (input_byte == 'y') {
				code_len = pos;
				for (size_t i = 0; i < code_len; i++) code[i] = new_code[i];
				print_string("\n\rcode changed\n\r");
			}
			else print_string("\n\rcanceling the code change\n\r");
			pos = 0;
		}
		else {
			if (check_input()) {
				new_code[pos] = input_byte;
				pos++;
			}
			else if (input_byte != 13) {
				code_changing_mode = 0;
				pos = 0;
				print_string("\n\rincorrect symbol\n\rcanceling the code change\n\r");
			}
			if (input_byte == 13 || pos == 8) {
				code_changing_waiting_for_confirmation = 1;
				print_string("\n\rconfirm the code change (y)\n\r");
			}
		}
	}
	else if (HAL_GetTick() - time_last_input > TIMEOUT) {
		code_changing_mode = 0;
		pos = 0;
		code_changing_waiting_for_confirmation = 0;
		print_string("\n\rcanceling the code change\n\rtoo slow\n\r");
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
