//
// Created by Kivanc Gunalp on 12.06.2026.
//

#include "bno055.h"
#include "stdio.h"

#define check_success(x) if(x!= BNO055_SUCCESS){ printf("BNO055 ERROR: %s, File : %s, Line : %d\n", #x,__FILE__,__LINE__); return x; }

s8 Mock_BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
s8 Mock_BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt);
void Mock_BNO055_delay_msek(u32 msek);

void initialize_emulation_rom_data(void);


int main(int argc, char *argv[]) {


    struct bno055_t bno055 = {};
    bno055.dev_addr = BNO055_I2C_ADDR1;
    bno055.bus_read = Mock_BNO055_I2C_bus_read;
    bno055.bus_write = Mock_BNO055_I2C_bus_write;
    bno055.delay_msec = Mock_BNO055_delay_msek;

    initialize_emulation_rom_data();
    /*--------------------------------------------------------------------------*
  *  This API used to assign the value/reference of
  *  the following parameters
  *  I2C address,Bus Write,Bus read,Chip id,Page id;Accel,Mag,Gyro revision id
  *  Boot loader and Software revision id
  *-------------------------------------------------------------------------*/
    check_success(bno055_init(&bno055));

    /*  For initializing the BNO sensor it is required to the operation mode
   * of the sensor as NORMAL
   * Normal mode can set from the register
   * Page - page0, register - 0x3E, bit positions - 0 and 1*/
    check_success(bno055_set_power_mode(BNO055_POWER_MODE_NORMAL));
    check_success(bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF));

    double x, y, z;

    printf("Reading gravity vector\n");
    check_success(bno055_convert_gravity_double_x_msq(&x));
    check_success(bno055_convert_gravity_double_y_msq(&y));
    check_success(bno055_convert_gravity_double_z_msq(&z));

    printf("Gravity vector : (x: %.2f, y: %.2f, z: %.2f) m/sec2\n", x, y, z);


    //-------------------- orientation -------------//
    printf("Reading orientation vector\n");
    check_success(bno055_convert_double_euler_r_deg(&x));
    check_success(bno055_convert_double_euler_p_deg(&y));
    check_success(bno055_convert_double_euler_h_deg(&z));

    printf("Orientation vector : (x: %.2f, y: %.2f, z: %.2f)\n", x, y, z);

    printf("Powering down BNO055\n");
    check_success(bno055_set_power_mode(BNO055_POWER_MODE_SUSPEND));

    return 0;

}

u8 g_emulation_register_map_page0[256];
void initialize_emulation_rom_data(void) {

    g_emulation_register_map_page0[0] = 0xa0; // Chip ID
    g_emulation_register_map_page0[1] = 0xfb; // ACC ID
    g_emulation_register_map_page0[2] = 0x32; // MAG ID
    g_emulation_register_map_page0[3] = 0x0f; // Gyro ID

    g_emulation_register_map_page0[4] = 0x11; // SW Rev LSB
    g_emulation_register_map_page0[5] = 0x03; // SW Rev MSB

    //Gravity X
    g_emulation_register_map_page0[0x2f] = 1;
    g_emulation_register_map_page0[0x2e] = 20;


    //Gravity Y
    g_emulation_register_map_page0[0x31] = 1;
    g_emulation_register_map_page0[0x30] = 60;


    //Gravity Z
    g_emulation_register_map_page0[0x33] = 1;
    g_emulation_register_map_page0[0x32] = 100;


    //Orientation

    //Heading
    g_emulation_register_map_page0[0x1b] = 1;
    g_emulation_register_map_page0[0x1a] = 30;

    //Roll
    g_emulation_register_map_page0[0x1d] = 1;
    g_emulation_register_map_page0[0x1c] = 60;

    //Pitch
    g_emulation_register_map_page0[0x1f] = 1;
    g_emulation_register_map_page0[0x1e] = 90;




}

s8 Mock_BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {

    printf("BNO055 I2C bus write called. dev_addr : %x, reg_addr : %x, first data : %x, cnt : %d\n",dev_addr,reg_addr,reg_data[0],cnt);

    if (reg_addr == 7 && reg_data[0] != 0) {
        fprintf(stderr, "Emulation other than page zero not supported");
    }

    for (unsigned int i = 0; i < cnt; i++) {
        if ( reg_addr + i < 256) {
            g_emulation_register_map_page0[reg_addr + i] = reg_data[i];
        }else {
            fprintf(stderr, "Emulation ram out of bounds");
        }
    }

    return BNO055_SUCCESS;
}

s8 Mock_BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {

    printf("BNO055 I2C bus read called. dev_addr : %x, reg_addr : %x, first data : %x, cnt : %d\n",dev_addr,reg_addr,reg_data[0],cnt);

    for (unsigned int i = 0; i < cnt; i++) {
        if ( reg_addr + i < 256) {
             reg_data[i] = g_emulation_register_map_page0[reg_addr + i];
        }else {
            fprintf(stderr, "Emulation ram out of bounds");
        }
    }
    return BNO055_SUCCESS;
}

void Mock_BNO055_delay_msek(u32 msek){
    printf("BNO055_delay_msek called with %d ms\n", msek);
}
