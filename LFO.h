#pragma once

class LFO {
    public:
    LFO(const double* rate, const  int* samplerate, const int* waveform) {
        this->rate = rate; // hz
        this->samplerate = samplerate;
        this->waveform = waveform;

        through = 0.0;
    }
    double Tick() {
        const double periodSamples = *samplerate / *rate;
        const double delta = 1.0/periodSamples;
        through += delta;
        if (delta > 1.0) {
            // too fast
            return 0.0;
        }
        if (through > 1.0) {
        through -= 1.0;
      }
        if (through > 0.75) {
            double subThrough = through - 0.75;
            return 1.0 - (subThrough * 4.0);
        } else if (through > 0.5) {
            double subThrough = through - 0.5;
            return subThrough * 4.0;
        } else if (through > 0.25) {
            double subThrough = through - 0.25;
            return -1.0 + (subThrough * 4.0);
        }
        else {
            return (through * 4.0) *  -1.0;
        }
    };

    private:
    // config
    const int *samplerate;
    const double *rate;
    const int* waveform;

    // state
    double through;  // 0 > 1
};
