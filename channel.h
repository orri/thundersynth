/** 
*  @file channel.h
*  @brief defines major building blocks for Thundersynth
*  @author Orri Tómasson
*/

#ifndef CHANNEL_H
#define CHANNEL_H


#include <stdint.h>
#include <stdio.h>
#include "notes.h"

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int16_t s16;
typedef uint32_t u32;


#define ATTACK 0
#define DECAY 1
#define SUSTAIN 2
#define RELEASE 3
#define REST 5

#define TRIG 0
#define SINE 1
#define SAW 2
#define SQUARE 3
#define RANDOM 7


#define SINE_TABLE_LEN 512
#define DELTAY SINE_TABLE_LEN

#if SINE_TABLE_LEN == 512
#define SINE_TABLE sine512
#define SINE_TABLE_LOG 9
#elif SINE_TABLE_LEN == 1024

#define SINE_TABLE sine1024;
#define SINE_TABLE_LOG 10
#endif


// used to pre program

typedef struct {
  u8 *list;
  u8 *durations;
  u16 length; // length of notelist 
  u16 curr_note_length,  amount_done; // samples
  u16 next_note; // index to list and durations
} notelist;



typedef struct {
  u16 deltax; // always same as period_len
  u16 deltay; 
  u16 x; // x is an index that goes throguh every value in period_len
  // y is the output of the algorithm. it is then used as an index to 
  // to the look up table
  u16 y; 
  
  // error and ystep are parameters used by the algorith.
  s16 error; 
  u16 ystep;
  

  /* a python implementation of the modified line drawing algorithm 
  yl = []
  def line((x0, x1), (y0, y1)):
    deltax = x1 - x0
    deltay = y1 - y0
    error = deltax / 2
    ystep = 1

    ystep = (deltay + deltax -1)/deltax
    deltay = deltay % deltax
    
    y = y0
    for x in range(x0, x1+1):
        yl.append(y)
        error = error - deltay
        if error < 0:
            error += deltax
            y+=ystep
        else:
          y+= ystep-1
  */
  

} linealg;


// oscillator that updates one sample at a time


typedef struct {
  u16 period_len;
  linealg *la; 
} step_sine_osc;
/*
typedef struct {
  u16 period_len;
  double phase;
  double step;
} step_dsine_osc;
*/

typedef struct {
  u16 period_len;
  linealg *la; // not used by SQUARE
  u16 per_count; // only used by SQUARE 
  u16 cut_point; // duty cycle for sqare waves
} step_osc;

typedef struct {
  u16 period_len;
  linealg *la;
} step_trig_osc;

typedef struct {
  u16 period_len;
  linealg *la;
} step_saw_osc;

typedef struct {
  u16 period;
  u16 period_count;
  u16 cut_point; // indicated duty cycle, cut_point/period = duty_cycle 
} step_square_osc;


typedef struct {
  u16 attack_coeff, decay_coeff, release_coeff; // all are Q1.15
  u8 sustain_level, attack_level, adsr_off;
} adsr_params;
  
typedef struct {
  u16 amp_scale, prev_ampscale; // Q0.16
  u8 phase;
} adsr;

typedef struct {
  step_osc **osc;
  adsr **adsr;
  u8 num_osc;
  u8 wavetype; 
  adsr_params *ap;
  u8 last_osc;
  u8 duty_cycle; // only for square w
} channel;





//void get_new_period(oscillator* osc, u16 frequency);
//void run_oscillator(oscillator* osc, u8* buffer, u16 len);
void note_press(channel *c, u8 key_val, u8 velocity );
void note_release(channel *c, u8 key_val);

//u8 get_next_note(channel* chan);
//u8 next_chan_block(channel* chan);
//void adsr_block(adsr *a, u8* block, u16 len);
u8 scale_amplitude(u8 in, u8 scale); // scales 
//void calc_release_time(adsr *a, u16 note_length);
void initialize_line(linealg* l, u16 period_len);

inline u8 next_channel_sample(channel *c);

inline u16 next_line_val(linealg* l);
inline u8 adsr_sample(adsr *a, adsr_params *ap, u8 sample);


inline u8 next_sine_sample(step_osc *osc);
inline u8 next_trig_sample(step_osc *osc);
inline u8 next_saw_sample(step_osc *osc);
inline u8 next_square_sample(step_osc *osc);
// sine, trig and sawtooth
inline void new_per(step_osc *osc,  u16 period_len);
inline void new_square_per(step_osc *osc,  u16 period_len, u8 duty);






extern const u16 notes[];
extern const u16 notesperiod[];

extern const u8 sine512[];
extern const u8 sine1024[];


#endif // CHANNEL_H
