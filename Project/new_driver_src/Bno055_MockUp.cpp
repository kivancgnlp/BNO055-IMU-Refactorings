//
// Created by Kivanc Gunalp on 12.06.2026.
//

#include "Bno055_MockUp.h"

#include <cstdio>

namespace kiv {
    Bno055_MockUp::Bno055_MockUp() {

        m_emulation_register_map_page0[0] = 0xa0; // Chip ID
        m_emulation_register_map_page0[1] = 0xfb; // ACC ID
        m_emulation_register_map_page0[2] = 0x32; // MAG ID
        m_emulation_register_map_page0[3] = 0x0f; // Gyro ID

        m_emulation_register_map_page0[4] = 0x11; // SW Rev LSB
        m_emulation_register_map_page0[5] = 0x03; // SW Rev MSB

        //Gravity X
        m_emulation_register_map_page0[0x2f] = 1;
        m_emulation_register_map_page0[0x2e] = 20;


        //Gravity Y
        m_emulation_register_map_page0[0x31] = 1;
        m_emulation_register_map_page0[0x30] = 60;


        //Gravity Z
        m_emulation_register_map_page0[0x33] = 1;
        m_emulation_register_map_page0[0x32] = 100;


        //Orientation

        //Heading
        m_emulation_register_map_page0[0x1b] = 1;
        m_emulation_register_map_page0[0x1a] = 30;

        //Roll
        m_emulation_register_map_page0[0x1d] = 1;
        m_emulation_register_map_page0[0x1c] = 60;

        //Pitch
        m_emulation_register_map_page0[0x1f] = 1;
        m_emulation_register_map_page0[0x1e] = 90;



    }

    Bno055_MockUp::~Bno055_MockUp() {
    }

    bool Bno055_MockUp::i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) {

        printf("BNO055 I2C bus write called. dev_addr : %x, reg_addr : %x, first data : %x, cnt : %d\n",dev_addr,reg_addr,reg_data[0],cnt);

        if (reg_addr == 7 && reg_data[0] != 0) {
            fprintf(stderr, "Emulation other than page zero not supported");
        }

        for (unsigned int i = 0; i < cnt; i++) {
            if ( reg_addr + i < 256) {
                m_emulation_register_map_page0[reg_addr + i] = reg_data[i];
            }else {
                fprintf(stderr, "Emulation ram out of bounds");
                return false;
            }
        }

        return true;
    }

    bool Bno055_MockUp::i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) {

        printf("BNO055 I2C bus read called. dev_addr : %x, reg_addr : %x, first data : %x, cnt : %d\n",dev_addr,reg_addr,reg_data[0],cnt);

        for (unsigned int i = 0; i < cnt; i++) {
            if ( reg_addr + i < 256) {
                reg_data[i] = m_emulation_register_map_page0[reg_addr + i];
            }else {
                fprintf(stderr, "Emulation ram out of bounds");
                return false;
            }
        }

        return true;
    }

    void Bno055_MockUp::delay_msec(uint32_t msec) {

        printf("BNO055_delay_msek called with %d ms\n", msec);
    }
} // kiv