
#include "common.h"
#include "include.h"
  
/*!   
 *  @brief      main函数   
 *  @since      v6.0    
 *  @note       山外 PIT 计时实验   
 */    


void main()
{
  
char ad0[]="L1:";
char ad1[]="L:";
char ad2[]="M:";
char ad3[]="R:";
char ad4[]="R1:";
//char ad5[]="A:";
//char ad6[]="G:";
//char ad7[]="R1:";



   DisableInterrupts;
   uart_init (UART2,115200);      ////用上位机时打开

   
   adc_init(ADC0_SE0);
   adc_init(ADC0_SE1);
   adc_init(ADC0_SE2);
   adc_init(ADC0_SE3);
   adc_init(ADC0_SE4);
   //adc_init(ADC0_SE5);
 
   gpio_init(PTG4,GPI,0);/////////////
   gpio_init(PTG5,GPI,0);
   //MPU_IIC_Init();    /////tly
   
   
    //gpio_init(PTG0,GPO,1);        ////输出模式                       
   // gpio_init(PTG1,GPO,1);                         
    //gpio_init(PTG2,GPO,1);                         
    //gpio_init(PTG3,GPO,1);                         

    
    OLED_Init();
    LED_CLS();                   ////oled清屏
    LED_P6x8Str(0,0,ad0);
    LED_P6x8Str(0,1,ad1);
    LED_P6x8Str(0,2,ad2);
    LED_P6x8Str(0,3,ad3);
    LED_P6x8Str(0,4,ad4);
    //LED_P6x8Str(0,5,ad5);
    //LED_P6x8Str(0,6,ad6);
    //LED_P6x8Str(0,7,ad7);
    
   

   ftm_pwm_init(FTM2,FTM_CH2,7300,0);//左后   //输出频率为100，占空比为 50/1000  
   ftm_pwm_init(FTM2,FTM_CH3,7300,0);//左前
   ftm_pwm_init(FTM2,FTM_CH0,7300,0);//右后
   ftm_pwm_init(FTM2,FTM_CH1,7300,0);//


   //gpio_init(PTI2,GPI,0);                      //初始化核心板独立按键，内部上拉电阻    
   //gpio_init(PTI3,GPI,0);                      ////正反转测试
  //port_pull(PTI2,PULLUP_ENBLE);                    
   //port_pull(PTI3,PULLUP_ENBLE);      

    
   ftm_pulse_init(FTM0, FTM_PS_1, PTE7); ///FTM2来脉冲计数。不分频，输入引脚为 TCLK1
   ftm_pulse_init(FTM1, FTM_PS_1, PTE0); /////
   
   
   
   //loadflash();
   
   
   
   ///////ftm_port_mux(FTM0,FTM_CH0);
   ///////ftm_port_mux(FTM1,FTM_CH0);
   
   
    //printf("\n*****PWT 脉冲计数 测试*****\n");

   /////ftm_pwm_init(FTM1, FTM_CH0, s, 60.0);    ////输出频率为100，占空比为 0/FTM0_PRECISON 。引脚为:FTM0_CH0

   //////ftm_pwm_init(FTM1, FTM_CH1, s, 60.0);    ///////FTM0_PRECISON 配置为60
   
   //pwt_pulse_init(PWT_IN0, PWT_PS_CFG(PWT_PS_1) | Pulse_Width_Enable_irq | Pulse_Over_Enable_irq | Cycle_Rising );   //PWT_IN0_PIN 脉冲输入  ,默认分频系数为1//////Cycle_Rising 这是脉冲计数的

   //pwt_pulse_init(PWT_IN1, PWT_PS_CFG(PWT_PS_1) | Pulse_Width_Enable_irq | Pulse_Over_Enable_irq | Cycle_Rising );
    
   
   //printf("脉冲理论周期数为:%d\n",   timer_clk_khz * 1000 / s / 1);    //打印计数值
   /////pwt时钟取决于芯片输入时钟，也就是timer_clk
    
   
   Init_8700_2100();//陀螺仪//加速度计

   pit_init_ms(PIT0, 7);                               //////20ms
   pit_init_ms(PIT1, 1);
   
   if(gpio_get(PTG4))
   {
     guiyihuaInit();
   }
   else
   {
   loadflash();
   enable_irq(PIT_CH0_IRQn);                             // 使能PIT_CH0中断
   enable_irq(PIT_CH1_IRQn);                             // 使能PIT_CH1中断
   
   }
   
   EnableInterrupts;

   while(1)
   {
      ;
   }


}

