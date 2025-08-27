/*
 * IsrHandler.c
 *
 *  Created on: Aug 7, 2025
 *      Author: MaximilianLell
 */

#include "../../Utils/CommonTypes.h"
#include "../../Utils/CustomAssert.h"
#include "IsrHandler.h"

static IsrHandler_Entry_t *g_atIsrTable;
static bool                g_bWasInitialized;

void IsrHandler_Init( IsrHandler_Entry_t *const inout_atIsrTable,
                      const u8                  in_u8TableSize )
{
    ASSERT( inout_atIsrTable );
    ASSERT( E_ISR_ID_LAST == in_u8TableSize );
    ASSERT( false == g_bWasInitialized );

    // Initialize all entries in the ISR table to be invalid and empty
    for( u8 u8ArrIdx = 0; u8ArrIdx < in_u8TableSize; u8ArrIdx++ )
    {
        inout_atIsrTable[u8ArrIdx].pfCallback = NULL;
        inout_atIsrTable[u8ArrIdx].ptContext = NULL;
    }

    // set the local static reference
    g_atIsrTable = inout_atIsrTable;
    g_bWasInitialized = true;

    return;
}

void IsrHandler_RegisterIsr( const Isr_Id_e      in_eId,
                             const IsrCallback_t in_pfIsrCallback,
                             const void *const   in_pContext )
{
    ASSERT( g_atIsrTable );
    ASSERT( in_pfIsrCallback );
    ASSERT( in_pContext );
    ASSERT( E_ISR_ID_LAST > in_eId );
    ASSERT( E_ISR_ID_INVALID != in_eId );

    // Check if the ISR ID is registered
    bool bIsrWasRegistered = ( ( NULL != g_atIsrTable[in_eId].pfCallback ) &&
                               ( NULL != g_atIsrTable[in_eId].ptContext ) );
    ASSERT( false == bIsrWasRegistered );

    // Place the ISR entry in the table
    g_atIsrTable[in_eId].pfCallback = in_pfIsrCallback;
    g_atIsrTable[in_eId].ptContext = in_pContext;

    return;
}

void IsrHandler_UnregisterIsr( const Isr_Id_e in_eId )
{
    ASSERT( g_atIsrTable );
    ASSERT( E_ISR_ID_LAST > in_eId );
    ASSERT( E_ISR_ID_INVALID != in_eId );

    // Check if the ISR ID is registered
    bool bIsrWasRegistered = ( ( NULL != g_atIsrTable[in_eId].pfCallback ) &&
                               ( NULL != g_atIsrTable[in_eId].ptContext ) );
    ASSERT( true == bIsrWasRegistered );

    // Clear the ISR entry
    g_atIsrTable[in_eId].pfCallback = NULL;
    g_atIsrTable[in_eId].ptContext = NULL;

    return;
}

void IsrHandler_DispatchIsr( const Isr_Id_e in_eId )
{
    ASSERT( g_atIsrTable );
    ASSERT( E_ISR_ID_LAST > in_eId );
    ASSERT( E_ISR_ID_INVALID != in_eId );

    bool bIsrIsRegistered = ( ( NULL != g_atIsrTable[in_eId].pfCallback ) &&
                              ( NULL != g_atIsrTable[in_eId].ptContext ) );
    if( true == bIsrIsRegistered )
    {
        // Call the registered ISR callback with its context
        g_atIsrTable[in_eId].pfCallback( g_atIsrTable[in_eId].ptContext );
    }
    return;
}
