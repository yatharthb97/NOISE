#pragma once


//Libraries
#ifdef TEENSYDUINO
  #include<Entropy.h>
#endif

#ifdef NOISE_PC_SYSTEM
	#include <random>
	#include <cmath>
#endif


/** @brief Provides a common interface for random number generation for Embedded system implementation and PC runs.
 * Imulates integer random() function for PC Systems to maintain constant behaviour across */
namespace Rand
{
	
	unsigned int Walker_Size = 0;
	unsigned int Edge = 0;
	unsigned int StepSize = 0;

	#ifdef NOISE_PC_SYSTEM
		std::mt19937 Engine; //MT PRNG
		std::uniform_real_distribution<double> Uni_dist(0.0, 1.0);
		
		/** @brief Integer random function emulator - [lower, upper). 
		 * \attention `upper` is an open limit. */
		int random(lower, upper) //overloads for emulating int based distributions on PC.
		{
			double result = lower + (Rand::Uni_dist(Rand::Engine()) * (upper-lower));
			result = std::floor(result);
			return int(result);
		}
	#endif

	void init(unsigned int walker_size, unsigned int edge, unsigned int step_size)
	{
			//Attention - No input validation
			Rand::Walker_Size = walker_size;
			Rand::Edge = edge;
			Rand::step_size = step_size + 1; //To compensate for the open limit	

		#ifdef NOISE_EMBEDDEDSYSTEMS  //EMBEDDED Specific Implementation
				 
				#ifdef TEENSYDUINO
				 	Entropy.Initialize();
				 	uint32_t seed_value = Entropy.random();
				 	randomSeed(seed_value);
				#else
				 	uint32_t seed_value = analogRead(NOISE_EMPTY_ENTROPY_PIN);
				 	randomSeed(seed_value);
				#endif //TEENSYDUINO

		#elif defined(NOISE_PC_SYSTEM)
				 	std::random_device r;
				 	Rand::Engine.seed(r());
		#endif

	} //End of Rand::init()


		int sign()
		{		
				unsigned int temp = random(0,2);
				temp += !(temp)*-1; //Only works if temp is 0 : 0 → -1, 1 → 1.
				return temp;
		}

		int step()
		{
			return random(0, Rand::StepSize);
		}


		//Return between 0 and Edge
		int in_edge()
		{
			return random(0, Rand::Edge);
		}
		  

		//Return a Random PartID/WalkerID
		unsigned int partid()
		{
			return random(0, Rand::Walker_Size);
		}

}; //End of namespace Rand


