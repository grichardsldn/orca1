#pragma once

struct OrcaConfig {
    // LFO
    double lfoRate;
    int lfoWaveform;
  
    // global
    double volume;
  
    // internal
    int samplerate;
  
    // mixer
    double pulseMix;
    double noiseMix;
    double subMix;
    int subType;
  
    // tone generator
    double pulseWidthManual;
};
