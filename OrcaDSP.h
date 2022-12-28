#pragma once

#include "OrcaChannel.h"
#include "LFO.h"
#include "ADSR.h"
#include "Protector.h"

#define NUM_CHANNELS (8)
class OrcaDSP {
    public:
    const OrcaConfig* config;

    private:
    //components
    OrcaChannel* channels[8];
    LFO* lfo[4];
    double modWheel = 0.0;
    double bendWheel = 0.0;
    Protector protector;

    // state
    double lfoValue[4];
    double currentNote = -1;

    public:
    // methods
    OrcaDSP(const OrcaConfig* config) {
        this->config = config;

        for (int i = 0; i < 4; i++) {
             lfo[i] = new LFO(i / 4.0, &config->lfoRate, &config->samplerate, &config->lfoWaveform);
        }

        for (int i = 0; i< 8; i++) {
            channels[i] = new OrcaChannel(config, &lfoValue[i % 4], &modWheel, &bendWheel);
        }
    }
    void ModWheel(double value) {
        modWheel = value;
    }
    void BendWheel(double value) {
        bendWheel = value;
    }
    void NoteOn(int note, int velocity) {
        if (currentNote == -1) {
            currentNote = note;
        }
        if (config->portamentoType == 1) { // off
            currentNote = note;
        }
        if (config->portamentoType == 0) { // auto
            if (!anyChannelGateOn()) {
                currentNote = note;
            }
        }
        if (config->poly == 1) {
            
            channels[0]->Trigger(note, (double)velocity / 127.0, currentNote);
            currentNote = note;
        } else {
            OrcaChannel *existingChannel = findNote(note);
            if (existingChannel != NULL) {
                existingChannel->Trigger(note, (double)velocity/127.0, currentNote);
                currentNote = note;
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
                channel->Trigger(note, (double)velocity / 127.0, currentNote);
                currentNote = note;
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
        if (config->lfoLinked == 0) { // LINKED 
            double lfo1 = lfo[0]->Tick();
            for (int i = 0 ; i  < 4; i++) {
                lfoValue[i] = lfo1;
             }
        } else {
            for (int i = 0 ; i  < 4; i++) {
                lfoValue[i] = lfo[i]->Tick();
            }
        }

        iplug::sample output = 0.0;
        int numChannels = (config->poly == 1) ? 1:8;
        for (int i=0;i<numChannels;i++) {
            output += channels[i]->Tick();
        }
        return protector.Tick(output);
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
    OrcaChannel* findNote(int note) {
      for (int i = 0 ; i<NUM_CHANNELS;i++) {
            if (channels[i]->getNote() == note) {
                return channels[i];
            }
        }
        return NULL;
    }
    bool anyChannelGateOn() {
        for (int i = 0 ; i<NUM_CHANNELS;i++) {
            int state = channels[i]->getState();

            if (state != idle && state != release) {
                return true;
            }
        }
        return false;
    }
};
