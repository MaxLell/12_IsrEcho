#include "DataModel.h"

#include <CustomAssert.h>
#include <stdio.h>

// ################################################################################
// static function declarations
// ################################################################################

/**
 * @brief Notifies all attached observers of a data change in the DataModel.
 * @param[in] in_ptDataModel Pointer to the DataModel instance.
 */
static void DataModel_NotifyObservers( const DataModel_t *const in_ptDataModel );

/**
 * @brief Copies a specified number of bytes from source to destination.
 * @param[out] au8Destination Pointer to the destination buffer.
 * @param[in] au8Source Pointer to the source buffer.
 * @param[in] tNofBytes Number of bytes to copy.
 */
static void DataModel_CopyBytes( u8 *au8Destination, const u8 *au8Source,
                                 size_t tNofBytes );

/**
 * @brief Checks the integrity of canary words in the DataModel for overflow
 * detection.
 * @param[in] in_ptDataModel Pointer to the DataModel instance.
 */
static void DataModel_CheckCanaryWords( const DataModel_t *const in_ptDataModel );

// ################################################################################
// function implementations
// ################################################################################

void DataModel_Init( DataModel_t *const inout_ptDataModel )
{
    { // Input Checks
        ASSERT( inout_ptDataModel );
        ASSERT( false == inout_ptDataModel->bIsInitialized );

        ASSERT( inout_ptDataModel->atObservers );
        ASSERT( inout_ptDataModel->au8Content );
    }

    // Set the Canary Values
    inout_ptDataModel->u32CanaryWordStart = DATAMODEL_CANARY_VALUE;
    inout_ptDataModel->u32CanaryWordEnd = DATAMODEL_CANARY_VALUE;

    // Reset the observer array
    for( u32 i = 0; i < DATAMODEL_MAX_OBSERVERS; i++ )
    {
        inout_ptDataModel->atObservers[i].pFnNotification = NULL;
        inout_ptDataModel->atObservers[i].u32Id = DATAMODEL_PLACEHOLDER;
    }

    // Reset the Nof Observers
    inout_ptDataModel->u32NofAttachedObservers = 0;

    // Clear out the static array
    for( u32 i = 0; i < DATAMODEL_MAX_CONTENT_SIZE_BYTES; i++ )
    {
        inout_ptDataModel->au8Content[i] = 0;
    }

    // Set the validity flag
    inout_ptDataModel->bIsDataValid = false;

    // Set the initialization flag
    inout_ptDataModel->bIsInitialized = true;
}

void DataModel_Write( DataModel_t *const inout_ptDataModel,
                      const void *const  in_ptDataBytes,
                      const size_t       in_tDataSizeBytes )
{
    { // Input Checks
        ASSERT( inout_ptDataModel );
        ASSERT( DATAMODEL_MAX_CONTENT_SIZE_BYTES >=
                inout_ptDataModel->u8ContentSizeBytes );
        ASSERT( true == inout_ptDataModel->bIsInitialized );
        ASSERT( in_ptDataBytes );
        ASSERT( DATAMODEL_MAX_CONTENT_SIZE_BYTES > in_tDataSizeBytes );
    }

    // Copy the data into the content pointer
    DataModel_CopyBytes( (u8 *)inout_ptDataModel->au8Content,
                         (const u8 *)in_ptDataBytes, in_tDataSizeBytes );

    // Update the internally stored content size
    inout_ptDataModel->u8ContentSizeBytes = in_tDataSizeBytes;

    // Notify observers about the data change
    DataModel_NotifyObservers( inout_ptDataModel );

    // Set validity
    inout_ptDataModel->bIsDataValid = true;

    // Check Canary Words
    DataModel_CheckCanaryWords( inout_ptDataModel );
}

void DataModel_Read( const DataModel_t *const in_ptDataModel,
                     void *const out_ptDataBytes, u32 *const out_ptDataSize )
{
    { // Input Checks
        ASSERT( in_ptDataModel );
        ASSERT( in_ptDataModel->bIsInitialized );
        ASSERT( DATAMODEL_MAX_CONTENT_SIZE_BYTES >=
                in_ptDataModel->u8ContentSizeBytes );

        ASSERT( out_ptDataBytes );
        ASSERT( out_ptDataSize );
        ASSERT( *out_ptDataSize == 0 );
    }

    // Copy the bytes from the stored content into the output buffer
    DataModel_CopyBytes( (u8 *)out_ptDataBytes,
                         (const u8 *)in_ptDataModel->au8Content,
                         in_ptDataModel->u8ContentSizeBytes );

    // Set the output data size
    *out_ptDataSize = in_ptDataModel->u8ContentSizeBytes;

    // Check Canary Words
    DataModel_CheckCanaryWords( in_ptDataModel );
}

bool DataModel_IsValid( const DataModel_t *const in_ptDataModel )
{
    { // Input Checks
        ASSERT( in_ptDataModel );
        ASSERT( in_ptDataModel->bIsInitialized );
    }

    // Check Canary Words
    DataModel_CheckCanaryWords( in_ptDataModel );
    return in_ptDataModel->bIsDataValid;
}

