#include "include.h"
#include "common.h"
#define   NM    3
 
int16  AD_valu[5],AD_V[5][NM],chazhi,chazhi_old;
float  AD[5],sensor_to_one[5];
float  Slope_AD_1;  // �����µ����
int16  maxmin[10];  //��б궨 �ɼ�ֵ
int16  Position_transit[4];  //��¼���ɵ��һ����ֵ
int16  AD_sum[5]; 
int16  AD_MAX_NUM;   //
int16  position = 2,position_back = 1;
float  max_value,AD_0_max,AD_1_max,AD_2_max,AD_3_max;

extern int B0;
unsigned short ADC_va[5];


flash_data_t data;


////////////////////////////////////���ֵ����
void guiyihuaInit()
{
   uint16  i,j;
//   int16   Position_transit_short[4];
   
  
       //LCD_Print(25,2,"Collecting"); 
       //LCD_Print(28,4,"samples..."); 
   //////////���Լ���led��˸
       led_init(LED1);
       led(LED1, LED_ON);////LED0����˵����д��flash
       
       maxmin[0] = maxmin[1] = maxmin[2]= maxmin[3]= maxmin[4]=0;///������飬ǰ���������ֵ������������Сֵ
       maxmin[8] = maxmin[5] = maxmin[6]= maxmin[7]= maxmin[9]=2000;//  
       for(i=0;i<15000;i++) 
       {
           /*AD_valu[0] = ad_ave(ADC1,AD9,ADC_10bit,6);  //PTC0  ͨ��      
           AD_valu[1] = ad_ave(ADC1,AD8,ADC_10bit,6);  //PTC1  ͨ��     
           AD_valu[2] = ad_ave(ADC1,AD15,ADC_10bit,6); //PTE25 ͨ��     
           AD_valu[3] = ad_ave(ADC1,AD11,ADC_10bit,6); //PTE24 ͨ�� 
           AD_valu[4] = ad_ave(ADC1,AD13,ADC_10bit,6); //PTE24 ͨ�� 
           */
         
           ADC_va[0]=ADC_Ave(ADC0_SE0,ADC_12bit,10);
           ADC_va[1]=ADC_Ave(ADC0_SE1,ADC_12bit,10);
           ADC_va[2]=ADC_Ave(ADC0_SE2,ADC_12bit,10);
           ADC_va[3]=ADC_Ave(ADC0_SE3,ADC_12bit,10);
           ADC_va[4]=ADC_Ave(ADC0_SE4,ADC_12bit,10);
           //ADC_value[3]=ADC_Ave(ADC0_SE13,ADC_12bit,10);
           //ADC_value[4]=ADC_Ave(ADC0_SE14,ADC_12bit,10);
           
           for(j=0;j<5;j++) 
           {      
               if(ADC_va[j] >= maxmin[j]) 
               {
                   maxmin[j] = ADC_va[j];
                   /*if(j==0)    Position_transit_short[0] =  AD_valu[1];  //��¼���ɵ� ���ֵ
                   if(j==2)    Position_transit_short[1] =  AD_valu[1];  
                   if(j==3)    Position_transit_short[2] =  AD_valu[4];  
                   if(j==4)    Position_transit_short[3] =  AD_valu[3];  */
               }
               if(ADC_va[j] <= maxmin[j+5])
               {
                   maxmin[j+5] = ADC_va[j];
               }
           }
           //DELAY_MS(1);               //��ʱ    
       }
       
       
       /***************              ��¼�Ĺ��ɵ��һ��                     ******************/
      /* sensor_1 = (float)(Position_transit_short[0] - min_v[1])/(float)(max_v[1] - min_v[1]); 
       if(sensor_1 <= 0.0)  sensor_1 = 0.001;
       if(sensor_1 >= 1.0)  sensor_1 = 1.0; 
       
       sensor_2 = (float)(Position_transit_short[1] - min_v[1])/(float)(max_v[1] - min_v[1]); 
       if(sensor_2 <= 0.0)  sensor_2 = 0.001;
       if(sensor_2 >= 1.0)  sensor_2 = 1.0; 
       
       sensor_3 = (float)(Position_transit_short[2] - min_v[4])/(float)(max_v[4] - min_v[4]); 
       if(sensor_3 <= 0.0)  sensor_3 = 0.001;
       if(sensor_3 >= 1.0)  sensor_3 = 1.0; 
        
       sensor_4 = (float)(Position_transit_short[3] - min_v[3])/(float)(max_v[3] - min_v[3]); 
       if(sensor_4 <= 0.0)  sensor_4 = 0.001;
       if(sensor_4 >= 1.0)  sensor_4 = 1.0; 
       
       Position_transit[0] = (int16)(100 * sensor_1);
       Position_transit[1] = (int16)(100 * sensor_2); 
       Position_transit[2] = (int16)(100 * sensor_3);
       Position_transit[3] = (int16)(100 * sensor_4); */
       
       
       
       
       data.sectornum_start    = FLASH_SECTOR_NUM - 18;     //��ʼ����      ������3����������Ϊ�������    
       data.sectornum_end    = FLASH_SECTOR_NUM - 9;       //��������    
  
       data.data_addr      = &maxmin;                          //���ݵĵ�ַ    
       data.data_size      = sizeof(maxmin);                  //���ݵĴ�С    
    
      //��ʼ��������    
     flash_data_init(&data);    
     if(flash_data_save(&data))////LED0����˵����д��flash
     {
       led(LED1, LED_OFF);
     }
     
     
     /*if(gpio_get(PTD0))
     {
        while(1)
        {
          ;
          }
     }*/
     
      //����flash(����������)    
      //flash_data_reset(&data);    
    
    /*   //һ��ʼ����֪�������Ƿ���Ч��    
      if(flash_data_load(&data))    
      {    
          //�������һ�δ洢�����ݳɹ�    
     
      }    
      else   
      {    
          //����������Ч��flash�����ǿհ׵�    
     
          flash_data_reset(&data);        //����һ��flash����    
     
          md.a = 0x1234;                  //�Ա�����ʼ��    
          md.b = 0x5678;    
     
          //д���ʼ��ֵ    
          flash_data_save(&data) ;    
     
          //����������    
         md.a = 0;    
          md.b = 0;    
         flash_data_load(&data) ;    
     }    

     
     
     
     
       flash_erase_sector(SECTOR_ADM);       //����254����    
       for(i=0; i<5; i++)                    //��б궨�����ֵд������
       { 
           flash_write(SECTOR_ADM,i*4,max_v[i]);     
       }  
       for(i=0;i<4;i++)                     //���ɵ��һ��ֵд������ 
       {
          flash_write(SECTOR_ADM,20+i*4,Position_transit[i]);  
       }
   }
   else     ////////������flash�����ݣ�ֱ�Ӷ�
   {
      for(i=0;i<3;i++)
      { 
         for(j=0;j<5;j++)   //��ȡ�����еĲ����궨�����ֵ
         {
             max_v[j] = flash_read(SECTOR_ADM,j*4,int16);
         }
         for(j=0;j<4;j++)  //��ȡ���ɵ�
         {
             Position_transit[j] = flash_read(SECTOR_ADM,20+j*4,int16);
         }
            
         //LCD_Print(29,2,"Reading"); 
         //LCD_Print(28,4,"samples..."); 
         //delayms(10);           
      }*/
   
   //LCD_CLS();             
   //Beer_ON; 
   //delayms(25);  
   //Beer_OFF;
}



