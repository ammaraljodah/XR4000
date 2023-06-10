#include "galilcontroller.h"
//! A trivial, C++ style return code check used in Galil's examples and demos.
/*!
*  Throws GReturn if return value is not `G_NO_ERROR`.
*  See Commands_Example.cpp for example usage and catch() handler.
*/
void GalilController::e(GReturn rc)
{
	if (rc != G_NO_ERROR)
		throw rc;
}

//! An example of error handling and debugging information.
void GalilController::error(GReturn rc)
{
    std::cout<<"HERE ==========================="<<std::endl;
    char buf[G_SMALL_BUFFER];
    GError(rc, buf, G_SMALL_BUFFER); //Get Error Information
    std::cout << buf << '\n'; 
	if (g)
	{
	    GSize size = sizeof(buf);
	    GUtility(g, G_UTIL_ERROR_CONTEXT, buf, &size);
	
	    if (buf[0])
    		std::cout << buf << '\n'; //further context
    		
		if ((rc == G_BAD_RESPONSE_QUESTION_MARK) 
		    && (GCommand(g, "TC1", buf, G_SMALL_BUFFER, 0) == G_NO_ERROR))
		{
		    std::cout << buf << '\n'; //Error code from controller
		}
	}
}

//! Pauses console apps for a user key stroke.
void GalilController::wait_for_user()
{
    std::cout << "Press enter..." << std::endl;
    std::getchar();
}
   
int GalilController::connect(void){
    GReturn rc = GALIL_OK;
    char buf[G_SMALL_BUFFER];
    //var used to refer to a unique connection. A valid connection is nonzero.
    try
    {
        char* address = (char*)"/dev/galilpci0";  //set the address 
        e(GOpen(address, &g)); //Opens a connection at the provided address
    }
    catch (GReturn gr)
    {
        error(gr);
        wait_for_user();
        return GALIL_ERROR;
    }
    return GALIL_OK;
    }
int GalilController::sendCommand(char const *command){
    if (g == 0) //Bad connection
        {
            std::cout<< "There was an error with the connection." <<std::endl;
            return G_CONNECTION_NOT_ESTABLISHED;
        }
    try
    {e(GCmd(g, (char*)command));}
    catch (GReturn gr)
	{
        std::cout<<"Error happend"<<std::endl;
	    error(gr);
	    wait_for_user();
        GCmd(g, "AB");
        throw gr;
	}
    return	GALIL_OK;
}
int GalilController::motionComplete(char const *axis){
    e(GMotionComplete(g, (char*)axis)); 
    return	GALIL_OK;
}
double GalilController::sendCommandD(char const *command){
    double d_value = 0.0;
    if (g == 0) //Bad connection
        {
            std::cout<< "There was an error with the connection." <<std::endl;
            return G_CONNECTION_NOT_ESTABLISHED;
        }
    e(GCmdD(g, (char*)command, &d_value));
    return	d_value;
}


