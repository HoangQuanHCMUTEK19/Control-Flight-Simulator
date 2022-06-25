//NHOM: NGUYEN DINH NHAT-19146070
//      NGUYEN LE HOANG QUAN- 19146378
//      LAI TIEN QUANG  -19146376
// LOP 191462
//--------------CODE C------------------------//
#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPiI2C.h>
#include <stdint.h>

#define SMPRT_DIV_REGEGISTER            0x19    // Sample Rate Divider (register 25)
#define CONFIG_REGISTER                 0x1A    // Configuration (register 26)
#define GYRO_CONFIG_REGISTER            0x1B    // Gyroscope Configuration (register 27)
#define ACCEL_CONFIG_REGISTER           0x1C    // Accelerometer Configuration (register 28)
#define INT_ENABLE_REGISTER             0x38    // Interrupt Enable (register 56)
#define PWR_MGMT_1_REGISTER             0x6B    // Power Management 1 (register 107)
#define ACCEL_XOUT_REGISTER             0x3B    // ACCEL_XOUT_H (register 59)
#define ACCEL_YOUT_REGISTER             0x3D    // ACCEL_YOUT_H (register 61)
#define ACCEL_ZOUT_REGISTER             0x3F    // ACCEL_ZOUT_H (register 63)
#define GYRO_XOUT_REGISTER              0x43    // GYRO_XOUT_H (register 67)
#define GYRO_YOUT_REGISTER              0x45    // GYRO_YOUT_H (register 69)
#define GYRO_ZOUT_REGISTER              0x47    // GYRO_ZOUT_H (register 71)
#define INT_ENABLE_REGISTER             0x38    // Interrupt enable (register 56)
#define INT_STATUS_REGISTER             0x3A    // Interrupt Status (register 58)

//khai bao chan ngat cho nut nhan
#define PU 0 //pin 11
#define PD 1 //pin 12
#define PT 2 //pin 13
#define PP 3 //pin 15
#define F 4 //pin 16
#define SF 5 //pin 18
uint8_t mpu;
float ax, ay, az, roll, pitch;

int16_t MPU6050_ReadRegister(uint8_t register_address)
{
    int16_t high_value, low_value, data;

    high_value = wiringPiI2CReadReg8(mpu, register_address);
    low_value = wiringPiI2CReadReg8(mpu, register_address + 1);
    data = (high_value << 8) | low_value;

    return data;
}
void MPU6050_Init()
{
    // sample rate = 100 Hz
    wiringPiI2CWriteReg8(mpu, SMPRT_DIV_REGEGISTER, 0x09);    
    // khong su dung nguon xung ngoai, DLFP < 44 Hz
    wiringPiI2CWriteReg8(mpu, CONFIG_REGISTER, 0x03);
    // gyro FS: +- 500 o/s
    wiringPiI2CWriteReg8(mpu, GYRO_CONFIG_REGISTER, 0x08);
    // acc FS: +- 2g
    wiringPiI2CWriteReg8(mpu, ACCEL_CONFIG_REGISTER, 0x00);
    // enable data ready interrupt
    wiringPiI2CWriteReg8(mpu, INT_ENABLE_REGISTER, 0x01);
    // power management
    wiringPiI2CWriteReg8(mpu, PWR_MGMT_1_REGISTER, 0x00);
}
int dem =0;

void interrupt_Up()
{
    dem=6;
    
}
void interrupt_Do()
{
    dem=7;
    
}
void interrupt_PP()
{
    dem=8;
}
void interrupt_PT()
{
    dem=9;
}
void interrupt_F()
{
    dem=10;
}
void interrupt_SF()
{
    dem=11;
}
int main(void)
{
    int fd ;
    mpu=wiringPiI2CSetup(0x68);
    //Setup Uart
    fd = serialOpen ("/dev/ttyAMA0",9600);
    //Setup chan 
    wiringPiSetup();
    wiringPiISR(PD,INT_EDGE_RISING,&interrupt_Do);
    wiringPiISR(PU,INT_EDGE_RISING,&interrupt_Up);
    wiringPiISR(PT,INT_EDGE_RISING,&interrupt_PT);
    wiringPiISR(PP,INT_EDGE_RISING,&interrupt_PP);
    wiringPiISR(F,INT_EDGE_RISING,&interrupt_F);
    wiringPiISR(SF,INT_EDGE_RISING,&interrupt_SF);
    printf("Batdau...\n");
    delay(6000);
    MPU6050_Init();
    
    while(1)
    {
        ax = MPU6050_ReadRegister(ACCEL_XOUT_REGISTER)/8192;
        ay = MPU6050_ReadRegister(ACCEL_YOUT_REGISTER)/8192;
        az = MPU6050_ReadRegister(ACCEL_ZOUT_REGISTER)/8192;
        roll = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2)))*180/M_PI;
        pitch = atan2(ax, sqrt(pow(ay, 2) + pow(az, 2)))*180/M_PI;
        if (pitch > 20 && roll >= -10 && roll <= 10)  
        {
	    serialPrintf(fd,"1\n");
        }
        else if (pitch < -20 && roll >= -10 && roll <= 10)  
        {
	    serialPrintf(fd,"2\n");
        }
        else if (roll > 20 && pitch >= -10 && pitch <= 10) 
        {
	    serialPrintf(fd,"3\n");
        }
        else if (roll < -20 && pitch >= -10 && pitch <= 10) 
        {
	    serialPrintf(fd,"4\n");
        }
	else if (roll > -20 && roll < 20 && pitch >= -10 && pitch <= 10) 
        {
	    serialPrintf(fd,"5\n");
        }
	if(dem==6)
	    serialPrintf(fd,"6\n");
	else if (dem==7)
	    serialPrintf(fd,"7\n");
	else if (dem==8)
	    serialPrintf(fd,"8\n");
	else if (dem==9)
	    serialPrintf(fd,"9\n");
	else if (dem==10)
	    serialPrintf(fd,"10\n");
	else if (dem==11)
	    serialPrintf(fd,"11\n");
	//printf("%d\n",count);
	dem=0;
	delay(300);
        
    }
    serialClose(fd);
    return 0;
}

