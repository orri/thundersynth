

notes = ["c", "db", "d", "eb", "e", "f", "gb", "g", "ab", "a", "bb", "b"]
dups  = ["",  "c#", "",  "d#", "", "",  "f#", "" , "g#", "",  "a#", ""]

def from_A(i):
    return 440 * 2**(i/12.)

note_num = 0
note_freqs = []
for k in range(1,8):
    for i in range (12):
        note_freq = str(int(round(from_A( (k-4)*12 + i + 3))))
        note_freqs.append(note_freq)
        print "#define " + notes[i] + str(k) + " "  + str(note_num)
        if dups[i] != "":
            print "#define " + dups[i] + str(k) + " " + str(note_num)
        note_num += 1

print "\nconst unsigned char notes[] = {",
for i in note_freqs:
    print i + ",",
print "0};",
