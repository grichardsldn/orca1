#pragma once

#include "OrcaChannel.h"
#include "LFO.h"

class OrcaDSP {
    public:
    const OrcaConfig* config;

    private:
    //components
    OrcaChannel* channel;
    LFO* lfo;

    // state
    double lfo_value;

    public:
    // methods
    OrcaDSP(const OrcaConfig* config) {
        this->config = config;
        lfo = new LFO(&config->lfoRate, &config->lfoWaveform);
        channel = new OrcaChannel(config, &lfo_value);
    }

    void NoteOn(int note, int velocity) {
        channel->Trigger(note, (double)velocity);
    };
    void NoteOff(int note) {
        channel->Release();
    }

    iplug::sample Tick() {
        lfo_value = lfo->Tick();
        const iplug::sample channel_output = channel->Tick();
      return (iplug::sample)(channel_output);
        // return (iplug::sample)(channel_output * config.volume);
    };
};
