#ifndef MLX90393_H
#define MLX90393_H

#include <stdint.h>

#define I2C_ADDR 0x0C
#define MAG_XYZ 0x0E

#define MLX90393_REG_CONF1 0x00
#define MLX90393_REG_CONF3 0x02

typedef struct mlx_i2c_t mlx_i2c_t;
typedef struct mlx_cfg_t mlx_cfg_t;

typedef int32_t (*mlx_wr_ptr)(mlx_i2c_t *handle, uint8_t *buf, size_t len);
typedef int32_t (*mlx_rd_ptr)(mlx_i2c_t *handle, uint8_t *data, size_t len); // read the bus
typedef void (*mlx_mdelay_ptr)(uint32_t ms);

typedef enum mlx90393_gain {
  MLX90393_GAIN_5X = (0x00),
  MLX90393_GAIN_4X,
  MLX90393_GAIN_3X,
  MLX90393_GAIN_2_5X,
  MLX90393_GAIN_2X,
  MLX90393_GAIN_1_67X,
  MLX90393_GAIN_1_33X,
  MLX90393_GAIN_1X
} mlx90393_gain_t;

typedef enum mlx90393_resolution {
  MLX90393_RES_16,
  MLX90393_RES_17,
  MLX90393_RES_18,
  MLX90393_RES_19,
} mlx90393_resolution_t;

typedef enum mlx90393_filter {
  MLX90393_FILTER_0,
  MLX90393_FILTER_1,
  MLX90393_FILTER_2,
  MLX90393_FILTER_3,
  MLX90393_FILTER_4,
  MLX90393_FILTER_5,
  MLX90393_FILTER_6,
  MLX90393_FILTER_7,
} mlx90393_filter_t;

typedef enum mlx90393_oversampling {
  MLX90393_OSR_0,
  MLX90393_OSR_1,
  MLX90393_OSR_2,
  MLX90393_OSR_3,
} mlx90393_oversampling_t;


/**
 * @brief MLX90393 IIC device structure
 * 
 */
struct mlx_i2c_t{
    void *handle;
    mlx_cfg_t *settings;
    mlx_wr_ptr write_function;
    mlx_rd_ptr read_function;
    mlx_mdelay_ptr mdelay;
    
};

/**
 * @brief MLX settings (CONF1 & CONF3 registers) structure
 * 
 */
struct mlx_cfg_t{
    mlx90393_gain_t gain;
    mlx90393_resolution_t resolution_x;
    mlx90393_resolution_t resolution_y;
    mlx90393_resolution_t resolution_z;
    mlx90393_filter_t filter;
    mlx90393_oversampling_t oversampling;
};

// USER FUNCTIONS
int32_t MLX90393_Init(mlx_i2c_t *dev);
int32_t MLX90393_GetSettings(mlx_i2c_t *dev);
int32_t MLX90393_ApplySettings(mlx_i2c_t *dev, mlx_cfg_t *new_settings);
int32_t MLX90393_readXYZ(mlx_i2c_t *dev, float *xyz);
uint8_t count_set_bits(uint8_t zyxt);

// COMMANDS
int32_t MLX90393_EX(mlx_i2c_t *dev, uint8_t *statusBuffer);
int32_t MLX90393_SB(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer);
int32_t MLX90393_SWOC(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer);
int32_t MLX90393_SM(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer);
int32_t MLX90393_RM(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer, uint8_t *dataBuffer);
int32_t MLX90393_RR(mlx_i2c_t *dev, uint8_t *statusBuffer, int reg_addr, uint8_t *dataBuffer);
int32_t MLX90393_WR(mlx_i2c_t *dev, uint8_t *statusBuffer, int reg_addr, int data);
int32_t MLX90393_HR(mlx_i2c_t *dev, uint8_t *statusBuffer);
int32_t MLX90393_HS(mlx_i2c_t *dev, uint8_t *statusBuffer);
int32_t MLX90393_RT(mlx_i2c_t *dev, uint8_t *statusBuffer);
int32_t MLX90393_NOP(mlx_i2c_t *dev, uint8_t *statusBuffer);


#endif
