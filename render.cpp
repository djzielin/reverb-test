#include <Bela.h>
#include "feedback_comb_filter.h"
#include "allpass_filter.h"

#include <Scope.h>

float sampleRate;

Scope scope;
float delay_times[8]={1116.0f/44100.0f,
                      1188.0f/44100.0f,
                      1277.0f/44100.0f,
                      1356.0f/44100.0f,
                      1422.0f/44100.0f,
                      1491.0f/44100.0f, 
                      1557.0f/44100.0f,
                      1617.0f/44100.0f};
                      
float allpass_times[4]={225.0f/44100.0f,
                        556.0f/44100.0f,
                        441.0f/44100.0f,
                        341.0f/44100.0f};


feedback_comb_filter *fbcf_delay;

feedback_comb_filter *fbcf[8];
allpass_filter *apf[4];

bool setup(BelaContext *context, void *userData)
{
	scope.setup(3, context->audioSampleRate);
	sampleRate=context->audioSampleRate;
	
	for(int i=0;i<8;i++)
	{
       fbcf[i]=new feedback_comb_filter(sampleRate, 10.0f);
       // fbcf[i]->set_delay_time(delay_times[i]);

       fbcf[i]->set_delay_time(delay_times[i]*5.0f);
       //fbcf[i]->set_feedback(0.84f);
	   fbcf[i]->set_feedback(0.97f);
    
	}
	
	for(int i=0;i<4;i++)
	{
		apf[i]=new allpass_filter(sampleRate,1.0f);
	    apf[i]->set_delay_time(allpass_times[i]);
	    apf[i]->set_feedback(0.5f);
	}	
	
	return true;
	
	
}
int count=10;


void render(BelaContext *context, void *userData)
{
	float gIn0=0;
	float out=0;
	
	
	for(unsigned int n = 0; n < context->audioFrames; n++) 
	{
		if(count<100)
		  gIn0=0;
		else
				gIn0 = audioRead(context, n, 0);
	    out=0;
	    
	    float delayed=gIn0; //fbcf_delay->tick(gIn0);
	    
        //out=gIn0;
	    for(int i=0;i<8;i++)
	       out+=fbcf[i]->tick(delayed+gIn0)*0.1f;
	    
	    for(int i=0;i<4;i++)
	       out=apf[i]->tick(out);
	    
	    out=gIn0*0.4f+out*0.6f; //add dry signal
	    
     	audioWrite(context, n, 0, out);
     	audioWrite(context, n, 1, out);

    	scope.log(gIn0,out);
	}
	count++;
}

void cleanup(BelaContext *context, void *userData)
{

}