#include "Orca1.h"

#include "IPlug_include_in_plug_src.h"
#include "version.h"

Orca1::Orca1(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamPortamentoType)->InitEnum("P-Mode", 1, {"Auto", "Off", "On"} );
  GetParam(kParamLfoWaveform)->InitEnum("LFOWaveform", 0, {"Triangle", "Square", "Step", "Drift"} );
  GetParam(kParamRange)->InitEnum("Range", 0, {"16", "8", "4", "2"} );
  GetParam(kParamPulseSource)->InitEnum("Source", 0, {"LFO", "Manual", "Env"} );
  GetParam(kParamSubType)->InitEnum("Sub", 0, {"-1", "-2", "-2+"} );
  GetParam(kParamAmpType)->InitEnum("Amp", 0, {"Env", "Gate+Rel"} );
  GetParam(kParamPoly)->InitEnum("Mode", 0, {"Poly", "Mono"} );
//  kParamPortamento,
  GetParam(kParamPortamento)->InitDouble("Glide", 0.01, 0.01, 0.5, 0.005, "");
  
//  kParamLFORate,
  GetParam(kParamLfoRate)->InitDouble("LfoRate", 0.3, 0.05, 10.0, 0.01, "");
  
  GetParam(kParamLfoLinked)->InitEnum("LFO", 0, {"Linked", "Separate"} );
  
  GetParam(kParamPitchMod)->InitDouble("Pitch mod", 0.4, 0., 6.0, 0.01, "");
  
  GetParam(kParamPitchBend)->InitDouble("Pitch bend", 2.0, 0.0, 12.0, 0.01, "");
  
//  kParamPulseWidth,
  GetParam(kParamPulseWidthManual)->InitDouble("PulseWidthManual", .5, 0., 0.7, 0.01, "%");
  
//  kParamMixerPulse,
  GetParam(kParamPulseMix)->InitDouble("PulseMix", 1., 0., 1.0, 0.01, "",IParam::kFlagsNone, "Mix", IParam::ShapePowCurve(2.));
  
//  kParamMixerSaw,
  GetParam(kParamSawMix)->InitDouble("SawMix", 0., 0., 1.0, 0.01, "", IParam::kFlagsNone, "Mix", IParam::ShapePowCurve(2.));
  
//  kParamMixerSub,
  GetParam(kParamSubMix)->InitDouble("SubMix", 0., 0., 1.0, 0.01, "", IParam::kFlagsNone, "Mix", IParam::ShapePowCurve(2.));
  
//  kParamMixerNoise,
  GetParam(kParamNoiseMix)->InitDouble("NoiseMix", 0., 0., 1.0, 0.01, "",IParam::kFlagsNone, "Mix", IParam::ShapePowCurve(2.));
  
//  kParamVCFFreq,
  GetParam(kParamFilterFrequency)->InitDouble("Freq", 4.0, 0., 8., 0.1, "");
  
//  kParamVCFResonanse,
  GetParam(kParamFilterResonance)->InitDouble("Res", 0.5, 0., 1.0, 0.01, "");
  
  GetParam(kParamFilterEnv)->InitDouble("FEnv", 0.5, 0., 1.0, 0.01, "");
  
  GetParam(kParamFilterLfo)->InitDouble("FLFO", 0.0, 0., 1.0, 0.01, "");
  
  GetParam(kParamFilterKey)->InitDouble("FKey", 0.5, 0., 1.0, 0.01, "");
  
  GetParam(kParamFilterBend)->InitDouble("Filter Bend", 0., 0., 24.0, 0.01, "");
  
  // env generator
  GetParam(kParamAttack)->InitDouble("Attack", 50000., 200.0, 50000.0, 0.01, "",IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(4.));
  
  GetParam(kParamDecay)->InitDouble("Decay", 25000., 200.0, 25000.0, 0.01, "",IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(4.));
  GetParam(kParamSustain)->InitDouble("Sustain", 1.0, 0., 1.0, 0.01, "",IParam::kFlagsNone, "ADSR");
  GetParam(kParamRelease)->InitDouble("Release", 20000., 50.0, 20000.0, 0.01, "",IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(4.));
  
  GetParam(kParamTune)->InitDouble("Tune", 0., -1., 1., 0.05, "");
  GetParam(kParamVolume)->InitDouble("Volume", 0.5,0.0,1.0,0.1,"");

  // GetParam(kParamNoteGlideTime)->InitMilliseconds("Note Glide Time", 0., 0.0, 30.);
  // GetParam(kParamAttack)->InitDouble("Attack", 10., 1., 1000., 0.1, "ms", IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(3.));
  
    
#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);
    
#ifdef OS_WEB
    pGraphics->AttachPopupMenuControl();
#endif

//    pGraphics->EnableLiveEdit(true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IText bigLabel {36, COLOR_WHITE, "Roboto-Regular", EAlign::Near, EVAlign::Top, 0};
    const IText smallLabel {18, COLOR_DARK_GRAY, "Roboto-Regular", EAlign::Near, EVAlign::Top, 0};
    
    const IRECT b = pGraphics->GetBounds().GetPadded(-10.f);
    // const IRECT lfoPanel = b.GetFromLeft(300.f).GetFromTop(200.f);
    // IRECT keyboardBounds = b.GetFromBottom(300);
    // IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);
    // pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
    // pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5)), kCtrlTagBender);
    // pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5, true), IMidiMsg::EControlChangeMsg::kModWheel));
