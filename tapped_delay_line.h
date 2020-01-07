/***** tapped_delay_line.h *****/
#ifndef DJZ_TAPPED_DELAY_LINE
#define DJZ_TAPPED_DELAY_LINE

class tapped_delay_line
{
public:
   tapped_delay_line(float sample_rate, float max_delay_seconds, int max_taps);

   void set_delay_time(float dt);
   void set_delay_samples(int delay_samples);
   
   void set_tap_time(float tt, int tap_index);
   float tick(float input);

private:
   unsigned int _delay_line_max;
   unsigned int _max_taps;
   
   float *_delay_line;
   unsigned int *_taps;

   float _sample_rate;

   int _read_pos;
};


#endif