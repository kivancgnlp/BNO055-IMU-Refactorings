//
// Created by Kiv on 6/12/2026.
//

#ifndef PROJECT_BNO055_I2C_MOCK_INTERFACE_H
#define PROJECT_BNO055_I2C_MOCK_INTERFACE_H
#include <array>

#include "Bno055_I2C_Interface.h"

namespace kiv::embedded::drivers::imu {
    class Bno055_I2C_Mock_Interface : public Bno055_I2C_Interface{

    public:
        Bno055_I2C_Mock_Interface();
        ~Bno055_I2C_Mock_Interface() override = default;
    protected:
        bool i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) override;

        bool i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) override;

        void delay_msec(uint32_t msec) override;

    private:
        std::array<uint8_t,256> m_emulation_register_map_page0{};
    };

} // kiv

#endif //PROJECT_BNO055_I2C_MOCK_INTERFACE_H