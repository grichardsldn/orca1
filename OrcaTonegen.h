#pragma once
#include <math.h>

class OrcaTonegen {
    private:
    // params
    const int* note;
    const int* samplerate;
    const int* range;
    const double* modifyAmount;
    const double* sawMix;
    const double* pulseMix;
    const double* noiseMix;
    const double* pulseWidthManual;
    const double* subMix;
    const int* subType;

    // state
    double through;  // 0 > 1
    double hz;

    private:
    // functions
    double pulseFunction(double through) {
      through = fmod(through, 1.0);
      if( through > (0.5 + (*pulseWidthManual/1.8)) ){
        return 1.0;
      } else {
        return -1.0;
      }
    };
    double noiseFunction() {
      return ((double)(rand()%256)-128.0) / 128.0;
    }

    public:
    OrcaTonegen(const int *note, const int*samplerate, const int *range, const double* modifyAmount,
      const double* pulseWidthManual, const double* pulseMix, const double* noiseMix ) {
        
      this->range = range;
      this->note = note;
      this->samplerate = samplerate;
      this->pulseWidthManual = pulseWidthManual;
      this->pulseMix = pulseMix;
      this->noiseMix = noiseMix;

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

      double speed = 1.0;
      switch (*range) {
        case 0:
          speed = 1.0;
        break;
        case 1: 
          speed = 2.0;
        break;
        case 2:
          speed = 4.0;
        break;
        case 3:
          speed = 8.0;
        break;
      }

      const double pulse = pulseFunction(through * speed) * *pulseMix;
      const double noise = noiseFunction() * *noiseMix;
      return (pulse + noise) / 2.;
    };

    void Restart() {
      through = 0.0;
    }
};
