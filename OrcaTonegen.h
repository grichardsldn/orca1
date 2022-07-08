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

    // state
    int left;
  int length;
    public:
    OrcaTonegen(const int *note, const int*samplerate, const double* modify_amount ) {
        this->note = note;
        this->samplerate = samplerate;
      left = 0;
      length = 100;
    };
    double Tick() {
      length = 1000 / (*note + 1);
      if (left > 0) {
        left--;
        if (left > length / 2) {
          return -1.0;
        } else {
          return 1.0;
        }
      } else {
        left = length;
        return 0.0;
      }

    };
    void Restart() {
      left = 0;
    }
};
