#pragma once

class Filter {
    private:
    // config
    const int* samplerate;
    const double *octaveNumber; // 0-8 Middle A  (a4) is in octave 4
    const double *resonance;

    // state
    double pos = 0.0;
    double vel = 0.0;

    public:
    Filter(const int*samplerate, const double *octaveNumber, const double *resonance) {
        this->samplerate = samplerate;
        this->octaveNumber = octaveNumber;
        this->resonance = resonance;
    }
    double Tick(double input) {
        double frequency = 27.5 * pow(2, *octaveNumber);
        input /= 2.;
        double inputForce = input - pos;
        double restoringForce = (0 - pos) * 0.1; // low cut

        double mass = 2000.0 / (frequency * frequency);
        double resistance = vel * -1.0 * 0.01;

        double impulse = (inputForce + restoringForce + resistance) / mass / (*samplerate);
        vel += impulse;

        pos += vel * *resonance;
        pos += impulse * (1.0 - *resonance) * 5.0;
        

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
