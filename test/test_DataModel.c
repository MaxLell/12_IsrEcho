#include "CommonTypes.h"
#include "DataModel.h"
#include "unity.h"

#define DATA_MODEL_MAX_NUMBER_OF_SUBSCRIBER ( 5U )
static bool g_bObserverNotified = false;

static void TestSupport_ObserverCallback( void );

void setUp( void )
{
    g_bObserverNotified = false;
}

void tearDown( void ) {}

void TestSupport_ObserverCallback( void )
{
    g_bObserverNotified = true;
}

void test_DataModel_Init_DoesSetupDataModel( void )
{

    DataModel_t tTestDataModel = { 0 };
    DataModel_Init( &tTestDataModel );

    TEST_ASSERT_EQUAL( true, tTestDataModel.bIsInitialized );
    TEST_ASSERT_EQUAL( false, tTestDataModel.bIsDataValid );
    TEST_ASSERT_EQUAL( DATAMODEL_CANARY_VALUE, tTestDataModel.u32CanaryWordStart );
    TEST_ASSERT_EQUAL( DATAMODEL_CANARY_VALUE, tTestDataModel.u32CanaryWordEnd );
    for( u32 i = 0; i < DATAMODEL_MAX_OBSERVERS; i++ )
    {
        TEST_ASSERT_EQUAL( DATAMODEL_PLACEHOLDER,
                           tTestDataModel.atObservers[i].u32Id );
        TEST_ASSERT_EQUAL( NULL, tTestDataModel.atObservers[i].pFnNotification );
    }
};

void test_DataModel_Validity_CanBeSetAndReset( void )
{

    DataModel_t tTestDataModel = { 0 };
    DataModel_Init( &tTestDataModel );
    TEST_ASSERT_EQUAL( false, tTestDataModel.bIsDataValid );

    DataModel_SetValidity( &tTestDataModel, true );
    bool bIsValid = DataModel_IsValid( &tTestDataModel );
    TEST_ASSERT_EQUAL( true, bIsValid );

    DataModel_SetValidity( &tTestDataModel, false );
    bIsValid = DataModel_IsValid( &tTestDataModel );
    TEST_ASSERT_EQUAL( false, bIsValid );
}

void test_DataModel_Add_and_Remove_Observer( void )
{
    DataModel_t tTestDataModel = { 0 };
    DataModel_Init( &tTestDataModel );

    u32 u32NofObserversBefore = DataModel_GetNumberOfAttachedObservers(
        &tTestDataModel );

    Observer_t tObserver = { 0 };
    tObserver.u32Id = 1;
    tObserver.pFnNotification = TestSupport_ObserverCallback;
    DataModel_AddObserver( &tTestDataModel, &tObserver );

    u32 u32NofObserversAfter = DataModel_GetNumberOfAttachedObservers(
        &tTestDataModel );
    TEST_ASSERT_EQUAL( u32NofObserversBefore + 1, u32NofObserversAfter );

    DataModel_RemoveObserver( &tTestDataModel, &tObserver );
    u32 u32NofObserversAfterRemoval = DataModel_GetNumberOfAttachedObservers(
        &tTestDataModel );
    TEST_ASSERT_EQUAL( u32NofObserversBefore, u32NofObserversAfterRemoval );
}

void test_DataModel_Writing_SendsOutNotifications( void )
{
    DataModel_t tTestDataModel = { 0 };
    DataModel_Init( &tTestDataModel );
    Observer_t tObserver = { 0 };
    tObserver.u32Id = 1;
    tObserver.pFnNotification = TestSupport_ObserverCallback;
    DataModel_AddObserver( &tTestDataModel, &tObserver );

    DataModel_Write( &tTestDataModel, "Test", 4 );

    TEST_ASSERT_EQUAL( true, g_bObserverNotified );
}

void test_DataModel_WriteRead_ComplexData( void )
{
    DataModel_t tTestDataModel = { 0 };
    DataModel_Init( &tTestDataModel );
    Observer_t tObserver = { 0 };
    tObserver.u32Id = 1;
    tObserver.pFnNotification = TestSupport_ObserverCallback;
    DataModel_AddObserver( &tTestDataModel, &tObserver );

    char   acSourceString[5] = "Test";
    size_t tSourceStringSizeBytes = sizeof( acSourceString );
    DataModel_Write( &tTestDataModel, acSourceString, tSourceStringSizeBytes );

    TEST_ASSERT_EQUAL( true, g_bObserverNotified );

    char   acDestString[5] = { 0 };
    size_t tDestStringSizeBytes = 0;
    DataModel_Read( &tTestDataModel, acDestString, &tDestStringSizeBytes );

    TEST_ASSERT_EQUAL_STRING( acSourceString, acDestString );
}

void test_DataModel_WriteRead_SimpleData( void )
{
    DataModel_t tTestDataModel = { 0 };
    DataModel_Init( &tTestDataModel );
    Observer_t tObserver = { 0 };
    tObserver.u32Id = 1;
    tObserver.pFnNotification = TestSupport_ObserverCallback;
    DataModel_AddObserver( &tTestDataModel, &tObserver );

    uint32_t u32SourceValue = 1234;
    size_t   tSourceValueSizeBytes = sizeof( u32SourceValue );
    DataModel_Write( &tTestDataModel, &u32SourceValue, tSourceValueSizeBytes );

    TEST_ASSERT_EQUAL( true, g_bObserverNotified );

    uint32_t u32DestValue = 0;
    size_t   tDestValueSizeBytes = 0;
    DataModel_Read( &tTestDataModel, &u32DestValue, &tDestValueSizeBytes );

    TEST_ASSERT_EQUAL( u32SourceValue, u32DestValue );
}
