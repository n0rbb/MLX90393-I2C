#include <string.h>
#include <stdlib.h>
#include "unity.h"
#include "MLX90393.h"
#include "MLX90393_cmds.h"
#include "mock_test_MLX90393.h"

static mlx_i2c_t *fake_mlx_ptr;

void setUp(void) { 
    mlx_i2c_t fake_mlx = {
        .handle = (void *) 1,
        .write_function = write_function,
        .read_function = read_function,
        .mdelay = delay_function
    };

    mlx_cfg_t fake_settings = {
        .gain = MLX90393_GAIN_1X,
        .resolution_x = MLX90393_RES_17,
        .resolution_y = MLX90393_RES_17,
        .resolution_z = MLX90393_RES_16,
        .filter = MLX90393_FILTER_3,
        .oversampling = MLX90393_OSR_2
    };

    fake_mlx_ptr = (mlx_i2c_t *) malloc(sizeof(mlx_i2c_t));
    *(fake_mlx_ptr) = fake_mlx;
    fake_mlx_ptr->settings = (mlx_cfg_t *) malloc(sizeof(mlx_cfg_t));
    *(fake_mlx_ptr->settings) = fake_settings;
}

void teardown(){
    free(fake_mlx_ptr->settings);
    free(fake_mlx_ptr);
}

//Command tests: Correct writing of commands to bus, early exiting on writing failure, register reading status reporting

void test_MLX90393_EX_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_EX(fake_mlx_ptr, &status));
}

void test_MLX90393_EX_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_EX(fake_mlx_ptr, &status));
}

void test_MLX90393_EX_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_EX(fake_mlx_ptr, &status));
}




void test_MLX90393_SB_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_SB(fake_mlx_ptr, 0x0E, &status));
}

void test_MLX90393_SB_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_SB(fake_mlx_ptr, 0x0E, &status));
}

void test_MLX90393_SB_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_SB(fake_mlx_ptr, 0x0E, &status));
}




void test_MLX90393_SWOC_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_SWOC(fake_mlx_ptr, 0x0E, &status));
}

void test_MLX90393_SWOC_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_SWOC(fake_mlx_ptr, 0x0E, &status));
}

void test_MLX90393_SWOC_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_SWOC(fake_mlx_ptr, 0x0E, &status));
}




void test_MLX90393_SM_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_SM(fake_mlx_ptr, 0x0E, &status));
}

void test_MLX90393_SM_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_SM(fake_mlx_ptr, 0x0E, &status));
}

void test_MLX90393_SM_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_SM(fake_mlx_ptr, 0x0E, &status));
}




void test_MLX90393_RM_ReturnsOnWriteFailure(void){
    uint8_t status;
    uint8_t data[6];
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_RM(fake_mlx_ptr, 0x0E, &status, data));
}

void test_MLX90393_RM_ReturnsOnReadFailure(void){
    uint8_t status;
    uint8_t data[6];
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_RM(fake_mlx_ptr, 0x0E, &status, data));
}

void test_MLX90393_RM_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    uint8_t data[6];
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_RM(fake_mlx_ptr, 0x0E, &status, data));
}





void test_MLX90393_RR_ReturnsOnWriteFailure(void){
    uint8_t status;
    uint8_t data;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_RR(fake_mlx_ptr, &status, 0, &data));
}

void test_MLX90393_RR_ReturnsOnReadFailure(void){
    uint8_t status;
    uint8_t data;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_RR(fake_mlx_ptr, &status, 0, &data));
}

void test_MLX90393_RR_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    uint8_t data;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_RR(fake_mlx_ptr, &status, 0, &data));
}




void test_MLX90393_WR_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_WR(fake_mlx_ptr, &status, 0, 1));
}

void test_MLX90393_WR_ReturnsOnReadFailure(void){
    uint8_t status;
    
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_WR(fake_mlx_ptr, &status, 0, 1));
}

void test_MLX90393_WR_SunnyDay(void){ //Everything works correctly
    uint8_t status;

    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_WR(fake_mlx_ptr, &status, 0, 1));
}




void test_MLX90393_HR_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_HR(fake_mlx_ptr, &status));
}

void test_MLX90393_HR_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_HR(fake_mlx_ptr, &status));
}

void test_MLX90393_HR_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_HR(fake_mlx_ptr, &status));
}




void test_MLX90393_HS_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_HS(fake_mlx_ptr, &status));
}

void test_MLX90393_HS_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_HS(fake_mlx_ptr, &status));
}

void test_MLX90393_HS_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_HS(fake_mlx_ptr, &status));
}




void test_MLX90393_RT_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_RT(fake_mlx_ptr, &status));
}

void test_MLX90393_RT_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_RT(fake_mlx_ptr, &status));
}

void test_MLX90393_RT_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_RT(fake_mlx_ptr, &status));
}





void test_MLX90393_NOP_ReturnsOnWriteFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_NOP(fake_mlx_ptr, &status));
}

void test_MLX90393_NOP_ReturnsOnReadFailure(void){
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(1);
    TEST_ASSERT_EQUAL(1, MLX90393_NOP(fake_mlx_ptr, &status));
}

void test_MLX90393_NOP_SunnyDay(void){ //Everything works correctly
    uint8_t status;
    write_function_IgnoreAndReturn(0);
    read_function_IgnoreAndReturn(0);
    TEST_ASSERT_EQUAL(0, MLX90393_NOP(fake_mlx_ptr, &status));
}




