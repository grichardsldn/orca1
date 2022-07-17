#pragma once

#include "OrcaChannel.h"
#include "LFO.h"
#include "ADSR.h"

#define NUM_CHANNELS (8)
class OrcaDSP {
    public:
    const OrcaConfig* config;

    private:
    //components
    OrcaChannel* channels[8];
    LFO* lfo;

    // state
    double lfoValue;
    double currentNote = 0;

    public:
    // methods
    OrcaDSP(const OrcaConfig* config) {
        this->config = config;
        lfo = new LFO(&config->lfoRate, &config->samplerate, &config->lfoWaveform);
        for (int i = 0; i< 8; i++) {
            channels[i] = new OrcaChannel(config, &lfoValue);
        }
    }

    void NoteOn(int note, int velocity) {
        currentNote = note;
        OrcaChannel *existingChannel = findNote(note);
        if (existingChannel != NULL) {
            existingChannel->Trigger(note, (double)velocity);
            return;
        }
        OrcaChannel *channel = idleChannel();
        if (channel != NULL) {
            channel->Trigger(note, (double)velocity);
            return;
        }   
    };
    void NoteOff(int note) {
        OrcaChannel *channel = findNote(note);
        if (channel != NULL) {
            channel->Release();
        }
    }

    iplug::sample Tick() {
        lfoValue = lfo->Tick();
        iplug::sample output = 0.0;
        for (int i=0;i<8;i++) {
            output += channels[i]->Tick();
        }
        return output;
    };

    private:
    OrcaChannel* idleChannel() {
      for (int i = 0 ; i<NUM_CHANNELS;i++) {
            if (channels[i]->getState() == idle) {
                return channels[i];
            }
        }
        return NULL;
    }
    OrcaChannel* findNote(int note) {
      for (int i = 0 ; i<NUM_CHANNELS;i++) {
            if (channels[i]->getNote() == note) {
                return channels[i];
            }
        }
        return NULL;
    }
};
