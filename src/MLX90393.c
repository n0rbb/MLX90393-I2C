#include "MLX90393.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper function that calls automatically MLX90393_GetSettings(). This function is created for ease of unit testing.
 * 
 * @param dev Handle to MLX90393 device
 * @return int32_t MLX90393_GetSettings error code
 */
__attribute__((weak)) int32_t GetSettings(mlx_i2c_t *dev){
    return MLX90393_GetSettings(dev);
}

__attribute__((weak)) int32_t ApplySettings(mlx_i2c_t *dev, mlx_cfg_t *settings){
    return MLX90393_ApplySettings(dev, settings);
}

__attribute__((weak)) void *mlx_malloc(size_t s){
    return malloc(s);
}

/** Helper functions**/
uint8_t count_set_bits(uint8_t zyxt){
    uint8_t result = 0;
    while(zyxt){ //Mientras haya al menos un 1
        result++;
        zyxt &= zyxt - 1; //Quito el LSb que tenga un 1
    }
    if(result > 4){
        result = 0;
    }
    return result;
}
//LOOKUPS
//Magnetic sensitivity [Gain (0 - 7)][Res (16 - 19)][SensXY/SensZ]
const float MLX90393_Sensitivity_LookUp[8][4][2] = {
    //Gain = 0 (5X)
    {{0.805, 1.468}, {1.610, 2.936}, {3.220, 5.872}, {6.440, 11.744}},
    //Gain = 1 (4X)
    {{0.644, 1.174}, {1.288, 2.349}, {2.576, 4.698}, {5.152, 9.395}},
    //Gain = 2 (3X)
    {{0.483, 0.881}, {0.966, 1.762}, {1.932, 3.523}, {3.864, 7.046}},
    //Gain = 3 (2.5X)
    {{0.403, 0.734}, {0.805, 1.468}, {1.610, 2.936}, {3.220, 5.872}},
    //Gain = 4 (2X)
    {{0.322, 0.587}, {0.644, 1.174}, {1.288, 2.349}, {2.576, 4.698}},
    //Gain = 5 (1.67X)
    {{0.268, 0.489}, {0.537, 0.979}, {1.073, 1.957}, {2.147, 3.915}},
    //Gain = 6 (1.33X)
    {{0.215, 0.391}, {0.429, 0.783}, {0.859, 1.566}, {1.717, 3.132}},
    //Gain = 7 (1X)
    {{0.161, 0.294}, {0.322, 0.587}, {0.644, 1.174}, {1.288, 2.349}}
};

//Tconv for a single measurement [Filter (0 - 8)][Oversampling rate (0 - 3)]
const float MLX90393_Tconv_LookUp[8][4] = {
    //Dig_filt 0
    {1.27, 1.84, 3.00, 5.30},
    //Dig_filt 1
    {1.46, 2.23, 3.76, 6.84},
    //Dig_filt 2
    {1.84, 3.00, 5.30, 9.91},
    //Dig_filt 3
    {2.61, 4.53, 8.37, 16.05},
    //Dig_filt 4
    {4.15, 7.60, 14.52, 28.34},
    //Dig_filt 5
    {7.22, 13.75, 26.80, 52.92},
    //Dig_filt 6
    {13.36, 26.04, 51.38, 102.07},
    //Dig_filt 7
    {25.65, 50.61, 100.53, 200.37}
};

//COMMANDS
/**
 * @brief Exit function
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */

int32_t MLX90393_EX(mlx_i2c_t *dev, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = 0x80;
    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

/**
 * @brief Start Burst Measurement 
 * 
 * @param dev Handle to MLX90393 device
 * @param zyxt Magnetic axes-temperature measurement setting
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_SB(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = (0x10)|(zyxt); // 0001 zxyt
    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

/**
 * @brief Start Wake-up On Change 
 * 
 * @param dev Handle to MLX90393 device
 * @param zyxt Magnetic axes-temperature measurement setting
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_SWOC(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = (0x20)|(zyxt);
     // 0010 zxyt
    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

/**
 * @brief Start Measurement (Polling)
 * 
 * @param dev Handle to MLX90393 device
 * @param zyxt Magnetic axes-temperature measurement setting
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_SM(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = (0x30) | (zyxt);
    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

/**
 * @brief Read magnetic field and temperature measurement, started either as a burst or polling fashion
 * 
 * @param dev Handle to MLX90393 device
 * @param zyxt Magnetic axes-temperature measurement setting
 * @param statusBuffer Buffer to store status byte from the sensor
 * @param dataBuffer Buffer to store data bytes from the sensor measurements
 * @return int32_t Error code
 */
int32_t MLX90393_RM(mlx_i2c_t *dev, char zyxt, uint8_t *statusBuffer, uint8_t *dataBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer[10];
    
    //Compute the number of bytes to receive (1 byte per '1' in zyxt)
    uint8_t databytes = count_set_bits((uint8_t) zyxt);
    uint8_t receiveBuffer[1 + 2*databytes];

    writeBuffer[0] = (0x40) | (zyxt);
    ret = dev->write_function(dev, writeBuffer, 1); 
    //Check if I shall need to write 1 + 2*databytes in order to receive all the measurements in the w-r fashion
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, receiveBuffer, 1 + 2 * databytes);
    *statusBuffer = receiveBuffer[0];
    for (int i = 1; i < sizeof(receiveBuffer); i++) {
        dataBuffer[i - 1] = receiveBuffer[i];
    }
    return ret;
}

