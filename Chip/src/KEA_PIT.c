/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_pit.c
 * @brief      pit��ʱ��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-23
 */
#include "common.h"
#include  "KEA_PIT.h"     //�����жϼ�ʱ��
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
unsigned short ADC_value[5];//////����ԭ����8
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
        LED_PrintValueI(30, 0,ADC_value[0]);//////////���ڼ��ϲ��뿪��
        LED_PrintValueI(30, 1,ADC_value[1]);
        LED_PrintValueI(30, 2,ADC_value[2]);
        LED_PrintValueI(30, 3,ADC_value[3]);
        LED_PrintValueI(30, 4,ADC_value[4]);
        //LED_PrintValueI(30, 5,ADC_value[5]);
       }
        
        
        else
       {
        LED_PrintValueI(30, 0,guiyivalue[0]);//////////���ڼ��ϲ��뿪��
        LED_PrintValueI(30, 1,guiyivalue[1]);
        LED_PrintValueI(30, 2,guiyivalue[2]);
        LED_PrintValueI(30, 3,guiyivalue[3]);
        //Test_2100_8700();
        LED_PrintValueI(30, 4,guiyivalue[4]);////////pp��mm����ת��
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
 /////////�ٶȿ���
  
  
  // dir_pwm=dir_p05*(ADC_value[0]-ADC_value[5]-diferent_05);
   //Data_Send(UART1,pp);///////��λ��


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
     // angle_count++;//�������ǶȲɼ�����
      Read_2100_8700(FX_2100_ADDR,bit8_data,bit16_data16);//������2100����16λ����

      gyro_z_h=bit8_data[2];
      gyro_z_l=bit8_data[3];

      gyro_z=((unsigned short)gyro_z_h<<8)|gyro_z_l;////,,,,,,���16λ����
      


      Read_2100_8700(FXOS_8700_ADDR,bit8_data,bit16_data16);//���ٶȼ�8700��14λ���ȣ�����������16λ���ȣ�

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
      PIT_Flag_Clear(PIT1);       //���жϱ�־λ
}
/*!
 *  @brief      PITn��ʱ�ж�
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @param      cnt         ��ʱ�ж�ʱ��(��λΪbusʱ������)
 *  @since      v5.0
 *  Sample usage:
                pit_init(PIT0, 1000);                          //��ʱ 1000 ��busʱ�� ���ж�
                set_vector_handler(PIT0_VECTORn,pit_hander);   // �����жϷ��������ж���������
                enable_irq(PIT0_IRQn);                         // ʹ��PIT�ж�
 */
void pit_init(PITn_e pitn, uint32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    ASSERT( cnt > 0 );                          //�ö��Լ�� ʱ����벻��Ϊ 0

    SIM_SCGC       |= SIM_SCGC_PIT_MASK;          //ʹ��PITʱ��

    PIT_MCR         = (0
                       //| PIT_MCR_MDIS_MASK       //����PIT��ʱ��ʱ��ѡ��0��ʾʹ��PIT��1��ʾ��ֹPIT��
                       //| PIT_MCR_FRZ_MASK        //����ģʽ��ֹͣ���У�0��ʾ�������У�1��ʾֹͣ���У�
                      );

    PIT_LDVAL(pitn)  = cnt - 1 ;                    //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
                         | PIT_TCTRL_TIE_MASK        //��PITn�ж�
                       );

    //enable_irq((int)pitn + PIT_CH0_IRQn);            //���ж�
}

/*!
 *  @brief      PITn��ʱ
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @param      cnt         ��ʱʱ��(��λΪbusʱ������)
 *  @since      v5.0
 *  Sample usage:
                    pit_delay(PIT0, 1000);                         //��ʱ 1000 ��busʱ��
 */
void pit_delay(PITn_e pitn, uint32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    ASSERT( cnt > 0 );              //�ö��Լ�� ʱ����벻��Ϊ 0

    SIM_SCGC       |= SIM_SCGC_PIT_MASK;          //ʹ��PITʱ��

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //����PIT ���Ա����ü���ֵ��Ч

    PIT_LDVAL(pitn)  = cnt - 1;                                     //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //ʹ�� PITn��ʱ��
                         //| PIT_TCTRL_TIE_MASK                      //��PITn�ж�
                       );

    while( !(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK));

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ
}

/*!
 *  @brief      PITn��ʱ��ʼ
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @since      v5.0
 *  Sample usage:
                    pit_time_start(PIT0);                          //PIT0��ʱ��ʼ
 */
void pit_time_start(PITn_e pitn)
{
    //PIT �õ��� Bus Clock ����Ƶ��
    //������� = ����Ƶ�� * ʱ��

    SIM_SCGC       |= SIM_SCGC_PIT_MASK;          //ʹ��PITʱ��

    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //����PIT ���Ա����ü���ֵ��Ч

    PIT_LDVAL(pitn)  = ~0;                                          //��������ж�ʱ��

    PIT_Flag_Clear(pitn);                                           //���жϱ�־λ

    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_TCTRL(pitn)  = ( 0
                         | PIT_TCTRL_TEN_MASK                        //ʹ�� PITn��ʱ��
                         //| PIT_TCTRL_TIE_MASK                      //��PITn�ж�
                       );
}

/*!
 *  @brief      ��ȡ PITn��ʱʱ��(��ʱʱ��ر� ��ʱ��)
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @since      v5.0
 *  Sample usage:
                        uint32 time = pit_time_get(PIT0);                         //��ȡ PITn��ʱʱ��
                        if(time != ~0)       //û��ʱ
                        {
                            printf("\n��ʱʱ��Ϊ��%d us",time*1000/bus_clk_khz);
                        }
 */
uint32 pit_time_get(PITn_e pitn)
{
    uint32 val;

    val = (~0) - PIT_CVAL(pitn);

    if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //�ж��Ƿ�ʱ�䳬ʱ
    {
        PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
        PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
        return ~0;
    }

    if(val == (~0))
    {
        val--;              //ȷ�� ������ ~0
    }
    return val;
}

/*!
 *  @brief      �ر� pit
 *  @param      PITn        ģ��ţ�PIT0~PIT3��
 *  @since      v5.0
 *  Sample usage:
                        pit_close(PIT0);                         //�ر�PIT
 */
void pit_close(PITn_e pitn)
{
    PIT_Flag_Clear(pitn);                                       //���жϱ�־λ
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //��ֹPITn��ʱ����������ռ���ֵ��
}