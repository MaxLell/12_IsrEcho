/**
 * @file Button.h
 * @brief Button debouncing and event handling for STM32.
 *
 * This module provides debouncing and event detection for push buttons.
 * The button timer interrupt service routine (ISR) must be called at 1kHz
 * (every 1ms) for correct debouncing and event detection.
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

#include "../../Utils/CommonTypes.h"

/**
 * @brief Button state enumeration.
 */
typedef enum
{
    E_BUTTON_STATE_INVALID = 0,
    E_BUTTON_STATE_RELEASED = 1,
    E_BUTTON_STATE_PRESSED = 2,
} Button_PinState_e;

typedef enum
{
    E_BUTTON_EVENT_INVALID = -1,
    E_BUTTON_EVENT_PRESSED,
    E_BUTTON_EVENT_RELEASED,
    E_BUTTON_EVENT_LONG_PRESSED,
    E_BUTTON_EVENT_NO_EVENT,
} Button_Event_e;

/**
 * @brief Button polarity enumeration.
 */
typedef enum
{
    E_BUTTON_POLARITY_INVALID = 0,
    E_BUTTON_POLARITY_ACTIVE_LOW,
    E_BUTTON_POLARITY_ACTIVE_HIGH,
} Button_Polarity_e;

/**
 * @brief Debounce state flags for a button.
 */
typedef struct
{
    u16  u16LongPressedMsecCounter; /**< Counter for long press detection. */
    u32  u32BitState;               /**< Bit-shift register for debouncing. */
    bool bLongPressTriggered; /**< Indicates if long press event was triggered.
                               */
} Button_DebounceFlags_t;

/**
 * @brief Button configuration and state structure.
 */
typedef struct
{
    Button_PinState_e      eButtonState; /**< Current debounced button state. */
    Button_Polarity_e      ePolarity; /**< Button polarity (active high/low). */
    Button_Event_e         eLastButtonEvent; /**< Last detected button event. */
    Button_DebounceFlags_t tDebounceFlags;   /**< Debounce and long press state.
                                              */
    void *pGpioPort;                         /**< GPIO port pointer. */
    u16   u16GpioPin;                        /**< GPIO pin number. */
} Button_Config_t;

/**
 * @brief Call this function from a 1kHz timer interrupt to update the button
 * state.
 * @param[in,out] inout_ptButton Pointer to the button configuration structure.
 */
void Button_TimIsr( Button_Config_t *const inout_ptButton );

/**
 * @brief Update the debounced state and event of the button.
 * @param[in,out] inout_ptButton Pointer to the button configuration structure.
 */
void Button_GetDebouncedState( Button_Config_t *const inout_ptButton );

/**
 * @brief Read the current (raw) pin state and update the debounced state.
 * @param[in,out] inout_ptButton Pointer to the button configuration structure.
 * @return The debounced pin state.
 */
Button_PinState_e Button_GetPinState( Button_Config_t *const inout_ptButton );

/**
 * @brief Get the last detected button event.
 * @param[in,out] inout_ptButton Pointer to the button configuration structure.
 * @return The last button event.
 */
Button_Event_e Button_GetLastEvent( Button_Config_t *const inout_ptButton );

/**
 * @brief Clear the last detected button event.
 * @param[in,out] inout_ptButton Pointer to the button configuration structure.
 */
void Button_ClearLastEvent( Button_Config_t *const inout_ptButton );

#endif /* BUTTON_BUTTON_H_ */