//    pGraphics->AttachControl(new IVMultiSliderControl<4>(b.GetGridCell(0, 2, 2).GetPadded(-30), "", DEFAULT_STYLE, kParamAttack, EDirection::Vertical, 0.f, 1.f));
    const IRECT controls = b.GetGridCell(0, 8, 8);
    const IRECT oscControls = b.GetGridCell(0,1,5);
    const IRECT modControls = b.GetGridCell(1,1,5);
    const IRECT envControls = b.GetGridCell(2,1,5).GetGridCell(0,2,1);
    const IRECT mixControls = b.GetGridCell(2,1,5).GetGridCell(1,2,1);
    const IRECT filterControls = b.GetGridCell(3,1,5);
    const IRECT outputControls = b.GetGridCell(4,1,5);
    const int size = 50;

    const IVStyle YELLOW_STYLE = DEFAULT_STYLE.WithColor(kFG, COLOR_YELLOW);
    const IVStyle WHITE_STYLE = DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE);

    // oscControls
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(0,2,5).GetCentredInside(size), kParamRange, "Range", DEFAULT_STYLE.WithShowValue(false)), kNoTag, "")->DisablePrompt(true);
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(1,2,5).GetCentredInside(size), kParamPortamento, "Glide",
                                               YELLOW_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(2,2,5).GetCentredInside(size), kParamPulseWidthManual, "Width",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVRadioButtonControl(oscControls.GetGridCell(3,2,5).GetCentredInside(size), kParamPulseSource, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(5,2,5).GetCentredInside(size), kParamPitchBend, "Bend",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVRadioButtonControl(oscControls.GetGridCell(6,2,5).GetCentredInside(size), kParamPortamentoType, {}, "", YELLOW_STYLE.WithShowLabel(true)));
   
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(7,2,5).GetCentredInside(size), kParamPitchMod, "Mod",
                                               DEFAULT_STYLE.WithShowValue(false)));

    pGraphics->AttachControl(new IVRadioButtonControl(oscControls.GetGridCell(8,2,5).GetCentredInside(size), kParamPoly, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
   
        
    // modControls
    pGraphics->AttachControl(new IVKnobControl(modControls.GetGridCell(0,2,5).GetCentredInside(size), kParamLfoRate, "LFO",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(modControls.GetGridCell(1,2,5,EDirection::Horizontal,2).GetCentredInside(size * 2, size), kParamLfoWaveform, {}, "", DEFAULT_STYLE.WithShowLabel(false)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(modControls.GetGridCell(5,2,5,EDirection::Horizontal,2).GetCentredInside(size * 2, size), kParamLfoLinked, {}, "", DEFAULT_STYLE.WithShowLabel(false)));
    // the label
    pGraphics->AttachControl(new ITextControl(modControls.GetGridCell(7,2,5,EDirection::Horizontal,2).GetCentredInside(size * 2, size), " ORCA-1", bigLabel));
  

    // mixControls
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(0,1,5).GetCentredInside(size), kParamPulseMix, "Pulse",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(1,1,5).GetCentredInside(size), kParamSawMix, "Saw",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(3,1,5).GetCentredInside(size), kParamSubMix, "Sub",
                                               YELLOW_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(2,1,5).GetCentredInside(size), kParamNoiseMix, "Noise",
                                                DEFAULT_STYLE.WithShowValue(false)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(mixControls.GetGridCell(4,1,5).GetCentredInside(size), kParamSubType, {}, "", YELLOW_STYLE.WithShowLabel(false)));
                 
    // env controls
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(0,1,5).GetCentredInside(size), kParamAttack, "Attack",
                                               WHITE_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(1,1,5).GetCentredInside(size), kParamDecay, "Decay",
                                               WHITE_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(2,1,5).GetCentredInside(size), kParamSustain, "Sustain",
                                               WHITE_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(3,1,5).GetCentredInside(size), kParamRelease, "Release",
                                               WHITE_STYLE.WithShowValue(false)));
    
    // filterControls
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(0,2,5).GetCentredInside(size), kParamFilterFrequency, "Freq",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(1,2,5).GetCentredInside(size), kParamFilterResonance, "Res",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(5,2,5).GetCentredInside(size), kParamFilterEnv, "Env",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(6,2,5).GetCentredInside(size), kParamFilterLfo, "LFO",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(7,2,5).GetCentredInside(size), kParamFilterKey, "Kbd",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(8,2,5).GetCentredInside(size), kParamFilterBend, "Bend",
                                               DEFAULT_STYLE.WithShowValue(false)));
    // pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetVShifted(-75), kParamGain, "Gain",  DEFAULT_STYLE.WithShowValue(false)));
    
    
    // outputControls
    pGraphics->AttachControl(new IVKnobControl(outputControls.GetGridCell(0,2,5).GetCentredInside(size), kParamTune, "Tune",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVRadioButtonControl(outputControls.GetGridCell(1,2,5,EDirection::Horizontal,2).GetCentredInside(size * 2, size), kParamAmpType, {}, "", YELLOW_STYLE.WithShowLabel(true)));

    pGraphics->AttachControl(new IVKnobControl(outputControls.GetGridCell(3,2,5).GetCentredInside(size), kParamVolume, "Volume",
                                               DEFAULT_STYLE.WithShowValue(false)));
                                               
    pGraphics->AttachControl(new ITextControl(outputControls.GetGridCell(5,2,5,EDirection::Horizontal,2).GetCentredInside(size).GetHShifted(-18.0).GetVShifted(31.0), ORCA1_VERSION, smallLabel));
  
   

    // pGraphics->SetQwertyMidiKeyHandlerFunc([pGraphics](const IMidiMsg& msg) {
                           //                   //pGraphics->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>()->SetNoteFromMidi(msg// // .NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
                             //              });
  };
