//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : simpledist.h
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __simpledist__
#define __simpledist__

#include "../../vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.h"
#include "../../vstsdk2.4/public.sdk/source/vst2.x/audioeffect.h"
#include "../../vstsdk2.4/pluginterfaces/vst2.x/aeffect.h"
#include "bmp4AudioLibrary\bmp4AudioLibrary.h"
#include "boost\any.hpp"

//-------------------------------------------------------------------------------------------------------
class SimpleDist : public AudioEffectX
{
public:
	SimpleDist (audioMasterCallback audioMaster);
	~SimpleDist ();

	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
	template <class T> void processReplacingTemplate(T** inputs, T** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	float m_fGain;
	float m_fDist;
	char programName[kVstMaxProgNameLen + 1];
	//bmp4<boost::any> effect;
	bmp4<float> effectFloat;
	bmp4<double> effectDouble;

};

#endif
