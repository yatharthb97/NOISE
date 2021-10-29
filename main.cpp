#ifdef NOISE_PC_SYSTEM

	#include "hypercube.hpp"
	#include "rand.hpp"
	#include "pulse.hpp"

	Hypercube<NOISE_DIMENSIONS, NOISE_NO_WALKERS, NOISE_OBS_CUBE_EDGE, NOISE_MAXSTEP> hcube;

	int main()
	{
			double Gate_time_us = 10;
			
			//Init → Software Resources
			Rand::init(Walkers_No, Edge, StepSize);
			Pulse::init(gatetime_us, pulseduration_us);
			hcube.init();


			hcube.evolve();
	}
#endif