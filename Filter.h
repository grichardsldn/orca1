#pragma once

class Filter {
    private:
    // config
    const int* samplerate;
    const double *frequency;
    const double *resonance;

    // state
    double pos = 0.0;
    double vel = 0.0;

    public:
    Filter(const int*samplerate, const double *freq, const double *resonance) {
        this->samplerate = samplerate;
        this->frequency = freq;
        this->resonance = resonance;
    }
    double Tick(double input) {
        double diff = input - pos;
        vel += diff / *samplerate * 1000.0 * *frequency;
        vel += input / *samplerate * 1000.0;
        vel *= (1.0 - *resonance);
        pos += vel;
        return pos;
    }
};
