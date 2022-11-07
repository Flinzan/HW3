#include "mbed.h"
#include <cstdio>

#define MAXIMUM_BUFFER_SIZE 6
Thread thread1;
Thread thread2;

static BufferedSerial device1(D10,D9);
static BufferedSerial device2(D1,D0);

// main() runs in its own thread in the OS
void uart_master(){
    char buf0[MAXIMUM_BUFFER_SIZE]={0,0,1,0,2,0};
    for(int i=0;i<6;i++){
        device1.write(&buf0[i],1);
        ThisThread::sleep_for(1s);
    }
}
void uart_slave(){
    char buf[MAXIMUM_BUFFER_SIZE];
    char mode;
    char value;
    char msg[2];


    while (1) {
        if (device2.size()) { // with at least 1 char
            device2.read(buf, 1);
            mode = buf[0];
            if (device2.size()) { // with at least 1 char
                device2.read(buf, 1);
                value = buf[0];
                printf("Slave Read: mode=%d, value=%d\n", mode, value);

                if (device2.writable()) {
                    if (mode == 1) { // mode!=0
                        value = value + 1;
                        msg[0] = mode;
                        msg[1] = value;
                        printf("Slave Write: mode=%d, value=%d\n", mode, value);
                        device2.write(msg, 2);
                    } else if (mode == 2) {
                        value = value + 2;
                        msg[0] = mode;
                        msg[1] = value;
                        printf("Slave Write: mode=%d, value=%d\n", mode, value);
                        device2.write(msg, 2);
                    } else {
                        msg[0] = mode;
                        msg[1] = value;
                        printf("Slave Write: mode=%d, value=%d\n", mode, value);
                        device2.write(msg, 2);
                    }
                    ThisThread::sleep_for(100ms); //wait after write
                }
            }
        }
    }
}

int main()
{
    while (true) {
        thread1.start(uart_master);
        thread2.start(uart_slave);
    }
}

