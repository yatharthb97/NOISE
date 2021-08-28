#pragma once


//Defines system specific macro → bimodality - {PC_SYSTEMS, EMBEDDED_SYSTEMS}
#if defined(TEENSYDUINO) || defined(AVR_ATmega328) //For Ardunio Uno
  #define NOISE_EMBEDDEDSYSTEMS
#endif //def (TEENSYDUINO || AVR_ATmega328)

#if defined(_WIN32) || defined(__linux__) //For PC Systems
  #define NOISE_PC_SYSTEMS
#endif



//Macro Definations → Implemantation Specific
#ifdef NOISE_EMBEDDEDSYSTEMS //------------------------------------
  
  #define NOISE_SYNC_OPERATION 1
  
#elif defined(NOISE_PC_SYSTEMS) //==================================
  
  #define NOISE_DSEP ','
  #define NOISE_FRAME_EXPORT 0

#endif //------------------------------------------------------------

#define NOISE_EMBEDDED_SERIALOUT_FN send_now


#define NOISE_FIXED_STEP_SIZE
#define NOISE_MOVE_DSTEPs




//PINS
#ifdef NOISE_EMBEDDEDSYSTEMS
  #define COUNTLOSS_PIN
  #define INVOL_PIN
  #define TTL_PIN
  #define RUN_LED
  #define SETUP_LED
  #define NOISE_EMPTY_ENTROPY_PIN
#endif


