//
// Created by Kivanc Gunalp on 13.06.2026.
//
#include <iostream>
#include <ostream>
#include <cassert>

#include "../new_driver_src/BitUtils.h"

int main(int argc, char *argv[]) {

    constexpr uint8_t ne = get_bit_mask<0,3>();
    std::cout << (unsigned)ne << std::endl;
    assert(ne == 7);

    uint8_t some_value = 0x55;
    update_bit_field<0,3>(some_value,7);
    std::cout << std::hex <<(unsigned)some_value << std::endl;
    assert(some_value == 0x57);

    update_bit_field<6,1>(some_value,0);
    assert(some_value == 0x17);
    std::cout << std::hex <<(unsigned)some_value << std::endl;

    some_value = get_bit_field<0,3>(some_value);
    assert(some_value == 0x07);
    std::cout << std::hex <<(unsigned)some_value << std::endl;

}
