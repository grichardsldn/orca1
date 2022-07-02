#pragma once

class ADSR {
    private:
    // config
    const int *samplerate;

    // state
    double output;

    public:
    ADSR(const int* samplerate) {
      samplerate = samplerate;
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
