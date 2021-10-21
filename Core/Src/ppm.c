/*
 * PPM.c
 *
 *  Created on: Oct 20, 2021
 *      Author: Jesus
 */

#include <stdint.h>

#include "gpio.h"
#include "ppm.h"
#include "tim.h"

/* Private macro ------------------------------------------------------------*/
#define SPEED_HIGH      10U
#define SPEED_MEDIUM    90U
#define SPEED_LOW       140U

#define PHASE_LOW       0x01U
#define PHASE_MID       0x02U
#define PHASE_HIGH      0x04U

#define SPEED_START     170U

#define TIME_DELAY_SOFT 10U

/*------------------------------------------------------------*/

volatile uint8_t G_phase_Value = PHASE_MID;
uint8_t G_Angule_Value = 0x00U;
Direction_increment_T gflag_increment;
uint8_t lock_swticin = 0x00U;


static void delay_us(uint32_t microseconds);
static void soft_switching(uint8_t angule_start, uint8_t angule_end, Direction_increment_T direction);
static uint16_t angule_to_aime(uint8_t angule);
static void armonic_reduction(void);

/**
  * @brief  This function is intended to Generate the Pulse to be modulated
  * @param  specific angle were the pulse in modulated
  * @note   The angle value must be between 10 to 170 degrees
  * @retval None
  */
void speed_set(void)
{
	static uint8_t lphase = PHASE_MID;
	uint8_t langule = 0x00U;

	/* Checking button status*/
	if(lphase != G_phase_Value)
	{

		switch(G_phase_Value)
		{
		case PHASE_LOW:
			langule = SPEED_LOW;
			break;
		case PHASE_MID:
			langule = SPEED_MEDIUM;
			break;
		case PHASE_HIGH:
			langule = SPEED_HIGH;
			break;
		default:
			break;
		}

		soft_switching(G_Angule_Value, langule, gflag_increment);

		/* Updating local variable with current phase*/
		lphase = G_phase_Value;
		/* Updating Global variable with last angle value*/
		G_Angule_Value = langule;
	}

	else
	{
		if(G_Angule_Value <= SPEED_LOW && G_Angule_Value >= SPEED_HIGH)
		{
			armonic_reduction();
		}
	}
}

/**
  * @brief  This function is intended to Generate the Pulse to be modulated
  * @param  specific angle were the pulse in modulated
  * @note   The angle value must be between 10 to 170 degrees
  * @retval None
  */
void ppm(uint8_t angle)
{
	HAL_GPIO_WritePin(TRIGGER_PIN_PORT, TRIGGER_PIN, GPIO_PIN_RESET);
	delay_us(angule_to_aime(angle));
	HAL_GPIO_WritePin(TRIGGER_PIN_PORT, TRIGGER_PIN, GPIO_PIN_SET);
	delay_us(PULSE_WITH_TRIGGER);
	HAL_GPIO_WritePin(TRIGGER_PIN_PORT, TRIGGER_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  This function is intended to Generate the Pulse to be modulated
  * @param  specific angle were the pulse in modulated
  * @note   The angle value must be between 10 to 170 degrees
  * @retval None
  */
void soft_start(void)
{
	soft_switching(SPEED_START, SPEED_MEDIUM, decrement);
}

/**
  * @brief  This function is intended to Generate the Pulse to be modulated
  * @param  specific angle were the pulse in modulated
  * @note   The angle value must be between 10 to 170 degrees
  * @retval None
  */
static void delay_us(uint32_t microseconds)
{
	__HAL_TIM_SET_COUNTER(&htim7, 0x00U);

	while( __HAL_TIM_GET_COUNTER(&htim7) < microseconds);
}

/**
  * @brief  This function will convert Angle to a time interval
  * @param  angle to be converted
  * @note   The angle value must be between 10 to 170 degrees
  * @retval time equivalence
  */
static uint16_t angule_to_aime(uint8_t angule)
{
	uint16_t ltime_result_convertion = 0x00U;

	ltime_result_convertion = ((9000U * angule) / (SPEED_START));

	return ltime_result_convertion;

}

/**
  * @brief  This algorithm will reduce the arminc
  * @param  None
  * @retval None
  */
static void armonic_reduction(void)
{
	G_Angule_Value += 5;
	HAL_Delay(8);
	G_Angule_Value -= 5;
	HAL_Delay(8);
}

/**
  * @brief  This will generate a soft trasition between angles
  * @param  Staring Angle
  * @param  End Angle
  * @param  Direction, this can be one of two posible values from Direction_increment_T:
  *         -increment
  *         -decrement
  * @retval None
  */
static void soft_switching(uint8_t angule_start, uint8_t angule_end, Direction_increment_T direction)
{
	uint8_t liter = 0;
	if(direction == increment)
	{
		for(liter = angule_start; liter <= angule_end; liter++)
		{
			G_Angule_Value = liter;
			HAL_Delay(TIME_DELAY_SOFT);
		}
	}
	else if(direction == decrement)
	{
		for(liter = angule_start; liter >= angule_end; liter--)
		{
			G_Angule_Value = liter;
			HAL_Delay(TIME_DELAY_SOFT);
		}
	}
}

/*
 *  USER_ID     DATE        Description
 *  Jesus R    10/17/2021   Creation of module
 *  Jesus R    10/18/2021   Creation of Static fuctions
 *  Jesus R    10/20/2021   Documentation of each Method
 */
