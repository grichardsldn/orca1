#pragma once

class ADSR {
    private:
    // config
    const int *samplerate;
    const double *attackMs;
    const double *decayMs;
    const double *sustainLevel;
    const double *releaseMs;

    // state
    double output;

    public:
    ADSR(const int* samplerate, const double *attack, const double *decay, const double *sustain, const double* release) {
        samplerate = samplerate;
        attackMs = attack;
        decayMs = decay;
        sustainLevel = sustain;
        releaseMs = release;

        output = 0.0;
    }
    double Tick(double input) {
        return this->output;
    };

    void Trigger() {
        this->output = 1.0;
    };
    void Release() {
        this->output = 0.0;
    };
};
