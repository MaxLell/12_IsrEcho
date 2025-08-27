/*
 * IsrHandler.h
 *
 *  Created on: Aug 7, 2025
 *      Author: MaximilianLell
 */

#ifndef ISRHANDLER_ISRHANDLER_H_
#define ISRHANDLER_ISRHANDLER_H_

#include "../../Utils/CommonTypes.h"
#include "../../Utils/CustomAssert.h"
#include "IsrIds.h"

/**
 * @brief Function pointer type for ISR callbacks.
 * @param pContext Pointer to user context data (as registered).
 */
typedef void ( *IsrCallback_t )( const void *pContext );

/**
 * @brief Table entry for an ISR handler.
 *
 * Holds the callback and context pointer for a registered interrupt.
 */
typedef struct
{
    IsrCallback_t pfCallback;
    const void   *ptContext;
} IsrHandler_Entry_t;

/**
 * @brief Initializes the ISR handler table.
 *
 * @param[in,out] inout_atIsrTable Pointer to the ISR handler table array.
 * @param[in] in_u8TableSize Number of entries in the table.
 */
void IsrHandler_Init( IsrHandler_Entry_t *const inout_atIsrTable,
                      const u8                  in_u8TableSize );

/**
 * @brief Registers an ISR callback and context for a given ID.
 *
 * @param[in] in_eId Interrupt ID to register.
 * @param[in] in_pfIsrCallback Callback function pointer.
 * @param[in] in_pContext Pointer to user context data.
 */
void IsrHandler_RegisterIsr( const Isr_Id_e      in_eId,
                             const IsrCallback_t in_pfIsrCallback,
                             const void *const   in_pContext );

/**
 * @brief Unregisters an ISR callback for a given ID.
 *
 * @param[in] in_eId Interrupt ID to unregister.
 */
void IsrHandler_UnregisterIsr( const Isr_Id_e in_eId );

/**
 * @brief Dispatches the registered ISR callback for a given ID.
 *
 * @param[in] in_eIsrId Interrupt ID to dispatch.
 */
void IsrHandler_DispatchIsr( const Isr_Id_e in_eIsrId );

#endif /* ISRHANDLER_ISRHANDLER_H_ */
