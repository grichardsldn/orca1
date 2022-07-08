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
    Filter* filter;
    ADSR *adsr;

    // outputs to subcomponents
    double modify_amount;
    public:
    OrcaChannel(const OrcaConfig *config, const double *lfo) {
        this->config = config;
        this->lfo = lfo;

        tonegen = new OrcaTonegen(&note, &config->samplerate, &modify_amount); 
        filter = new Filter( &config->samplerate);
        adsr = new ADSR(&config->samplerate);
    };

    void Trigger( int note, double velocity ) {
        this->note = note;
        this->velocity = velocity;
        this->adsr->Trigger();
    };
    void Release() {
        this->adsr->Release();
    };

    double Tick() {
        // set the modify amount
        modify_amount = config->pulse_width_manual; // for now stick to 'manual'   

        const double raw_tone = tonegen->Tick();
        const double filtered = filter->Tick(raw_tone);
      const double envelope = adsr->Tick(filtered);
        const double enveloped = filtered * envelope;
        return enveloped;
    }
};
