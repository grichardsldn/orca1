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
    double lfoValue;
    double currentNote = 0;

    public:
    // methods
    OrcaDSP(const OrcaConfig* config) {
        this->config = config;
        lfo = new LFO(&config->lfoRate, &config->samplerate, &config->lfoWaveform);
        channel = new OrcaChannel(config, &lfoValue);
    }

    void NoteOn(int note, int velocity) {
        currentNote = note;
        channel->Trigger(note, (double)velocity);
    };
    void NoteOff(int note) {
        if (note == currentNote) {
            channel->Release();
        } // else it's some previous note
    }

    iplug::sample Tick() {
        lfoValue = lfo->Tick();
        const iplug::sample channel_output = channel->Tick();
        // return (iplug::sample)((channel_output) * lfoValue);
        return (iplug::sample)(channel_output);
        // return (iplug::sample)(channel_output * config.volume);
    };
};
