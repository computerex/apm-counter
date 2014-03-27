filename = raw_input("filename?: ")
file = open(filename,"r")
hits = []
mouse = []
for line in file:
  hits.append(int(line.replace("\n","").split()[0]))
  mouse.append(int(line.replace("\n", "").split("m")[1]))
# remove lines with zero hits
while 0 in hits:
  hits.remove(0)
while 0 in mouse:
  mouse.remove(0)
avgspeed = sum(mouse)/len(mouse)
avghits = float(sum(hits))/len(hits)
print( "avg hit/sec: " + str(avghits) + " avg bpm: " + str(avghits*60))
print("max bpm: " + str(max(hits)*60.))
print("avg mouse speed: " + str(avgspeed))