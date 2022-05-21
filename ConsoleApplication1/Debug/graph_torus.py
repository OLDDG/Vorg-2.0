import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import axes3d
import numpy as np
from matplotlib import animation



def sq_to_torus(x):
    return 2 * np.pi * x

def convert(test):
    sum1 = 0
    cnt = 1
    for i in test:
        sum1 += int(i) / (2**cnt)
        cnt += 1
    return(sum1)

def rotate(ax, angle):
     ax.view_init(azim=angle)

def graph_on_torus(n_graph, ax):
    colors = ['r', 'g', 'm', 'y', 'w', 'k']

    x_p = []
    y_p = []
    z_p = []

    f_torus = open('Values' + str(n_graph) + '.txt', 'r')

# 0.01010101010111  0.0000010101011
    for line in f_torus:
        tmp = line.split()
        #tmp = ['0.1010101...', '0.00001010101...']
        x_p.append( (3 + 1 * np.cos(sq_to_torus( convert(tmp[1].split('.')[1]) ))) * np.cos(sq_to_torus( convert(tmp[0].split('.')[1]) )) )
        y_p.append( (3 + 1 * np.cos(sq_to_torus( convert(tmp[1].split('.')[1]) ))) * np.sin(sq_to_torus( convert(tmp[0].split('.')[1]) )) )
        z_p.append(np.sin( sq_to_torus( convert(tmp[1].split('.')[1])) ))
    
    print(n_graph)
    print(len(x_p), x_p[435], y_p[436], z_p[435])
    ax.scatter(x_p, y_p, z_p, s = 0.5, c = colors[n_graph % len(colors)])
    return
    
#torus
def torus_and_graph(N):
    fig = plt.figure(figsize = (10,10))
    ax = fig.add_subplot(111, projection = '3d')

    ax.set_title("Torus projection")
    ax.set_xlabel('x-axis')
    ax.set_ylabel('y-axis')
    ax.set_zlabel('z-axis')

    ax.set_zlim(-5, 5)

    u, v = np.mgrid[0 : 2*np.pi : 30j, 0 : 2* np.pi : 30j]
    x = (3 + 1*np.cos(v)) * np.cos(u)
    y = (3 + 1*np.cos(v)) * np.sin(u)
    z = np.sin(v)

    ax.plot_wireframe(x, y, z, rstride= 8, cstride=5)

    for i in range(N):
        graph_on_torus(i, ax)

    #angle = 3
    #ani = animation.FuncAnimation(fig, rotate(ax), frames=np.arange(0, 360, angle), interval=50)
    #ani.save('thorus_test.gif', writer=animation.PillowWriter(fps=20))
    
    
    plt.show()
    
source = open('number_graphs.txt', 'r')

n = int(source.readline())

torus_and_graph(n)