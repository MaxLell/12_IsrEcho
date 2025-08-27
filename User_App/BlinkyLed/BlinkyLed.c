/*
 * blinky_led.c
 *
 *  Created on: Jul 30, 2025
 *      Author: MaximilianLell
 */

#include "../../Core/Inc/main.h"
#include "../../Utils/CommonTypes.h"
#include "../../Utils/CustomAssert.h"
#include "BlinkyLed.h"

void BlinkyLed_VerifyArguments( BlinkyLed_Config_t *inout_pLed )
{
    ASSERT( inout_pLed );
    ASSERT( inout_pLed->pGpioPort );
    ASSERT( inout_pLed->eLedState <= E_BLINKY_LED_ON );
}

void BlinkyLed_Enable( BlinkyLed_Config_t *inout_pLed )
{
    BlinkyLed_VerifyArguments( inout_pLed );
    HAL_GPIO_WritePin( (GPIO_TypeDef *)inout_pLed->pGpioPort,
                       (u16)inout_pLed->u16GpioPin, GPIO_PIN_SET );
    inout_pLed->eLedState = E_BLINKY_LED_ON;
}

void BlinkyLed_Disable( BlinkyLed_Config_t *inout_pLed )
{
    BlinkyLed_VerifyArguments( inout_pLed );
    HAL_GPIO_WritePin( (GPIO_TypeDef *)inout_pLed->pGpioPort,
                       (u16)inout_pLed->u16GpioPin, GPIO_PIN_RESET );
    inout_pLed->eLedState = E_BLINKY_LED_OFF;
}

void BlinkyLed_Toggle( BlinkyLed_Config_t *inout_pLed )
{
    BlinkyLed_VerifyArguments( inout_pLed );
    inout_pLed->eLedState = !inout_pLed->eLedState;
    HAL_GPIO_WritePin( (GPIO_TypeDef *)inout_pLed->pGpioPort,
                       (u16)inout_pLed->u16GpioPin, (int)inout_pLed->eLedState );
}
