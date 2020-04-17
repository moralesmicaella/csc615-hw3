#ifndef motor_h
    #define motor_h

    extern int keyboard_interrupt;

    typedef struct Motor {
        int num, e, f, r;
    } Motor;

    typedef struct Arrow {
        int af, ar;
    } Arrow;

    int setup(Motor motors[], int n, Arrow arrows);
    void cleanup(Motor motors[], int n, Arrow arrows);
    void stop(Motor motors[], int n, Arrow arrows);
    void forward(Motor motors[], int n, int speed, Arrow arrows);
    void backward(Motor motors[], int n, int speed, Arrow arrows);
    void change_speed(Motor motors[], int n, int speed);
#endif
