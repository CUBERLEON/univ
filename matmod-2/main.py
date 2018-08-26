import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.optimize import leastsq


def approx_func(a, t, f):
    res = a[0] * t ** 3 + a[1] * t ** 2 + a[2] * t + a[3]

    for i in range(len(f)):
        res += a[i + 4] * math.sin(2 * math.pi * f[i] * t)

    return res


def error_func(a, t, y, f):
    return [(approx_func(a, t[i], f) - y[i]) ** 2 for i in range(len(t))]


def solve(t, y, f):
    K = len(f) + 4
    N = len(t)

    b = np.zeros(K)
    b[0] = np.dot(y, pow(t, 3.0))
    b[1] = np.dot(y, pow(t, 2.0))
    b[2] = np.dot(y, t)
    b[3] = np.sum(y)
    b[4] = np.dot(y, np.sin(2 * np.pi * f[0] * t))

    a = np.zeros((K, K))
    a[0][0] = np.sum(pow(t, 6.0))
    a[0][1] = np.sum(pow(t, 5.0))
    a[0][2] = np.sum(pow(t, 4.0))
    a[0][3] = np.sum(pow(t, 3.0))
    a[0][4] = np.sum(np.sin(2 * np.pi * f[0] * t) * pow(t, 3.0))

    a[1][0] = np.sum(pow(t, 5.0))
    a[1][1] = np.sum(pow(t, 4.0))
    a[1][2] = np.sum(pow(t, 3.0))
    a[1][3] = np.sum(pow(t, 2.0))
    a[1][4] = np.sum(np.sin(2 * np.pi * f[0] * t) * pow(t, 2.0))

    a[2][0] = np.sum(pow(t, 4.0))
    a[2][1] = np.sum(pow(t, 3.0))
    a[2][2] = np.sum(pow(t, 2.0))
    a[2][3] = np.sum(t)
    a[2][4] = np.sum(np.sin(2 * np.pi * f[0] * t) * t)

    a[3][0] = np.sum(pow(t, 3.0))
    a[3][1] = np.sum(pow(t, 2.0))
    a[3][2] = np.sum(t)
    a[3][3] = N
    a[3][4] = np.sum(np.sin(2 * np.pi * f[0] * t))

    a[4][0] = np.sum(pow(t, 3.0) * np.sin(2 * np.pi * f[0] * t))
    a[4][1] = np.sum(pow(t, 2.0) * np.sin(2 * np.pi * f[0] * t))
    a[4][2] = np.sum(t * np.sin(2 * np.pi * f[0] * t))
    a[4][3] = N * np.sum(np.sin(2 * np.pi * f[0] * t))
    a[4][4] = np.sum(np.sin(2 * np.pi * f[0] * t) * np.sin(2 * np.pi * f[0] * t))

    A = np.linalg.inv(a)

    result = np.hstack(np.dot(A, np.vstack(b)))

    return result


with open('f9.txt', 'r') as file:
    y = list(map(float, file.read().split(' ')))

T = 5.
N = len(y)

t = np.linspace(0, T, N)

c_k = [(1 / N) * sum([y[m] * pow(math.e, -1j * 2 * math.pi * k * m / N) for m in range(N)]) for k in range(N)]

c_k_abs = [abs(value) for value in c_k]

delta_f = 1 / T
f_star = []
for k in range(2, N // 2 - 2):
    if c_k_abs[k] > c_k_abs[k - 1] and c_k_abs[k] > c_k_abs[k + 1]:
        print("Found local maximum at k={}, value={}.".format(k, c_k_abs[k]))
        f_star.append(k * delta_f)
print("f_i: {}".format(f_star))

K = len(f_star) + 3
print("K={}".format(K))

a = np.zeros(K + 1)
# a,cov,infodict,mesg,ier = leastsq(error_func, a[:], args=(t, y, f_star), full_output=True)
a = solve(t, y, f_star)

print("a_i: {}".format(a))
print("Error value: {}".format(sum(error_func(a, t, y, f_star))))

y_approx = np.zeros(N)
for i in range(N):
    y_approx[i] = approx_func(a, t[i], f_star)

plt.figure()
plt.plot(t, y_approx, 'b-')
plt.legend(['Least Squares Approximation'])
plt.show()

plt.plot(t, y, 'y-')
plt.legend(['Input'])
plt.show()