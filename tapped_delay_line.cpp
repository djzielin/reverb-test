/***** tapped_delay_line.cpp *****/
#include <stdio.h>
#include <stdlib.h> 

#include "tapped_delay_line.h"

// based on CCRMA:
// https://ccrma.stanford.edu/realsimple/Delay/Allpass_Filters_I.html

tapped_delay_line::tapped_delay_line(float sample_rate, float max_delay_seconds, int max_taps)
{
   printf("initializing taped delay line: max %f seconds\n",max_delay_seconds);

   _read_pos=0;
   _sample_rate=sample_rate;
   _delay_line_max=0;
   _max_taps=max_taps;
   _taps=new unsigned int[max_taps]; //TODO: does number of taps need to be changable during runtime?
   
   set_delay_time(max_delay_seconds);

   for(int i=0;i<_delay_line_max;i++)
      _delay_line[i]=0.0f;

   printf("   DONE!\n");
}

void tapped_delay_line::set_tap_time(float tt, int tap_index)
{
   unsigned int tap_time=(unsigned int)(tt*_sample_rate);
   if(tap_time>_delay_line_max)
   {
   	  printf("error: tap time of %f is too long!\n",tt);
      return;
   }
   _taps[tap_index]=tap_time;
   
   //printf("setting up tap %d to delay: %d\n",tap_index,tap_time);
}

void tapped_delay_line::set_delay_time(float dt)
{
   unsigned int new_delay=(unsigned int)(dt*_sample_rate);
   
   if(new_delay<1) //sanity check. don't allow 0 length delay line 
      new_delay=1;
   
   if(_delay_line_max!=0)
   {
   	
      printf("delay line already exists, to removing old one first!\n");
      delete _delay_line;
   }

   _delay_line_max=new_delay;
   _delay_line=new float[_delay_line_max];
   
   printf("   current delay is now: %d samples\n",_delay_line_max);

}

float tapped_delay_line::tick(float input)
{
   float out_val=0;
   
   for(int i=0;i<_max_taps;i++)
   {
      out_val+=_delay_line[(_read_pos+_delay_line_max-_taps[i]) % _delay_line_max]; 
   }

   _delay_line[_read_pos]=input; //store latest incoming samples
   
   _read_pos=(_read_pos+1) % _delay_line_max; //redundant for now (until we allow modulation)
   
   return out_val;
}