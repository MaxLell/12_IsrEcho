/*
 * user_app.c
 *
 *  Created on: Jul 28, 2025
 *      Author: MaximilianLell
 */

#ifndef USER_MAIN_C_
#define USER_MAIN_C_

#include "../../Core/Inc/main.h"
#include "../../Core/Inc/usart.h"
#include "../../Utils/CommonTypes.h"
#include "../../Utils/CustomAssert.h"
#include "../BlinkyLed/BlinkyLed.h"

#include <string.h>

#define BUFFER_SIZE         ( 50U )
#define TIMEOUT_MSEC        ( 100U )
#define NOF_BLINKS          ( 10U )
#define BLINK_DURATION_MSEC ( 70U )

static BlinkyLed_Config_t tBlinkyLed = { 0 };
static u8                 au8InputBuffer[BUFFER_SIZE] = { 0 };

void UserMain_Init()
{
    // Initialize the Blinky LED
    tBlinkyLed.eLedState = E_BLINKY_LED_OFF;
    tBlinkyLed.pGpioPort = BLINKY_LED_GPIO_Port;
    tBlinkyLed.u16GpioPin = BLINKY_LED_Pin;
}

void UserMain_Loop()
{
    bool              bFoundNewLine = false;
    u8                u8Counter = 0;
    HAL_StatusTypeDef tStatus = HAL_ERROR;

    // Poll for a new message and store it in a buffer
    tStatus = HAL_UART_Receive( &huart2, au8InputBuffer, BUFFER_SIZE,
                                TIMEOUT_MSEC );
    ASSERT( HAL_ERROR != tStatus );

    // Check the currently sampled Buffer for a new line character (so a
    // complete message)
    for( u8 u8IdxNr = 0; u8IdxNr < BUFFER_SIZE; ++u8IdxNr )
    {
        if( au8InputBuffer[u8IdxNr] == '\n' )
        {
            u8Counter = u8IdxNr;
            bFoundNewLine = true;
            break;
        }
    }
    if( true == bFoundNewLine )
    {
        // Report back what was received
        tStatus = HAL_UART_Transmit( &huart2, au8InputBuffer, u8Counter,
                                     TIMEOUT_MSEC );
        ASSERT( HAL_ERROR != tStatus );
        memset( au8InputBuffer, 0, BUFFER_SIZE );

        // Blink the LED a couple of times
        for( u8 u8BlinkyTimes = 0; u8BlinkyTimes < NOF_BLINKS; u8BlinkyTimes++ )
        {
            BlinkyLed_Toggle( &tBlinkyLed );
            HAL_Delay( BLINK_DURATION_MSEC );
        }
    }
}

void UserMain( void )
{
    // Init
    UserMain_Init();

    // Loop
    for( ;; )
    {
        UserMain_Loop();
    }
}

#endif /* USER_MAIN_C_ */
