#pragma once


//Defines system specific macro → bimodality - {PC_SYSTEMS, EMBEDDED_SYSTEMS}
#if defined(TEENSYDUINO) || defined(AVR_ATmega328) //For Ardunio Uno
  #define NOISE_EMBEDDEDSYSTEMS
#endif //def (TEENSYDUINO || AVR_ATmega328)

#if defined(_WIN32) || defined(__linux__) //For PC Systems
  #define NOISE_PC_SYSTEMS
#endif


// General Options --------------------------------------------- 

#define NOISE_DIMENSIONS 3
#define NOISE_NO_WALKERS 15


#define NOISE_BOX_CUBE_EDGE 100
#define NOISE_OBS_CUBE_EDGE 10


#define NOISE_INFINITE_EVOLVE 0
#define NOISE_MAXSWEEPS 10000

#define NOISE_MOVE_DSTEP 0 
#define NOISE_FIXED_STEP_SIZE 4


#define NOISE_WARNINGS 1
#define NOISE_HIGH_DENSITY_WARNING 1





#ifdef NOISE_EMBEDDEDSYSTEMS
  
  //Defines a pin that is set to high to enable syncing operation between two micro-controllers. (handshake before start)
  #define NOISE_SYNC_OPERATION 1
  #define NOISE_SYNC_PIN 12
  
  #define NOISE_LED_INDICATORS

  
  //  PIN ASSIGNMENTS ------------------------------ ↓

  //Is set to high, if due to the minimum resolution and finite width of the pulse, there is loss of count.
  #define COUNTLOSS_PIN
  
  //Is set to high if one or more particles are in the Observation volume
  #define INVOL_PIN
  
  //Pin on which TTL (pulse) output is generated (digital)
  #define TTL_PIN
  
  //Indicates evolution phase
  #define RUN_LED

  //Indicates Setup and Init phase
  #define SETUP_LED
  
  //Used to read random analog input - attach a long open wire for better results
  #define NOISE_EMPTY_ENTROPY_PIN

#endif


#ifdef NOISE_PC_SYSTEMS


#define NOISE_GENERATE_VIEW 0
#define NOISE_DSEP '.'

#endif