#pragma once

class OrcaTonegen {
    private:
    // params
    const int* note;
    const int* samplerate;
    const double* modify_amount;
    const double* saw_mix;
    const double* pulse_mix;
    const double* noise_mix;
    const double* sub_mix;
    const int* sub_type;

    public:
    OrcaTonegen(const int *note, const int*samplerate, const double* modify_amount ) {
        this->note = note;
        this->samplerate = samplerate;
    };
    double Tick() {
        return 0.0; // enjoy the silence
    };
};
