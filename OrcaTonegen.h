#pragma once
#include <math.h>

class OrcaTonegen {
    private:
    // params
    const double* note;
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
    double pulseFunction(double through, double width) {
      through = fmod(through, 1.0);
      if( through > (0.5 + (width/1.5)) ){
        return 1.0;
      } else {
        return -1.0;
      }
    };
    
    double noiseFunction() {
      return ((double)(rand()%256)-128.0) / 128.0;
    }
    double sawFunction(double through) {
      through = fmod(through, 1.0);
      return ((through * 2.0) - 1.0);
    }

    public:
    OrcaTonegen(const double *note, const int*samplerate, const int *range, const double* modifyAmount,
      const double* pulseWidthManual, const double* pulseMix, const double *sawMix, const double *subMix, const int *subType, const double* noiseMix ) {
        
      this->range = range;
      this->note = note;
      this->modifyAmount = modifyAmount;
      this->samplerate = samplerate;
      this->pulseWidthManual = pulseWidthManual;
      this->pulseMix = pulseMix;
      this->sawMix = sawMix;
      this->subMix = subMix;
      this->subType = subType;
      this->noiseMix = noiseMix;
      
      through = 0.0;
      hz = 440.0; // default
    };

    double Tick() {
      double a = 440; //frequency of A (coomon value is 440Hz)
      hz = (a / 32) * pow(2, (((*note + *modifyAmount) - 9.0) / 12.0));
      hz /= 4.; // hz is for lowest sub osc
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
          speed = 4.0;
        break;
        case 1: 
          speed = 8.0;
        break;
        case 2:
          speed = 16.0;
        break;
        case 3:
          speed = 32.0;
        break;
      }

      const double pulse = pulseFunction(through * speed, *pulseWidthManual) * *pulseMix;
      const double noise = noiseFunction() * *noiseMix;
      const double saw = sawFunction(through * speed) * *sawMix;

      double subSpeed = 1.0;
      double subPulse = 0.0;
      switch (*subType) {
        case 0: // 1 oct down square
          subSpeed = 2.0;
        break;
        case 1: // 2 oct down square
        break;
        case 2: // 2 oct down pulse
          subPulse = 0.5;
        break;
      }
      const double sub = pulseFunction(through * subSpeed, subPulse) * *subMix;
      
      return (pulse + noise + saw + sub) / 4.;
    };

    void Restart() {
      through = 0.0;
    }
};
