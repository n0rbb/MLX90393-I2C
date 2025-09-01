#ifndef _TEST_MLX90393_H
#define _TEST_MLX90393_H

#include "MLX90393.h"

int32_t write_function(mlx_i2c_t *dev, uint8_t *buf, size_t len);
int32_t read_function(mlx_i2c_t *dev, uint8_t *data, size_t len);
void delay_function(uint32_t ms);

int32_t GetSettings(mlx_i2c_t *dev); 
int32_t ApplySettings(mlx_i2c_t *dev, mlx_cfg_t *settings);
void *mlx_malloc(size_t s);
void mlx_free(void *memory);


#endif