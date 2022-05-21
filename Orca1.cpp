#include "Orca1.h"
#include "IPlug_include_in_plug_src.h"

Orca1::Orca1(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGain)->InitDouble("Volume", 100., 0., 100.0, 0.01, "");
  GetParam(kParamPortamentoType)->InitEnum("P-Mode", 0, {"Auto", "Off", "On"} );
  GetParam(kParamLFOWaveform)->InitEnum("LFOWaveform", 0, {"Triangle", "Square", "Random", "Noise"} );
  GetParam(kParamVCORange)->InitEnum("Range", 0, {"16", "8", "4", "2"} );
  GetParam(kParamPulseSource)->InitEnum("PWMSource", 0, {"LFO", "Manual", "Env"} );
  GetParam(kParamSubType)->InitEnum("Sub", 0, {"1 Oct sq", "2 Oct sq", "2 oct pulse"} );
  GetParam(kParamVCAType)->InitEnum("Amp", 0, {"Env", "Gate"} );
  
//  kParamPortamento,
  GetParam(kParamPortamento)->InitDouble("Glide", 100., 0., 100.0, 0.01, "");
  
//  kParamLFORate,
  GetParam(kParamLFORate)->InitDouble("LFO", 100., 0., 100.0, 0.01, "");
  
//  kParamLFOBend, // what is this?
  GetParam(kParamLFOBend)->InitDouble("LFOBend", 100., 0., 100.0, 0.01, "");
  
//  kParamVCOMod,
  GetParam(kParamVCOMod)->InitDouble("Osc mod", 100., 0., 100.0, 0.01, "");
  
//  kParamVCOBend,
  GetParam(kParamVCOBend)->InitDouble("Osc bend", 100., 0., 100.0, 0.01, "");
  
//  kParamPulseWidth,
  GetParam(kParamPulseWidth)->InitDouble("Width", 100., 0., 100.0, 0.01, "%");
  
//  kParamMixerPulse,
  GetParam(kParamMixerPulse)->InitDouble("Pulse lvl", 100., 0., 100.0, 0.01, "");
  
//  kParamMixerSaw,
  GetParam(kParamMixerSaw)->InitDouble("Saw lvl", 100., 0., 100.0, 0.01, "");
  
//  kParamMixerSub,
  GetParam(kParamMixerSub)->InitDouble("Sub lvl", 100., 0., 100.0, 0.01, "");
  
//  kParamMixerNoise,
  GetParam(kParamMixerNoise)->InitDouble("Noise", 100., 0., 100.0, 0.01, "");
  
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
    const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
    // const IRECT lfoPanel = b.GetFromLeft(300.f).GetFromTop(200.f);
    // IRECT keyboardBounds = b.GetFromBottom(300);
    // IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);
    // pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
    // pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5)), kCtrlTagBender);
    // pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5, true), IMidiMsg::EControlChangeMsg::kModWheel));