int16 guiyi(unsigned short a[5],int16 b[5])
{
       float   sensor_1,sensor_2,sensor_3,sensor_4,sensor_5;
       
       
       
       sensor_1 = (float)(a[0] - maxmin[5])/(float)(maxmin[0] - maxmin[5]); 
       if(sensor_1 <= 0.0)  sensor_1 = 0.001;
       if(sensor_1 >= 1.0)  sensor_1 = 1.0; 
       
       sensor_2 = (float)(a[1] - maxmin[6])/(float)(maxmin[1] - maxmin[6]); 
       if(sensor_2 <= 0.0)  sensor_2 = 0.001;
       if(sensor_2 >= 1.0)  sensor_2 = 1.0; 
       
       sensor_3 = (float)(a[2] - maxmin[7])/(float)(maxmin[2] - maxmin[7]); 
       if(sensor_3 <= 0.0)  sensor_3 = 0.001;
       if(sensor_3 >= 1.0)  sensor_3 = 1.0; 
       
       sensor_4 = (float)(a[3] - maxmin[8])/(float)(maxmin[3] - maxmin[8]); 
       if(sensor_4 <= 0.0)  sensor_4 = 0.001;
       if(sensor_4 >= 1.0)  sensor_4 = 1.0;
       
       sensor_5 = (float)(a[4] - maxmin[9])/(float)(maxmin[4] - maxmin[9]); 
       if(sensor_5 <= 0.0)  sensor_5 = 0.001;
       if(sensor_5 >= 1.0)  sensor_5 = 1.0; 
        
       /*sensor_4 = (float)(Position_transit_short[3] - min_v[3])/(float)(max_v[3] - min_v[3]); 
       if(sensor_4 <= 0.0)  sensor_4 = 0.001;
       if(sensor_4 >= 1.0)  sensor_4 = 1.0; */
       
       b[0] = (int16)(100 * sensor_1);
       b[1] = (int16)(100 * sensor_2); 
       b[2] = (int16)(100 * sensor_3);
       b[3] = (int16)(100 * sensor_4);
       b[4] = (int16)(100 * sensor_5);
       //Position_transit[3] = (int16)(100 * sensor_4);
       return b[5];
}


void loadflash()
{    
   data.sectornum_start    = FLASH_SECTOR_NUM - 18;     //��ʼ����      ������3����������Ϊ�������    
   data.sectornum_end    = FLASH_SECTOR_NUM - 9;       //��������    
  
   data.data_addr      = &maxmin;                          //���ݵĵ�ַ    
   data.data_size      = sizeof(maxmin);                  //���ݵĴ�С    
   flash_data_init(&data);
   //flash_data_load(&data);
   if(flash_data_load(&data))    
   {    
          
     //�������һ�δ洢�����ݳɹ�    
     
    }    
    else   
    {    
          //����������Ч��flash�����ǿհ׵�    
        guiyihuaInit();
        flash_data_load(&data) ;    
     }    
}