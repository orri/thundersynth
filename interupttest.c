#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>




void timer_interupt_handler(){
 
};




int main(){
  int i;
  struct sigaction sa; 
  struct itimerval timer; 
 
  /* Install timer_handler as the signal handler for SIGVTALRM.  */ 
  signal(SIGALRM, &timer_interupt_handler);

  /* Configure the timer to expire after 250 msec...  */ 
  timer.it_value.tv_sec = 0; 
  timer.it_value.tv_usec = 250000; 
  /* ... and every 250 msec after that.  */ 
  timer.it_interval.tv_sec = 0; 
  timer.it_interval.tv_usec = 250000; 
  /* Start a virtual timer. It counts down whenever this process is 
     executing.  */ 
  setitimer (ITIMER_REAL, &timer, 0); 
 
  /* Do busy work.  */ 
  for (i=0; i<10; i++){
    printf("here I am\n");
    pause(); 
  }
}