//    pGraphics->AttachControl(new IVMultiSliderControl<4>(b.GetGridCell(0, 2, 2).GetPadded(-30), "", DEFAULT_STYLE, kParamAttack, EDirection::Vertical, 0.f, 1.f));
    const IRECT controls = b.GetGridCell(0, 8, 8);
    // pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetVShifted(-75), kParamGain, "Gain",  DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetVShifted(-70).GetHShifted(-300), kParamTune, "Tune",
                                               DEFAULT_STYLE.WithShowValue(false)));
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetVShifted(0).GetHShifted(-300), kParamVolume, "Volume",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(controls.GetGridCell(0, 2, 2, 3).GetCentredInside(60), kParamPortamentoType, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(controls.GetGridCell(0, 2, 4, 3).GetCentredInside(60).GetVShifted(100), kParamLFOWaveform, {}, "", DEFAULT_STYLE.WithShowLabel(false)));

    pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(0, 2, 2, 3).GetCentredInside(60).GetVShifted(170), kParamVCORange, "Range"), kNoTag, "Range")->DisablePrompt(false);
    
    pGraphics->AttachControl(new IVRadioButtonControl(controls.GetGridCell(0, 2, 2, 3).GetCentredInside(60).GetVShifted(240), kParamPulseSource, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(controls.GetGridCell(0, 2, 2, 3).GetCentredInside(60).GetVShifted(310), kParamSubType, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
    
    pGraphics->AttachControl(new IVRadioButtonControl(controls.GetGridCell(0, 2, 2, 3).GetCentredInside(60).GetVShifted(380), kParamVCAType, {}, "", DEFAULT_STYLE.WithShowLabel(true)));
    
    
    
//    kParamPortamento,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-230).GetVShifted(-210), kParamPortamento, "Glide",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
//    kParamLFORate,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-230).GetVShifted(-140), kParamLFORate, "LFO",
                                               DEFAULT_STYLE.WithShowValue(false)));

//    kParamLFOBend, // what is this?
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-230).GetVShifted(-70), kParamLFOBend, "LFOBend",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCOMod,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-230).GetVShifted(0), kParamVCOMod, "PMod",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCOBend,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-230).GetVShifted(70), kParamVCOBend, "PMod",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamPulseWidth,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-230).GetVShifted(140), kParamPulseWidth, "Width",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamMixerPulse,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-160).GetVShifted(-210), kParamMixerPulse, "vPulse",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
//    kParamMixerSaw,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-160).GetVShifted(-140), kParamMixerSaw, "Saw",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamMixerSub,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-160).GetVShifted(-70), kParamMixerSub, "Sub",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamMixerNoise,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-160).GetVShifted(0), kParamMixerNoise, "Noise",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCFFreq,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-90).GetVShifted(-210), kParamVCFFreq, "Freq",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCFResonanse,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-90).GetVShifted(-140), kParamVCFResonanse, "Res",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCFEnv,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-90).GetVShifted(-70), kParamVCFEnv, "Env",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCFMod,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-90).GetVShifted(0), kParamVCFMod, "Mod",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCFKeyboard,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-90).GetVShifted(70), kParamVCFKeyboard, "Kbd",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamVCFBend,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-90).GetVShifted(140), kParamVCFBend, "Bend",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamEnvAttack,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-20).GetVShifted(-210), kParamEnvAttack, "Attack",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamEnvDecay,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-20).GetVShifted(-140), kParamEnvDecay, "Decay",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamEnvSustain,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-20).GetVShifted(-70), kParamEnvSustain, "Sustain",
                                               DEFAULT_STYLE.WithShowValue(false)));
//    kParamEnvRelease,
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(70).GetHShifted(-20).GetVShifted(0), kParamEnvRelease, "Release",
                                               DEFAULT_STYLE.WithShowValue(false)));
    
    // pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(0, 2, 2, 3).GetCentredInside(60), kParamPortamentoType, "Mode"), kNoTag, "Portamento")->DisablePrompt(false);
    
  
    //pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(0, 2, 6).GetCentredInside(90), kParamGain, "Gain"));
//#ifdef OS_IOS
//    if(!IsOOPAuv3AppExtension())
//    {
//      pGraphics->AttachControl(new IVButtonControl(b.GetFromTRHC(100, 100), [pGraphics](IControl* pCaller) {
//                               dynamic_cast<IGraphicsIOS*>(pGraphics)->LaunchBluetoothMidiDialog(pCaller->GetRECT().L, pCaller->GetRECT().MH());
//                               SplashClickActionFunc(pCaller);
//                             }, "BTMIDI"));
//    }
//#endif
    
    pGraphics->SetQwertyMidiKeyHandlerFunc([pGraphics](const IMidiMsg& msg) {
                                              pGraphics->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>()->SetNoteFromMidi(msg.NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
                                           });
  };
#endif
}

#if IPLUG_DSP
void Orca1::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
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
