//
// Created by Kivanc Gunalp on 12.06.2026.
//

#ifndef PROJECT_BNO055_H
#define PROJECT_BNO055_H
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
        /* Power mode*/
        X,
        Y,
        Z
    };


    class Bno055_Driver {

    private:
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


        static constexpr uint8_t I2C_ADDR = 0x28;

        static constexpr float GRAVITY_DIV_MSQ = 100.0;

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

        bool get_operation_mode(OperationMode &op_mode);

        bool set_power_mode(PowerMode pm);


        bool assure_page_id(uint8_t page_id);

        bool set_operation_mode(OperationMode op_mode);

        bool init();


        std::optional<int16_t> read_gravity_raw(Axis axis) {

            bool ok = assure_page_id(0);
            if (!ok) return std::nullopt;

            uint16_t data_u16{};

            uint8_t target_adr;

            switch (axis) {
                case Axis::X:
                    target_adr = GRAVITY_DATA_X_LSB_ADDR;
                    break;
                case Axis::Y:
                    target_adr = GRAVITY_DATA_Y_LSB_ADDR;
                    break;
                case Axis::Z:
                    target_adr = GRAVITY_DATA_Z_LSB_ADDR;
                    break;
            }

            ok  = m_i2c_interface.i2c_bus_read(I2C_ADDR,target_adr, reinterpret_cast<uint8_t *>(&data_u16),2); // TODO : It is ok for little endian

            if (!ok) return std::nullopt;

            return {data_u16};
        }

        std::optional<float> read_gravity_vector_axis_in_msq(Axis axis){


            /* Read raw gravity of x*/
            auto gravity_val = read_gravity_raw(axis);

            if (gravity_val) {
                return {gravity_val.value() / GRAVITY_DIV_MSQ};
            }

           return std::nullopt;

        }


        std::optional<std::tuple<float,float,float>> read_gravity_vector_in_msq(){

            float gravity_val_float[3];
            bool ok = false;

            for (int i = 0; i < 3; i++) {
                auto gravity_val = read_gravity_raw(static_cast<Axis>(i));

                if (gravity_val) {
                    gravity_val_float[i] = gravity_val.value() / GRAVITY_DIV_MSQ;
                }else {
                    return std::nullopt;
                }

            }

            return std::make_tuple(gravity_val_float[0], gravity_val_float[1], gravity_val_float[2]);

        }

    protected:

    };



} // kiv

#endif //PROJECT_BNO055_H