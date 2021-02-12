/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_pit.c
 * @brief      pit定时器函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-23
 */
#include "common.h"
#include  "KEA_PIT.h"     //周期中断计时器
#include "Calculate.h"
#include "Serial_oscilloscope.h"
#include "KEA_ftm.h"
#include "VCAN_computer.h"

extern double g_fGyroscopeAngleSpeed;
extern double g_fGravityAngle;

//unsigned short gyro_zzz;
extern short Angle;
extern short gyro_z;


uint16 ddd[8];
unsigned short ADC_value[5];//////这里原来是8
uint8 bit8_data[6];
short bit16_data16[3];
short diferent_05,diferent_14,diferent_23;
uint16 speed_count;
uint16 angle_count;
int16 guiyivalue[5];
//short Speed_collcet;
short Angle_PWM,Speed_PWML,Speed_PWMR;
short speed_l,speed_r;

short last_speed_PWML,last_speed_PWMR;
short last_angle_PWM;
float dir_p05=0.9,dir_p14=0.8,dir_p23=0,dir_pwm,dir_D=0.05;
uint16 dir_a,dir_b;
uint16 pp[3];
extern int Speed_L;
extern int Speed_R;
char ccount=0;
int countL=0,countR=0;
char circle=0;

void getvalue()
{  
   ADC_value[0]=ADC_Ave(ADC0_SE0,ADC_12bit,10);
   ADC_value[1]=ADC_Ave(ADC0_SE1,ADC_12bit,10);
   ADC_value[2]=ADC_Ave(ADC0_SE2,ADC_12bit,10);
   ADC_value[3]=ADC_Ave(ADC0_SE3,ADC_12bit,10);
   ADC_value[4]=ADC_Ave(ADC0_SE4,ADC_12bit,10);
   //ADC_value[5]=ADC_Ave(ADC0_SE5,ADC_12bit,10);
   
   
   if(ADC_value[2]>(ADC_value[1]*1.5)&&ADC_value[2]>(ADC_value[3]*1.5))
      circle=1;
   guiyi(ADC_value,guiyivalue);
   dir_pwm=directions(guiyivalue);
}




void pit_ch0_irq()
{

  


     countL = ftm_pulse_get(FTM0);
     countR = ftm_pulse_get(FTM1);
     //ftm_pulse_clean(FTM0) ;
     pp[0]=countL;
     //ftm_pulse_clean(FTM1);
     pp[1]=countR;
     Speed_PWML=(short)SpeedPIDL(countL,20);
     Speed_PWMR=(short)SpeedPIDR(countR,20);
     countL=0;   
     countR=0;   
     getvalue();
     if(gpio_get(PTG5))
      {
        LED_PrintValueI(30, 0,ADC_value[0]);//////////后期加上拨码开关
        LED_PrintValueI(30, 1,ADC_value[1]);
        LED_PrintValueI(30, 2,ADC_value[2]);
        LED_PrintValueI(30, 3,ADC_value[3]);
        LED_PrintValueI(30, 4,ADC_value[4]);
        //LED_PrintValueI(30, 5,ADC_value[5]);
       }
        
        
        else
       {
        LED_PrintValueI(30, 0,guiyivalue[0]);//////////后期加上拨码开关
        LED_PrintValueI(30, 1,guiyivalue[1]);
        LED_PrintValueI(30, 2,guiyivalue[2]);
        LED_PrintValueI(30, 3,guiyivalue[3]);
        //Test_2100_8700();
        LED_PrintValueI(30, 4,guiyivalue[4]);////////pp，mm左右转速
        LED_PrintValueI(30, 5,Angle); 
        LED_PrintValueI(30, 6,gyro_z);
       //  LED_PrintValueI(30, 5,pp[0]);
        //LED_PrintValueI(30, 6,pp[1]);
        
       
      
        pp[2]=60;
        Data_Send(UART2,pp);
       } 
     PIT_Flag_Clear(PIT0);   
     ftm_pulse_clean(FTM0);  
     ftm_pulse_clean(FTM1); 

   }
//   ddd[6]=ADC_Ave(ADC1,ADC1_SE16,ADC_12bit,10);//ADC1_SE16
//   ddd[7]=ADC_Ave(ADC0,ADC0_SE16,ADC_12bit,10);//ADC0_SE16
  // Read_2100_8700(FX_2100_ADDR,bit8_data,bit16_data16);
   
  //dir_pwm=dir_p05*(ADC_value[0]-ADC_value[5]-diferent_05)+dir_p14*(ADC_value[1]-ADC_value[4])+dir_p23*(ADC_value[2]-ADC_value[3]-diferent_23)-dir_D*(bit16_data16[0]-50);
 /////////速度控制
  
  
  // dir_pwm=dir_p05*(ADC_value[0]-ADC_value[5]-diferent_05);
   //Data_Send(UART1,pp);///////上位机


short gyro_z,accle_z;
uint8 gyro_z_h,gyro_z_l,accle_z_h,accle_z_l;

short real_pwm;
extern double g_fCarAngle;
extern double g_fGyroscopeAngleSpeed ;
short gyro_z,accle_z;
uint8 gyro_z_h,gyro_z_l,accle_z_h,accle_z_l;

short real_pwm;
extern double g_fCarAngle;
extern double g_fGyroscopeAngleSpeed ;





