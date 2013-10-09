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

	fGain = 1.f;			// default to 0 dB
	fDist = .1f;			
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
			fGain = value;
			break;
		case 1: 
			fDist = -.0999f * value + .1f;//value*100;
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
			return fGain;
		case 1: 
			return -(fDist-.1f)/.0999f; //fDist/100;
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
			dB2string (fGain, text, kVstMaxParamStrLen);
			break;
		case 1: 
			float2string (-100*(fDist-.1f)/.0999f, text, kVstMaxParamStrLen);
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

float SimpleDist::sign(float v)
{
	return v > 0 ? 1.f : (v < 0 ? -1.f : 0.f);
}

double SimpleDist::sign(double v)
{
	return v > 0 ? 1 : (v < 0 ? -1 : 0);
}

//-----------------------------------------------------------------------------------------
void SimpleDist::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	float temp1, temp2;// q, z, mix = 1.;

    while (--sampleFrames >= 0)
    {
        //(*out1++) = (*in1++) * fGain;
        //(*out2++) = (*in2++) * fGain;
		
	
		//temp1 = (*in1++) * fGain;
  //      temp2 = (*in2++) * fGain;
	
		// ******** distortion as per early DSP musically speaking      *********
		// ******** to activate need to set fDist *100 and get it /100  *********
		//if (temp1 * fDist > 1.0)
		//	(*out1++) = 1.0;
		//else if (temp1 * fDist < -1.0)
		//	(*out1++) = -1.0;
		//else
		//	(*out1++) = temp1;

		//if (temp2 * fDist > 1.0)
		//	(*out2++) = 1.0;
		//else if (temp2 * fDist < -1.0)
		//	(*out2++) = -1.0;
		//else
		//	(*out2++) = temp2;

		//****************** overdrive as per p. 142 DAFX 2ed ******************
		temp1 = (*in1++);// * fGain;
        temp2 = (*in2++);// * fGain;
		//fDist = 1.f/1.f;
		
		if (abs(temp1) < fDist)			// UNDER THRESHOLD
			(*out1++) = fGain * 2 * (temp1);
		else if (abs(temp1) > 2*fDist){	// BIGGER THAN 2X TRESHOLD 
			if (temp1 > 0) (*out1++) = fGain * 1;
			if (temp1 < 0) (*out1++) = fGain * -1;
		}
		else{							//BIGGER THAN THRESHOLD, SMALLER THAN 2X THRESHOLD
			if (temp1 > 0) (*out1++) =  fGain * ( 3- pow((2-temp1*3), 2) )/3;
			if (temp1 < 0) (*out1++) =  fGain * -( 3- pow((2-abs(temp1)*3), 2) )/3;
		}

		if (abs(temp2) < fDist)			// UNDER THRESHOLD
			(*out2++) = fGain * 2 * (temp2);
		else if (abs(temp2) > 2*fDist){	// BIGGER THAN 2X TRESHOLD 
			if (temp2 > 0) (*out2++) = fGain * 1;
			if (temp2 < 0) (*out2++) = fGain * -1;
		}
		else{							//BIGGER THAN THRESHOLD, SMALLER THAN 2X THRESHOLD
			if (temp2 > 0) (*out2++) = fGain *  ( 3- pow((2-temp2*3), 2) )/3;
			if (temp2 < 0) (*out2++) = fGain * -( 3- pow((2-abs(temp2)*3), 2) )/3;
		}

		//****************** distortion as per p. 144 DAFX 2ed ******************
		
		//q = temp1*fDist;
		//z = sign(q)*(1-exp(-abs(q)));
		//(*out1++) = mix*z+(1-mix)*temp1;

		//q = temp2*fDist;
		//z = sign(q)*(1-exp(-abs(q)));
		//(*out2++) = mix*z+(1-mix)*temp2;

    }
}



//-----------------------------------------------------------------------------------------
void SimpleDist::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double dGain = fGain;
	double temp1, temp2;//, q, z, mix = 1.;

    while (--sampleFrames >= 0)
    {
  //      //(*out1++) = (*in1++) * dGain;
  //      //(*out2++) = (*in2++) * dGain;
		//temp1 = (*in1++) * dGain ;
       // temp2 = (*in2++) * dGain;

		//	
		//	if (temp1 * fDist > 1.0)
		//		(*out1++) = 1.0;
		//	else if (temp1 * fDist < -1.0)
		//		(*out1++) = -1.0;
		//	else
		//		(*out1++) = temp1;

		//	if (temp2 * fDist > 1.0)
		//		(*out2++) = 1.0;
		//	else if (temp2 * fDist < -1.0)
		//		(*out2++) = -1.0;
		//	else
		//		(*out2++) = temp2;

		//****************** overdrive as per p. 142 DAFX 2ed ******************
		temp1 = (*in1++);// * dGain ;
        temp2 = (*in2++);// * dGain;
		//fDist = 1.f/10000.f;
		if (abs(temp1) < fDist)			// UNDER THRESHOLD
			(*out1++) = dGain * 2 * (temp1);
		else if (abs(temp1) > 2*fDist){	// BIGGER THAN 2X TRESHOLD 
			if (temp1 > 0) (*out1++) = dGain * 1;
			if (temp1 < 0) (*out1++) = dGain * -1;
		}
		else{							//BIGGER THAN THRESHOLD, SMALLER THAN 2X THRESHOLD
			if (temp1 > 0) (*out1++) = dGain *  ( 3- pow((2-temp1*3), 2) )/3;
			if (temp1 < 0) (*out1++) = dGain * -( 3- pow((2-abs(temp1)*3), 2) )/3;
		}

		if (abs(temp2) < fDist)			// UNDER THRESHOLD
			(*out2++) = dGain * 2 * (temp2);
		else if (abs(temp2) > 2*fDist){	// BIGGER THAN 2X TRESHOLD 
			if (temp2 > 0) (*out2++) = dGain * 1;
			if (temp2 < 0) (*out2++) = dGain * -1;
		}
		else{							//BIGGER THAN THRESHOLD, SMALLER THAN 2X THRESHOLD
			if (temp2 > 0) (*out2++) = dGain *  ( 3- pow((2-temp2*3), 2) )/3;
			if (temp2 < 0) (*out2++) = dGain * -( 3- pow((2-abs(temp2)*3), 2) )/3;
		}


		//****************** distortion as per p. 144 DAFX 2ed ******************
		
		//q = temp1*fDist;
		//z = sign(q)*(1-exp(-abs(q)));
		//(*out1++) = mix*z+(1-mix)*temp1;

		//q = temp2*fDist;
		//z = sign(q)*(1-exp(-abs(q)));
		//(*out2++) = mix*z+(1-mix)*temp2;
    }
}
