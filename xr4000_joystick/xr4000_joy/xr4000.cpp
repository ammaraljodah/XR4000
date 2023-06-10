#include <iostream> 
#include <vector>
#include <string>
#include <sstream>
#include "galilcontroller.h"
#include "serialib.h"

#define UP 1
#define DOWN -1
#define STOP 0
#define MAXSPEED 30000
#define MINSPEED 0
#define ACTIVE 0
#define SHOUTDOWN 1
#define SERIAL_PORT "/dev/ttyACM0"
#define SPEED_POT_CENTRE 825
#define SPEED_FULL_UP 0
#define SPEED_FULL_DOWN 924

#define STEER_POT_CENTRE 845
#define STEER_FULL_LEFT 0
#define STEER_FULL_RIGHT 917
GalilController gc;

void rotate(int angle);
void initilizeMotors(void);
void activateMotors(void);
void jog(int speed);
void emergency_stop(void);
int homing(void); 
void oriente_wheels_forward(void);
void enablePositionTracking(void);
void readJoystick(serialib &serial, int &pot1, int &pot2, int &b1, int &b2);

int main(int argc, char * argv[])
{
    int speed = 0;
    int moving = 1;
    int steering = 0;
    int robotStatus = ACTIVE;
    char c;
    int pot1=0, pot2=0, b1=0, b2=0;
    char const *message = " ************************************ \n up: moving forward/stop \n down: moving backward/stop \n right: steer right by +10 deg \n left: steer left by -10 deg \n a: increase speed by +1000 \n s: decrease speed by -1000 \n z: Emergency stop \n q: quit \n ************************************ \n";
   
	std::cout<<"connecting"<<std::endl;
	gc.connect();
	std::cout<<"connection done"<<std::endl;
    initilizeMotors();
	homing();
    oriente_wheels_forward();
    activateMotors();
    enablePositionTracking();

	system("clear");
    std::cout << message;

    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, 9600);
    if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n",SERIAL_PORT);
    
while(1) {

        readJoystick(serial, pot1, pot2, b1, b2);
        int speedpot = 0; 
        int steerpot = 0; 
        if (pot2>=SPEED_FULL_UP && pot2 <= SPEED_POT_CENTRE)
            speedpot = int( (SPEED_POT_CENTRE-pot2)/float(SPEED_POT_CENTRE-SPEED_FULL_UP)*MAXSPEED);
        if (pot2<=SPEED_FULL_DOWN && pot2 >= SPEED_POT_CENTRE)
            speedpot = -int( (pot2 - SPEED_POT_CENTRE)/float(SPEED_FULL_DOWN - SPEED_POT_CENTRE)*MAXSPEED);
        if (pot1>=STEER_FULL_LEFT && pot1 <= STEER_POT_CENTRE)
            steerpot = int( (STEER_POT_CENTRE-pot1)/float(STEER_POT_CENTRE-STEER_FULL_LEFT)*90);
        if (pot1<=STEER_FULL_RIGHT && pot1 >= STEER_POT_CENTRE)
            steerpot = int( (pot1 - STEER_POT_CENTRE)/float(STEER_FULL_RIGHT - STEER_POT_CENTRE)*-90);
       
        if (speedpot < 0)
            steerpot *= -1;

        system("clear");
        std::cout << message;
        std::cout << "speed: " << speed<< std::endl;
        std::cout << "steering: " << steering<< std::endl;
        if (b1 == true)
            {emergency_stop();
            robotStatus = SHOUTDOWN;}
        else if (b1 == false && robotStatus == SHOUTDOWN)
            {robotStatus = ACTIVE;
            initilizeMotors();
            activateMotors();
            enablePositionTracking();
            }



        steering = steerpot;
        speed = speedpot;
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
    gc.sendCommand(cmd);

    sprintf(cmd,"PAF=%i", int((double)angle/360*27360));
    gc.sendCommand(cmd);

    sprintf(cmd,"PAG=%i", int((double)angle/360*27360));
    gc.sendCommand(cmd);

    sprintf(cmd,"PAH=%i", int((double)angle/360*27360));
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

    gc.sendCommand("SPE=15000");
    gc.sendCommand("SPF=15000");
    gc.sendCommand("SPG=15000");
    gc.sendCommand("SPH=15000");


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
    char c;
	std::cout<<"Turn on the Motors Power Supply and remove the Emergency stop, press Enter when done"<<std::endl;
	std::getchar();

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

void readJoystick(serialib &serial, int &pot1, int &pot2, int &b1, int &b2){
    char buffer[500];
    std::string s = "0";
    while (s.length()!= 12) // checking if we recive full length message 12 char
    {
        serial.readString(buffer, '\n' ,500,100000);
        s = buffer;
    }
    
    
    std::cout<<s.length()<<std::endl;
    std::stringstream ss(buffer);
    std::vector<std::string> result;

    while( ss.good() )
    {
        std::string substr;
        std::getline( ss, substr, ',' );
        result.push_back( substr );
    }

    std::istringstream ( result[0] ) >> pot1;
    std::istringstream ( result[1] ) >> pot2;
    std::istringstream ( result[2] ) >> b1;
    std::istringstream ( result[3] ) >> b2;
}
    
