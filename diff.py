f = open("plotfile.dat","r")
lines = f.read().split('\n')
for i in range(1, len(lines)-1):
    last = int(lines[i-1].split('\t')[1])
    curr = int(lines[i].split('\t')[1])
    if (abs(last-curr) > 6):
        print "%d\t%d\t%d\t%d\t%s" % (i-1, last-curr, last, curr, lines[i].split('\t')[2])
    
f.close()
