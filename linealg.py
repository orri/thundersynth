from pylab import *

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




xs = (0, 10)
ys = (0,57)

line(xs, ys)

ax = arange(xs[0],xs[1]+1,1)
ay = array(yl)

print (len(ax), len(ay))
plot(ax, ay,'x')
show()

