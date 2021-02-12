#include "include.h"
#include "common.h"

#define dirK1  5
#define dirK2  9
#define dirK3  5
#define dirK4  5
extern char circle;

float directions(int16 C[5])   //四个电感，后期可以加上d
{
  float dir_pwm=0;
  

 // int16 xx=(x1-x2)-(x3-x4);

  if((C[0]>40&&C[4]<8)||(C[4]>40&&C[0]<8))///////curve(C[3]-C[1]<20)||(C[1]-C[3]<20)
  {
    dir_pwm=dirK1*(C[1]-C[3]);
    
    if(dir_pwm>400)
      dir_pwm=400;
    if(dir_pwm<-400)
      dir_pwm=-400;

  }
  
  else if((C[0]>20&&C[4]<5&&C[2]<5)||(C[4]>20&&C[0]<5&&C[2]<5))
  {
    dir_pwm=dirK3*(C[0]-C[4]);
    
    if(dir_pwm>400)
      dir_pwm=400;
    if(dir_pwm<-400)
      dir_pwm=-400;
  }
  
    else if((C[1]>20&&C[3]<5&&C[2]<5)||(C[3]>20&&C[1]<5&&C[2]<5))
  {
    dir_pwm=dirK4*(C[0]-C[4]);
    
    if(dir_pwm>400)
      dir_pwm=400;
    if(dir_pwm<-400)
      dir_pwm=-400;
  }
  else  /////straight C[0]<10||C[2]<10
  {
    dir_pwm=dirK2*(C[1]-C[3]);
    
    if(dir_pwm>200)
      dir_pwm=200;
    if(dir_pwm<-200)
      dir_pwm=-200;

  }
  return(dir_pwm); 
}

