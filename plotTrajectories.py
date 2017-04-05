from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

H = 4e3
L = 12e6
# Open a file
t = np.loadtxt("out/time.txt")
Y = np.loadtxt("out/Y.txt")
Z = np.loadtxt("out/Z.txt")

plt.plot(Y[:,0],Z[:,0])#,'o-')
plt.plot(Y[0,0],Z[0,0],'r.')
plt.plot(Y[-1,0],Z[-1,0],'g.','MarkerSize',15)
plt.plot(.9*L*np.ones(100), np.linspace(0,H,100),':k')
plt.plot(np.linspace(0,L,100), H*7./8.*np.ones(100),':k')
plt.show()