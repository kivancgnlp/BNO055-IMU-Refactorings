//
// Created by Kivanc Gunalp on 12.06.2026.
//

#include <cstdio>
#include <iostream>
#include <ostream>

#include "Bno055_Driver.h"
#include "Bno055_I2C_Mock_Interface.h"

int main(int argc, char *argv[]) {

    using namespace kiv::embedded::drivers::imu;

    Bno055_I2C_Mock_Interface mock_interface;
    Bno055_Driver bno055_driver{mock_interface};

    bool ok{};

    ok = bno055_driver.init();

    if (!ok) {
        return -1;
    }
    ok = bno055_driver.set_power_mode(PowerMode::NORMAL);

    if (!ok) {
        return -1;
    }

    ok = bno055_driver.set_operation_mode(OperationMode::NDOF);

    if (!ok) {
        return -1;
    }


    if (auto gravity = bno055_driver.read_gravity_vector_in_msq()) {
        const auto [x,y,z] = gravity.value();
        //printf("Gravity vector : (x: %.2f, y: %.2f, z: %.2f) m/sec2\n", x, y, z);
        std::cout << std::format("Gravity vector : (x: {:.2f}, y: {:.2f}, z: {:.2f}) m/sec2\n",x,y,z);

    }

    if (auto gravity = bno055_driver.read_euler_orientation_vector_in_degrees()) {
        const auto [x,y,z] = gravity.value();
        printf("Orientation vector : (Heading: %.2f, Roll: %.2f, Pitch: %.2f) degree\n", x, y, z);

    }

    bno055_driver.set_power_mode(PowerMode::SUSPEND);

    auto constexpr nn = get_text_rep(OrientationAxis::HEADING);
    std::cout << nn  << std::endl;






    return ok;

}
