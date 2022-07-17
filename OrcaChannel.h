#pragma once

#include "OrcaConfig.h"
#include "OrcaTonegen.h"
#include "Filter.h"
#include "ADSR.h"
#include "OrcaTonegen.h"

class OrcaChannel {
    private:
    // inputs
    const double *lfo;
    const OrcaConfig *config;

    // state
    int note;
    double velocity;

    // components
    OrcaTonegen* tonegen;
    Filter* filter1;
    Filter* filter2;

    ADSR *adsr;

    // outputs to subcomponents
    double modify_amount;
    double pulseWidth = 0.0;
    double filterAmount = 0.0;
  
    public:
    OrcaChannel(const OrcaConfig *config, const double *lfo) {
        this->config = config;
        this->lfo = lfo;
        tonegen = new OrcaTonegen(&note, &config->samplerate, &modify_amount, &pulseWidth, &config->pulseMix, &config->noiseMix);
        filter1 = new Filter( &config->samplerate, &filterAmount, &config->filterResonance, &config->volume);
        filter2 = new Filter( &config->samplerate, &filterAmount, &config->filterResonance, &config->volume);
        adsr = new ADSR(&config->samplerate, &config->attack, &config->decay, &config->sustain, &config->release);
    };

    int getNote() {
        return note;
    }

    State getState() {
        return adsr->getState();
    }

    void Trigger( int note, double velocity ) {
        this->note = note;
        this->velocity = velocity;
        this->tonegen->Restart();
        this->adsr->Trigger();
    };
    void Release() {
        this->adsr->Release();
    };

    double Tick() {
        const double envelope = adsr->Tick();
        // filterAmount = envelope * config->filterFrequency;
        filterAmount = config->filterFrequency;

        switch (config->pulseSource ) {
            case 0:// lfo
                pulseWidth = config->pulseWidthManual * ((*lfo + 1.0) / 2.0);
            break;
            case 1: // manual
                pulseWidth = config->pulseWidthManual;
            break;
            case 2: // env
                pulseWidth = config->pulseWidthManual * envelope;
            break;
        }
    
        const double raw_tone = tonegen->Tick();
        const double filtered = filter1->Tick(filter2->Tick(raw_tone));
        
        const double attenuation = 1.0 - envelope;
        const double enveloped = filtered / pow(10.0, attenuation * 2.0);

        return enveloped;
    }
};
