from pylab import *

file = open("thout.dat", 'r')
lines = file.readlines()

samples = []
for l in lines:
    samples.append(int(l.replace('\n','').split(' ')[0]) - 127.5)

#print array(samples)
print len(samples)
a = fft(array(samples)[40000:50000], 44100)
b = fft(array(samples)[80000:85000], 44100)
c = fft(array(samples)[00000:10000], 44100)
d = fft(array(samples), 44100)

aa = subplot(221)
aa.plot(range(len(a))[0:len(a)], log(abs(a)/log(10))[0:len(a)])
bb = subplot(222) 
bb.plot(range(len(a))[0:len(a)], log(abs(b)/log(10))[0:len(a)])
cc = subplot(223)
cc.plot(range(len(a))[0:len(a)], log(abs(c)/log(10))[0:len(a)])
dd = subplot(224)
dd.plot(range(len(a))[0:len(a)], log(abs(d)/log(10))[0:len(a)])

show()
