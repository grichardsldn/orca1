#pragma once
#include <math.h>

class OrcaTonegen {
    private:
    // params
    const int* note;
    const int* samplerate;
    const double* modify_amount;
    const double* saw_mix;
    const double* pulse_mix;
    const double* noise_mix;
    const double* pulse_width_manual;
    const double* sub_mix;
    const int* sub_type;

    // state
    double through;  // 0 > 1
    double hz;

    public:
    OrcaTonegen(const int *note, const int*samplerate, const double* modify_amount,
      const double* pulse_width_manual ) {
      this->note = note;
      this->samplerate = samplerate;
      this->pulse_width_manual = pulse_width_manual;
      through = 0.0;
      hz = 440.0; // default
    };

    double Tick() {
      double a = 440; //frequency of A (coomon value is 440Hz)
      hz = (a / 32) * pow(2, ((*note - 9) / 12.0));
      const double samplerate = (double)(*this->samplerate);
      // const double samplerate = 44000.0;
      const double periodSamples = samplerate / hz;
      const double delta = 1.0/periodSamples;
      through += delta;
      if (delta > 1.0) {
        return -1;
      }
      if (through > 1.0) {
        through -= 1.0;
      }
      if( through > (0.5 + (*pulse_width_manual/1.8)) ){
        return 1.0;
      } else {
        return -1.0;
      }
    };

    void Restart() {
      through = 0.0;
    }
};
