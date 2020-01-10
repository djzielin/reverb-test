#include <Bela.h>
#include "feedback_comb_filter.h"
#include "allpass_filter.h"
#include "tapped_delay_line.h"
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
tapped_delay_line *tdl;

feedback_comb_filter *fbcf[8];
allpass_filter *apf[4];
int gOutputPin=0;
int gOutputPin2=1;

#define NUM_TAPS 100

bool setup(BelaContext *context, void *userData)
{
	scope.setup(3, context->audioSampleRate);
	sampleRate=context->audioSampleRate;
	
	tdl=new tapped_delay_line(sampleRate,1.0f,NUM_TAPS);
	
	for(int i=0;i<NUM_TAPS;i++)
	{
		float tap_time=(float)rand()/(float)RAND_MAX*0.100f; //0ms to 100ms;
		tdl->set_tap_time(tap_time,i);
	}
	
	for(int i=0;i<8;i++)
	{
       fbcf[i]=new feedback_comb_filter(sampleRate, 1.0f);

       fbcf[i]->set_delay_time(delay_times[i]*1.0f);
       ///fbcf[i]->set_feedback(0.84f);
	   fbcf[i]->set_feedback(0.99f);
    
	}
	
	for(int i=0;i<4;i++)
	{
		apf[i]=new allpass_filter(sampleRate,1.0f);
	    apf[i]->set_delay_time(allpass_times[i]);
	    apf[i]->set_feedback(0.5f);
	}	
	
	pinMode(context, 0, gOutputPin, OUTPUT); // Set gOutputPin as output
	pinMode(context, 0, gOutputPin2, OUTPUT); // Set gOutputPin as output


	return true;
	
	
}
int count=10;


int inputTimeout=0;
int outputTimeout=0;

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
	    
	    if(gIn0>0.1)
	    {
		 	 if(inputTimeout==0)
		 	    digitalWrite(context, n, gOutputPin, 1);
	    	 inputTimeout=5000;
	    	  //rt_printf("input is high\n");
	    }
	    if(inputTimeout==1)
	      digitalWrite(context, n, gOutputPin, 0);

	    if(inputTimeout>0)
	       inputTimeout--;
	       
	   out=tdl->tick(gIn0)*0.25f+gIn0;   
	    
	  float tap_out=out;

	  
	    
	  /*  if(inputTimeout==0) //gate it
     	    out=0;
	
	    for(int i=0;i<8;i++) //PROCESS FEEDBACK COMB FILTERS
	        out+=fbcf[i]->tick(tap_out)*0.1f;
	    
	    for(int i=0;i<4;i++) //PROCESS ALL PASS FILTERS
	       out=apf[i]->tick(out);
	
	*/
	    //out+=tap_out; 
	    
	    if(fabs(out)>1.0f)
	    {
		 	 if(outputTimeout==0)
		 	    digitalWrite(context, n, gOutputPin2, 1);
	    	 outputTimeout=1000;
	    }
	    if(outputTimeout==1)
	      digitalWrite(context, n, gOutputPin2, 0);

	    if(outputTimeout>0)
	       outputTimeout--;
	       
     	audioWrite(context, n, 0, tanh(out));
     	audioWrite(context, n, 1, gIn0);

    	scope.log(gIn0,out);
	}
	count++;
}

void cleanup(BelaContext *context, void *userData)
{

}