/*
 * IsrIds.h
 *
 *  Created on: Aug 7, 2025
 *      Author: MaximilianLell
 */

#ifndef ISRHANDLER_ISRIDS_H_
#define ISRHANDLER_ISRIDS_H_

typedef enum
{
    E_ISR_ID_INVALID = -1,

    E_ISR_ID_BUTTON_TIM_ISR,
    E_ISR_ID_COMBINED_BUTTON_AND_BLINKY_ISR,

    E_ISR_ID_LAST,
} Isr_Id_e;

#endif /* ISRHANDLER_ISRIDS_H_ */
