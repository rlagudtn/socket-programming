#include <stdio.h>
#include <pigpio.h> 

int main(){
  int yellow=18;
  int red=23;
  int green=25;

  gpioInitialise();
    gpioSetMode(yellow,PI_OUTPUT);
    gpioSetMode(green,PI_OUTPUT);
    gpioSetMode(red,PI_OUTPUT);

  while(1){
    gpioWrite(green,1);
    gpioDelay(1000000);
    gpioWrite(green,0);
    gpioWrite(yellow,1);
    gpioDelay(1000000);
    gpioWrite(yellow,0);

    gpioWrite(red,1);
    gpioDelay(1000000);
    gpioWrite(red,0);
  }
  

  gpioTerminate();
  printf("terminated\n");
  return 0;
}