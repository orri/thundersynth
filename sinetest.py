from pylab import *

length = 512

x = arange(0, 2*pi,2*pi/length)
s = 127.5*sin(x)+127.5
ss = zeros(length)
for i in range(len(s)):
    ss[i] = round(s[i])


plot(x, ss)
sinestr = ""
for m in ss:
    sinestr += str(int(m)) + ", "
print sinestr
print(x[-1])
print 2*pi
show()

