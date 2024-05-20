#include <stdio.h>
#include <pigpio.h>

int main(){
    int pin = 18;
    int switch_pin = 24;
    int led_state = 0;
    int switch_state = 1;
    int last_switch_state = 1;

    gpioInitialise();

    gpioSetMode(pin, PI_OUTPUT);
    gpioSetMode(switch_pin, PI_INPUT);

    while(1){
        switch_state = gpioRead(switch_pin);
        if (switch_state == 0 && last_switch_state == 1) {  // 스위치가 눌렸을 때
            led_state = !led_state;  // LED 상태를 토글
            gpioWrite(pin, led_state);
            gpioDelay(200000);  
        }
        last_switch_state = switch_state;
        gpioDelay(10000);  
    }

    gpioTerminate();
    return 0;
}
