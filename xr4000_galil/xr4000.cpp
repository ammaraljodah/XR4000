#include "galilcontroller.h"
#include <iostream> 

#define UP 1
#define DOWN -1
#define STOP 0
#define MAXSPEED 30000
#define MINSPEED 0
#define ACTIVE 0
#define SHOUTDOWN 1

GalilController gc;

void rotate(int angle);
void initilizeMotors(void);
void activateMotors(void);
void jog(int speed);
void emergency_stop(void);
int homing(void); 
void oriente_wheels_forward(void);
void enablePositionTracking(void);

int main(int argc, char * argv[])
{
	std::cout<<"connecting"<<std::endl;
	gc.connect();
	std::cout<<"connection done"<<std::endl;

    initilizeMotors();
	homing();
    oriente_wheels_forward();
    
    activateMotors();
    enablePositionTracking();

	int speed = 0;
    int moving = 0;
    int steering = 0;
    int robotStatus = ACTIVE;
    char c;
    char const *message = " ************************************ \n up: moving forward/stop \n down: moving backward/stop \n right: steer right by +10 deg \n left: steer left by -10 deg \n a: increase speed by +1000 \n s: decrease speed by -1000 \n z: Emergency stop \n q: quit \n ************************************ \n";
    system("clear");
    std::cout << message;
   // Set the terminal to raw mode
    while(1) {
        system("stty raw");
        c = getchar(); 
        system("stty cooked");
        switch (c)
        {
        case 'a':
            speed += 1000;
            if (speed > MAXSPEED) speed = MAXSPEED;
            break;
        case 's':
            speed -= 1000;
            if (speed < MINSPEED) speed = MINSPEED;
            break;
        case 'A':
            if (moving == UP) moving = STOP; else moving = UP;
            break;
        case 'B':
            if (moving == DOWN) moving = STOP; else moving = DOWN;
            break;
         case 'C':
            steering += 10;
            if (steering > 90) steering = 90;
            break;
        case 'D':
            steering -= 10;
            if (steering < -90) steering = -90;
            break;
        case 'z':
            robotStatus = !robotStatus;
            speed = 0;
            steering = 0;
            moving = STOP;
            if (robotStatus == ACTIVE) {
                initilizeMotors();
                activateMotors();
                enablePositionTracking();
                } 
            else
                emergency_stop();
            break;
        }

        if (robotStatus == SHOUTDOWN){
            speed = 0;
            steering = 0;
            moving = STOP;}

        
        system("clear");
        std::cout << message;
        std::cout << "speed: " << speed<< std::endl;
        std::cout << "steering: " << steering<< std::endl;
        if (moving == UP) std::cout << "Moving forward" << std::endl;
        if (moving == DOWN) std::cout << "Moving backword" << std::endl;
        if (moving == STOP) std::cout << "Not moving" << std::endl;
        if (robotStatus == SHOUTDOWN) std::cout << "Emergency Stop" << std::endl;

        if (robotStatus == ACTIVE){
            rotate(steering);
            jog(moving*speed);
        }

        if(c == 'q') {
            std::cout << "Exiting..."<< std::endl;
            emergency_stop();
            break;
        } 

    }
    std::cout<<"Program has ended"<<std::endl;
    return 0; 
}

void rotate(int angle){
    char cmd[200]; 


    sprintf(cmd,"PAE=%i", int((double)angle/360*27360));
    std::cout<<cmd<<std::endl;
    gc.sendCommand(cmd);

    sprintf(cmd,"PAF=%i", int((double)angle/360*27360));
    std::cout<<cmd<<std::endl;
    gc.sendCommand(cmd);

    sprintf(cmd,"PAG=%i", int((double)angle/360*27360));
    std::cout<<cmd<<std::endl;
    gc.sendCommand(cmd);

    sprintf(cmd,"PAH=%i", int((double)angle/360*27360));
    std::cout<<cmd<<std::endl;
    gc.sendCommand(cmd);

}


void jog(int speed){
    char cmd[200];
    sprintf(cmd, "JG%i,%i,%i,%i",speed,speed,speed,speed);
    std::cout<<cmd;
	gc.sendCommand(cmd);
}

