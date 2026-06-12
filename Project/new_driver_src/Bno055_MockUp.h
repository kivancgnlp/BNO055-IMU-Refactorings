//
// Created by Kivanc Gunalp on 12.06.2026.
//

#ifndef PROJECT_BNO055_MOCKUP_H
#define PROJECT_BNO055_MOCKUP_H
#include "Bno055.h"

#include <array>

namespace kiv {
    class Bno055_MockUp : public Bno055{
        public:
        Bno055_MockUp();
        ~Bno055_MockUp() override;
    protected:
        bool i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) override;

        bool i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) override;

        void delay_msec(uint32_t msec) override;

        private:
        std::array<uint8_t,256> m_emulation_register_map_page0{};

    };
} // kiv

#endif //PROJECT_BNO055_MOCKUP_H