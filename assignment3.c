/**************************************************************** 
* Class: CSC-615-01 Spring 2020
* Name: Micaella Morales
* Student ID: 916832064
* Project: Assignment 3 - Motors & Motor Shield
* File: assignment3.c
*
* Description:
* This assignment focuses on controlling two motors. The motors 
* should be able to move forward, backward, and stop. The speed 
* of the motors should also be changeable using the Pulse-Width 
* Modulation (or PWM).
* Specifically, in this program, the motors have an intial 10%
* speed. They move forward for 5 seconds, stop for 2 seconds, 
* move backward for 5 seconds, and stop for another 2 seconds. 
* It is an infinite cycle that increments the speed by 10% until 
* it reaches its 100% speed. The cycle stops only when the user 
* triggers a keyboard interrupt (ctrl+c).
*
* To compile this file, enter make
* To run this file, enter ./assignment3
****************************************************************/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>


int keyboard_interrupt = 0;

struct Motor {
    int num, e, f, r;
};

struct Arrows {
    int af, ar;
};

// handles a signal interrupt
void sigint_handler(int sig_num) {
    printf("Keyboard Interrupt\n");
    keyboard_interrupt = 1;
}

// initializes the wiringPi and sets the motor and arrow pins to output mode
void setup(struct Motor motors[], int n, struct Arrows arrows) {
    wiringPiSetup();
    for (int i = 0; i < n; i++) {
        pinMode(motors[i].e, OUTPUT);
        pinMode(motors[i].f, OUTPUT);
        pinMode(motors[i].r, OUTPUT);
        softPwmCreate(motors[i].e, 0, 100);
        digitalWrite(motors[i].e, HIGH);
        digitalWrite(motors[i].f, LOW);
        digitalWrite(motors[i].r, LOW);
    }
    pinMode(arrows.af, OUTPUT);
    pinMode(arrows.ar, OUTPUT);
}

// sets all the motor and arrow pins to low and switches them into input mode
void cleanup(struct Motor motors[], int n, struct Arrows arrows) {
    for (int i = 0; i < n; i++) {
        softPwmWrite(motors[i].e, 0);
        digitalWrite(motors[i].e, LOW);
        digitalWrite(motors[i].f, LOW);
        digitalWrite(motors[i].r, LOW);
        pinMode(motors[i].e, INPUT);
        pinMode(motors[i].f, INPUT);
        pinMode(motors[i].r, INPUT);
    }

    digitalWrite(arrows.af, LOW); 
    digitalWrite(arrows.ar, LOW); 
    pinMode(arrows.af, INPUT); 
    pinMode(arrows.ar, INPUT);
}

// changes the speed to 0% and sets the forward and reverse pins to low
void stop(struct Motor motors[], int n, struct Arrows arrows) {
    for (int i = 0; i < n; i++) {
        printf("Motor%d stops...\n", motors[i].num);
        softPwmWrite(motors[i].e, 0);
        digitalWrite(motors[i].f, LOW);
        digitalWrite(motors[i].r, LOW);
    }

    // turns off the light of the arrows in the motorshield
    digitalWrite(arrows.af, LOW); 
    digitalWrite(arrows.ar, LOW); 

    // cleans up the pins when there is a keyboard interrupt and exits the program
    if (keyboard_interrupt) {
        printf("Cleaning up...\n");
        cleanup(motors, n, arrows);
        exit(1);
    }
}

// sets the enable pin of the motors with the given speed 
// and sets the forward pin to high and the reverse pin to low
void forward(struct Motor motors[], int n, int speed, struct Arrows arrows) {
    for (int i = 0; i < n; i++) {
        printf("Motor%d is moving forward with %d%% speed\n", motors[i].num, speed);
        softPwmWrite(motors[i].e, speed);
        digitalWrite(motors[i].f, HIGH);
        digitalWrite(motors[i].r, LOW);
    }

    // lights up the forward arrow in the motorshield
    digitalWrite(arrows.af, HIGH); 
    digitalWrite(arrows.ar, LOW); 

    // cleans up the pins when there is a keyboard interrupt and exits the program
    if (keyboard_interrupt) {
        printf("Cleaning up...\n");
        cleanup(motors, n, arrows);
        exit(1);
    }
}

// sets the enable pin of the motors with the given speed 
// and sets the forward pin to low and the reverse pin to high
void backward(struct Motor motors[], int n, int speed, struct Arrows arrows) {
    for (int i = 0; i < n; i++) {
        printf("Motor%d is moving backward with %d%% speed\n", motors[i].num, speed);
        softPwmWrite(motors[i].e, speed);
        digitalWrite(motors[i].f, LOW);
        digitalWrite(motors[i].r, HIGH);
    }

    // lights up the backward arrow in the motorshield
    digitalWrite(arrows.af, LOW); 
    digitalWrite(arrows.ar, HIGH); 

    // cleans up the pins when there is a keyboard interrupt and exits the program
    if (keyboard_interrupt) {
        printf("Cleaning up...\n");
        cleanup(motors, n, arrows);
        exit(1);
    }
}

// sets the enable pin of the motors with the given speed 
void change_speed(struct Motor motors[], int n, int speed) {
    for (int i = 0; i < n; i++) {
        printf("The speed of Motor%d is changing to %d%% speed\n", motors[i].num, speed);
        softPwmWrite(motors[i].e, speed);
    }
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
    
    setup(motors, n, arrows);
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

