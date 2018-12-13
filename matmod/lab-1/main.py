import numpy as np
import matplotlib.pyplot as plt
import math

with open('f9.txt', 'r') as file:
    y = list(map(float, file.read().split(' ')))

T = 5.
N = len(y)

c_k = [(1 / N) * sum([y[m] * pow(math.e, -1j * 2 * math.pi * k * m / N) for m in range(N)]) for k in range(N)]

c_k_abs = [abs(value) for value in c_k]

delta_f = 1 / T
f_star = []
for k in range(2, N - 2):
    if c_k_abs[k] > c_k_abs[k - 1] and c_k_abs[k] > c_k_abs[k + 1]:
        print("Found local maximum at k={}, value={}.".format(k, c_k_abs[k]))
        f_star.append(k * delta_f)
print("Frequencies: {}".format(f_star))

plt.plot(np.arange(0, T, T / N), c_k_abs)
plt.show()