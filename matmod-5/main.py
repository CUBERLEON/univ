import numpy as np
import matplotlib.pyplot as plt


def fmap(fs, x):
    return np.array([f(*x) for f in fs])


def runge_kutta4_system(fs, x, y0):
    h = x[1] - x[0]
    y = np.ndarray((len(x), len(y0)))
    y[0] = y0
    for i in range(1, len(x)):
        k1 = h * fmap(fs, [x[i - 1], *y[i - 1]])
        k2 = h * fmap(fs, [x[i - 1] + h/2, *(y[i - 1] + k1/2)])
        k3 = h * fmap(fs, [x[i - 1] + h/2, *(y[i - 1] + k2/2)])
        k4 = h * fmap(fs, [x[i - 1] + h, *(y[i - 1] + k3)])
        y[i] = y[i - 1] + (k1 + 2*k2 + 2*k3 + k4) / 6
    return y


def gen_A(m1, m2, m3, c1, c2, c3, c4):
    A = np.zeros((6, 6))
    A[0, 1] = 1
    A[1, 0] = -(c2 + c1)/m1
    A[1, 2] = c2/m1
    A[2, 3] = 1
    A[3, 0] = c2/m2
    A[3, 2] = -(c3 + c2)/m2
    A[3, 4] = c3/m2
    A[4, 5] = 1
    A[5, 2] = c3/m3
    A[5, 4] = -(c4 + c3)/m3
    return A


def eval_y(A, t, y0):
    fs = []
    for i in range(6):
        fun = (lambda i: lambda *args: np.dot(A[i], np.array(args[1:])))(i)
        fs.append(fun)
    return np.transpose(runge_kutta4_system(fs, t, y0))


def plt_y(t, y, title):
    plt.figure()
    plt.plot(t, y[0], 'r', label='y1(t)')
    plt.plot(t, y[1], 'r--', label='y2(t)')
    plt.plot(t, y[2], 'g', label='y3(t)')
    plt.plot(t, y[3], 'g--', label='y4(t)')
    plt.plot(t, y[4], 'b', label='y5(t)')
    plt.plot(t, y[5], 'b--', label='y6(t)')
    plt.xlabel('t')
    plt.ylabel('y')
    plt.title(title)
    plt.legend(loc='best')
    plt.show()


y_ans = np.transpose(np.loadtxt('y9.txt').T)
y0 = y_ans[:, 0]

n = 251
eps = 1e-7

dt, T = 0.2, 50
t = np.linspace(0, T, n)
beta0 = np.array([0.2, 0.1, 9]) # c2, c4, m1
c1, c3, m2, m3 = 0.14, 0.2, 28, 18

I = 1
beta = beta0

while I > eps:
    sum1 = 0
    sum2 = 0
    sumI = 0

    A = gen_A(beta[2], m2, m3, c1, beta[0], c3, beta[1])

    dAdc2 = np.zeros((6, 6))
    dAdc4 = np.zeros((6, 6))
    dAdm1 = np.zeros((6, 6))

    dAdc2[1][0] = -1 / beta[2]
    dAdc2[  1][2] = 1 / beta[2]
    dAdc2[3][0] = 1 / m2
    dAdc2[3][2] = -1 / m2

    dAdc4[5][4] = -1 / m3

    dAdm1[1][0] = (beta[0] + c1) / (beta[2] * beta[2])
    dAdm1[1][2] = -beta[0] / (beta[2] * beta[2])

    U = np.zeros((6, 3))
    y1 = np.vstack(y0)
    y2 = np.zeros((6, 1))

    for i in range(1, n):
        array = np.dot(dAdc2, y1)
        array = np.hstack((array, np.dot(dAdc4, y1)))
        array = np.hstack((array, np.dot(dAdm1, y1)))
        K1 = dt * (np.dot(A, U) + array)
        K2 = dt * (np.dot(A, U + np.dot(1 / 2, K1)) + array)
        K3 = dt * (np.dot(A, U + np.dot(1 / 2, K2)) + array)
        K4 = dt * (np.dot(A, U + K3) + array)
        U = U + 1 / 6 * (K1 + 2 * K2 + 2 * K3 + K4)
        sum1 = sum1 + np.dot(np.transpose(U), U)

        k1 = dt * np.dot(A, y1)
        k2 = dt * np.dot(A, y1 + 1 / 2 * k1)
        k3 = dt * np.dot(A, y1 + 1 / 2 * k2)
        k4 = dt * np.dot(A, y1 + k3)
        y2 = y1 + 1 / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
        sum2 = sum2 + np.dot(np.transpose(U), np.vstack(y_ans[:, i]) - y2)
        sumI = sumI + np.dot(np.transpose(np.vstack(y_ans[:, i]) - y2), np.vstack(y_ans[:, i]) - y2)
        y1 = y2

    dBeta = np.dot(np.linalg.inv(sum1 * dt), (sum2 * dt))
    beta = beta + np.transpose(dBeta)[0]
    I = (sumI * dt)
beta_res = beta

print("beta(c2,c4,m1) =", beta_res)
print("I =", I)

plt_y(t, y_ans, 'Original data from file')

c2, c4, m1 = beta_res
A = gen_A(m1, m2, m3, c1, c2, c3, c4)
y_gen = eval_y(A, t, y0)

plt_y(t, y_gen, 'Generated data')