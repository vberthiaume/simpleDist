//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : simpledist.cpp
// Created by   : Bonzai Studios (Vincent Berthiaume)
// Description  : Stereo plugin which applies distortion
//
// © 2013, Vincent Berthiaume, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "simpleDist.h"
#include "math.h"
#include "stdlib.h"
#include "bmp4AudioLibrary\bmp4AudioLibrary.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SimpleDist (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
SimpleDist::SimpleDist (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 4)	// 1 program, 4 parameters
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID (atoi("c48b62e6"));	// identifym used crc32 of "Bonzai Studios - Simple Distortion"
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	//distortion parameters
	m_fGain = 1.f;			// default to 0 dB
	m_fDist = .1f;			

	//delay parameters
	m_lDelay = 0;
	m_fFeedBack = 0;
	

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}


//-------------------------------------------------------------------------------------------------------
SimpleDist::~SimpleDist ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void SimpleDist::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void SimpleDist::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void SimpleDist::setParameter (VstInt32 index, float value)
{
	switch (index){
	case kGain:
		m_fGain = value;
		break;
	case kDist:
		m_fDist = value*75; 
		break;
	case kDelay:
		m_lDelay = (long)(value * (44100.f - 1.f));
		break;
	case kFeedBack:
		m_fFeedBack = value;
		break;

	default: 
		//throw some exception
		break;
	}
}

//-----------------------------------------------------------------------------------------
float SimpleDist::getParameter (VstInt32 index)
{
	switch (index){
	case kGain:
		return m_fGain;
	case kDist: 
		return m_fDist/75;
	case kDelay:
		return static_cast<float> (m_lDelay / (44100.f - 1.f));
		break;
	case kFeedBack:
		return m_fFeedBack;
		break;
	default:
		return -1;
	}
}

//-----------------------------------------------------------------------------------------
void SimpleDist::getParameterName (VstInt32 index, char* label)
{
	switch (index){
	case 0:
		vst_strncpy (label, "Gain", kVstMaxParamStrLen);
		break;
	case 1: 
		vst_strncpy (label, "Distortion", kVstMaxParamStrLen);
		break;
	case kDelay:
		vst_strncpy (label, "Delay", kVstMaxParamStrLen);
		break;
	case kFeedBack:
		vst_strncpy (label, "Delay feedback", kVstMaxParamStrLen);
		break;
	default:
		//throw some exception
		break;
	}
}

//-----------------------------------------------------------------------------------------
void SimpleDist::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index){
	case kGain:
		dB2string (m_fGain, text, kVstMaxParamStrLen);
		break;
	case kDist: 
		float2string (m_fDist/75, text, kVstMaxParamStrLen);
		break;
	case kDelay:
		float2string (static_cast<float> (m_lDelay / (44100.f - 1.f)), text, kVstMaxParamStrLen);
		break;
	case kFeedBack:
		float2string (m_fFeedBack, text, kVstMaxParamStrLen);
		break;
	default:
		//throw some exception
		break;
	}
}

//-----------------------------------------------------------------------------------------
void SimpleDist::getParameterLabel (VstInt32 index, char* label)
{
	
	switch (index){
	case kGain:
		vst_strncpy (label, "dB", kVstMaxParamStrLen);
		break;
	case kDist: 
		vst_strncpy (label, "%", kVstMaxParamStrLen);
		break;
	case kDelay:
		vst_strncpy (label, "%", kVstMaxParamStrLen);
		break;
	case kFeedBack:
		vst_strncpy (label, "%", kVstMaxParamStrLen);
		break;
	default:
		//throw some exception
		break;
	}
}

//------------------------------------------------------------------------
bool SimpleDist::getEffectName (char* name)
{
	vst_strncpy (name, "Simple Distortion", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool SimpleDist::getProductString (char* text)
{
	vst_strncpy (text, "Simple Distortion", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool SimpleDist::getVendorString (char* text)
{
	vst_strncpy (text, "BMP4", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 SimpleDist::getVendorVersion ()
{ 
	return 1000; 
}


//-----------------------------------------------------------------------------------------
void SimpleDist::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame)
	{
		effectFloat.distortionStereo(inputs[0][iCurFrame], outputs[0][iCurFrame], inputs[1][iCurFrame], outputs[1][iCurFrame], m_fDist, m_fGain);
		effectFloat.delayMonoInput(inputs[0][iCurFrame], outputs[0][iCurFrame], outputs[1][iCurFrame], m_lDelay, m_fFeedBack);
	} 
}

//-----------------------------------------------------------------------------------------
void SimpleDist::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame)
	{
		effectDouble.distortionStereo(inputs[0][iCurFrame], outputs[0][iCurFrame], inputs[1][iCurFrame], outputs[1][iCurFrame], m_fDist, m_fGain);
		effectDouble.delayMonoInput(inputs[0][iCurFrame], outputs[0][iCurFrame], outputs[1][iCurFrame], m_lDelay, m_fFeedBack);
	} 
}

//TODO: processReplacingTemplate should be called in both processReplacing and processReplacingTemplate in a truly generic case
//template <class T> void SimpleDist::processReplacingTemplate(T** inputs, T** outputs, VstInt32 sampleFrames)
//{
//	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame) {
//		effect.distortionMono(inputs[0][iCurFrame], outputs[0][iCurFrame], m_fDist, m_fGain);
//		effect.distortionMono(inputs[1][iCurFrame], outputs[1][iCurFrame], m_fDist, m_fGain);
//	}
//
//}





