#include "observer.h"
#include "sensor.h"

using namespace Common;

int main( int argc, char** argv )
{
	Sensor sensor;
	std::string xmlfilename ("./config/system_model.xml");
	Observer obs;
	obsParam_t obsparam = {&obs, &sensor, &xmlfilename};
//	std::cout << "OI" << std::endl;
	Observer::runObserver((void *) &obsparam);
	
	exit (EXIT_SUCCESS);
}