#endif
  
#if IPLUG_DSP
  dsp = new OrcaDSP(&config);
#endif
}

#if IPLUG_DSP
void Orca1::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  //const double gain = GetParam(kGain)->Value() / 100.;

  // tone generator
  config.pulseWidthManual = GetParam(kParamPulseWidthManual)->Value();
  config.pulseSource = GetParam(kParamPulseSource)->Int();
  config.pulseMix = GetParam(kParamPulseMix)->Value();
  config.sawMix = GetParam(kParamSawMix)->Value();
  config.subMix = GetParam(kParamSubMix)->Value();
  config.subType = GetParam(kParamSubType)->Value();
  config.noiseMix = GetParam(kParamNoiseMix)->Value();
  config.range = GetParam(kParamRange)->Value();
  config.tune = GetParam(kParamTune)->Value();
  config.pitchMod = GetParam(kParamPitchMod)->Value();
  config.pitchBend = GetParam(kParamPitchBend)->Value();

  // lfo
  config.lfoRate = GetParam(kParamLfoRate)->Value();
  config.lfoWaveform = GetParam(kParamLfoWaveform)->Value();
  config.lfoLinked = GetParam(kParamLfoLinked)->Value();

  // env generator
  config.attack = GetParam(kParamAttack)->Value();
  config.decay = GetParam(kParamDecay)->Value();
  config.sustain = GetParam(kParamSustain)->Value();
  config.release = GetParam(kParamRelease)->Value();
  // filter
  config.filterFrequency = GetParam(kParamFilterFrequency)->Value();
  config.filterResonance = GetParam(kParamFilterResonance)->Value();
  config.filterEnv = GetParam(kParamFilterEnv)->Value();
  config.filterKey = GetParam(kParamFilterKey)->Value();
  config.filterLfo = GetParam(kParamFilterLfo)->Value();
  config.filterBend = GetParam(kParamFilterBend)->Value();

  // global
  config.volume = GetParam(kParamVolume)->Value();
  config.poly = GetParam(kParamPoly)->Int();
  config.portamentoType = GetParam(kParamPortamentoType)->Int();
  config.portamento = GetParam(kParamPortamento)->Value();

  // amp
  config.ampType = GetParam(kParamAmpType)->Int();

  const int nChans = NOutChansConnected();
  config.samplerate = GetSampleRate();
  
  for (int s = 0; s < nFrames; s++) {
    const iplug::sample output = dsp->Tick();
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = output;
    }
  }
}

void Orca1::OnIdle()
{
}

void Orca1::OnReset()
{
}

void Orca1::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  
  int status = msg.StatusMsg();
  
  switch (status)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
    case IMidiMsg::kPolyAftertouch:
    case IMidiMsg::kControlChange:
    case IMidiMsg::kProgramChange:
    case IMidiMsg::kChannelAftertouch:
    case IMidiMsg::kPitchWheel:
    {
      goto handle;
    }
    default:
      return;
  }
  
handle:
  if(status == IMidiMsg::kNoteOn) {
    dsp->NoteOn(msg.NoteNumber(), msg.Velocity());
  }
  if (status == IMidiMsg::kNoteOff) {
    dsp->NoteOff(msg.NoteNumber());
  }
  if (status == IMidiMsg::kControlChange) {
    if (msg.ControlChangeIdx() == IMidiMsg::kModWheel) {
      dsp->ModWheel(msg.ControlChange(IMidiMsg::kModWheel));
    }
  }
  if (status == IMidiMsg::kPitchWheel) {
    dsp->BendWheel(msg.PitchWheel());
  }
  // mDSP.ProcessMidiMsg(msg);
  SendMidiMsg(msg);
}

void Orca1::OnParamChange(int paramIdx)
{
  // mDSP.SetParam(paramIdx, GetParam(paramIdx)->Value());
}

bool Orca1::OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData)
{
  if(ctrlTag == kCtrlTagBender && msgTag == IWheelControl::kMessageTagSetPitchBendRange)
  {
    const int bendRange = *static_cast<const int*>(pData);
    // mDSP.mSynth.SetPitchBendRange(bendRange);
  }
  
  return false;
}
#endif
