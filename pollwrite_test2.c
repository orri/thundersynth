/*
 *  This small demo sends a simple sinusoidal wave to your speakers.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sched.h>
#include <errno.h>
//#include <getopt.h>
#include <alsa/asoundlib.h>
#include <sys/time.h>
#include <math.h>

#include <pthread.h>
#include "read_input.h"
#include "channel.h"
#include "alsacrap.h"


static char *device = "default";                     /* playback device */
static snd_pcm_format_t format = SND_PCM_FORMAT_U8;    /* sample format */
static unsigned int rate = 44100;                       /* stream rate */

static double freq = 1;                               /* sinusoidal wave frequency in Hz */
static int verbose = 0;                                 /* verbose flag */

static snd_output_t *output = NULL;


static void generate_sine(unsigned char *samples,
                          int count, 
			  double *_phase)
{

  static double max_phase = 2. * M_PI;
  double phase = *_phase;
  double step = max_phase*freq/(double)rate;
  
  int format_bits = snd_pcm_format_width(format);
  unsigned int maxval = (1 << (format_bits - 1)) - 1;
  int to_unsigned = snd_pcm_format_unsigned(format) == 1;
  int is_float = (format == SND_PCM_FORMAT_FLOAT_LE ||
		  format == SND_PCM_FORMAT_FLOAT_BE);

  while (count-- > 0) {
    union {
      float f;
      int i;
    } fval;
    int res;
    if (is_float) {
      fval.f = sin(phase) * maxval;
      res = fval.i;
    } else
      res = sin(phase) * maxval;
    if (to_unsigned)
      res ^= 1U << (format_bits - 1);
    *(samples++) = res;
    phase += step;
    if (phase >= max_phase)
      phase -= max_phase;
  }
  *_phase = phase;
}




void process_input(midi_t *m){
  if (m->type == 0x90){ // press note
    freq = notes[m->value1 - 0x24];
  } 
  else if (m->type == 0x80) {
    if (freq == notes[m->value1 - 0x24])
      freq = 1;
  }
  
}

typedef struct {
  u8 *samples;
  input_params *iparams;
  double *phase;
  int period_s;
} mainloop_args_t;
 
// called from inside 
void main_loop(void *args)
{
  mainloop_args_t *a = (mainloop_args_t*)args; 
  while(get_new_input(a->iparams)){
    process_input(a->iparams->last_input);
  }
  generate_sine(a->samples, a->period_s, a->phase);
}


int main(int argc, char *argv[])
{

  snd_pcm_t *handle;
  int err;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;
  unsigned char *samples;
  snd_pcm_sframes_t period_s;

  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);
 
     
  err = snd_output_stdio_attach(&output, stdout, 0);
  if (err < 0) {
    printf("Output failed: %s\n", snd_strerror(err));
    return 0;
  }
  printf("Playback device is %s\n", device);

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    printf("Playback open error: %s\n", snd_strerror(err));
    return 0;
  }
        
  if ((err = set_hwparams(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED, &period_s)) < 0) {
    printf("Setting of hwparams failed: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
  if ((err = set_swparams(handle, swparams)) < 0) {
    printf("Setting of swparams failed: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
  if (verbose > 0)
    snd_pcm_dump(handle, output);
  samples = malloc(period_s);
  if (samples == NULL) {
    printf("Not enough memory\n");
    exit(EXIT_FAILURE);
  }
        

  // setup read_input
  input_params i_params;
  i_params.filename = "/dev/midi1";
  i_params.amount_read = 0;
  i_params.amount_consumed = 0;
  printf("1");
  short i;
  for (i = 0; i < 256; i++)
    i_params.midi_inputs[i] = malloc(sizeof(midi_t));
  printf("2");

  pthread_t input_thread;
  if(pthread_create(&input_thread, NULL, (void*)(&read_input), (void*)(&i_params))) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
    
  }


  mainloop_args_t mainloop_args;
  mainloop_args.samples =samples;
  mainloop_args.iparams = &i_params;
  *(mainloop_args.phase) = 0;
  mainloop_args.period_s = period_s;
  printf("period_s: %d\n", period_s);
  

  err = write_and_poll_loop(handle, samples, (void*)(&main_loop), 
			    (void*)(&mainloop_args));
  
  if (err < 0)
    printf("Transfer failed: %s\n", snd_strerror(err));

  free(samples);
  snd_pcm_close(handle);
  return 0;
}
