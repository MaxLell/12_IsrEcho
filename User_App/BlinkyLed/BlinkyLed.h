/*
 * blinky_led.h
 *
 *  Created on: Jul 30, 2025
 *      Author: MaximilianLell
 */

#ifndef BLINKYLED_BLINKYLED_H_
#define BLINKYLED_BLINKYLED_H_

#include "../../Utils/CommonTypes.h"

/**
 * @brief LED state enumeration for BlinkyLed module.
 */
typedef enum
{
    E_BLINKY_LED_OFF = 0,
    E_BLINKY_LED_ON = 1
} BlinkyLed_State_e;

/**
 * @brief Configuration structure for a Blinky LED instance.
 *
 * Holds the state and hardware configuration for a single LED.
 */
typedef struct
{
    BlinkyLed_State_e eLedState;
    void             *pGpioPort;
    u16               u16GpioPin;
} BlinkyLed_Config_t;

/**
 * @brief Turns the LED on.
 *
 * Sets the GPIO pin to the active state and updates the configuration.
 * @param[in,out] inout_pSelf Pointer to the LED configuration structure.
 */
void BlinkyLed_Enable( BlinkyLed_Config_t *inout_pSelf );

/**
 * @brief Turns the LED off.
 *
 * Sets the GPIO pin to the inactive state and updates the configuration.
 * @param[in,out] inout_pSelf Pointer to the LED configuration structure.
 */
void BlinkyLed_Disable( BlinkyLed_Config_t *inout_pSelf );

/**
 * @brief Toggles the LED state.
 *
 * Inverts the current state of the LED and updates the hardware and
 * configuration.
 * @param[in,out] inout_pSelf Pointer to the LED configuration structure.
 */
void BlinkyLed_Toggle( BlinkyLed_Config_t *inout_pSelf );

#endif /* BLINKYLED_BLINKYLED_H_ */
