#ifndef galilcontroller_h
#define galilcontroller_h

#define _CRT_SECURE_NO_WARNINGS //use traditional C calls like sprintf()

#include "gclib.h"
#include "gclibo.h"
#include <iostream> //std::cout
#include <cstdio> //std::getchar

#define GALIL_OK 0//return code for correct code execution
#define GALIL_ERROR -100 //return code for error in example code

class GalilController{
	 private:
        GCon g=0;
		void error(GReturn rc);
		void e(GReturn rc);
		void wait_for_user();
	public:             // Access specifier
    	int connect(void); 
		int sendCommand(char const *command);
		int motionComplete(char const *axis);
		double sendCommandD(char const *command);
};
#endif //galilcontroller_h


