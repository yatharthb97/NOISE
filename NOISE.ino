//Main ile


// Libraries  ------->
#include "macros.hpp"
#include "rand.h" //For random number generation
#include "hypercube_engine.hpp"

//Cube
#define NOISE_OBS_CUBE_EDGE
#define NOISE_BOX_CUBE_EDGE

Hypercube<NOISE_DIMENSIONS, NOISE_NO_WALKERS, NOISE_OBS_CUBE_EDGE, NOISE_MAXSTEP> hcube;

void setup()
{
	
	//Init Serial
	Serial.begin(115200);
	while(!Serial){}
	
	//Pin Modes


	//Init → Software Resources
	Rand::init(Walkers_No, Edge, StepSize);
	Pulse::init(GateTime, )
	hcube.init();

}

void loop()
{
	#if NOISE_SYNC_OPERATION == 1
		while(digitalRead(SYNC_PIN) != HIGH) {delay(10);}
	#endif

	digitalWrite(RUN_LED, HIGH);
	
	hcube.evolve();

	digitalWrite(RUN_LED, LOW);
	exit(0); //Stall processor till reset or exit program successfully on PC

} //End of loop()



