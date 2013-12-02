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
			m_fDist = value*100;//-.0999f * value + .1f;
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
			return m_fDist/100;//-(m_fDist-.1f)/.0999f;
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
			float2string (m_fDist, text, kVstMaxParamStrLen);//(-100*(m_fDist-.1f)/.0999f, text, kVstMaxParamStrLen);
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
	vst_strncpy (text, "Bonzai Studios", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 SimpleDist::getVendorVersion ()
{ 
	return 1000; 
}

template<class T> T SimpleDist::sign(T &v)
{
	return v > 0.0 ? 1.0 : (v < 0.0 ? -1.0 : 0.0);
}


//-----------------------------------------------------------------------------------------
void SimpleDist::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame){
		//overdriveSingleChannel(inputs[0][iCurFrame], outputs[0][iCurFrame]);
		//overdriveSingleChannel(inputs[1][iCurFrame], outputs[1][iCurFrame]);
		distortionSingleChannel(inputs[0][iCurFrame], outputs[0][iCurFrame]);
		distortionSingleChannel(inputs[1][iCurFrame], outputs[1][iCurFrame]);
	}
}

//-----------------------------------------------------------------------------------------
void SimpleDist::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	for (int iCurFrame = 0; iCurFrame < sampleFrames; ++iCurFrame){
		//overdriveSingleChannel(inputs[0][iCurFrame], outputs[0][iCurFrame]);
		//overdriveSingleChannel(inputs[1][iCurFrame], outputs[1][iCurFrame]);
		distortionSingleChannel(inputs[0][iCurFrame], outputs[0][iCurFrame]);
		distortionSingleChannel(inputs[1][iCurFrame], outputs[1][iCurFrame]);
	}
}


//****************** overdrive as per p. 142 DAFX 2ed ******************
template<class T> void SimpleDist::overdriveSingleChannel(T &p_in, T &p_out)
{
	//safely cast parameters to avoid problems	
	T gain = static_cast<T> (m_fGain);
	T dist = 1.0;//static_cast<T> (m_fDist);

	//mix goes with the gain param for now, but need to implement its own parameter
	//T mix = gain;
	
	//save original frame value
	T originalFrame = p_in * gain;
		      
	// UNDER THRESHOLD
	if (abs(originalFrame) < dist){			
		p_out = gain * 2 * (originalFrame);
	}
		
	// BIGGER THAN 2X TRESHOLD 
	else if (abs(originalFrame) > 2*dist){	
		if (originalFrame > 0) p_out = gain * 1;
		if (originalFrame < 0) p_out = gain * -1;
	}

	//BIGGER THAN THRESHOLD, SMALLER THAN 2X THRESHOLD
	else {							
		if (originalFrame > 0) p_out = gain *  ( 3- pow((2-originalFrame*3), 2) )/3;
		if (originalFrame < 0) p_out = gain * -( 3- pow((2-abs(originalFrame)*3), 2) )/3;
	}

	//p_out = mix * p_out + (1-mix)* originalFrame;
}

//****************** distortion as per p. 144 DAFX 2ed ******************
template<class T> void SimpleDist::distortionSingleChannel(T &p_in, T &p_out) {
	
		//mix is simply the distortion param 
		T mix = static_cast<T> (m_fDist);	//should vary from 0 to 1

		//amplified original sample
		T q	= p_in * static_cast<T> (m_fGain);

		//distorted sample
		T distortedSample = sign(q)*(1-exp(-abs(q)));

		//output is combination of distorted and original samples
		p_out = mix * distortedSample + (1-mix) * q;
				
}

template<class T> void SimpleDist::bypassSingleChannel(T &input, T &output){
		output = input;
}
