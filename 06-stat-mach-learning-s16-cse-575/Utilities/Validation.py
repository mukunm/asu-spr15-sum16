import numpy as np

x = 500
y = 16146

r = np.random.randint(2, size=(x, y))
dt = np.random.randint(1, size=(x, y))
va = np.random.randint(1, size=(x, y))

count = 0

print "Running"

for i in range(0, x):
    for j in range(0, y):
        if r[i, j] == 1:
            count += 1
            if count%5 == 0:
                va[i, j] = 1
            else:
                dt[i, j] = 1

print "Done"
