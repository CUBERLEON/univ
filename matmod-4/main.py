import numpy as np
import math
import matplotlib.pyplot as plt

t = 10
n = 1000
h = t / n

c1 = 7
c2 = 7
r1 = 5
r2 = 1
l = 7


def v_main(t):
    return 10


def dv1(t, v):
    return (-(1 / r1 + 1 / r2) * v[0] + 1 / r2 * v[1] + 1 / r1 * v_main(t)) / c1


def dv2(t, v):
    return (-(1 / r1 + 1 / r2) * v[0] - (c1 * r2 / l - 1 / r2) * v[1] + c1 * r2 / l * v[2] + v_main(t)) / c1


def dv3(t, v):
    return (v[0] - v[1]) / c2 / r2


def f(t, v):
    return np.array([dv1(t, v), dv2(t, v), dv3(t, v)])


def runge_kutte(t, v):
    k1 = h * f(t, v)
    k2 = h * f(t + h / 2, v + k1 / 2)
    k3 = h * f(t + h / 2, v + k2 / 2)
    k4 = h * f(t + h, v + k3)
    return (k1 + 2 * k2 + 2 * k3 + k4) / 6


x = np.linspace(0, t, n)
y = np.zeros(shape=(n, 3))
y[0] = np.array([0, 5, 0])

for i in range(1, n):
    y[i] = y[i - 1] + runge_kutte(i * h, y[i - 1])

plt.figure()
plt.plot(x, y[:, 0], 'r-')
plt.plot(x, y[:, 1], 'g-')
plt.plot(x, y[:, 2], 'b-')
plt.show()