void enablePositionTracking(void){
    gc.sendCommand("STE");
    gc.sendCommand("STF");
    gc.sendCommand("STG");
    gc.sendCommand("STH");

    gc.sendCommand("SPE=20000");
    gc.sendCommand("SPF=20000");
    gc.sendCommand("SPG=20000");
    gc.sendCommand("SPH=20000");


    gc.sendCommand("PAE=0");
    gc.sendCommand("PAF=0");
    gc.sendCommand("PAG=0");
    gc.sendCommand("PAH=0");


    gc.sendCommand("BGE");
    gc.sendCommand("BGF");
    gc.sendCommand("BGG");
    gc.sendCommand("BGH");

    gc.sendCommand("PTE=1");
    gc.sendCommand("PTF=1");
    gc.sendCommand("PTG=1");
    gc.sendCommand("PTH=1");

}

void initilizeMotors(void){
    gc.sendCommand("MT-1,-1,-1,-1,-1,-1,-1,-1");
	gc.sendCommand("AC20000,20000,20000,20000,20000,20000,20000,20000");
	gc.sendCommand("DC20000,20000,20000,20000,20000,20000,20000,20000");
	gc.sendCommand("JG0,0,0,0,0,0,0,0");
	gc.sendCommand("SP0,0,0,0,0,0,0,0");
	gc.sendCommand("SHABCDEFGH");
	gc.sendCommand("ST");
    gc.sendCommand("AB");
	gc.sendCommand("CB2");
}
void activateMotors(void){
    gc.sendCommand("AB");
    gc.sendCommand("SHABCDEFGH");
    gc.sendCommand("BGABCDEFGH");
}
void emergency_stop(void){
	gc.sendCommand("SB2");
	gc.sendCommand("ST");
	gc.sendCommand("AB");
}

void oriente_wheels_forward(void){
    // the 360 deg rotation is 27360 ticks 
    // oriente s1 
    std::cout<<"oriente s1 started"<<std::endl;
    gc.sendCommand("AB");
	gc.sendCommand("PAE=13680"); // 180 deg 
    gc.sendCommand("SPE=2000");
    gc.sendCommand("BGE");
    gc.motionComplete("E");
    gc.sendCommand("DPE=0");
    std::cout<<"oriente s1 ended"<<std::endl;

    // oriente s2
    std::cout<<"oriente s2 started"<<std::endl;
    gc.sendCommand("AB");
	gc.sendCommand("PAF=6840"); // 90 deg 
    gc.sendCommand("SPF=2000");
    gc.sendCommand("BGF");
    gc.motionComplete("F");
    gc.sendCommand("DPF=0");
    std::cout<<"oriente s2 ended"<<std::endl;


    // s3 already in the right orientetion 

    // oriente s4
    std::cout<<"oriente s4 started"<<std::endl;
    gc.sendCommand("AB");
	gc.sendCommand("PAH=-6840"); // -90 deg 
    gc.sendCommand("SPH=2000");
    gc.sendCommand("BGH");
    gc.motionComplete("H");
    gc.sendCommand("DPH=0");
    std::cout<<"oriente s4 ended"<<std::endl;

}

int homing(void){
	double d_value; 
	std::cout<<"Turn on the Motors Power Supply and remove the Emergency stop, press Enter when done"<<std::endl;
	pause();

	gc.sendCommand("CN,1");
	
	
	// Homing S1 

	std::cout<<"Homing S1 started"<<std::endl;
	gc.sendCommand("SP0,0,0,0,2000,0,0,0");
	gc.sendCommand("FEE");
	gc.sendCommand("BGE");
	gc.motionComplete("E");
	std::cout<<"Homing S1 done"<<std::endl;
	gc.sendCommand("DPE=0");

	// Homing S2

	std::cout<<"Homing S2 started"<<std::endl;
	gc.sendCommand("SP0,0,0,0,0,2000,0,0");
	gc.sendCommand("BGF");
	while (gc.sendCommandD("MG @IN[20]") != 0.0);	
	gc.sendCommand("ST");
    gc.motionComplete("F");
	std::cout<<"Homing S2 done"<<std::endl;
	gc.sendCommand("DPF=0");

	// Homing S3 
        
	std::cout<<"Homing S3 started"<<std::endl;
   
	gc.sendCommand("SP0,0,0,0,0,0,2000,0");
	gc.sendCommand("FEG");
	gc.sendCommand("BGG");
	gc.motionComplete("G");
	std::cout<<"Homing S3 done"<<std::endl;
	gc.sendCommand("DPG=0");


	// Homing S4

	std::cout<<"Homing S4 started"<<std::endl;
	gc.sendCommand("SP0,0,0,0,0,0,0,2000");
	gc.sendCommand("FEH");
	gc.sendCommand("BGH");
	gc.motionComplete("H");
	std::cout<<"Homing S4 done"<<std::endl;
	gc.sendCommand("DPH=0");

	return 1;
}