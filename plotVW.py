import numpy as np

def phi(x,x0):
	chi = (x <= x0)
	return chi*(x*(2*x0-x)/(x0**2))

def dphi(x,x0):
	chi = (x <= x0)
	return chi*(2*(x0-x))/(x0**2)

def psi(y,z):
	return .25*PSI*(phi(y,y0)*phi(z,z0) + phi(y,y0)*phi(H-z,H-z0)
		+ phi(L-y,L-y0)*phi(H-z,H-z0) + phi(L-y,L-y0)*phi(z,z0))

def V(y,z):
	return .25*PSI*(-phi(y,y0)*dphi(z,z0)
		+ phi(y,y0)*dphi(H-z,H-z0)
		+ phi(L-y,L-y0)*dphi(H-z,H-z0)
		- phi(L-y,L-y0)*dphi(z,z0));

def W(y,z):
	return .25*PSI*(dphi(y,y0)*phi(z,z0)
		+ dphi(y,y0)*phi(H-z,H-z0)
		- dphi(L-y,L-y0)*phi(H-z,H-z0)
		- dphi(L-y,L-y0)*phi(z,z0));


PSI = 1
z0 = .75
y0 = .75
H = 1
L = 1

x = np.linspace(0, 1, 200)
y = np.linspace(0, 1, 200)
X, Y = np.meshgrid(x, y)
Z = V(X,Y)
W = W(X,Y)


f1 = plt.figure(1)
ax = f1.gca(projection='3d')
# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

ax.set_xlabel('Y')
ax.set_ylabel('Z')
ax.set_zlabel('V')

# Add a color bar which maps values to colors.
f1.colorbar(surf, shrink=0.5, aspect=5)

f1.show()


f2 = plt.figure(2)
ax = f2.gca(projection='3d')
ax.plot_surface(X, Y, Z, rstride=8, cstride=8, alpha=0.3)
cset = ax.contourf(X, Y, Z, zdir='z', offset=-1, cmap=cm.coolwarm)
cset = ax.contourf(X, Y, Z, zdir='x', offset=0, cmap=cm.coolwarm)
cset = ax.contourf(X, Y, Z, zdir='y', offset=1, cmap=cm.coolwarm)

ax.set_xlabel('X')
ax.set_xlim(0, 1)
ax.set_ylabel('Y')
ax.set_ylim(0, 1)
ax.set_zlabel('V')
ax.set_zlim(-1, 2)

f2.show()

f3 = plt.figure(3)
ax = f3.gca(projection='3d')
# Plot the surface.
surf = ax.plot_surface(X, Y, W, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

ax.set_xlabel('Y')
ax.set_ylabel('Z')
ax.set_zlabel('W')

# Add a color bar which maps values to colors.
f3.colorbar(surf, shrink=0.5, aspect=5)

f3.show()


f4 = plt.figure(4)
ax = f4.gca(projection='3d')
ax.plot_surface(X, Y, W, rstride=8, cstride=8, alpha=0.3)
cset = ax.contourf(X, Y, W, zdir='z', offset=-2, cmap=cm.coolwarm)
cset = ax.contourf(X, Y, W, zdir='x', offset=0, cmap=cm.coolwarm)
cset = ax.contourf(X, Y, W, zdir='y', offset=1, cmap=cm.coolwarm)

ax.set_xlabel('X')
ax.set_xlim(0, 1)
ax.set_ylabel('Y')
ax.set_ylim(0, 1)
ax.set_zlabel('W')
ax.set_zlim(-2, 1)

f4.show()
plt.show()