void pit_ch1_irq()
{
  

      
 
      //getvalue();
     // speed_count++;
     // angle_count++;//，，，角度采集次数
      Read_2100_8700(FX_2100_ADDR,bit8_data,bit16_data16);//陀螺仪2100数据16位精度

      gyro_z_h=bit8_data[2];
      gyro_z_l=bit8_data[3];

      gyro_z=((unsigned short)gyro_z_h<<8)|gyro_z_l;////,,,,,,变成16位精度
      


      Read_2100_8700(FXOS_8700_ADDR,bit8_data,bit16_data16);//加速度计8700，14位精度？？？？？？16位精度？

      accle_z_h=bit8_data[4];
      accle_z_l=bit8_data[5];
      accle_z=((unsigned short)accle_z_h<<8)|accle_z_l;
      accle_z=accle_z;

      Kalman_Filter(gyro_z,accle_z);
      AngleCalculate();
      Angle_PWM=(short)AngleControl();
      
      //last_angle_PWM=AAangPWMOut(Angle_PWM,last_angle_PWM,angle_count);
      //angle_count=0;
    //  real_pwm= MotorSpeedOut(-Angle_PWM ,last_speed_PWM,0);
    MotorSpeedOut(-Angle_PWM ,Speed_PWML,Speed_PWMR,dir_pwm);
    //getvalue();
    /*  ddd[0]=gyro_z;
      ddd[1]=accle_z;
      ddd[2]=Angle;
      ddd[3]=(int)g_fCarAngle;
      ddd[4]=(int)g_fGravityAngle;
      ddd[5]=(int)g_fGyroscopeAngleSpeed ;
      ddd[6]=pp[0];
      ddd[7]=pp[1];*/
      //uart_putchar (UART2,0x90);
      //vcan_sendware((uint8_t *)ddd, sizeof(ddd));
      //Data_Send(UART2,ddd);
      PIT_Flag_Clear(PIT1);       //清中断标志位
}
/*!
 *  @brief      PITn定时中断
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @param      cnt         定时中断时间(单位为bus时钟周期)
 *  @since      v5.0
 *  Sample usage:
                pit_init(PIT0, 1000);                          //定时 1000 个bus时钟 后中断
                set_vector_handler(PIT0_VECTORn,pit_hander);   // 设置中断服务函数到中断向量表里
                enable_irq(PIT0_IRQn);                         // 使能PIT中断
 */
void pit_init(PITn_e pitn, uint32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    ASSERT( cnt > 0 );                          //用断言检测 时间必须不能为 0

    SIM_SCGC       |= SIM_SCGC_PIT_MASK;          //使能PIT时钟

    PIT_MCR         = (0
                       //| PIT_MCR_MDIS_MASK       //禁用PIT定时器时钟选择（0表示使能PIT，1表示禁止PIT）
                       //| PIT_MCR_FRZ_MASK        //调试模式下停止运行（0表示继续运行，1表示停止运行）
                      );

    PIT_LDVAL(pitn)  = cnt - 1 ;                    //设置溢出中断时间

    PIT_Flag_Clear(pitn);                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //使能 PITn定时器
                         | PIT_TCTRL_TIE_MASK        //开PITn中断
                       );

    //enable_irq((int)pitn + PIT_CH0_IRQn);            //开中断
}

/*!
 *  @brief      PITn延时
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @param      cnt         延时时间(单位为bus时钟周期)
 *  @since      v5.0
 *  Sample usage:
                    pit_delay(PIT0, 1000);                         //延时 1000 个bus时钟
 */
void pit_delay(PITn_e pitn, uint32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    ASSERT( cnt > 0 );              //用断言检测 时间必须不能为 0

    SIM_SCGC       |= SIM_SCGC_PIT_MASK;          //使能PIT时钟

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效

    PIT_LDVAL(pitn)  = cnt - 1;                                     //设置溢出中断时间

    PIT_Flag_Clear(pitn);                                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                         //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                       );

    while( !(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK));

    PIT_Flag_Clear(pitn);                                           //清中断标志位
}

/*!
 *  @brief      PITn计时开始
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                    pit_time_start(PIT0);                          //PIT0计时开始
 */
void pit_time_start(PITn_e pitn)
{
    //PIT 用的是 Bus Clock 总线频率
    //溢出计数 = 总线频率 * 时间

    SIM_SCGC       |= SIM_SCGC_PIT_MASK;          //使能PIT时钟

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效

    PIT_LDVAL(pitn)  = ~0;                                          //设置溢出中断时间

    PIT_Flag_Clear(pitn);                                           //清中断标志位

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                         //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                       );
}

/*!
 *  @brief      获取 PITn计时时间(超时时会关闭 定时器)
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                        uint32 time = pit_time_get(PIT0);                         //获取 PITn计时时间
                        if(time != ~0)       //没超时
                        {
                            printf("\n计时时间为：%d us",time*1000/bus_clk_khz);
                        }
 */
uint32 pit_time_get(PITn_e pitn)
{
    uint32 val;

    val = (~0) - PIT_CVAL(pitn);

    if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //判断是否时间超时
    {
        PIT_Flag_Clear(pitn);                                       //清中断标志位
        PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
        return ~0;
    }

    if(val == (~0))
    {
        val--;              //确保 不等于 ~0
    }
    return val;
}

/*!
 *  @brief      关闭 pit
 *  @param      PITn        模块号（PIT0~PIT3）
 *  @since      v5.0
 *  Sample usage:
                        pit_close(PIT0);                         //关闭PIT
 */
void pit_close(PITn_e pitn)
{
    PIT_Flag_Clear(pitn);                                       //清中断标志位
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
}