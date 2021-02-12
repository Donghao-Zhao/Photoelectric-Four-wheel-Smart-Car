#ifndef __Calculate_H__
#define __Calculate_H__ 

extern double g_fGravityAngle ;

//float SpeedPID(int  collect_speed,int AmSpeed);
void AngleCalculate(void);
int MotorSpeedOut(float anglePWM ,float speedPWML,float speedPWMR,float dirPWM );
float AngleControl(void);
short SpeedPWMOut(int16_t NewspeedPWM ,int16_t LastspeedPWM,uint8_t PeriodCount);
short AAangPWMOut(int16_t NewAangPWM ,int16_t LastAangPWM,uint8_t PeriodCount);


extern float SpeedPIDL(int  collect_speed,int AmSpeed);
extern float SpeedPIDR(int  collect_speed,int AmSpeed);
#endif