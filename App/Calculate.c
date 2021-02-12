#include "include.h"


double fGyroscopeAngleIntegral=0;
double g_fCarAngle ;
double g_fGyroscopeAngleSpeed ;
double g_fGravityAngle ;
#define GRAVITY_OFFSET   14500//,,,,,�������ٶ� ����//,,,,8060//,,,,,,2500
#define GYROSCOPE_OFFSET 0//,,,,������ ����///,,,,-140
#define GYROSCOPE_ANGLE_SIGMA_FREQUENCY	 100//200//15//8   //���������������ǽǶ� ������ Ƶ��
#define GRAVITY_ADJUST_TIME_CONSTANT	 2//1.2//0.05   //������������ ���� ʱ�䳣��
#define GYROSCOPE_ANGLE_RATIO	2.15//0.95              4r   //�������������� �Ƕ� ����////2.5
#define GRAVITY_ANGLE_RATIO            1//0.89//0.089  //�������������� �Ƕ� ����




extern short gyro_z;
void AngleCalculate(void)
{

 double   fDeltaValue=0;

	g_fGravityAngle = (Angle+100- GRAVITY_OFFSET) * GRAVITY_ANGLE_RATIO;  //���ٶ�
	g_fGyroscopeAngleSpeed = (gyro_z - GYROSCOPE_OFFSET ) * GYROSCOPE_ANGLE_RATIO;   //������

	g_fCarAngle = fGyroscopeAngleIntegral;//�������������ǽǶȷ�Χ����
	fDeltaValue =(g_fGravityAngle - g_fCarAngle) / GRAVITY_ADJUST_TIME_CONSTANT;      //������


	fGyroscopeAngleIntegral +=(g_fGyroscopeAngleSpeed + fDeltaValue) / GYROSCOPE_ANGLE_SIGMA_FREQUENCY;//�������������ǽǶȷ�Χ����

}

float g_fAngleControlOut;
float AngleControl(void) {

        double fValue=0;
        float   ANGLE_CONTROL_P ;
        float   ANGLE_CONTROL_D ;



            ANGLE_CONTROL_P =-2.5;//2.2/-2.7//,,,,-1.5
            ANGLE_CONTROL_D =-0.7;//,,,-0.08;//0.5//,,,-0.12-0.5


            fValue = (0 - g_fCarAngle) * ANGLE_CONTROL_P +                     //���ٶȿ���ϵ��
	      (0 - g_fGyroscopeAngleSpeed) * ANGLE_CONTROL_D;//�Ƕȿ���ϵ��


	if(fValue > 4000)      fValue = 4000;
	else if(fValue < -4000) fValue =- 4000;
	g_fAngleControlOut = fValue;



        return g_fAngleControlOut ;

}

   float  SpeedPWMKL=0,SpeedPWMKR=0;
   float  SpeedPWMKPL ,SpeedPWMKIR,SpeedPWMKPR ,SpeedPWMKIL ;
   float  SpeedDifferenceL=0,SpeedDifferenceR=0;
   float  new_speedL,new_speedR;
   float lastspeeddifferenceL;
float SpeedPIDL(int collect_speed,int AmSpeed)
 {






     float  SpeedKPL =25;//1.5//0.4;//0.2;//0.05;//0.1;//0.005;//,,,,,40////////1.2////////////10
     float  SpeedKIL =0.18;//0.25;//0.004// 0.5;//;//////////1.7//////////0.05����һ��//////0.03������/////0.04

   ////////10������������0.03����



     SpeedDifferenceL = AmSpeed - collect_speed  ;


     /////////////////-----P------/////////////////////

          SpeedPWMKPL = SpeedKPL*SpeedDifferenceL;
 
     //////////////////------I------////////////////////////


          SpeedPWMKIL = SpeedKIL* SpeedDifferenceL;

          SpeedPWMKL += SpeedPWMKIL ;
          if(SpeedPWMKL>1500)
            SpeedPWMKL=1500;
          if(SpeedPWMKL<-1500)
            SpeedPWMKL=-1500;


   ///////////////////////////////////////////////////

          //new_speedL=SpeedPWMKPL+SpeedPWMKL;/////////λ��ʽƫ��
         new_speedL+=SpeedKPL*(SpeedDifferenceL-lastspeeddifferenceL)+SpeedPWMKIL;///////////����ʽ

         lastspeeddifferenceL=SpeedDifferenceL;
         /////////////////////////////////////
          if(new_speedL>2500)
            new_speedL=2500;
         if(new_speedL<-2500)
            new_speedL=-2500;


          return new_speedL;
          
}

