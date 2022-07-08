#include "Orca1.h"

#include "IPlug_include_in_plug_src.h"

Orca1::Orca1(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGain)->InitDouble("Volume", 80, 0., 100.0, 0.01, "");
  GetParam(kParamPortamentoType)->InitEnum("P-Mode", 0, {"Auto", "Off", "On"} );
  GetParam(kParamLfoWaveform)->InitEnum("LFOWaveform", 0, {"Triangle", "Square", "Random", "Noise"} );
  GetParam(kParamVCORange)->InitEnum("Range", 0, {"16", "8", "4", "2"} );
  GetParam(kParamPulseSource)->InitEnum("Source", 0, {"LFO", "Manual", "Env"} );
  GetParam(kParamSubType)->InitEnum("Sub", 0, {"1 Oct sq", "2 Oct sq", "2 oct pulse"} );
  GetParam(kParamVCAType)->InitEnum("Amp", 0, {"Env", "Gate"} );
  GetParam(kParamVCOMode)->InitEnum("Mode", 0, {"Poly", "Mono"} );
//  kParamPortamento,
  GetParam(kParamPortamento)->InitDouble("Glide", 0., 0., 100.0, 0.01, "");
  
//  kParamLFORate,
  GetParam(kParamLfoRate)->InitDouble("LFO", 100., 0., 100.0, 0.01, "");
  
//  kParamLFOBend, // what is this?
  GetParam(kParamLfoBend)->InitDouble("LFOBend", 100., 0., 100.0, 0.01, "");
  
//  kParamVCOMod,
  GetParam(kParamVCOMod)->InitDouble("Osc mod", 100., 0., 100.0, 0.01, "");
  
//  kParamVCOBend,
  GetParam(kParamVCOBend)->InitDouble("Osc bend", 0., 0., 100.0, 0.01, "");
  
//  kParamPulseWidth,
  GetParam(kParamPulseWidthManual)->InitDouble("PulseWidthManual", 100., 0., 100.0, 0.01, "%");
  
//  kParamMixerPulse,
  GetParam(kParamPulseMix)->InitDouble("Pulse lvl", 100., 0., 100.0, 0.01, "");
  
//  kParamMixerSaw,
  GetParam(kParamSawMix)->InitDouble("Saw lvl", 100., 0., 100.0, 0.01, "");
  
//  kParamMixerSub,
  GetParam(kParamSubMix)->InitDouble("Sub lvl", 100., 0., 100.0, 0.01, "");
  
//  kParamMixerNoise,
  GetParam(kParamNoiseMix)->InitDouble("Noise", 100., 0., 100.0, 0.01, "");
  
//  kParamVCFFreq,
  GetParam(kParamVCFFreq)->InitDouble("Freq", 100., 0., 100.0, 0.01, "");
  
//  kParamVCFResonanse,
  GetParam(kParamVCFResonanse)->InitDouble("Res", 100., 0., 100.0, 0.01, "");
  
//  kParamVCFEnv,
  GetParam(kParamVCFEnv)->InitDouble("FEnv", 100., 0., 100.0, 0.01, "");
  
//  kParamVCFMod,
  GetParam(kParamVCFMod)->InitDouble("FMod", 100., 0., 100.0, 0.01, "");
  
//  kParamVCFKeyboard,
  GetParam(kParamVCFKeyboard)->InitDouble("FKey", 100., 0., 100.0, 0.01, "");
  
//  kParamVCFBend,
  GetParam(kParamVCFBend)->InitDouble("FBend", 100., 0., 100.0, 0.01, "");
  
//  kParamEnvAttack,
  GetParam(kParamEnvAttack)->InitDouble("Attack", 100., 0., 100.0, 0.01, "");
//  kParamEnvDecay,
  GetParam(kParamEnvDecay)->InitDouble("Decay", 100., 0., 100.0, 0.01, "");
//  kParamEnvSustain,
  GetParam(kParamEnvSustain)->InitDouble("Sustain", 100., 0., 100.0, 0.01, "");
