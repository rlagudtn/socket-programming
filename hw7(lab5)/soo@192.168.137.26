#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
int main(){
    int trig=17;
    int echo=27;
    unsigned int start,end;
    float distance;

    if(gpioInitialise()==-1){
        perror("intial error");
        exit(1);
    }

    gpioSetMode(trig,PI_OUTPUT);
    gpioSetMode(echo,PI_INPUT);
    gpioWrite(trig,0);
    gpioDelay(100000);

    while(1){
        gpioWrite(trig,1);
        gpioDelay(10);
        gpioWrite(trig,0);
        while(gpioRead(echo)==0)
            start=gpioTick();
        
        while(gpioRead(echo)==1)
            end=gpioTick();
        
        distance=(end-start)/58.0f;
        printf("iff: %u, distance (cm): %f\n",end-start,distance);

        gpioWrite(trig,0);
        gpioDelay(100000);
    }
    gpioTerminate();

    return 0;
}