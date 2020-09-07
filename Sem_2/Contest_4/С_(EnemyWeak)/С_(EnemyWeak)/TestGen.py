import random


maxVal = 1000000000

fout = open("input.txt", "w")

N = 1000000
fout.write(str(N) + '\n')

for i in range(0, N):
    curNum = random.randint(0, maxVal)
    fout.write(str(curNum) + ' ')
fout.write('\n')

fout.close()