float SpeedPIDR(int  collect_speed,int AmSpeed)
{






     float  SpeedKPR =27;//1.5//0.4;//0.2;//0.05;//0.1;//0.005;//,,,,,40////////1.2
     float  SpeedKIR =0.2;//0.25;//0.004// 0.5;//0.1;//////////1.7

     static float lastspeeddifferenceR;



     SpeedDifferenceR = AmSpeed - collect_speed  ;


     /////////////////-----P------/////////////////////

          SpeedPWMKPR = SpeedKPR*SpeedDifferenceR;

     //////////////////------I------////////////////////////



          SpeedPWMKIR = SpeedKIR* SpeedDifferenceR;

          ////SpeedPWMK += SpeedPWMKI ;
          if(SpeedPWMKR>1500)
            SpeedPWMKR=1500;
          if(SpeedPWMKR<-1500)
            SpeedPWMKR=-1500;


   ///////////////////////////////////////////////////

           //new_speedR=SpeedPWMKPR+SpeedPWMKR;/////////λ��ʽƫ��
         new_speedR+=SpeedKPR*(SpeedDifferenceR-lastspeeddifferenceR)+SpeedPWMKIR;///////////����ʽ

         lastspeeddifferenceR=SpeedDifferenceR;
         /////////////////////////////////////
          if(new_speedR>2500)
            new_speedR=2500;
         if(new_speedR<-2500)
            new_speedR=-2500;


          return new_speedR;

}

int Speed_L=0;
int Speed_R=0;

int MotorSpeedOut(float anglePWM ,float speedPWML,float speedPWMR,float dirPWM )
 {
   // int L_last,R_last;
   int deadpwml=200;
   int deadpwmr=200;

  
  if(anglePWM<0)        
  {     
    deadpwml=-deadpwml;
    deadpwmr=-deadpwmr;
  }
  Speed_L =(int)(anglePWM-speedPWML-dirPWM +deadpwml) ;
  Speed_R = (int)(anglePWM-speedPWMR+dirPWM+deadpwmr) ;
  int limit=5000;

 if(Speed_L > limit) Speed_L = limit;
 if(Speed_L < -limit) Speed_L = -limit;
 if(Speed_R > limit) Speed_R = limit;
 if(Speed_R < -limit) Speed_R = -limit;


   if(Speed_L > 0) ////////����ǰ��
    {
       ftm_pwm_duty(FTM2, FTM_CH2, 0);/////////���
       ftm_pwm_duty(FTM2, FTM_CH3, Speed_L);//////��ǰ
    }
     else
    {
       ftm_pwm_duty(FTM2, FTM_CH2, -Speed_L);
       ftm_pwm_duty(FTM2, FTM_CH3, 0);
    }



     if(Speed_R > 0)
     {
       ftm_pwm_duty(FTM2, FTM_CH0,Speed_R);///////�Һ�
       ftm_pwm_duty(FTM2, FTM_CH1, 0);       ///////��ǰ
     }
     else
     {
       ftm_pwm_duty(FTM2, FTM_CH0,0);
       ftm_pwm_duty(FTM2, FTM_CH1, -Speed_R);
     }

    return ((Speed_L +Speed_R)/2);
 }

/***********************************************************
�������ƣ�SpeedPWMOut

***********************************************************/
#define SPEEDPERIODFAV  100
short SpeedPWMOut(short NewspeedPWM ,short LastspeedPWM,uint8_t PeriodCount)
{
    int16_t  speedPWMfav ;
    int16_t  SpeedOUT ;
    speedPWMfav = NewspeedPWM - LastspeedPWM ;
    SpeedOUT = speedPWMfav *(PeriodCount)/SPEEDPERIODFAV + LastspeedPWM ;

    return SpeedOUT ;

}

/***********************************************************
�������ƣ�AAangPWMOut

***********************************************************/
#define AANGPERIODFAV  (5)
short AAangPWMOut(short NewAangPWM ,short LastAangPWM,uint8_t PeriodCount)
{
    int16_t  AangPWMfav ;
    int16_t  AangOUT ;
    AangPWMfav = NewAangPWM - LastAangPWM ;
    AangOUT = AangPWMfav *(PeriodCount)/AANGPERIODFAV + LastAangPWM ;

    return AangOUT ;

}
