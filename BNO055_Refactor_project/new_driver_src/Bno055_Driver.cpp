//
// Created by Kivanc Gunalp on 12.06.2026.
//

#include "Bno055_Driver.h"

#include "BitUtils.h"

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
            m_page_id = -1;
            return false;
        }else {
            m_page_id = 0;
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

    std::optional<std::tuple<float, float, float>> Bno055_Driver::read_gravity_vector_in_msq() {

        std::optional<float> gravity_val_raw[3];
        constexpr std::array<uint8_t, 3> gravity_adrs = {GRAVITY_DATA_X_LSB_ADDR, GRAVITY_DATA_Y_LSB_ADDR, GRAVITY_DATA_Z_LSB_ADDR};

        gravity_val_raw[0] = read_float_value_from_short_adr_by_dividing_scale_factor<gravity_adrs.at(0),GRAVITY_DIV_MSQ>();
        gravity_val_raw[1] = read_float_value_from_short_adr_by_dividing_scale_factor<gravity_adrs.at(1),GRAVITY_DIV_MSQ>();
        gravity_val_raw[2] = read_float_value_from_short_adr_by_dividing_scale_factor<gravity_adrs.at(2),GRAVITY_DIV_MSQ>();


        if ( gravity_val_raw[0] && gravity_val_raw[1] && gravity_val_raw[2]) {
            return {{gravity_val_raw[0].value(),gravity_val_raw[1].value(),gravity_val_raw[2].value()}};
        }

        return std::nullopt;

    }

    std::optional<std::tuple<float, float, float>> Bno055_Driver::read_euler_orientation_vector_in_degrees() {

        std::optional<float> euler_val_raw[3];
        constexpr std::array<uint8_t, 3> euler_adrs = {EULER_HEADING_LSB_ADDR, EULER_ROLL_LSB_ADDR, EULER_PITCH_LSB_ADDR};

        euler_val_raw[0] = read_float_value_from_short_adr_by_dividing_scale_factor<euler_adrs.at(0),EULER_DIV_DEG>();
        euler_val_raw[1] = read_float_value_from_short_adr_by_dividing_scale_factor<euler_adrs.at(1),EULER_DIV_DEG>();
        euler_val_raw[2] = read_float_value_from_short_adr_by_dividing_scale_factor<euler_adrs.at(2),EULER_DIV_DEG>();


        if ( euler_val_raw[0] && euler_val_raw[1] && euler_val_raw[2]) {
            return {{euler_val_raw[0].value(),euler_val_raw[1].value(),euler_val_raw[2].value()}};
        }

        return std::nullopt;
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

        update_bit_field<0,2>(power_mode_val, static_cast<uint8_t>(pm));

        ok = m_i2c_interface.i2c_bus_write(I2C_ADDR, PWR_MODE_ADDR, &power_mode_val, 1);

        if (!ok) {
            return false;
        }

        if (previous_operation_mode != OperationMode::CONFIG) {
            /* return to previous operation mode*/
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


    template<Axis axis>
    std::optional<float> Bno055_Driver::read_gravity_vector_axis_in_msq() {

        constexpr std::array<uint8_t, 3> gravity_adrs = {GRAVITY_DATA_X_LSB_ADDR, GRAVITY_DATA_Y_LSB_ADDR, GRAVITY_DATA_Z_LSB_ADDR};
        constexpr uint8_t target_adr = gravity_adrs.at(static_cast<uint8_t>(axis));

        return read_float_value_from_short_adr_by_dividing_scale_factor<target_adr,GRAVITY_DIV_MSQ>();

    }

    template<uint8_t ADR, float scale_divisor>
    std::optional<float> Bno055_Driver::read_float_value_from_short_adr_by_dividing_scale_factor() {
        bool ok = assure_page_id(0);
        if (!ok) return std::nullopt;

        uint16_t data_u16{};
        ok  = m_i2c_interface.i2c_bus_read(I2C_ADDR,ADR, reinterpret_cast<uint8_t *>(&data_u16),2); // TODO : It is ok for little endian

        if (!ok) return std::nullopt;

        return data_u16 / scale_divisor;

    }

    template<OrientationAxis axis>
    std::optional<float> Bno055_Driver::read_euler_orientation() {

        constexpr uint8_t target_adr = get_adr(axis);

        return read_float_value_from_short_adr_by_dividing_scale_factor<target_adr,EULER_DIV_DEG>();

    }

    template std::optional<float> Bno055_Driver::read_gravity_vector_axis_in_msq<Axis::X>();
    template std::optional<float> Bno055_Driver::read_gravity_vector_axis_in_msq<Axis::Y>();
    template std::optional<float> Bno055_Driver::read_gravity_vector_axis_in_msq<Axis::Z>();

    template std::optional<float> Bno055_Driver::read_euler_orientation<OrientationAxis::ROLL>();
    template std::optional<float> Bno055_Driver::read_euler_orientation<OrientationAxis::PITCH>();
    template std::optional<float> Bno055_Driver::read_euler_orientation<OrientationAxis::HEADING>();

} // kiv