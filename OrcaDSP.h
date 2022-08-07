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
    double modWheel = 0.0;

    // state
    double lfoValue;
    double currentNote = 0;

    public:
    // methods
    OrcaDSP(const OrcaConfig* config) {
        this->config = config;
        lfo = new LFO(&config->lfoRate, &config->samplerate, &config->lfoWaveform);
        for (int i = 0; i< 8; i++) {
            channels[i] = new OrcaChannel(config, &lfoValue, &modWheel);
        }
    }
    void ModWheel(double value) {
        modWheel = value;
    }
    void NoteOn(int note, int velocity) {
        if (config->poly == 1) {
            channels[0]->Trigger(note, (double)velocity / 127.0);
        } else {
            currentNote = note;
            OrcaChannel *existingChannel = findNote(note);
            if (existingChannel != NULL) {
                existingChannel->Trigger(note, (double)velocity/127.0);
                return;
            }
            OrcaChannel *channel = findChannel(idle);
            if (channel == NULL) {
                channel = findChannel(release);
            }
            if (channel == NULL) {
                channel = findChannel(sustain);
            }
            if (channel == NULL) {
                channel = findChannel(decay);
            }
            if (channel == NULL) {
                channel = findChannel(attack);
            }
            if (channel != NULL) {
                channel->Trigger(note, (double)velocity / 127.0);
            }
            return;
        }
    };

    void NoteOff(int note) {
        if (config->poly == 1) {
            if (channels[0]->getNote() == note) {
                channels[0]->Release();
            }
        } else {
            OrcaChannel *channel = findNote(note);
            if (channel != NULL) {
                channel->Release();
            }
        }
        
    }

    iplug::sample Tick() {
        lfoValue = lfo->Tick();
        iplug::sample output = 0.0;
        int numChannels = (config->poly == 1) ? 1:8;
        for (int i=0;i<numChannels;i++) {
            output += channels[i]->Tick();
        }
        return output;
    };

    private:
    OrcaChannel* findChannel(State state) {
      for (int i = 0 ; i<NUM_CHANNELS;i++) {
            if (channels[i]->getState() == state) {
                return channels[i];
            }
        }
        return NULL;  
    }
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
