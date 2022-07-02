#pragma once

struct OrcaConfig {
    // LFO
    double lfo_rate;
    int lfo_waveform;
  
    // global
    double volume;
  
    // internal
    int samplerate;
  
    // mixer
    double pulse_mix;
    double noise_mix;
    double sub_mix;
    int sub_type;
  
    // tone generator
    double pulse_width_manual;
};
