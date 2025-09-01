#include <string.h>
#include <stdlib.h>
#include "unity.h"
#include "MLX90393.h"
#include "mock_test_MLX90393.h"
//#include "mock_test_MLX90393_cmds.h"

//Int to store calls within a test
static int count_malloc = 0;

static mlx_i2c_t fake_mlx;
static uint8_t status_buf[10];

static void *cb_malloc(size_t s, int n)
{   
    count_malloc++; //count_malloc = n; 
    return malloc(s);
}

static void cb_free(void *memory, int n){
    free(memory);
}


void setUp(void) { 
    //Get a full-function structure before for the tests that require the low-level functions to be called
    int fake_mlx_n = 1;
    fake_mlx.handle = (void *) &fake_mlx_n;
    fake_mlx.write_function = write_function;
    fake_mlx.read_function = read_function;
    
    fake_mlx.mdelay = delay_function; 

}

void tearDown(void) {
    //Reset calls to malloc
    count_malloc = 0;
}

void test_CountSetBitsCountsBits(void){
    TEST_ASSERT_EQUAL(0, count_set_bits(0x00));
    TEST_ASSERT_EQUAL(1, count_set_bits(0x01));
    TEST_ASSERT_EQUAL(2, count_set_bits(0x03));
    TEST_ASSERT_EQUAL(3, count_set_bits(0x07));
    TEST_ASSERT_EQUAL(4, count_set_bits(0x0F));
}
void test_CountSetBitsReturns0WhenCountIsGreaterThan4(void){
    TEST_ASSERT_EQUAL(0, count_set_bits(0xFF));
    TEST_ASSERT_EQUAL(0, count_set_bits(0xD9));
}

void test_MLX90393_Init_callsGetSettingsWhenNullSettings(void){
    //GetSettings_AddCallback(cb_GetSettings);
    GetSettings_ExpectAndReturn(&fake_mlx, 0);
    MLX90393_Init(&fake_mlx, NULL);
}

void test_MLX90393_Init_callsApplySettingsWhenSettings(void){

    mlx_cfg_t settings = {
        .gain = MLX90393_GAIN_1X,
        .resolution_x = MLX90393_RES_17,
        .resolution_y = MLX90393_RES_17,
        .resolution_z = MLX90393_RES_16,
        .filter = MLX90393_FILTER_3,
        .oversampling = MLX90393_OSR_2
    };
    //ApplySettings_AddCallback(cb_ApplySettings);
    ApplySettings_ExpectAndReturn(&fake_mlx, &settings, 0);
    MLX90393_Init(&fake_mlx, &settings);
}

void test_MLX90393_Init_Returns1WhenNullDev(void){
    mlx_cfg_t *settings;
    TEST_ASSERT_EQUAL(1, MLX90393_Init(NULL, NULL));
    TEST_ASSERT_EQUAL(1, MLX90393_Init(NULL, settings));
}

void test_MLX90393_Init_Returns2WhenNoFullDevStruct(void){
    fake_mlx.read_function = NULL;
    TEST_ASSERT_EQUAL(2, MLX90393_Init(&fake_mlx, NULL));

    fake_mlx.write_function = NULL;
    fake_mlx.read_function = read_function;
    TEST_ASSERT_EQUAL(2, MLX90393_Init(&fake_mlx, NULL));

    fake_mlx.mdelay = NULL;
    fake_mlx.write_function = write_function;
    TEST_ASSERT_EQUAL(2, MLX90393_Init(&fake_mlx, NULL));
}

void test_MLX90393_GetSettings_Returns1WhenNullDevStruct(void){
    TEST_ASSERT_EQUAL(1, MLX90393_GetSettings(NULL));
}
void test_MLX90393_GetSettings_WontCallMallocWhenSettingsProvided(void){ //It calls it just once to create the new_settings struct with the readings of the sensor

    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);

    fake_mlx.settings = (mlx_cfg_t *) malloc(sizeof(mlx_cfg_t));
    mlx_cfg_t settings = {
        .gain = MLX90393_GAIN_1X,
        .resolution_x = MLX90393_RES_17,
        .resolution_y = MLX90393_RES_17,
        .resolution_z = MLX90393_RES_16,
        .filter = MLX90393_FILTER_3,
        .oversampling = MLX90393_OSR_2
    };
    
    *(fake_mlx.settings) = settings;
    (void) MLX90393_GetSettings(&fake_mlx); 
    free(fake_mlx.settings);

}
void test_MLX90393_GetSettings_AllocatesMemoryIfNullSettingsAndThenProceedsAsUsual(void){
    //We expect malloc to be called. 
    mlx_malloc_Stub(cb_malloc);
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    fake_mlx.settings = NULL; 
    (void) MLX90393_GetSettings(&fake_mlx); 
    free(fake_mlx.settings);

    TEST_ASSERT_EQUAL(1, count_malloc);
}

void test_MLX90393_GetSettings_ReturnsNeg1IfMemoryAllocationFails(void){
    mlx_malloc_ExpectAndReturn(sizeof(mlx_cfg_t), NULL);
    fake_mlx.settings = NULL; 
    TEST_ASSERT_EQUAL(-1, MLX90393_GetSettings(&fake_mlx));
}

