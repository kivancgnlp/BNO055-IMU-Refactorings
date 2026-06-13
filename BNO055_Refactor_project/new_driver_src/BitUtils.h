//
// Created by Kivanc Gunalp on 13.06.2026.
//

#ifndef BNO055_REFACTOR_PROJECT_BITUTILS_H
#define BNO055_REFACTOR_PROJECT_BITUTILS_H
#include <cstdint>
#include <cassert>

template <uint8_t start_bit,uint8_t bit_count>
consteval uint8_t get_bit_mask() {
    static_assert(bit_count >= 1, "bit_count must be at least 1");
    static_assert(start_bit < 8, "start_bit must be in [0, 7]");
    static_assert(start_bit + bit_count <= 8, "mask must fit within 8 bits (start_bit + bit_count <= 8)");

    return static_cast<uint8_t>(((1u << bit_count) - 1u) << start_bit);

}

template <uint8_t start_bit, uint8_t bit_count>
constexpr void update_bit_field(uint8_t &original_value, uint8_t bit_field_to_embed) {
    constexpr uint8_t mask = get_bit_mask<start_bit, bit_count>();
    assert((bit_field_to_embed >> bit_count) == 0); //"field to embed exceeds it's size, possible bug"
    const uint8_t shifted = static_cast<uint8_t>((bit_field_to_embed << start_bit) & mask);
    original_value = static_cast<uint8_t>((original_value & ~mask) | shifted);
}



#endif //BNO055_REFACTOR_PROJECT_BITUTILS_H
