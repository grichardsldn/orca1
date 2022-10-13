#pragma once

#include "OrcaConfig.h"
#include "OrcaTonegen.h"
#include "Filter.h"
#include "ADSR.h"
#include "OrcaTonegen.h"
#include "math.h"

class OrcaChannel {
    private:
    // inputs
    const double *lfo;
    const double *modWheel;
    const OrcaConfig *config;

    // state
    int noteNumber;
    double note;
    double velocity = 0.0;

    // components
    OrcaTonegen* tonegen;
    Filter* filter1;
    Filter* filter2;

    ADSR *adsr;
    ADSR *gate;

    // outputs to subcomponents
    double modifyAmount;
    double pulseWidth = 0.0;
    double filterOctave = 0.0;
    double gateFull = 50000.0;
    double one = 1.0;

    public:
    OrcaChannel(const OrcaConfig *config, const double *lfo, const double *modWheel) {
        this->config = config;
        this->lfo = lfo;
        this->modWheel = modWheel;
          
        tonegen = new OrcaTonegen(&note, &config->samplerate, &config->range, &modifyAmount, &pulseWidth, &config->pulseMix, &config->sawMix, &config->subMix, &config->subType, &config->noiseMix);
        filter1 = new Filter( &config->samplerate, &filterOctave, &config->filterResonance, &config->filterLfo);
        filter2 = new Filter( &config->samplerate, &filterOctave, &config->filterResonance, &config->filterLfo);
        adsr = new ADSR(&config->samplerate, &config->attack, &config->decay, &config->sustain, &config->release);
        gate = new ADSR(&config->samplerate, &gateFull, &gateFull, &one, &config->release);

    };

    int getNote() {
        return noteNumber;
    }

    State getState() {
        return adsr->getState();
    }

    void Trigger( int note, double velocity, int startingNote ) {
        this->noteNumber = note;
        this->note = startingNote;
        this->velocity = (double)velocity;
        this->velocity = velocity;
        this->tonegen->Restart();
        this->adsr->Trigger();
        this->gate->Trigger();
    };
    void Release() {
        this->adsr->Release();
        this->gate->Release();
    };

    double Tick() {
        double noteDiff = noteNumber - note;
        note += noteDiff / (config->samplerate * config->portamento);

        modifyAmount = config->tune + (config->pitchMod * *modWheel * *lfo);

        const double envelope = adsr->Tick();
        const double gateValue = gate->Tick();

        filterOctave = config->filterFrequency
            + (((config->filterEnv * (envelope - 0.5)) + 0.5) * 2.0)
            + (config->filterKey * (((double)note  / 12.0) - 4.0))
            + (config->filterLfo * (*lfo * 4.0))
        ;

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
        
        const double amped = config->ampType == 0 ? 
            (filtered * envelope * velocity)
             : (filtered * gateValue * velocity);

        return amped;
    }
};
