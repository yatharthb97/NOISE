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

  /** @brief Intializes and assigns random position to the particles.*/
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

  /** @brief Function that governs the evolution of the box. */
  void evolve() __attribute__((flatten))
  {
    const int outerloop = NOISE_MAXSTEP_ / NOISE_NO_WALKERS_;

    #if NOISE_INFINITE_EVOLVE == 0
      for(int itr_sweeps = 0; itr_sweeps < outerloop; itr_sweeps++)
    #elif NOISE_INFINITE_EVOLVE == 1
      while(true)
    #endif
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
      Pulse::out_pulse(InVolParticles, SimTime);

      //Generate view of the box.
      #if (defined NOISE_PC_SYSTEM) && (NOISE_GENERATE_VIEW == 1)
        //BoxView::write(NOISE_NO_WALKERS_, NOISE_DIMENSIONS_, Pos, InVol, SimTime);
      #endif

    } //End of the Event loop

  } //End of evolve()


  /** @brief Moves the position component randomly and applies periodic boundary conditions. */
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



