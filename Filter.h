#pragma once

class Filter {
    private:
    // config
    const int* samplerate;
    const double *frequency;
    const double *resonance;
    const double *testParam;

    // state
    double pos = 0.0;
    double vel = 0.0;

    public:
    Filter(const int*samplerate, const double *freq, const double *resonance, const double *testParam) {
        this->samplerate = samplerate;
        this->frequency = freq;
        this->resonance = resonance;
        this->testParam = testParam;
    }
    double Tick(double input) {
        input /= 2.;
        double diff = input - pos;
        double restoring = (0 - pos) * 0.00001;
        vel += (diff + restoring) / *samplerate * 1000.0 * *frequency;
        vel *= *resonance;
         
        // end stops
        if (pos > 1.0) {
            if (vel>0.0) {
                vel = 0.0;
            }
        }
        if (pos < -1.0) {
            if (vel<0.0) {
                vel = 0.0;
            }
        }
        pos += vel;
        return pos;
    }
};
