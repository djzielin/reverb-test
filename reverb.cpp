/***** reverb.cpp *****/
#include "reverb.h"

void reverb::set_delay_times(float factor)
{
	for(int i=0;i<8;i++)
	{
      fbcf[i]->set_delay_time(delay_times[i]*factor);
	}
}

void reverb::set_feedback(float val)
{
	for(int i=0;i<8;i++)
	{
      fbcf[i]->set_feedback(0.84f);
	}
}


reverb::reverb(float sample_rate)
{
	sampleRate=sample_rate;
	
	for(int i=0;i<8;i++)
	{
       fbcf[i]=new feedback_comb_filter(sampleRate, 1.0f);
	}
	
	set_delay_times(5.0f);
	set_feedback(0.84f);
   
	for(int i=0;i<4;i++)
	{
		apf[i]=new allpass_filter(sampleRate,1.0f);
	    apf[i]->set_delay_time(allpass_times[i]);
	    apf[i]->set_feedback(0.5f);
	}	
}

float reverb::tick(float in)
{
	 float out=0.0f;
	
	 for(int i=0;i<8;i++) //PROCESS PARRALLELFEEDBACK COMB FILTERS
	    out+=fbcf[i]->tick(in)*0.1f;
	    
	 for(int i=0;i<4;i++) //PROCESS ALL PASS FILTERS
	    out=apf[i]->tick(out);
	    
	 return out;
	    
}

