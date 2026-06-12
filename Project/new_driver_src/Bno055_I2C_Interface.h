//
// Created by Kiv on 6/12/2026.
//

#ifndef PROJECT_BNO055_I2C_INTERFACE_H
#define PROJECT_BNO055_I2C_INTERFACE_H

#include <cstdint>

namespace kiv::embedded::drivers::imu {
    class Bno055_I2C_Interface {


        public:
        virtual bool i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) = 0;
        virtual bool i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) = 0;
        virtual void delay_msec(uint32_t msec) = 0;

        virtual ~Bno055_I2C_Interface() = default;


    };
} // kiv

#endif //PROJECT_BNO055_I2C_INTERFACE_H