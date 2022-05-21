
import matplotlib.pyplot as plt
import numpy as np


f_big = open('Values_b2.txt', 'r')

x = []
y = []
z = []

for line in f_big:
    tmp = line.split()
    x.append(int(tmp[0]))
    y.append(int(tmp[1]))
    z.append(int(tmp[2]))

c = 2**63

fig_b = plt.figure(figsize = (20,20))

ax_b = fig_b.add_subplot(111, projection = '3d')
ax_b.set_title("Cube projection")
ax_b.set_xlabel('x-axis')
ax_b.set_ylabel('y-axis')
ax_b.set_zlabel('z-axis')

ax_b.set_xlim(0, c)
ax_b.set_ylim(0, c)
ax_b.set_zlim(0, c)



ax_b.scatter(x, y, z, s = 1, c = 'k')