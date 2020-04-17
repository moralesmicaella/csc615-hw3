#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "Motor.h"

int keyboard_interrupt = 0;

// initializes the wiringPi and sets the motor and arrow pins to output mode
int setup(Motor motors[], int n, Arrow arrows) {
    if (wiringPiSetup() == -1) {
        printf("WiringPi Setup failed!\n");
        return -1;
    } 
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
    return 0;
}

// sets all the motor and arrow pins to low and switches them into input mode
void cleanup(Motor motors[], int n, Arrow arrows) {
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
void stop(Motor motors[], int n, Arrow arrows) {
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
void forward(Motor motors[], int n, int speed, Arrow arrows) {
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
void backward(Motor motors[], int n, int speed, Arrow arrows) {
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
void change_speed(Motor motors[], int n, int speed) {
    for (int i = 0; i < n; i++) {
        printf("The speed of Motor%d is changing to %d%% speed\n", motors[i].num, speed);
        softPwmWrite(motors[i].e, speed);
    }
}
