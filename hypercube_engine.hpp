#pragma once

#include "macros.hpp"
#include "minmax.hpp"

#include <cmath>
#ifdef NOISE_PC_SYSTEM
  #include <iostream>
#endif



template <unsigned int NOISE_DIMENSIONS_, unsigned int NOISE_NO_WALKERS_, unsigned int NOISE_MAXSTEP_>
class HyperCube
{  
  int Pos[NOISE_NO_WALKERS_][NOISE_DIMENSIONS_] = {0};
  bool InVol[NOISE_NO_WALKERS_] = {false};
  
  //MinMax Boundaries[NOISE_DIMENSIONS_] = {MinMax(0, NOISE_BOX_CUBE_EDGE)}; Not used → Assumes cubic box
  
  MinMax ObsVol[NOISE_DIMENSIONS_] = {MinMax(NOISE_BOX_CUBE_EDGE/2 - NOISE_OBS_CUBE_EDGE/2, 
                                             NOISE_BOX_CUBE_EDGE/2 + NOISE_OBS_CUBE_EDGE/2)};
  
public:

  uint32_t SimTime = 0;

  
  void init() __attribute__((flatten))
  {
    for(unsigned int i = 0; i < NOISE_NO_WALKERS_; i++)
    {
      bool local_invol = true;
      for(unsigned int j = 0; j < NOISE_DIMENSIONS_; j++)
      {
        Pos[i][j] = Rand::in_edge();
        local_invol &= ObsVol[j].within_bounds(Pos[i][j]);
      }
      InVol[i] = local_invol;
    }
  }


  void evolve() __attribute__((flatten))
  {
    const int outerloop = NOISE_MAXSTEP_ / NOISE_NO_WALKERS_;
    for(int itr_sweeps = 0; itr_sweeps < outerloop; itr_sweeps++)
    {          
      for(int partid = 0; partid < NOISE_NO_WALKERS_; partid++) //Sweep Start
      {
        //Move the particle
        for(int i = 0; i < NOISE_DIMENSIONS_; i++)
        {
          this->move(Pos[partid][i]);
        }
          
        //Check If the Particle is within Observation Volume
        bool now_InVol = true;
        for(int i = 0; i < NOISE_DIMENSIONS_; i++)
        {
          now_InVol &= ObsVol[i].within_bounds(Pos[partid][i]);
        }
       
        //A or B
        //A → Particle has entered the observation Volume
        if(now_InVol && !InVol[partid])
        {
          InVol[partid] = true;
          InVolParticles++;
        }
              
        //B. Particle has left the observation Volume
        if(!now_InVol && InVol[partid])
        {
          InVolParticles--;
          InVol[partid] = false;
        }
        //Move is complete ----------------------------------------------
             
        SimTime++;

      } //End of Sweep

      //Generate TTL Pulse ----------------------------------------------
      Pulse::out_pulse(InVolParticles);

    } //End of the Event loop

  } //End of evolve()


  void move(int &pos_comp) __attribute__((always_inline))
  {
    #ifdef NOISE_MOVE_DSTEP == 1
      pos_comp += Rand::step() * Rand::sign();
    #elif NOISE_MOVE_DSTEP == 0
      pos_comp += NOISE_FIXED_STEP_SIZE * Rand::sign();
    #endif

    //Apply Assymetic Box PBC
    pos_comp += (pos_comp > NOISE_BOX_CUBE_EDGE) * (-NOISE_BOX_CUBE_EDGE) + 
                (pos_comp < 0) * (NOISE_BOX_CUBE_EDGE); 
  } // End of move()

}; // End of class Box



namespace Pulse
{
    const double DefaultPulsePeriod = 5; //us
    double GateTime = 50; //us
    constexpr unsigned int UpperLimit = std::floor(Pulse::GateTime / Pulse::DefaultPulsePeriod);
    double PulseDuration = Pulse::DefaultPulsePeriod;

    void set_pulse_resoution(const int res_microseconds) __attribute__((always_inline))
    {
      if(res_microseconds <= Pulse::GateTime)
      {
        Pulse::UpperLimit = int(Pulse::GateTime)/int(res_microseconds); //Explicit truncation error
        Pulse::PulseDuration = double(Pulse::GateTime) / Pulse::UpperLimit; //Attention → discretisation error
      }

      else
        { Pulse::set_pulse_resoution(Pulse::DefaultPulsePeriod); } //Recursive Call
    }

    void init(double gatetime_us, double pulseduration_us) __attribute__((flatten))
    {
      Pulse::GateTime = gatetime;
      Pulse::set_pulse_resoution(pulseduration);
    }


    /** @brief Assumes an implicit clock. */
    void out_pulse(int pulse_count) __attribute__((always_inline))
    {
      #ifdef NOISE_EMBEDDEDSYSTEMS
        
        #if NOISE_LED_INDICATORS == 1 && NOISE_WARNINGS == 1
          #if NOISE_HIGH_DENSITY_WARNING == 1
            digitalWriteFast(HIGH_DENSITY_WARNING_PIN, (pulse_count > 1));
          #endif
            digitalWriteFast(COUNTLOSS_PIN, (pulse_count > Pulse::UpperLimit));
            digitalWriteFast(INVOL_PIN, (pulse_count > 0));
        #endif

        for(unsigned int i = 0; i < Pulse::UpperLimit; i++)
        {
          bool state = false || (pulse_count > 0);
          Serial.NOISE_EMBEDDED_SERIALOUT_FN(state);
          digitalWriteFast(TTL_PIN, state);
          delayMicroseconds(Pulse::PulseDuration);
            
          pulse_count--;
        }
      #endif //Embedded Systems Implementation

      #ifdef NOISE_PC_SYSTEM
          
        #if NOISE_WARNINGS == 1
          #if NOISE_HIGH_DENSITY_WARNING == 1
            if(pulse_count > 1) { std::cerr << "High Density!\n"; }
          #endif
            if(pulse_count > Pulse::UpperLimit){ std::cerr << "Count Loss!\n" };
            // No invol indicator
        #endif

        for(unsigned int i = 0; i < pulse_count; i++)
        {
          bool state = false || (pulse_count > 0);
          std::cout << state;
          pc_sleep(Pulse::PulseDuration);
          pulse_count--;
        }
        std::cout << std::flush;

      #endif //End of PC System Implementation
    } //End of Pulse::out_pulse()

}; //End of namespace Pulse



