#pragma once

class Filter {
    private:
    // config
    const int* samplerate;
    const double *octaveNumber; // 0-8 Middle A  (a4) is in octave 4
    const double *resonance;
    const double *lfo;

    // state
    double pos = 0.0;
    double vel = 0.0;

    public:
    Filter(const int*samplerate, const double *octaveNumber, const double *resonance, const double *lfo) {
        this->samplerate = samplerate;
        this->octaveNumber = octaveNumber;
        this->resonance = resonance;
        this->lfo = lfo;
    }
    double Tick(double input) {
        double frequency = 27.5 * pow(2, *octaveNumber);
        input /= 2.;
        double inputForce = input - pos;
        if (pos > 1.0 || pos < -1.0) {
            inputForce = 0;
        }
        double restoringForce = (0 - pos) * 0.1; // low cut

        double mass = 2000.0 / (frequency * frequency);
        double resistance = vel * -1.0 * 0.01;

        double impulse = (inputForce + restoringForce + resistance) / mass / (*samplerate);
        vel += impulse;

        pos += vel * *resonance;
        pos += impulse * (1.0 - *resonance) * 5.0;
        
        // end stops
        if (pos > 2.0) {
            pos = 2.0;
            if (vel>0.0) {
                vel = 0.0;
            }
        }
        if (pos < -2.0) {
            pos = -2.0;
            if (vel<0.0) {
                vel = 0.0;
            }
        }
        
        return pos;
    }
};
