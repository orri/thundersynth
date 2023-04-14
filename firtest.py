

from pylab import *
N = 2000
n = arange(0, N)
s = 64*256*sin(n*2*pi/50)
ss = []
for m in s:
    ss.append(int(random()*0x100))
s = array(ss)

sss = [0]
for i in range(1, len(ss)):
    sss.append(ss[i-1]*0.5 + ss[i]*0.5)

ff = fft(ss,N)
xx = [int(x) for x in abs(ff)]
#print len(abs(ff))
#print len(n)
for i in range(len(ff)):
    if xx[i] > 0:
        print "%d, %d" % (i, xx[i])

plot(n[1:],abs(ff)[1:])


#plot(n, abs(ff))

show()


'''
omeg = arange(0, 2*pi, 0.01)
fir = [0.5, 0.5]
m = zeros(len(omeg))
for i in range(len(fir)):
    m += fir[i]*exp(i*1j*omeg)

print m
plot(omeg, m)
show()
'''
