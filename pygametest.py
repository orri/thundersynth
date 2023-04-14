
from numpy import *
import pygame
print '0'
SRATE=44100 # sample rate in Hz
DURATION=1 # duration in sec
# an array of floating-point random numbers
noise= array(random.randn(SRATE*DURATION))
print '000'
pygame.mixer.pre_init(SRATE, 16, 1,4096)
pygame.init()
# make it int16, scale it for 16 bit
mysound=int16(noise.copy()*2**15)
print '1'
snd=pygame.sndarray.make_sound(mysound)
print '2'
snd.play()
print '3'
pygame.time.wait(DURATION*1000)

