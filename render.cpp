#include <Bela.h>
#include "tapped_delay_line.h"
#include <libraries/Scope/Scope.h>
#include "reverb.h"
float sampleRate;

Scope scope;


tapped_delay_line *tdl;
tapped_delay_line *td2;


reverb *ourRev;

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
	
	td2=new tapped_delay_line(sampleRate,1.0f,NUM_TAPS);
	
	for(int i=0;i<NUM_TAPS;i++)
	{
		float tap_time=(float)rand()/(float)RAND_MAX*0.100f; //0ms to 100ms;
		td2->set_tap_time(tap_time,i);
	}
	
	ourRev=new reverb(sampleRate);
 
	pinMode(context, 0, gOutputPin, OUTPUT); // Set gOutputPin as output
	pinMode(context, 0, gOutputPin2, OUTPUT); // Set gOutputPin as output


	return true;
	
	
}
int count=10;


int inputTimeout=0;
int inputTimeout2=0;

int outputTimeout=0;

void render(BelaContext *context, void *userData)
{
	float gIn0=0;
	float gIn1=0;
	
	float out=0;
    float out2=0;
	
	for(unsigned int n = 0; n < context->audioFrames; n++) 
	{
        gIn0 = audioRead(context, n, 0);
		gIn1 = audioRead(context, n, 1);
       
	    
	    if(gIn0>0.1 && inputTimeout==0)
	    {
	    	 inputTimeout=5000;
	    }
	    
	    if(gIn1>0.1 && inputTimeout2==0)
	    {
	    	 inputTimeout2=5000;
	    }


	    if(inputTimeout>0)
	       inputTimeout--;
	    if(inputTimeout2>0)
	       inputTimeout2--;
	       
	   out=tdl->tick(gIn0)*0.05f+gIn0;   
       out2=td2->tick(gIn1)*0.05f+gIn1;   

       if(inputTimeout==0) out=0; //apply the gating
	   if(inputTimeout2==0) out2=0;

	  
	 /*   float tap_out=out2;
	
	    for(int i=0;i<8;i++) //PROCESS FEEDBACK COMB FILTERS
	        out2+=fbcf[i]->tick(tap_out)*0.1f;
	    
	    for(int i=0;i<4;i++) //PROCESS ALL PASS FILTERS
	       out2=apf[i]->tick(out2);
	
	*/

	       
     	audioWrite(context, n, 0, tanh(out));
     	audioWrite(context, n, 1, tanh(out2));

    	scope.log(gIn0,gIn1);
	}
	count++;
}

void cleanup(BelaContext *context, void *userData)
{

}