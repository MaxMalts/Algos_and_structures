import random


maxVal = 1000

fout = open("index-max.in", "w")

N = 100000
fout.write(str(N) + '\n')

for i in range(0, N):
    curNum = random.randint(0, maxVal)
    fout.write(str(curNum) + ' ')
fout.write('\n')


M = 3000000
fout.write(str(M) + '\n')

for i in range(0, M):
        left = random.randint(1, N)
        right = random.randint(left, N)
        fout.write(str(left) + ' ' + str(right) + '\n')

fout.close()
