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

#define SAMPLING_FREQ 44100
#define BLOCKS_PER_SEC  1050
#define BLOCK_LEN SAMPLING_FREQ/BLOCKS_PER_SEC
#define BPM 120
#define SAMPLES_PER_BEAT SAMPLING_FREQ*60/BPM

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;

// ADSR coefficientes are in Q1.15
#define ATTACK_COEFF 0x8080 
#define DECAY_COEFF 0x7ff0
#define RELEASE_COEFF 0x7ff0
// ADSR times are simmulated using adsrtest.py
#define ATTACK_TIME 100//2260//2840
#define DECAY_TIME 271//4950
#define RELEASE_TIME 4209
#define ADR_TIME (ATTACK_TIME+DECAY_TIME+RELEASE_TIME)
// ADSR phases enums
#define ATTACK 0
#define DECAY 1
#define SUSTAIN 2
#define RELEASE 3

#define ATTACK_LEVEL 0xfe00
#define SUSTAIN_LEVEL 0x80
#define START_RELEASE 0x80

#define TRIG 0
#define SINE 1
#define SQUARE 0xc0
#define SQUARE75 0xe0
#define SQUARE25 0xa0
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


typedef struct {
  u8 *list;
  u8 *durations;
  u16 length; // length of notelist 
  u16 curr_note_length,  amount_done; // samples
  u16 next_note; // index to list and durations
} notelist;

typedef struct {
  u8 wavetype; // TRIG, SQUARE, sine ...
  // u8 phase;    // the phase the oscillator was in.
  u8 *period;
  u16 period_len; // number of allocated bytes for period.
  u16 period_index; 
} oscillator;







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

typedef struct {
  //  u16 period_len;
  linealg *la;
} step_trig_osc;

typedef struct {
  u16 period;
  linealg *la;
} step_saw_osc;

typedef struct {
  u16 period;
  u16 period_count;
  u16 cut_point; // indicated duty cycle, cut_point/period = duty_cycle 
} step_square_osc;


typedef struct {
  void* osc; // casted to another step oscillator type
  u8 wave_form;
  u8 (*next_sample)(void*);
  void (*new_per)(void*, u16);
} step_osc;

typedef struct {
  u16 current_amp_scale; // Q0.16
  u16 until_release;
  u8 current_phase;
} adsr;

typedef struct {
  u8 *channel_block; // [BLOCK_LEN];
  notelist *nlist;
  oscillator *osc;
  adsr *a;
} channel;

typedef struct {
  u8 amp_scale;
  u8 phase;
} step_adsr;



void get_new_period(oscillator* osc, u16 frequency);
void run_oscillator(oscillator* osc, u8* buffer, u16 len);
u8 get_next_note(channel* chan);
u8 next_chan_block(channel* chan);
void adsr_block(adsr *a, u8* block, u16 len);
u8 scale_amplitude(u8 in, u8 scale); // scales 
void calc_release_time(adsr *a, u16 note_length);
void initialize_line(linealg* l, u16 period_len);
inline u16 next_line_val(linealg* l);
inline u8 adsr_sample(step_adsr *, u8 sample);


inline u8 next_sine_sample(step_sine_osc *osc);
inline void new_sine_per(step_sine_osc *osc,  u16 period_len);

inline u8 next_trig_sample(step_trig_osc *osc);
inline void new_trig_per(step_trig_osc *osc,  u16 period_len);

inline u8 next_saw_sample(step_saw_osc *osc);
inline void new_saw_per(step_saw_osc *osc,  u16 period_len);

inline u8 next_osc_sample(step_osc *osc);
inline void next_osc_per(step_osc *osc, u16 period_len);

extern const u16 notes[];
extern const u16 notesperiod[];

extern const u8 sine512[];
extern const u8 sine1024[];


#endif // CHANNEL_H
