import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#to avoid "Broken pipe error"
from signal import signal, SIGPIPE, SIG_DFL
signal(SIGPIPE,SIG_DFL) 

# read files
t = np.loadtxt("out/time.txt")
Y = np.loadtxt("out/Y.txt")
Z = np.loadtxt("out/Z.txt")

x = Y[:]
y = Z[:]

fig, ax = plt.subplots()
line, = ax.plot(x, y, 'o-')

def update(num, x, y, line):
    line.set_data(x[:num], y[:num])
    # line.axes.axis([np.min(x), np.max(x), np.min(y), np.max(y)])
    line.axes.axis([0, 12e6, 0, 4e3])
    return line,

ani = animation.FuncAnimation(fig, update, len(x), fargs=[x, y, line],
                              interval=25, blit=True)
ani.save('test.mp4')
plt.show()