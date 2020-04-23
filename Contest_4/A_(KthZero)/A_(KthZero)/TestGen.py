import random


maxVal = 1000

fout = open("input.txt", "w")

N = 100000
fout.write(str(N) + '\n')

for i in range(0, N):
    curNum = random.randint(0, maxVal)
    fout.write(str(curNum) + ' ')
fout.write('\n')


M = 100000
fout.write(str(M) + '\n')

for i in range(0, M):
    curCommand = random.choice(['s', 'u'])
    fout.write(curCommand + ' ')
    
    if curCommand == 's':
        left = random.randint(1, N)
        right = random.randint(left, N)
        k = random.randint(1, 10)
        fout.write(str(left) + ' ' + str(right) + ' ' + str(k) + '\n')
    else:
        index = random.randint(1, N - 1)
        newVal = random.randint(0, maxVal)
        fout.write(str(index) + ' ' + str(newVal) + '\n')

fout.close()
