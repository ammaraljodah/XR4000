#include <iostream>

#define UP 1
#define DOWN -1
#define STOP 0
#define MAXSPEED 20000
#define MINSPEED 0
#define ACTIVE 0
#define SHOUTDOWN 1
void rotate(int angle);
void rotateS1(int angle);
void rotateS2(int angle);
void rotateS3(int angle);
void initilizeMotors(int angle);
void activate(void);
void jog(int speed);
void emergency_stop(void);

int main() {
    int speed = 0;
    int moving = 0;
    int steering = 0;
    int robotStatus = ACTIVE;
    char c;
    char const *message = " ************************************ \n up: moving forward/stop \n down: moving backward/stop \n right: steer right by +10 deg \n left: steer left by -10 deg \n a: increase speed by +1000 \n s: decrease speed by -1000 \n z: Emergency stop \n q: quite \n ************************************ \n";
    system("clear")
    ;
    std::cout << message;
   // Set the terminal to raw mode
    while(1) {
        system("stty cooked");
        system("stty raw");
        c = getchar(); 
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
            if (robotStatus == ACTIVE) {activate(); else emergency_stop();
            break;
        }

        if (robotStatus == SHOUTDOWN){
            speed = 0;
            steering = 0;
            moving = STOP;}

        system("stty cooked");
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
            jog(speed);
        }

        if(c == 'q') {
            system("stty cooked");
            std::cout << "Exiting..."<< std::endl;
            emergency_stop();
            break;
        } 

    }
    std::cout<<"Program has ended"<<std::endl;
    return 0; 
}

void rotate(int angle){
    rotateS1(angle);
    rotateS2(angle);
    rotateS3(angle);
    rotateS4(angle);

}

void rotateS1(int angle){
}

void rotateS2(int angle){
}

void rotateS3(int angle){ 
}

void rotateS4(int angle){
}

void jog(int speed){

}

void activate(void){
}

void emergency_stop(void){
}
