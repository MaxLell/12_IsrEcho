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

void HAL_UART_TxCpltCallback( UART_HandleTypeDef *huart )
{
    // Reset the Array
    memset( au8InputBuffer, 0, BUFFER_SIZE );
}

void UserMain_Init()
{
    HAL_StatusTypeDef tStatus = HAL_ERROR;

    // Initialize the Blinky LED
    tBlinkyLed.eLedState = E_BLINKY_LED_OFF;
    tBlinkyLed.pGpioPort = BLINKY_LED_GPIO_Port;
    tBlinkyLed.u16GpioPin = BLINKY_LED_Pin;

    memset( au8InputBuffer, 0, BUFFER_SIZE );

    tStatus = HAL_UART_Receive_IT( &huart2, au8InputBuffer, BUFFER_SIZE );
    ASSERT( HAL_ERROR != tStatus );
}

void UserMain_Loop()
{
    bool              bFoundNewLine = false;
    HAL_StatusTypeDef tStatus = HAL_ERROR;
    u8                u8Counter = 0;
    u8                u8IdxNr = 0;
    u8                u8FilledBufferEntriesCounter = 0;

    // Check the currently sampled Buffer for a new line character (so a
    // complete message)

    for( u8IdxNr = 0; u8IdxNr < BUFFER_SIZE; ++u8IdxNr )
    {
        if( au8InputBuffer[u8IdxNr] == '\n' )
        {
            // If everything went as expected then the au8InputBuffer is filled
            // from the very beginning. If the Sequence is not reset (via an
            // abort command) then the array is not filled from the front but
            // from somewhere in the middle
            ASSERT( 0 != au8InputBuffer[0] );

            u8Counter = u8IdxNr;
            bFoundNewLine = true;
            break;
        }
        if( au8InputBuffer[u8IdxNr] != 0 )
        {
            u8FilledBufferEntriesCounter++;
        }
    }

    // Make sure that the buffer is not completly overflowing
    if( ( u8FilledBufferEntriesCounter == BUFFER_SIZE ) &&
        ( au8InputBuffer[BUFFER_SIZE] != '\n' ) )
    {
        // Abort everything on the TX and RX Side
        tStatus = HAL_UART_AbortReceive_IT( &huart2 );
        ASSERT( HAL_ERROR != tStatus );

        // Clear out the Buffer
        memset( au8InputBuffer, 0, BUFFER_SIZE );

        // Restart the reception
        tStatus = HAL_UART_Receive_IT( &huart2, au8InputBuffer, BUFFER_SIZE );
        ASSERT( HAL_ERROR != tStatus );
    }

    if( true == bFoundNewLine )
    {
        // Report back what was received
        tStatus = HAL_UART_Transmit_IT( &huart2, au8InputBuffer, u8Counter );
        ASSERT( HAL_ERROR != tStatus );


        // Restart the reception process -> the abort function resets the
        // sequence
        tStatus = HAL_UART_AbortReceive_IT( &huart2 );
        ASSERT( HAL_ERROR != tStatus );
        tStatus = HAL_UART_Receive_IT( &huart2, au8InputBuffer, BUFFER_SIZE );
        ASSERT( HAL_ERROR != tStatus );

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
