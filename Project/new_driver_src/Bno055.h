//
// Created by Kivanc Gunalp on 12.06.2026.
//

#ifndef PROJECT_BNO055_H
#define PROJECT_BNO055_H
#include <cstdint>

namespace kiv {

    enum class OperationMode : uint8_t {
        /* Operation mode settings*/
        CONFIG = 0,
        ACCONLY,
        MAGONLY,
        GYRONLY,
        ACCMAG,
        ACCGYRO,
        MAGGYRO,
        AMG,
        IMUPLUS,
        COMPASS,
        M4G,
        NDOF_FMC_OFF,
        NDOF
    };

    enum class PowerMode : uint8_t {
        /* Power mode*/
        NORMAL = 0,
        LOWPOWER,
        SUSPEND
    };
    class Bno055 {

    protected:
        /* PAGE0 REGISTER DEFINITION START*/
        static constexpr uint8_t CHIP_ID_ADDR = 0;
        static constexpr uint8_t ACCEL_REV_ID_ADDR  = 1;
        static constexpr uint8_t MAG_REV_ID_ADDR  = 2;
        static constexpr uint8_t GYR_REV_ID_ADDR  = 3;
        static constexpr uint8_t SW_REV_ID_LSB_ADDR  = 4;
        static constexpr uint8_t SW_REV_ID_MSB_ADDR  = 5;
        static constexpr uint8_t PAGE_ID_ADDR = 7;

        static constexpr uint8_t I2C_ADDR = 0x28;

        //
        uint8_t m_chip_id{};
        uint8_t m_accel_rev_id{};
        uint8_t m_mag_rev_id{};
        uint8_t m_gyro_rev_id{};
        uint8_t m_page_id{};

        public:
        virtual ~Bno055() = default;

        bool set_power_mode(PowerMode pm) {
            // TODO
            return true;
        }

        bool get_operation_mode(OperationMode &op_mode) {
            // TODO
            op_mode = OperationMode::AMG;
            return true;
        }

        bool init() {
            bool success = true;

            uint8_t data_u8{};
            /* Write the default page as zero*/
            uint8_t bno055_page_zero_u8 = 0;
            success = i2c_bus_write(I2C_ADDR, PAGE_ID_ADDR, &bno055_page_zero_u8, 1);

            if (!success) {
                return false;
            }

            /* Read the chip id of the sensor from page
             * zero 0x00 register*/
            success = i2c_bus_read(I2C_ADDR, CHIP_ID_ADDR, &data_u8, 1);
            if (!success) {
                return false;
            }
            m_chip_id = data_u8;

            /* Read the accel revision id from page
             * zero 0x01 register*/
            success = i2c_bus_read(I2C_ADDR, ACCEL_REV_ID_ADDR, &data_u8, 1);
            if (!success) {
                return false;
            }
            m_accel_rev_id = data_u8;

            /* Read the mag revision id from page
             * zero 0x02 register*/
            success = i2c_bus_read(I2C_ADDR, MAG_REV_ID_ADDR, &data_u8, 1);
            if (!success) {
                return false;
            }

            m_mag_rev_id = data_u8;

            /* Read the gyro revision id from page
             * zero 0x02 register*/
            success = i2c_bus_read(I2C_ADDR, GYR_REV_ID_ADDR, &data_u8, 1);

            if (!success) {
                return false;
            }
            m_gyro_rev_id = data_u8;

            /* Read the boot loader revision from page
             * zero 0x06 register*/


            /* Read the software revision id from page
             * zero 0x04 and 0x05 register( 2 bytes of data)*/

            /* Read the page id from the register 0x07*/
            success = i2c_bus_read(I2C_ADDR, PAGE_ID_ADDR, &data_u8, 1);

            if (!success) {
                return false;
            }
            m_page_id = data_u8;

            return true;
        }

        protected:
        virtual bool i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) = 0;
        virtual bool i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) = 0;
        virtual void delay_msec(uint32_t msec) = 0;
    };



} // kiv

#endif //PROJECT_BNO055_H