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
static char *device = "default";                     /* playback device */
static snd_pcm_format_t format = SND_PCM_FORMAT_U8;    /* sample format */
static unsigned int rate = 44100;                       /* stream rate */
static unsigned int channels = 1;                       /* count of channels */
static unsigned int buffer_time = 2000;               /* ring buffer length in us */
static unsigned int period_time = 1000;               /* period time in us */
static double freq = 440;                               /* sinusoidal wave frequency in Hz */
static int verbose = 0;                                 /* verbose flag */
static int resample = 1;                                /* enable alsa-lib resampling */
static int period_event = 0;                            /* produce poll event after each period */
static snd_pcm_sframes_t buffer_size;
static snd_pcm_sframes_t period_size;
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

static int set_hwparams(snd_pcm_t *handle,
                        snd_pcm_hw_params_t *params,
                        snd_pcm_access_t access)
{
  unsigned int rrate;
  snd_pcm_uframes_t size;
  int err, dir;
  /* choose all parameters */
  err = snd_pcm_hw_params_any(handle, params);
  if (err < 0) {
    printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
    return err;
  }
  /* set hardware resampling */
  err = snd_pcm_hw_params_set_rate_resample(handle, params, resample);
  if (err < 0) {
    printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
    return err;
  }
  /* set the interleaved read/write format */
  err = snd_pcm_hw_params_set_access(handle, params, access);
  if (err < 0) {
    printf("Access type not available for playback: %s\n", snd_strerror(err));
    return err;
  }
  /* set the sample format */
  err = snd_pcm_hw_params_set_format(handle, params, format);
  if (err < 0) {
    printf("Sample format not available for playback: %s\n", snd_strerror(err));
    return err;
  }
  /* set the count of channels */
  err = snd_pcm_hw_params_set_channels(handle, params, channels);
  if (err < 0) {
    printf("Channels count (%i) not available for playbacks: %s\n", channels, snd_strerror(err));
    return err;
  }
  /* set the stream rate */
  rrate = rate;
  err = snd_pcm_hw_params_set_rate_near(handle, params, &rrate, 0);
  if (err < 0) {
    printf("Rate %iHz not available for playback: %s\n", rate, snd_strerror(err));
    return err;
  }
  if (rrate != rate) {
    printf("Rate doesn't match (requested %iHz, get %iHz)\n", rate, err);
    return -EINVAL;
  }
  /* set the buffer time */
  err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
  if (err < 0) {
    printf("Unable to set buffer time %i for playback: %s\n", buffer_time, snd_strerror(err));
    return err;
  }
  err = snd_pcm_hw_params_get_buffer_size(params, &size);
  if (err < 0) {
    printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
    return err;
  }
  buffer_size = size;
  /* set the period time */
  err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
  if (err < 0) {
    printf("Unable to set period time %i for playback: %s\n", period_time, snd_strerror(err));
    return err;
  }
  err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
  if (err < 0) {
    printf("Unable to get period size for playback: %s\n", snd_strerror(err));
    return err;
  }
  period_size = size;
  /* write the parameters to device */
  err = snd_pcm_hw_params(handle, params);
  if (err < 0) {
    printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
    return err;
  }
  return 0;
}
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams)
{
  int err;
  /* get the current swparams */
  err = snd_pcm_sw_params_current(handle, swparams);
  if (err < 0) {
    printf("Unable to determine current swparams for playback: %s\n", snd_strerror(err));
    return err;
  }
  /* start the transfer when the buffer is almost full: */
  /* (buffer_size / avail_min) * avail_min */
  err = snd_pcm_sw_params_set_start_threshold(handle, swparams, (buffer_size / period_size) * period_size);
  if (err < 0) {
    printf("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
    return err;
  }
  /* allow the transfer when at least period_size samples can be processed */
  /* or disable this mechanism when period event is enabled (aka interrupt like style processing) */
  err = snd_pcm_sw_params_set_avail_min(handle, swparams, period_event ? buffer_size : period_size);
  if (err < 0) {
    printf("Unable to set avail min for playback: %s\n", snd_strerror(err));
    return err;
  }
  /* enable period events when requested */
  if (period_event) {
    err = snd_pcm_sw_params_set_period_event(handle, swparams, 1);
    if (err < 0) {
      printf("Unable to set period event: %s\n", snd_strerror(err));
      return err;
    }
  }
  /* write the parameters to the playback device */
  err = snd_pcm_sw_params(handle, swparams);
  if (err < 0) {
    printf("Unable to set sw params for playback: %s\n", snd_strerror(err));
    return err;
  }
  return 0;
}
/*
 *   Underrun and suspend recovery
 */
 
static int xrun_recovery(snd_pcm_t *handle, int err)
{
  if (verbose)
    printf("stream recovery\n");
  if (err == -EPIPE) {    /* under-run */
    err = snd_pcm_prepare(handle);
    if (err < 0)
      printf("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
    return 0;
  } else if (err == -ESTRPIPE) {
    while ((err = snd_pcm_resume(handle)) == -EAGAIN)
      sleep(1);       /* wait until the suspend flag is released */
    if (err < 0) {
      err = snd_pcm_prepare(handle);
      if (err < 0)
	printf("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
    }
    return 0;
  }
  return err;
}

static int wait_for_poll(snd_pcm_t *handle, struct pollfd *ufds, unsigned int count)
{
  unsigned short revents;
  while (1) {
    poll(ufds, count, -1);
    snd_pcm_poll_descriptors_revents(handle, ufds, count, &revents);
    if (revents & POLLERR)
      return -EIO;
    if (revents & POLLOUT)
      return 0;
  }
}

static int write_and_poll_loop(snd_pcm_t *handle,
                               unsigned char *samples)
{
  struct pollfd *ufds;
  double phase = 0;
  unsigned char *ptr;
  int err, count, cptr, init;
  count = snd_pcm_poll_descriptors_count (handle);
  if (count <= 0) {
    printf("Invalid poll descriptors count\n");
    return count;
  }
  ufds = malloc(sizeof(struct pollfd) * count);
  if (ufds == NULL) {
    printf("No enough memory\n");
    return -ENOMEM;
  }
  if ((err = snd_pcm_poll_descriptors(handle, ufds, count)) < 0) {
    printf("Unable to obtain poll descriptors for playback: %s\n", snd_strerror(err));
    return err;
  }
  init = 1;
  while (1) {
    printf("in wait and poll while\n");
    if (!init) {
      err = wait_for_poll(handle, ufds, count);
      if (err < 0) {
	if (snd_pcm_state(handle) == SND_PCM_STATE_XRUN ||
	    snd_pcm_state(handle) == SND_PCM_STATE_SUSPENDED) {
	  err = snd_pcm_state(handle) == SND_PCM_STATE_XRUN ? -EPIPE : -ESTRPIPE;
	  if (xrun_recovery(handle, err) < 0) {
	    printf("Write error: %s\n", snd_strerror(err));
	    exit(EXIT_FAILURE);
	  }
	  init = 1;
	} else {
	  printf("Wait for poll failed\n");
	  return err;
	}
      }
    }
    generate_sine(samples, period_size, &phase);
    ptr = samples;
    cptr = period_size;
    while (cptr > 0) {
      err = snd_pcm_writei(handle, ptr, cptr);
      if (err < 0) {
	if (xrun_recovery(handle, err) < 0) {
	  printf("Write error: %s\n", snd_strerror(err));
	  exit(EXIT_FAILURE);
	}
	init = 1;
	break;  /* skip one period */
      }
      if (snd_pcm_state(handle) == SND_PCM_STATE_RUNNING)
	init = 0;
      ptr += err * channels;
      cptr -= err;
      if (cptr == 0)
	break;
      /* it is possible, that the initial buffer cannot store */
      /* all data from the last period, so wait awhile */
      err = wait_for_poll(handle, ufds, count);
      if (err < 0) {
	if (snd_pcm_state(handle) == SND_PCM_STATE_XRUN ||
	    snd_pcm_state(handle) == SND_PCM_STATE_SUSPENDED) {
	  err = snd_pcm_state(handle) == SND_PCM_STATE_XRUN ? -EPIPE : -ESTRPIPE;
	  if (xrun_recovery(handle, err) < 0) {
	    printf("Write error: %s\n", snd_strerror(err));
	    exit(EXIT_FAILURE);
	  }
	  init = 1;
	} else {
	  printf("Wait for poll failed\n");
	  return err;
	}
      }
    }
  }
}


int main(int argc, char *argv[])
{

  snd_pcm_t *handle;
  int err;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;
  unsigned char *samples;

  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);
 
     
  err = snd_output_stdio_attach(&output, stdout, 0);
  if (err < 0) {
    printf("Output failed: %s\n", snd_strerror(err));
    return 0;
  }
  printf("Playback device is %s\n", device);
  printf("Stream parameters are %iHz, %s, %i channels\n", rate, snd_pcm_format_name(format), channels);
  printf("Sine wave rate is %.4fHz\n", freq);
  printf("Using transfer method: write and poll\n");
  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    printf("Playback open error: %s\n", snd_strerror(err));
    return 0;
  }
        
  if ((err = set_hwparams(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    printf("Setting of hwparams failed: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
  if ((err = set_swparams(handle, swparams)) < 0) {
    printf("Setting of swparams failed: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
  if (verbose > 0)
    snd_pcm_dump(handle, output);
  samples = malloc(period_size);
  if (samples == NULL) {
    printf("Not enough memory\n");
    exit(EXIT_FAILURE);
  }
        

  err = write_and_poll_loop(handle, samples);
  if (err < 0)
    printf("Transfer failed: %s\n", snd_strerror(err));

  free(samples);
  snd_pcm_close(handle);
  return 0;
}
