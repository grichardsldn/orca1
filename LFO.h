#pragma once

class LFO {
    public:
    LFO(const double* rate, const int* waveform) {
        this->rate = rate;
        this->waveform = waveform;
    }
    double Tick() {
        return 0.0;
    };
    private:
    const double *rate;
    const int*waveform;
};
