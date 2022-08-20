#pragma once

struct OrcaConfig {
    // LFO
    double lfoRate;
    int lfoWaveform;
  
    // global
    double volume;
    int poly;
  
    // internal
    int samplerate;
  
    // mixer
    double pulseMix;
    double noiseMix;
    double sawMix;
    double subMix;
    int subType;

    // amp
    int ampType; // env/gate
  
    // tone generator
    double pulseWidthManual;
    int pulseSource;
    int range;
    double pitchMod;
    double tune;

    // envelope generator
    double attack; // ms to full rise
    double decay; // ms to full fall
    double sustain; // sustain level
    double release; // ms to full fall

    // filter
    double filterFrequency;
    double filterResonance;
    double filterEnv;
    double filterKey;
    double filterLfo;
};
