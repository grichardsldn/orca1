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
        double inputForce = input - pos;
        double restoringForce = (0 - pos) * *testParam;
        double mass = 2000.0 / (*frequency * *frequency);
        double resistance = vel * -1.0 * *resonance;

         vel += (inputForce + restoringForce + resistance) / mass / (*samplerate);
         pos += vel;
        // end stops
        if (pos > 1.0) {
            pos = 1.0;
            if (vel>0.0) {
                vel = 0.0;
            }
        }
        if (pos < -1.0) {
            pos = -1.0;
            if (vel<0.0) {
                vel = 0.0;
            }
        }
        
        return pos;
    }
};
