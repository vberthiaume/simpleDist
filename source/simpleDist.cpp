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
: AudioEffectX (audioMaster, 1, 2)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID (atoi("c48b62e6"));	// identifym used crc32 of "Bonzai Studios - Simple Distortion"
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	m_fGain = 1.f;			// default to 0 dB
	m_fDist = .1f;			
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
		case 0:
			m_fGain = value;
			break;
		case 1: 
			m_fDist = value*75;//-.0999f * value + .1f;
			break;
		default: //throw some exception
			break;
	}
}

//-----------------------------------------------------------------------------------------
float SimpleDist::getParameter (VstInt32 index)
{
	switch (index){
		case 0:
			return m_fGain;
		case 1: 
			return m_fDist/75;//-(m_fDist-.1f)/.0999f;
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
		default:
			//throw some exception
			break;
	}
}

//-----------------------------------------------------------------------------------------
void SimpleDist::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index){
		case 0:
			dB2string (m_fGain, text, kVstMaxParamStrLen);
			break;
		case 1: 
			float2string (m_fDist/75, text, kVstMaxParamStrLen);//(-100*(m_fDist-.1f)/.0999f, text, kVstMaxParamStrLen);
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
		case 0:
			vst_strncpy (label, "dB", kVstMaxParamStrLen);
			break;
		case 1: 
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
	//processReplacingTemplate(inputs, outputs, sampleFrames);
	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame)
	{
		effectFloat.distortionStereo(inputs[0][iCurFrame], outputs[0][iCurFrame], inputs[1][iCurFrame], outputs[1][iCurFrame], m_fDist, m_fGain);
		effectFloat.delayMonoInput(inputs[0][iCurFrame], outputs[0][iCurFrame], outputs[1][iCurFrame]);
	} 
}

//-----------------------------------------------------------------------------------------
void SimpleDist::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	//processReplacingTemplate(inputs, outputs, sampleFrames);
	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame)
	{
		effectDouble.distortionStereo(inputs[0][iCurFrame], outputs[0][iCurFrame], inputs[1][iCurFrame], outputs[1][iCurFrame], m_fDist, m_fGain);
		effectDouble.delayMonoInput(inputs[0][iCurFrame], outputs[0][iCurFrame], outputs[1][iCurFrame]);
	} 
}

template <class T> void SimpleDist::processReplacingTemplate(T** inputs, T** outputs, VstInt32 sampleFrames)
{
	/*for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame)
	{
		effect.distortionSingleChannel(inputs[0][iCurFrame], outputs[0][iCurFrame], m_fDist, m_fGain);
		effect.distortionSingleChannel(inputs[1][iCurFrame], outputs[1][iCurFrame], m_fDist, m_fGain);
	}*/

}





