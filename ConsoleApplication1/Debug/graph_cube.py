import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation

def convert(test):
    sum1 = 0
    cnt = 1
    for i in test:
        sum1 += int(i) / (2**cnt)
        cnt += 1
    return(sum1)

def rotate(angle):
    ax_c.view_init(azim=angle)

f = open('Values.txt', 'r')

x = []
y = []
z = []

for line in f:
    tmp = line.split()
    x.append(convert(tmp[0].split('.')[1]))
    y.append(convert(tmp[1].split('.')[1]))
    z.append(convert(tmp[2].split('.')[1]))



fig_c = plt.figure(figsize = (10,10))

ax_c = fig_c.add_subplot(111, projection = '3d')
ax_c.set_title("Cube projection")
ax_c.set_xlabel('x-axis')
ax_c.set_ylabel('y-axis')
ax_c.set_zlabel('z-axis')

ax_c.set_xlim(0, 1)
ax_c.set_ylim(0, 1)
ax_c.set_zlim(0, 1)



ax_c.scatter(x, y, z, s = 1, c = 'k')
# ax_c.view_init(90, -90)

#doing gif
#angle = 3
#ani = animation.FuncAnimation(fig_c, rotate, frames=np.arange(0, 360, angle), interval=50)
#ani.save('cube_graph.gif', writer=animation.PillowWriter(fps=20)) 
#end doing gif
plt.savefig('cube_graph.png')
plt.show()