void DataModel_SetValidity( DataModel_t *const inout_ptDataModel, bool bIsValid )
{
    { // Input Checks
        ASSERT( inout_ptDataModel );
        ASSERT( inout_ptDataModel->bIsInitialized );
    }

    inout_ptDataModel->bIsDataValid = bIsValid;

    // Check Canary Words
    DataModel_CheckCanaryWords( inout_ptDataModel );
}

void DataModel_AddObserver( DataModel_t *const      inout_ptDataModel,
                            const Observer_t *const in_ptObserver )
{
    { // Input Checks
        ASSERT( inout_ptDataModel );
        ASSERT( inout_ptDataModel->bIsInitialized );

        ASSERT( DATAMODEL_PLACEHOLDER != in_ptObserver->u32Id );
        ASSERT( in_ptObserver->pFnNotification );
    }

    // Check if there is space for a new observer
    bool bFoundSpace = false;
    bool bIsAlreadyAttached = false;
    for( u32 i = 0; i < DATAMODEL_MAX_OBSERVERS; i++ )
    {

        // Check for duplicate observers
        if( in_ptObserver->u32Id == inout_ptDataModel->atObservers[i].u32Id )
        {
            bIsAlreadyAttached = true;
        }

        // Look for an empty space
        if( ( NULL == inout_ptDataModel->atObservers[i].pFnNotification ) &&
            ( DATAMODEL_PLACEHOLDER == inout_ptDataModel->atObservers[i].u32Id ) )
        {
            inout_ptDataModel->atObservers[i].u32Id = in_ptObserver->u32Id;
            inout_ptDataModel->atObservers[i].pFnNotification =
                in_ptObserver->pFnNotification;
            bFoundSpace = true;
        }
    }

    ASSERT( true == bFoundSpace );
    ASSERT( false == bIsAlreadyAttached );

    inout_ptDataModel->u32NofAttachedObservers++;
    ASSERT( inout_ptDataModel->u32NofAttachedObservers <=
            DATAMODEL_MAX_OBSERVERS );

    // Check Canary Words
    DataModel_CheckCanaryWords( inout_ptDataModel );
}

void DataModel_RemoveObserver( DataModel_t *const      inout_ptDataModel,
                               const Observer_t *const in_ptObserver )
{
    { // Input Checks
        ASSERT( inout_ptDataModel );
        ASSERT( inout_ptDataModel->bIsInitialized );

        ASSERT( DATAMODEL_PLACEHOLDER != in_ptObserver->u32Id );
    }

    // Search for the observer and remove it
    bool bFoundObserver = false;
    for( u32 i = 0; i < DATAMODEL_MAX_OBSERVERS; i++ )
    {
        if( inout_ptDataModel->atObservers[i].u32Id == in_ptObserver->u32Id )
        {
            inout_ptDataModel->atObservers[i].u32Id = DATAMODEL_PLACEHOLDER;
            inout_ptDataModel->atObservers[i].pFnNotification = NULL;
            bFoundObserver = true;
            break;
        }
    }

    ASSERT( true == bFoundObserver );
    inout_ptDataModel->u32NofAttachedObservers--;
    ASSERT( inout_ptDataModel->u32NofAttachedObservers < DATAMODEL_MAX_OBSERVERS );

    // Check Canary Words
    DataModel_CheckCanaryWords( inout_ptDataModel );
}

void DataModel_NotifyObservers( const DataModel_t *const in_ptDataModel )
{
    { // Input Checks
        ASSERT( in_ptDataModel );
        ASSERT( in_ptDataModel->bIsInitialized );
    }

    bool bOneObserverIsNotified = false;
    for( u32 i = 0; i < DATAMODEL_MAX_OBSERVERS; i++ )
    {
        if( ( in_ptDataModel->atObservers[i].pFnNotification ) &&
            ( DATAMODEL_PLACEHOLDER != in_ptDataModel->atObservers[i].u32Id ) )
        {
            in_ptDataModel->atObservers[i].pFnNotification();
            bOneObserverIsNotified = true;
        }
    }
    ASSERT( true == bOneObserverIsNotified );
}

void DataModel_CheckCanaryWords( const DataModel_t *const in_ptDataModel )
{
    { // Input Checks
        ASSERT( in_ptDataModel );
        ASSERT( in_ptDataModel->bIsInitialized );
        ASSERT( DATAMODEL_CANARY_VALUE == in_ptDataModel->u32CanaryWordStart );
        ASSERT( DATAMODEL_CANARY_VALUE == in_ptDataModel->u32CanaryWordEnd );
    }
}

u32 DataModel_GetNumberOfAttachedObservers( const DataModel_t *const in_ptDataModel )
{
    { // Input Checks
        ASSERT( in_ptDataModel );
        ASSERT( in_ptDataModel->bIsInitialized );
        ASSERT( in_ptDataModel->atObservers );
        ASSERT( in_ptDataModel->u32NofAttachedObservers <=
                DATAMODEL_MAX_OBSERVERS );
    }
    return in_ptDataModel->u32NofAttachedObservers;
}

void DataModel_CopyBytes( u8 *au8Destination, const u8 *au8Source,
                          size_t tNofBytes )
{
    { // Input Checks
        ASSERT( au8Destination );
        ASSERT( au8Source );
    }
    for( size_t i = 0; i < tNofBytes; i++ )
    {
        au8Destination[i] = au8Source[i];
    }
}
