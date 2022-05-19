import numpy as np;
import matplotlib as plt;

np.random.seed(123)

x = np.random.randint(-5, 5, 40) #vector zahuyachit
y = np.random.randint(-5, 5, 40)
z = np.random.randint(-5, 5, 40)
s = np.random.randint(-5, 5, 40)

fig = plt.figure();
ax = fig.add_subplot(111, projection='3d')
ax.scatter (x, y, z, s=s)