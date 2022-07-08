#pragma once

#include "config.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "OrcaDSP.h"

const int kNumPresets = 1;

enum EParams
{
  kParamGain = 0,
  kParamTune,
  kParamVolume,
  kParamPortamento,
  kParamPortamentoType, // auto, off, on
  kParamLfoRate,
  kParamLfoWaveform, // tri, square, random, noise
  kParamLfoBend, // what is this?
  kParamVCOMod,
  kParamVCORange, // 16,8,4,2
  kParamVCOBend,
  kParamVCOMode, // mono/poly
  kParamPulseWidthManual,
  kParamPulseSource, // lfo, manual, env
  kParamPulseMix,
  kParamSawMix,
  kParamSubMix,
  kParamSubType, // 1 oct down square, 2 oct down square, 2 oct down pulse
  kParamNoiseMix,
  kParamVCFFreq,
  kParamVCFResonanse,
  kParamVCFEnv,
  kParamVCFMod,
  kParamVCFKeyboard,
  kParamVCFBend,
  kParamVCAType, // env/gate
  kParamEnvAttack,
  kParamEnvDecay,
  kParamEnvSustain,
  kParamEnvRelease,
  
  kNumParams
};

#if IPLUG_DSP
// will use EParams in Orca1_DSP.h

#endif

enum EControlTags
{
  kCtrlTagMeter = 0,
  kCtrlTagLFOVis,
  kCtrlTagScope,
  kCtrlTagRTText,
  kCtrlTagKeyboard,
  kCtrlTagBender,
  kNumCtrlTags
};

using namespace iplug;
using namespace igraphics;

class Orca1 final : public Plugin
{
public:
  Orca1(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData) override;

private:
  OrcaDSP* dsp;
#endif
};