void test_MLX90393_ApplySettings_Returns1WhenNullDevStructOrNullNewDevSettings(void){
    mlx_cfg_t new_settings;
    mlx_i2c_t stub_mlx;
    TEST_ASSERT_EQUAL(1, MLX90393_ApplySettings(NULL, &new_settings));
    TEST_ASSERT_EQUAL(1, MLX90393_ApplySettings(&stub_mlx, NULL));
}
void test_MLX90393_ApplySettings_WontCallMallocWhenDevSettingsProvided(void){ //It calls it just once to create the new_settings struct with the readings of the sensor

    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);

    fake_mlx.settings = (mlx_cfg_t *) malloc(sizeof(mlx_cfg_t));  
    mlx_cfg_t settings = {
        .gain = MLX90393_GAIN_1X,
        .resolution_x = MLX90393_RES_17,
        .resolution_y = MLX90393_RES_17,
        .resolution_z = MLX90393_RES_16,
        .filter = MLX90393_FILTER_3,
        .oversampling = MLX90393_OSR_2
    };

    mlx_cfg_t new_settings;
    
    *(fake_mlx.settings) = settings;
    (void) MLX90393_ApplySettings(&fake_mlx, &new_settings); 
    free(fake_mlx.settings);

}
void test_MLX90393_ApplySettings_AllocatesMemoryIfNullDevSettingsAndThenProceedsAsUsual(void){
    mlx_cfg_t new_settings;
    mlx_malloc_Stub(cb_malloc);
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    fake_mlx.settings = NULL; 
    (void) MLX90393_ApplySettings(&fake_mlx, &new_settings); 
    free(fake_mlx.settings);
    TEST_ASSERT_EQUAL(1, count_malloc);
}

void test_MLX90393_ApplySettings_ReturnsNeg1IfMemoryAllocationFails(void){
    mlx_cfg_t new_settings;
    mlx_malloc_ExpectAndReturn(sizeof(mlx_cfg_t), NULL);
    fake_mlx.settings = NULL; 
    TEST_ASSERT_EQUAL(-1, MLX90393_ApplySettings(&fake_mlx, &new_settings));
}


void test_MLX90393_readXYZ_returns1IfNullDevPointerOrNullXYZarray(void){
    float xyz[3];
    TEST_ASSERT_EQUAL(1, MLX90393_readXYZ(&fake_mlx, NULL));
    TEST_ASSERT_EQUAL(1, MLX90393_readXYZ(NULL, xyz));
}

void test_MLX90393_readXYZ_callsMdelayWithCorrectTime(void){
    //prepare settings
    fake_mlx.settings = (mlx_cfg_t *) malloc(sizeof(mlx_cfg_t));  
    mlx_cfg_t settings = {
        .gain = MLX90393_GAIN_1X,
        .resolution_x = MLX90393_RES_17,
        .resolution_y = MLX90393_RES_17,
        .resolution_z = MLX90393_RES_16,
        .filter = MLX90393_FILTER_3,
        .oversampling = MLX90393_OSR_2
    };
    *(fake_mlx.settings) = settings;
    float xyz[3];

    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    delay_function_Expect(9); //MLX90393_Tconv_LookUp[3][2] + 1

    TEST_ASSERT_EQUAL(0, MLX90393_readXYZ(&fake_mlx, xyz));
    
}

void test_MLX90393_readXYZsubtractsForHiResSettings(void){
    mlx_cfg_t settings = {
        .gain = MLX90393_GAIN_1X,
        .resolution_x = MLX90393_RES_18,
        .resolution_y = MLX90393_RES_18,
        .resolution_z = MLX90393_RES_18,
        .filter = MLX90393_FILTER_3,
        .oversampling = MLX90393_OSR_2
    };
    *(fake_mlx.settings) = settings;
    float xyz[3];

    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    delay_function_Ignore();

    TEST_ASSERT_EQUAL(0, MLX90393_readXYZ(&fake_mlx, xyz));

    fake_mlx.settings->resolution_x = MLX90393_RES_19;
    fake_mlx.settings->resolution_y = MLX90393_RES_19;
    fake_mlx.settings->resolution_z = MLX90393_RES_19;
    
    TEST_ASSERT_EQUAL(0, MLX90393_readXYZ(&fake_mlx, xyz)); //Testear de alguna manera que xyz está bien restado

}

void test_MLX90393_Free_IdlesWhenNullDevice(void){
    MLX90393_Free(NULL); 
}

void test_MLX90393_Free_FreesMemoryOnceWhenNonNullDev(void){
    mlx_i2c_t stub_mlx; //I don't need to use the full structure, trash memory is enough to check everything works
    stub_mlx.settings = NULL;
    mlx_free_Expect(&stub_mlx);
    MLX90393_Free(&stub_mlx);
}

void test_MLX90393_Free_FreesMemoryTwiceWhenNonNullDevNorSettings(void){
    mlx_i2c_t stub_mlx;
    stub_mlx.settings = (mlx_cfg_t *) malloc(sizeof(mlx_cfg_t));
    mlx_free_Expect(stub_mlx.settings);
    mlx_free_Expect(&stub_mlx);
    MLX90393_Free(&stub_mlx);
}   

