//
// Created by Kivanc Gunalp on 12.06.2026.
//

#ifndef PROJECT_BNO055_H
#define PROJECT_BNO055_H
#include <cassert>
#include <cstdint>


#include "Bno055_I2C_Interface.h"
#include "Bno055_I2C_Mock_Interface.h"
#include <tuple>
#include <optional>

namespace kiv::embedded::drivers::imu {

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

    enum class Axis : uint8_t {
        X,
        Y,
        Z
    };

    enum class OrientationAxis : uint8_t {
        HEADING,
        ROLL,
        PITCH
    };

    /* PAGE0 REGISTER DEFINITION START*/
    static constexpr uint8_t CHIP_ID_ADDR = 0;
    static constexpr uint8_t ACCEL_REV_ID_ADDR  = 1;
    static constexpr uint8_t MAG_REV_ID_ADDR  = 2;
    static constexpr uint8_t GYR_REV_ID_ADDR  = 3;
    static constexpr uint8_t SW_REV_ID_LSB_ADDR  = 4;
    static constexpr uint8_t SW_REV_ID_MSB_ADDR  = 5;
    static constexpr uint8_t PAGE_ID_ADDR = 7;
    static constexpr uint8_t OPR_MODE_ADDR = 0X3D;
    static constexpr uint8_t PWR_MODE_ADDR = 0X3E;

    /*Gravity data registers*/
    static constexpr uint8_t GRAVITY_DATA_X_LSB_ADDR = 0X2E;
    static constexpr uint8_t GRAVITY_DATA_Y_LSB_ADDR = 0X30;
    static constexpr uint8_t GRAVITY_DATA_Z_LSB_ADDR = 0X32;

    /*Euler data registers*/
    static constexpr uint8_t EULER_HEADING_LSB_ADDR = 0X1A;
    static constexpr uint8_t EULER_ROLL_LSB_ADDR = 0X1C;
    static constexpr uint8_t EULER_PITCH_LSB_ADDR = 0X1E;


    constexpr uint8_t get_adr(OrientationAxis item) {
        switch (item) {
            case OrientationAxis::HEADING: return EULER_HEADING_LSB_ADDR;
            case OrientationAxis::ROLL: return EULER_ROLL_LSB_ADDR;
            case OrientationAxis::PITCH: return EULER_PITCH_LSB_ADDR;
        }

        assert(false);
    }


    class Bno055_Driver {

    private:


        static constexpr uint8_t I2C_ADDR = 0x28;

        static constexpr float GRAVITY_DIV_MSQ = 100.0;
        static constexpr float EULER_DIV_DEG  = 16.0;

        //
        uint8_t m_chip_id{};
        uint8_t m_accel_rev_id{};
        uint8_t m_mag_rev_id{};
        uint8_t m_gyro_rev_id{};
        uint8_t m_page_id{};

        Bno055_I2C_Interface &m_i2c_interface;

        public:
        Bno055_Driver(Bno055_I2C_Interface &i2c_interface) : m_i2c_interface(i2c_interface) {
            //ctor
        }

        virtual ~Bno055_Driver() = default;

        bool init();
        bool assure_page_id(uint8_t page_id);

        bool set_power_mode(PowerMode pm);
        bool set_operation_mode(OperationMode op_mode);
        bool get_operation_mode(OperationMode &op_mode);


        template <Axis axis>
        std::optional<float> read_gravity_vector_axis_in_msq();

        std::optional<std::tuple<float,float,float>> read_gravity_vector_in_msq();


        template <OrientationAxis axis>
        std::optional<float> read_euler_orientation();

        std::optional<std::tuple<float,float,float>> read_euler_orientation_vector_in_degrees();

        private:
        template <uint8_t ADR, float scale_divisor>
        std::optional<float> read_float_value_from_short_adr_by_dividing_scale_factor();

    };



} // kiv

#endif //PROJECT_BNO055_H