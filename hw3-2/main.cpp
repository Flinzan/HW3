 #include "mbed.h"
Thread thread1;
Thread thread2;
char slave_addr = 0xA0;

I2C i2c_master(D14, D15);
I2CSlave i2c_slave(A4,A5);

void master_work(){
    char m_addr = 0x90;
    while(1){
        char msg[2]={2,1};
        i2c_master.write(slave_addr, msg ,2);
        char reg[2]={0,0};
        i2c_master.read(slave_addr, reg, 2);
        
        ThisThread::sleep_for(1s);
    }
}
void slave_work(){
    char mode;
    char value;
    char msg[2];
    char buf[2];
    i2c_slave.address(0xA0);
    while (1) {
    int receive_code = i2c_slave.receive();
    switch (receive_code) {
        case I2CSlave::ReadAddressed:
            if (mode==1) { // mode!=0
                value = value + 1;
                msg[0] = mode;
                msg[1] = value;
                printf("Slave Write: mode=%d, value=%d\n", mode, value);
                i2c_slave.write(msg, 2);
            } 
            else if (mode==2) {
                value = value + 2;
                msg[0] = mode;
                msg[1] = value;
                printf("Slave Write: mode=%d, value=%d\n", mode, value);
                i2c_slave.write(msg, 2);
            } 
            else {
                msg[0] = mode;
                msg[1] = value;
                printf("Slave Write: mode=%d, value=%d\n", mode, value);
                i2c_slave.write(msg, 2);
            }
            break;
        case I2CSlave::WriteGeneral:
            i2c_slave.read(buf, 2);
            mode = buf[0];
            value = buf[1];
            printf("Slave Read General: mode=%d, value=%d\n", mode, value);
            break;
        case I2CSlave::WriteAddressed:
            i2c_slave.read(buf, 2);
            mode = buf[0];
            value = buf[1];
            printf("Slave Read Addressed: mode=%d, value=%d\n", mode, value);
            break;
    }
    for (int i = 0; i < 10; i++) {
      buf[i] = 0; // Clear buffer
    }
  }
}


int main(){
    thread1.start(master_work);
    thread2.start(slave_work);
}
