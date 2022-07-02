#pragma once

class Filter {
    private:
    // config
    const int* samplerate;
    
    public:
    Filter(const int*samplerate) {
        this->samplerate = samplerate;
    }
    double Tick(double input) {
        return input; // straight through
    }
};