/**
 * @brief Function to read from MLX90393 register in the volatile RAM
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @param reg_addr Address of the register to read
 * @return int32_t Error code
 */
int32_t MLX90393_RR(mlx_i2c_t *dev, uint8_t *statusBuffer, int reg_addr, uint8_t *dataBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer[2];
    uint8_t receiveBuffer[3];
    writeBuffer[0] = 0x50;
    writeBuffer[1] = reg_addr << 2;
    ret = dev->write_function(dev, writeBuffer, 2);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, receiveBuffer, 3);
    *statusBuffer = receiveBuffer[0];
    dataBuffer[0] = receiveBuffer[1];
    dataBuffer[1] = receiveBuffer[2];
    return ret;
}

/**
 * @brief Write data to a MLX90393 register in the volatile RAM
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @param data Data to write to the register
 * @param reg_addr Address of the register to write to
 * @return int32_t Error code
 */
int32_t MLX90393_WR(mlx_i2c_t *dev, uint8_t *statusBuffer, int reg_addr, int data){
    int32_t ret = 0;
    uint8_t writeBuffer[4];

    writeBuffer[0] = 0x60;
    writeBuffer[1] = (data & 0xFF00) >> 8; //I take the and with 1111111100000000 (Keep the 8 Most significant bits) and shift right for them to fit in the uint8_t
    writeBuffer[2] = data & 0x00FF; //I take the least significant bits
    writeBuffer[3] = reg_addr << 2;

    ret = dev->write_function(dev, writeBuffer, 4);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;

}

/**
 * @brief Memory Recall: Overwrite volatile RAM with non-volatile RAM data)
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_HR(mlx_i2c_t *dev, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = 0xD0;
    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}


/**
 * @brief Memory store: Overwrite non-volatile RAM with volatile RAM data
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_HS(mlx_i2c_t *dev, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = 0xE0;
    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

/**
 * @brief Reset Command
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_RT(mlx_i2c_t *dev, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = 0xF0;

    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

/**
 * @brief No Operation
 * 
 * @param dev Handle to MLX90393 device
 * @param statusBuffer Buffer to store status byte from the sensor
 * @return int32_t Error code
 */
int32_t MLX90393_NOP(mlx_i2c_t *dev, uint8_t *statusBuffer){
    int32_t ret = 0;
    uint8_t writeBuffer = 0x00;

    ret = dev->write_function(dev, &writeBuffer, 1);
    if (ret != 0){
        return ret;
    }
    ret = dev->read_function(dev, statusBuffer, 1);
    return ret;
}

//USER FUNCTIONS
/**
 * @brief Get current settings of sensor by reading 0x00 and 0x02 registers
 * 
 * @param dev Handle to MLX90393 device
 * @return int32_t Error code
 */
int32_t MLX90393_GetSettings(mlx_i2c_t *dev){
    if (dev == NULL){
        return 1;
    }
    if(dev->settings == NULL){
        dev->settings = (mlx_cfg_t *) mlx_malloc(sizeof(mlx_cfg_t));

        if (dev->settings == NULL) return -1;
    }
    int32_t ret;
    uint8_t status;
    uint8_t databuffer[2];
    mlx_cfg_t *settings = (mlx_cfg_t *) mlx_malloc(sizeof(mlx_cfg_t));
    //Get current settings
    //Read Conf1
    ret = MLX90393_RR(dev, &status, MLX90393_REG_CONF1, databuffer);
    settings->gain = (mlx90393_gain_t) (databuffer[1] >> 4) & 0x07;
    //Read Conf3
    ret = MLX90393_RR(dev, &status, MLX90393_REG_CONF3, databuffer);
    settings->oversampling = (mlx90393_oversampling_t) databuffer[1] & 0x03;
    settings->filter = (mlx90393_filter_t) (databuffer[1] >> 2) & 0x07;
    
    settings->resolution_x = (mlx90393_resolution_t) (databuffer[1] >> 5) & 0x03;
    settings->resolution_y = (mlx90393_resolution_t) ((databuffer[0] << 1) & 0x02) | databuffer[1] >> 7;
    settings->resolution_z = (mlx90393_resolution_t) (databuffer[0] >> 1) & 0x03;
    
    *(dev->settings) = *settings;
    free(settings);
    return ret;
}


/**
 * @brief Apply the settings set in a mlx_cfg_t structure to the device
 * 
 * @param dev Handle to MLX90393 device
 * @param new_settings Pointer to a mlx_cfg_t structure with the new desired settings
 * @return int32_t Error code
 */
