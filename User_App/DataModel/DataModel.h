
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "CommonTypes.h"

#define DATAMODEL_MAX_CONTENT_SIZE_BYTES ( 10U )
#define DATAMODEL_MAX_OBSERVERS          ( 5U )
#define DATAMODEL_CANARY_VALUE           ( 0xA5A5A5A5U )
#define DATAMODEL_PLACEHOLDER            ( 0xFFFFFFFFU )

/**
 * @brief Observer structure for notification callbacks.
 */
typedef struct
{
    /** @brief Unique identifier for the observer. */
    u32 u32Id;

    /** @brief Pointer to the notification callback function. */
    void ( *pFnNotification )( void );
} Observer_t;

/**
 * @brief Data model structure for storing content and observers.
 */
typedef struct
{
    /** @brief Start canary word for integrity check. */
    u32 u32CanaryWordStart;

    /** @brief Array of attached observers. */
    Observer_t atObservers[DATAMODEL_MAX_OBSERVERS];

    /** @brief Number of attached observers. */
    u32 u32NofAttachedObservers;

    /** @brief Content buffer. */
    u8 au8Content[DATAMODEL_MAX_CONTENT_SIZE_BYTES];

    /** @brief Size of the content in bytes. */
    u8 u8ContentSizeBytes;

    /** @brief Data validity flag. */
    bool bIsDataValid;

    /** @brief Initialization flag. */
    bool bIsInitialized;

    /** @brief End canary word for integrity check. */
    u32 u32CanaryWordEnd;
} DataModel_t;

/**
 * @brief Initializes the DataModel structure.
 * @param[in,out] inout_ptDataModel Pointer to DataModel instance (must point to
 * valid static or heap-allocated memory).
 * @details The DataModel must be allocated either statically or on the heap
 * before calling this function.
 */
void DataModel_Init( DataModel_t *const inout_ptDataModel );

/**
 * @brief Writes data to the DataModel.
 * @param[in,out] inout_ptDataModel Pointer to DataModel instance (must point to
 * valid static or heap-allocated memory).
 * @param[in] in_ptDataBytes Pointer to the data to be written (caller must
 * ensure data remains valid during call).
 * @param[in] in_tDataSizeBytes Size of the data in bytes.
 * @details The DataModel must be allocated either statically or on the heap
 * before calling this function.
 */
void DataModel_Write( DataModel_t *const inout_ptDataModel,
                      const void *const  in_ptDataBytes,
                      const size_t       in_tDataSizeBytes );

/**
 * @brief Reads data from the DataModel.
 * @param[in] in_ptDataModel Pointer to DataModel instance (must point to
 * valid static or heap-allocated memory).
 * @param[out] out_ptDataBytes Pointer to buffer where data will be copied.
 * @param[in,out] out_ptDataSize Pointer to size variable; input: buffer
 * size, output: actual data size.
 * @details The DataModel must be allocated either statically or on the heap
 * before calling this function.
 */
void DataModel_Read( const DataModel_t *const in_ptDataModel,
                     void *const out_ptDataBytes, u32 *const out_ptDataSize );

/**
 * @brief Checks if the DataModel contains valid data.
 * @param[in] in_ptDataModel Pointer to DataModel instance (must point to valid
 * static or heap-allocated memory).
 * @return true if data is valid, false otherwise.
 */
bool DataModel_IsValid( const DataModel_t *const in_ptDataModel );

/**
 * @brief Sets the validity flag of the DataModel.
 * @param[in,out] inout_ptDataModel Pointer to DataModel instance (must point to
 * valid static or heap-allocated memory).
 * @param[in] bIsValid Validity flag to set.
 */
void DataModel_SetValidity( DataModel_t *const inout_ptDataModel, bool bIsValid );

/**
 * @brief Adds an observer to the DataModel.
 * @param[in,out] inout_ptDataModel Pointer to DataModel instance (must point to
 * valid static or heap-allocated memory).
 * @param[in] in_ptObserver Pointer to observer to add.
 */
void DataModel_AddObserver( DataModel_t *const      inout_ptDataModel,
                            const Observer_t *const in_ptObserver );

/**
 * @brief Removes an observer from the DataModel.
 * @param[in,out] inout_ptDataModel Pointer to DataModel instance (must point to
 * valid static or heap-allocated memory).
 * @param[in] in_ptObserver Pointer to observer to remove.
 */
void DataModel_RemoveObserver( DataModel_t *const      inout_ptDataModel,
                               const Observer_t *const in_ptObserver );

/**
 * @brief Returns the number of attached observers in the DataModel.
 * @param[in] in_ptDataModel Pointer to DataModel instance (must point to valid
 * static or heap-allocated memory).
 * @return Number of attached observers.
 */
u32 DataModel_GetNumberOfAttachedObservers( const DataModel_t *const in_ptDataModel );

#endif // DATAMODEL_H
