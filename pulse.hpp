#pragma once

namespace Pulse
{
    const double DefaultPulsePeriod = 5; //us
    double GateTime_us = 50; //us
    
    constexpr unsigned int UpperLimit = std::floor(Pulse::GateTime_us / Pulse::DefaultPulsePeriod);
    double PulseDuration = Pulse::DefaultPulsePeriod;

    void constexpr set_pulse_resoution(const int res_microseconds) __attribute__((always_inline))
    {
      if(res_microseconds <= Pulse::GateTime_us)
      {
        Pulse::UpperLimit = int(Pulse::GateTime_us)/int(res_microseconds); //Explicit truncation error
        Pulse::PulseDuration = double(Pulse::GateTime_us) / Pulse::UpperLimit; //Attention → discretisation error
      }

      else
        { Pulse::set_pulse_resoution(Pulse::DefaultPulsePeriod); } //Recursive Call
    }

    void constexpr init(double gatetime_us, double pulseduration_us) __attribute__((flatten))
    {
      Pulse::GateTime_us = gatetime;
      Pulse::set_pulse_resoution(pulseduration);
    }


    /** @brief Assumes an implicit clock. */
    void out_pulse(int pulse_count, uint32_t time) __attribute__((always_inline))
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
          Serial.print(state);
          digitalWriteFast(TTL_PIN, state);
          delayMicroseconds(Pulse::PulseDuration);
            
          pulse_count--;
        }
      #endif //Embedded Systems Implementation

      #ifdef NOISE_PC_SYSTEM
          
        #if NOISE_WARNINGS == 1
          #if NOISE_HIGH_DENSITY_WARNING == 1
            if(pulse_count > 1) { std::cerr << time << " : High Density!\n"; }
          #endif
            if(pulse_count > Pulse::UpperLimit){ std::cerr << time << " : Count Loss!\n" };
            // No invol indicator
        #endif

        for(unsigned int i = 0; i < pulse_count; i++)
        {
          bool state = false || (pulse_count > 0);
          std::cout << state;
          Utilities::pc_sleep(Pulse::PulseDuration);
          pulse_count--;
        }
        std::cout << std::flush;

      #endif //End of PC System Implementation
    } //End of Pulse::out_pulse()

}; //End of namespace Pulse