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
    double Tick(double input) {
        switch(state) {
            case idle: 
                output = 0.0;
            break;

            case attack:
                output += *attackRate / (double)*samplerate / 1000.0;
                if (output > 1.0) {
                    output = 1.0;
                    state = decay;
                }
            break;
            case decay:
                output -= *decayRate / (double)*samplerate / 1000.9;
                if (output < *sustainLevel) {
                    output = *sustainLevel;
                    state = sustain;
                }
            break;
            case sustain:
                output = *sustainLevel;
            break;
            case release:
                output -= *releaseRate / (double)*samplerate / 1000.9;
                if (output < 0) {
                    output = 0.0;
                    state = idle;
                }
            break;
            default:
                state = idle;
                output = 0.0;
        }
        return this->output;
    };

    void Trigger() {
        this->state = attack;
    };
    void Release() {
        this->state = release;
    };
};