int32_t MLX90393_ApplySettings(mlx_i2c_t *dev, mlx_cfg_t *new_settings){
    if ((dev == NULL || new_settings == NULL)){
        return 1;
    }
    if(dev->settings == NULL){
        dev->settings = (mlx_cfg_t *) mlx_malloc(sizeof(mlx_cfg_t));

        if(dev->settings == NULL) return -1; //Check the heap allocation doesn't fail
    }
    int32_t ret;
    uint8_t status;
    uint8_t databuffer[2];
    //Apply CONF1 register
    ret = MLX90393_RR(dev, &status, MLX90393_REG_CONF1, databuffer);
    int newbuf = databuffer[0] << 8 | databuffer[1];
    newbuf &= ~0x0070; //AND with an all - 1 word but the bits I want to reset
    newbuf |= (int) new_settings->gain << 4; //Recast the enum as an int and shift to the correct positions
    ret = MLX90393_WR(dev, &status, MLX90393_REG_CONF1, newbuf);

    //CONF3 register
    ret = MLX90393_RR(dev, &status, MLX90393_REG_CONF3, databuffer);
    newbuf = databuffer[0] << 8 | databuffer[1];
    newbuf &= ~0x07FF;
    newbuf |= (int) new_settings->oversampling;
    newbuf |= (int) new_settings->filter << 2;
    newbuf |= (int) new_settings->resolution_x << 5;
    newbuf |= (int) new_settings->resolution_y << 7;
    newbuf |= (int) new_settings->resolution_z << 9;
    ret  = MLX90393_WR(dev, &status, MLX90393_REG_CONF3, newbuf);
    //Overwrite dev settings structure with 
    *(dev->settings) = *new_settings;
    return ret;
}

int32_t MLX90393_readXYZ(mlx_i2c_t *dev, float *xyz){
    
    if(dev == NULL || xyz == NULL){
        return 1;
    }

    //Copy the pointer to the settings for easier access later
    mlx_cfg_t *curr_cfg = dev->settings;

    int32_t ret;
    uint8_t status;
    /*Start measurement*/
    ret = MLX90393_SM(dev, MLX90393_MAG_XYZ, &status);
    if (ret != 0){
        return ret;
    }

    /*Wait tconv*/
    dev->mdelay((int) MLX90393_Tconv_LookUp[curr_cfg->filter][curr_cfg->oversampling] + 1); 

    /*Read measurement*/
    uint8_t data[6];
    ret = MLX90393_RM(dev, MLX90393_MAG_XYZ, &status, data);
    if (ret != 0){
        return ret;
    }
    
    /*Convert to magnetic units */
    int16_t xyz_tmp[3];
    xyz_tmp[0] = (data[0] << 8) | data[1];
    xyz_tmp[1] = (data[2] << 8) | data[3];
    xyz_tmp[2] = (data[4] << 8) | data[5]; 

    if (curr_cfg->resolution_x == MLX90393_RES_18) xyz_tmp[0] -= 0x8000;
    if (curr_cfg->resolution_x == MLX90393_RES_19) xyz_tmp[0] -= 0x4000;
    if (curr_cfg->resolution_y == MLX90393_RES_18) xyz_tmp[1] -= 0x8000;
    if (curr_cfg->resolution_y == MLX90393_RES_19) xyz_tmp[1] -= 0x4000;
    if (curr_cfg->resolution_z == MLX90393_RES_18) xyz_tmp[2] -= 0x8000;
    if (curr_cfg->resolution_z == MLX90393_RES_19) xyz_tmp[2] -= 0x4000;


    xyz[0] = (float) xyz_tmp[0] * MLX90393_Sensitivity_LookUp[curr_cfg->gain][curr_cfg->resolution_x][0];
    xyz[1] = (float) xyz_tmp[1] * MLX90393_Sensitivity_LookUp[curr_cfg->gain][curr_cfg->resolution_y][0];
    xyz[2] = (float) xyz_tmp[2] * MLX90393_Sensitivity_LookUp[curr_cfg->gain][curr_cfg->resolution_z][1];
    
    return ret;
}

/**
 * @brief MLX90393 device initialisation function
 * 
 * @param dev Handle to MLX90393 device
 * @param settings [Optional] Pointer to MLX90393 settings structure to write to the device
 * @return int32_t 
 */
int32_t MLX90393_Init(mlx_i2c_t *dev, mlx_cfg_t *settings){
    if(dev == NULL){
        return 1;
    } 
    if(dev->read_function == NULL || dev->write_function == NULL || dev->mdelay == NULL){
        return 2;
    }
    if (settings == NULL){ //If the user doesn't provide its own "new" settings, we test the device by reading the current settings and storing them to the dev structure
        return GetSettings(dev);
    }
    return ApplySettings(dev, settings); //Otherwise, we write the user settings to the device registers
}