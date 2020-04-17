#include "Motor.h"

// handles a signal interrupt
void sigint_handler(int sig_num) {
    printf("Keyboard Interrupt\n");
    keyboard_interrupt = 1;
}

int main(void) {
    // creates the motors and sets their motor pins
    // wiringPi 0 = P11, wiringPi 2 = P13, wiringPi 3 = P15
    // wiringPi 6 = P22, wiringPi 4 = P16, wiringPi 5 = P18
    struct Motor m1 = {.num = 1, .e = 0, .f = 2, .r = 3};
    struct Motor m2 = {.num = 2, .e = 6, .f = 4, .r = 5}; 
    
    struct Motor motors[] = {m1, m2};
    int n = sizeof(motors) / sizeof(motors[0]);

    // wiringPi 25 = P37, wiringPi 23 = P33
    struct Arrows arrows = {.af = 25, .ar = 23};

    int speed = 10;
    
    // sets the sigint_handler to handle a signal interrupt
    signal(SIGINT, sigint_handler);
    
    if (setup(motors, n, arrows) == -1) 
        return -1;
    while (1) {
        // moves the motors forward for 5 seconds
        forward(motors, n, speed, arrows);
        delay(5000);
        
        // stops the motor for 2 seconds
        stop(motors, n, arrows);
        delay(2000);

        // moves the motors backward for 5 seconds
        backward(motors, n, speed, arrows);
        delay(5000);

        // stops the motor for 2 seconds
        stop(motors, n, arrows);
        delay(2000);
        
        // increments the speed by 10%
        if (speed <=90)
            speed += 10;
    }


    return 0;
}