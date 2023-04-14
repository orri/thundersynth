import math

sustain_level = 0xc0
o = 1
attack_coeff = 1+2**-9
decay_coeff = 1-2**-10
sustain_time = 12000
sample_num = 24000
release_coeff = 1-2**-10

samples = []
phase = 0 # 0 attack, 1 decay, 2 sustain, 3 release, rest

attack_count = 0
decay_count = 0
release_count = 0

f = open("adsr.dat", "w")

def mylog(i):
    if i<= 0:
        return 0
    return math.log(i)

for i in range(sample_num):
    if phase == 0: # attack
        o = o*attack_coeff
        if o > 255:
            o = 255
            phase = 1
        samples.append(o)
        attack_count += 1
    elif phase == 1: #decay
        o = sustain_level + (o-sustain_level)*decay_coeff
        if (o <= sustain_level+0.5):
            o = sustain_level
            phase = 2
        samples.append(o)
        decay_count += 1
    elif phase == 2: # sustain
        if sustain_time:
            sustain_time -= 1
        else:
            phase = 3
        o = sustain_level
        samples.append(o)
    elif phase == 3: # release
        o = o*release_coeff
        samples.append(o)
        release_count += 1
        
    f.write("%d\t%f\n" % (i, round(o)))
print "attack_time: %d, decay_time: %d, release_time: %d" % (attack_count, decay_count, release_count)
