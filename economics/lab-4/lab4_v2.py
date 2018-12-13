import math
from functools import partial

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import solve_ivp, odeint


### 1

# br - birth rate
# dr - death rate
# cr - inner competition rate
from scipy.optimize import broyden1


def f(t, n, br, dr, cr):
    return br * (n ** 2) / (n + 1) - dr * n - cr * (n ** 2)


br = 36
dr = 15
cr = 3
n_0 = 15

d = (cr + dr - br) ** 2 - 4 * cr * dr
n_max = (-cr - dr + br + math.sqrt(d)) / (2 * cr)
n_min = (-cr - dr + br - math.sqrt(d)) / (2 * cr)
print("N_min, N_max: {}, {}".format(n_min, n_max))

a, b, m = 0, 1, 100000
t_vals = np.linspace(a, b, m)

offset = 0.25
options = [
    (n_min / 2 - offset, "1"),
    (n_min / 2 + offset, "2"),
    (n_min, "3"),
    ((n_max + n_min) / 2 - 1, "4_1"),
    ((n_max + n_min) / 2 + offset, "4_2"),
    (n_max, "5"),
    (n_max + offset, "6")
]

for n_0, label in options:
    n_vals = solve_ivp(partial(f, br=br, dr=dr, cr=cr), [a, b], [n_0], t_eval=t_vals).y[0]
    plt.plot(t_vals, n_vals, label="{}: {}".format(label, n_0))

plt.xlabel('T')
plt.ylabel('N')
plt.legend()
plt.show()


### 2


def f(t, n):
    return 0.0004 * (n ** 2) - 0.06 * n


n_0_1 = 50
n_0_2 = 100
t1 = 20

options = [
    100,
    200,
]

a, b, m = 0, 22, 100000
t_vals = np.linspace(a, b, m)

for n_0 in options:
    n_vals = solve_ivp(f, [a, b], [n_0], t_eval=t_vals).y[0]
    print("N(0)={}: N(20)={}".format(n_0, n_vals[int((20 - a) / (b - a) * m)]))
    plt.plot(t_vals, n_vals, label="N(0)={}".format(n_0))

plt.xlabel('Month')
plt.ylabel('N')
plt.legend()
plt.show()


### 3


def draw_trajectory(f, n0, t):
    s = odeint(f, n0, t)

    plt.plot(t, s[:, 0], 'r--', linewidth=2.0)
    plt.plot(t, s[:, 1], 'b-', linewidth=2.0)
    plt.xlabel("t")
    plt.ylabel("N")
    plt.legend(['N1, N1(0)={}'.format(n0[0]),
                'N2, N2(0)={}'.format(n0[1])])
    plt.show()


def draw_phase_portrait(n1_vals, n2_vals, n1_dot, n2_dot, n1_eq, n2_eq):
    plt.streamplot(n1_vals, n2_vals, n1_dot, n2_dot)

    for i in range(len(n1_eq)):
        print("N1_eq={:.2f}, N2_eq={:.2f}".format(n1_eq[i], n2_eq[i]))

    plt.plot(n1_eq, n2_eq, 'ro')

    plt.xlabel('N1')
    plt.ylabel('N2')
    plt.grid()
    plt.show()


n1_vals, n2_vals = np.meshgrid(np.arange(1, 300, 1), np.arange(1, 300, 1))
t = np.linspace(0, 2)
n0 = [5, 50]

eps_1 = 150
gama_1 = 1
eps_2 = 20
gama_2 = 0.75

# 1
print("#1")


def f1(n, t=0):
    n1, n2 = n[0], n[1]
    dn1_dt = (eps_1 - gama_1 * n2) * n1
    dn2_dt = (-eps_2 + gama_2 * n1) * n2
    return [dn1_dt, dn2_dt]


draw_trajectory(f1, n0, t)

n1_eq = [0, eps_2 / gama_2]
n2_eq = [0, (eps_1 * gama_2) / (gama_1 * gama_2)]
draw_phase_portrait(n1_vals, n2_vals, *f1([n1_vals, n2_vals]), n1_eq, n2_eq)

# 2
print("#2")
gama_11 = 1


def f2(n, t=0):
    n1, n2 = n[0], n[1]
    dn1_dt = (eps_1 - gama_1 * n2 - gama_11 * n1) * n1
    dn2_dt = (-eps_2 + gama_2 * n1) * n2
    return [dn1_dt, dn2_dt]


draw_trajectory(f2, n0, t)

n1_eq = [0, eps_2 / gama_2, eps_1 / gama_11]
n2_eq = [0, (eps_1 * gama_2 - gama_11 * eps_2) / (gama_1 * gama_2), 0]
draw_phase_portrait(n1_vals, n2_vals, *f2([n1_vals, n2_vals]), n1_eq, n2_eq)

# 3
print("#3")
gama_11 = 1
gama_12 = 1
gama_21 = 1
gama_22 = 1


def f3(n, t=0):
    n1, n2 = n[0], n[1]
    dn1_dt = (eps_1 - gama_11 * n2 - gama_12 * n1) * n1
    dn2_dt = (-eps_2 + gama_21 * n1 - gama_22 * n2) * n2
    return [dn1_dt, dn2_dt]


draw_trajectory(f3, n0, t)

n1_eq = [0, (eps_2 * gama_11 + gama_22 * eps_1) / (gama_21 * gama_11 + gama_12 * gama_22),
         0,
         eps_1 / gama_12]
n2_eq = [0, (eps_1 - gama_12 * n1_eq[1]) / gama_11,
         -eps_2 / gama_22,
         0]
draw_phase_portrait(n1_vals, n2_vals, *f3([n1_vals, n2_vals]), n1_eq, n2_eq)

# 4
print("#4")
gama_1 = gama_2 = 2
alpha_1 = alpha_2 = 1
beta = 1


def f4(n, t=0):
    n1, n2 = n[0], n[1]
    dn1_dt = (eps_1 - gama_1 * n1) * n1 - alpha_1 * n1 * n2 / (1 + beta * n1)
    dn2_dt = -(eps_2 + gama_2 * n2) * n2 + alpha_2 * n1 * n2 / (1 + beta * n1)
    return [dn1_dt, dn2_dt]


draw_trajectory(f4, n0, t)

s_point = broyden1(f4, [100, -20])

n1_eq = [0, s_point[0],
         0, eps_1 / gama_1]
n2_eq = [0, s_point[1],
         -eps_2 / gama_2, 0]
draw_phase_portrait(n1_vals, n2_vals, *f4([n1_vals, n2_vals]), n1_eq, n2_eq)