//  kParamEnvRelease,
  GetParam(kParamEnvRelease)->InitDouble("Release", 100., 0., 100.0, 0.01, "");
  
  
  GetParam(kParamTune)->InitDouble("Tune", 0., -100., 100.0, 0.05, "");
  
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
    // oscControls
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(0,2,5).GetCentredInside(size), kParamVCORange, "Range"), kNoTag, "Range")->DisablePrompt(false);
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(1,2,5).GetCentredInside(size), kParamPortamento, "Glide",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(2,2,5).GetCentredInside(size), kParamPulseWidthManual, "Width",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVRadioButtonControl(oscControls.GetGridCell(3,2,5).GetCentredInside(size), kParamPulseSource, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(5,2,5).GetCentredInside(size), kParamVCOBend, "Bend",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVRadioButtonControl(oscControls.GetGridCell(6,2,5).GetCentredInside(size), kParamPortamentoType, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
   
    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(7,2,5).GetCentredInside(size), kParamVCOMod, "Mod",
                                               DEFAULT_STYLE.WithShowValue(false)));

    pGraphics->AttachControl(new IVKnobControl(oscControls.GetGridCell(8,2,5).GetCentredInside(size), kParamVCOMode, "Mode"), kNoTag, "Mode")->DisablePrompt(false);
    
        
    // modControls
    pGraphics->AttachControl(new IVKnobControl(modControls.GetGridCell(0,2,5).GetCentredInside(size), kParamLfoRate, "Rate",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(modControls.GetGridCell(1,2,5).GetCentredInside(size), kParamLfoWaveform, {}, "", DEFAULT_STYLE.WithShowLabel(false)));
    
    pGraphics->AttachControl(new IVKnobControl(modControls.GetGridCell(5,2,5).GetCentredInside(size), kParamLfoBend, "Bend",
                                               DEFAULT_STYLE.WithShowValue(false)));

    // mixControls
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(0,1,5).GetCentredInside(size), kParamPulseMix, "Pulse",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(1,1,5).GetCentredInside(size), kParamSawMix, "Saw",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(2,1,5).GetCentredInside(size), kParamSubMix, "Sub",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(mixControls.GetGridCell(3,1,5).GetCentredInside(size), kParamNoiseMix, "Noise",
                                                DEFAULT_STYLE.WithShowValue(false)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(mixControls.GetGridCell(4,1,5).GetCentredInside(size), kParamSubType, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
                 
    // env controls
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(0,1,5).GetCentredInside(size), kParamEnvAttack, "Attack",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(1,1,5).GetCentredInside(size), kParamEnvDecay, "Decay",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(2,1,5).GetCentredInside(size), kParamEnvSustain, "Sustain",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(envControls.GetGridCell(3,1,5).GetCentredInside(size), kParamEnvRelease, "Release",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
    // filterControls
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(0,2,5).GetCentredInside(size), kParamVCFFreq, "Freq",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(1,2,5).GetCentredInside(size), kParamVCFResonanse, "Res",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(5,2,5).GetCentredInside(size), kParamVCFEnv, "Env",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(6,2,5).GetCentredInside(size), kParamVCFMod, "Mod",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(7,2,5).GetCentredInside(size), kParamVCFKeyboard, "Kbd",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(filterControls.GetGridCell(8,2,5).GetCentredInside(size), kParamVCFBend, "Bend",
                                               DEFAULT_STYLE.WithShowValue(false)));
    // pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetVShifted(-75), kParamGain, "Gain",  DEFAULT_STYLE.WithShowValue(false)));
    
    
    // outputControls
    pGraphics->AttachControl(new IVKnobControl(outputControls.GetGridCell(0,2,5).GetCentredInside(size), kParamTune, "Tune",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVRadioButtonControl(outputControls.GetGridCell(1,2,5).GetCentredInside(size), kParamVCAType, {}, "", DEFAULT_STYLE.WithShowLabel(true)));

    pGraphics->AttachControl(new IVKnobControl(outputControls.GetGridCell(2,2,5).GetCentredInside(size), kParamVolume, "Volume",
                                               DEFAULT_STYLE.WithShowValue(false)));
   

    
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
  config.pulseWidthManual = GetParam(kParamPulseWidthManual)->Value() / 100.0;

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
