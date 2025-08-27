/*
 * Button.c
 *
 *  Created on: Aug 1, 2025
 *      Author: MaximilianLell
 */
#include "../../Core/Inc/main.h"
#include "../../Utils/CommonTypes.h"
#include "../../Utils/CustomAssert.h"
#include "Button.h"

#define BUTTON_DEBOUNCE_LONG_PRESS_PERIOD_MS 300U

#define BUTTON_DEBOUNCE_SHORT_PRESS_MASK 0x0000FFFFU
#define BUTTON_DEBOUNCE_RELEASE_MASK     0xFFFF0000U
#define BUTTON_DEBOUNCE_LONG_PRESS_MASK  0xFFFFFFFFU

void Button_VerifyArguments( Button_Config_t *inout_ptButton )
{
    // Verify that the Button configuration is valid
    ASSERT( NULL != inout_ptButton );
    ASSERT( NULL != inout_ptButton->pGpioPort );
    ASSERT( 0 != inout_ptButton->u16GpioPin );
    ASSERT( E_BUTTON_POLARITY_INVALID != inout_ptButton->ePolarity );
}

Button_PinState_e Button_GetPinState( Button_Config_t *inout_ptButton )
{
    Button_VerifyArguments( inout_ptButton );

    // Get the current Button State
    GPIO_PinState ePinState = HAL_GPIO_ReadPin( inout_ptButton->pGpioPort,
                                                inout_ptButton->u16GpioPin );

    // Determine the Button State based on the configured polarity
    if( E_BUTTON_POLARITY_ACTIVE_LOW == inout_ptButton->ePolarity )
    {
        inout_ptButton->eButtonState = ( GPIO_PIN_RESET == ePinState )
                                           ? E_BUTTON_STATE_PRESSED
                                           : E_BUTTON_STATE_RELEASED;
    }
    else
    {
        inout_ptButton->eButtonState = ( GPIO_PIN_SET == ePinState )
                                           ? E_BUTTON_STATE_PRESSED
                                           : E_BUTTON_STATE_RELEASED;
    }
    return inout_ptButton->eButtonState;
}

void Button_TimIsr( Button_Config_t *inout_pButton )
{
    ASSERT( inout_pButton );
    // Update the Debounced State of the Button
    Button_GetDebouncedState( inout_pButton );
    return;
}

Button_Event_e Button_GetLastEvent( Button_Config_t *const inout_ptButton )
{
    Button_VerifyArguments( inout_ptButton );
    Button_Event_e eLastEvent = inout_ptButton->eLastButtonEvent;
    return eLastEvent;
}

void Button_ClearLastEvent( Button_Config_t *const inout_ptButton )
{
    Button_VerifyArguments( inout_ptButton );
    inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_NO_EVENT;
}

/**
 * Taken from https://www.ganssle.com/debouncing-pt2.htm
 */
void Button_GetDebouncedState( Button_Config_t *const inout_ptButton )
{
    ASSERT( inout_ptButton );

    // Work directly with struct fields
    Button_PinState_e ePinState = Button_GetPinState( inout_ptButton );
    bool              bButtonPressed = ( ePinState == E_BUTTON_STATE_PRESSED );

    // Shift one bit, if the button is pressed otherwise shift a zero (happens
    // by default)
    inout_ptButton->tDebounceFlags.u32BitState <<= 1;
    if( bButtonPressed )
    {
        inout_ptButton->tDebounceFlags.u32BitState |= 0x00000001;
    }

    inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_INVALID;

    switch( inout_ptButton->tDebounceFlags.u32BitState )
    {
    case BUTTON_DEBOUNCE_SHORT_PRESS_MASK:
    {
        // Short Press -> 16 Msec consistent shifting of ones
        // Clear the current long button press progress so that it does not
        // carry over to the next button press
        inout_ptButton->tDebounceFlags.bLongPressTriggered = false;
        inout_ptButton->tDebounceFlags.u16LongPressedMsecCounter = 0;
        inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_PRESSED;
    }
    break;

    case BUTTON_DEBOUNCE_RELEASE_MASK:
    {
        // Button Released -> Zeros are shifted in (16 Msec consistent shifting
        // of zeros) Clear the current long button press progress so that it
        // does not carry over to the next button press
        inout_ptButton->tDebounceFlags.bLongPressTriggered = false;
        inout_ptButton->tDebounceFlags.u16LongPressedMsecCounter = 0;
        inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_RELEASED;
    }
    break;

    case BUTTON_DEBOUNCE_LONG_PRESS_MASK:
    {
        // Long Press -> 32 Msec consistent shifting of ones
        // Button is held down
        inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_NO_EVENT;
        if( inout_ptButton->tDebounceFlags.bLongPressTriggered == false )
        {
            // The button needs to be held down for the long press duration, so
            // that a real user-settable longpress can be detected
            inout_ptButton->tDebounceFlags.u16LongPressedMsecCounter++;

            if( BUTTON_DEBOUNCE_LONG_PRESS_PERIOD_MS ==
                inout_ptButton->tDebounceFlags.u16LongPressedMsecCounter )
            {
                // Clear the current long button press progress so that it does
                // not carry over to the next button press
                inout_ptButton->tDebounceFlags.bLongPressTriggered = true;
                inout_ptButton->tDebounceFlags.u16LongPressedMsecCounter = 0;
                inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_LONG_PRESSED;
            }
        }
    }
    break;

    default:
        inout_ptButton->eLastButtonEvent = E_BUTTON_EVENT_NO_EVENT;
        break;
    }

    ASSERT( inout_ptButton->eLastButtonEvent != E_BUTTON_EVENT_INVALID );
    return;
}
