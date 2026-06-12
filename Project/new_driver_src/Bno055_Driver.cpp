//
// Created by Kivanc Gunalp on 12.06.2026.
//

#include "Bno055_Driver.h"

namespace kiv::embedded::drivers::imu {
    bool Bno055_Driver::get_operation_mode(OperationMode &op_mode) {

        bool ok = assure_page_id(0);

        if (!ok) return false;

        uint8_t op_mode_val{};
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR,OPR_MODE_ADDR, &op_mode_val,1);

        if (!ok) return false;

        op_mode_val &= 0x0f;

        op_mode = static_cast<OperationMode>(op_mode_val);

        return true;
    }

    bool Bno055_Driver::set_operation_mode(OperationMode op_mode) {

        bool ok = assure_page_id(0);

        if (!ok) return false;

        uint8_t op_mode_val{};
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, OPR_MODE_ADDR, &op_mode_val, 1);

        if (!ok) return false;

        op_mode_val |= static_cast<uint8_t>(op_mode);
        ok = m_i2c_interface.i2c_bus_write(I2C_ADDR, OPR_MODE_ADDR, &op_mode_val, 1);

        if (!ok) return false;

        m_i2c_interface.delay_msec(600);
        return true;
    }

    bool Bno055_Driver::init() {
        bool ok = true;

        uint8_t data_u8{};
        /* Write the default page as zero*/
        uint8_t bno055_page_zero_u8 = 0;
        ok = m_i2c_interface.i2c_bus_write(I2C_ADDR, PAGE_ID_ADDR, &bno055_page_zero_u8, 1);

        if (!ok) {
            return false;
        }

        /* Read the chip id of the sensor from page
             * zero 0x00 register*/
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, CHIP_ID_ADDR, &data_u8, 1);
        if (!ok) {
            return false;
        }
        m_chip_id = data_u8;

        /* Read the accel revision id from page
             * zero 0x01 register*/
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, ACCEL_REV_ID_ADDR, &data_u8, 1);
        if (!ok) {
            return false;
        }
        m_accel_rev_id = data_u8;

        /* Read the mag revision id from page
             * zero 0x02 register*/
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, MAG_REV_ID_ADDR, &data_u8, 1);
        if (!ok) {
            return false;
        }

        m_mag_rev_id = data_u8;

        /* Read the gyro revision id from page
             * zero 0x02 register*/
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, GYR_REV_ID_ADDR, &data_u8, 1);

        if (!ok) {
            return false;
        }
        m_gyro_rev_id = data_u8;

        /* Read the boot loader revision from page
             * zero 0x06 register*/


        /* Read the software revision id from page
             * zero 0x04 and 0x05 register( 2 bytes of data)*/

        /* Read the page id from the register 0x07*/
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, PAGE_ID_ADDR, &data_u8, 1);

        if (!ok) {
            return false;
        }
        m_page_id = data_u8;

        return true;
    }



    bool Bno055_Driver::set_power_mode(PowerMode pm) {

        OperationMode previous_operation_mode{};

        bool ok = get_operation_mode(previous_operation_mode);

        if (!ok) {
            return false;
        }

        if (previous_operation_mode != OperationMode::CONFIG) {

            ok = set_operation_mode(OperationMode::CONFIG);

            if (!ok) {
                return false;
            }
        }

        uint8_t power_mode_val{};
        ok = m_i2c_interface.i2c_bus_read(I2C_ADDR, PWR_MODE_ADDR, &power_mode_val, 1);

        if (!ok) {
            return false;
        }

        power_mode_val |= static_cast<uint8_t>(pm);

        ok = m_i2c_interface.i2c_bus_write(I2C_ADDR, PWR_MODE_ADDR, &power_mode_val, 1);

        if (!ok) {
            return false;
        }

        if (previous_operation_mode != OperationMode::CONFIG)
        {
            /* set the operation mode
                 * of previous operation mode*/
            ok = set_operation_mode(previous_operation_mode);
        }


        return ok;
    }

    bool Bno055_Driver::assure_page_id(uint8_t page_id) {

        if (m_page_id == page_id) {
            return true;
        }

        uint8_t page_id_u8 = page_id;
        bool ok = m_i2c_interface.i2c_bus_write(I2C_ADDR,    PAGE_ID_ADDR, &page_id_u8, 1);

        if (!ok) {
            return false;
        }else{
            m_page_id = page_id;
            return true;
        }


    }


} // kiv