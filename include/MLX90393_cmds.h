#ifndef _MLX90393_CMDS_H
#define _MLX90393_CMDS_H

#include "MLX90393.h"

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