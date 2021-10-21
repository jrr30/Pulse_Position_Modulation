/*
 * PPM.h
 *
 *  Created on: Oct 17, 2021
 *      Author: Jesus
 */

#ifndef INC_PPM_H_
#define INC_PPM_H_

/* Exported macro ------------------------------------------------------------*/
#define TRIGGER_PIN_PORT GPIOC
#define TRIGGER_PIN      GPIO_PIN_8
#define SYNC_PIN         GPIO_PIN_11
#define INCREMENT_BUTTON GPIO_PIN_10
#define DECREMENT_BUTTON GPIO_PIN_12

/**
  * @brief  structure definition to set up counting up and down
  */
typedef enum Direction_increment_t
{
	increment,
	decrement
}Direction_increment_T;


/**
  * @brief  This API is intended to Generate the Pulse to be modulated
  * @param  specific angle were the pulse in modulated
  * @note   The angle value must be between 10 to 170 degrees
  * @retval None
  */
void PPM(uint8_t angle);

/**
  * @brief  This API purpose is perform a soft start with an angle of 170 to 90 degrees
  * @param  None
  * @retval None
  */
void Soft_Start(void);

/**
  * @brief  This API sets the speed
  * @param  None
  * @retval None
  */
void Set_Speed(void);

#endif /* INC_PPM_H_ */

/*
 *  USER_ID     DATE        Description
 *  Jesus R    10/17/2021   Creation of module
 *  Jesus R    10/18/2021   Creation of Static fuctions
 *  Jesus R    10/20/2021   Documentation of each Method
 */
