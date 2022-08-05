#pragma once

enum State { idle, attack, decay, sustain, release };

class ADSR {
    private:
    // config
    const int *samplerate;
    const double *attackRate;
    const double *decayRate;
    const double *sustainLevel;
    const double *releaseRate;

    // state
    double output;
    State state;

    public:
    ADSR(const int* samplerate, const double *attack, const double *decay, const double *sustain, const double* release) {
        this->samplerate = samplerate;
        this->attackRate = attack;
        this->decayRate = decay;
        this->sustainLevel = sustain;
        this->releaseRate = release;

        state = idle;
        output = 0.0;
    }

    State getState() {
        return state;
    }

    double Tick() {
        double diff;
        switch(state) {
            case idle: 
                output = 0.0;
            break;

            case attack:
                diff = 1.0 - output;
                output += diff * (*attackRate / (double)*samplerate / 100.0);
                if (output > 0.99) {
                    output = 0.99;
                    state = decay;
                }
            break;
            case decay:
                diff = output - *sustainLevel;
                output -= diff * (*decayRate / (double)*samplerate / 100.0);
                if (output < (*sustainLevel + 0.01)) {
                    output = *sustainLevel + 0.01;
                    state = sustain;
                }
            break;
            case sustain:
                output = *sustainLevel + 0.01;
            break;
            case release:
                double dropRate = *releaseRate;
                diff = output;
                
                if (output > *sustainLevel && *decayRate > *releaseRate) { 
                    dropRate = *decayRate;
                }
                output -= diff * (dropRate / *samplerate / 100.0);
                if (output < 0.01) {
                    output = 0.0;
                    state = idle;
                }
            break;

        }
        return this->output;
    };

    void Trigger() {
        const double startLevel = 0.2;
        if (output < startLevel) {
            output = startLevel; // start with something
        }
        this->state = attack;
    };
    void Release() {
        this->state = release;
    };
};
