import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp

A11 = np.array([[0.5, 0.2],
                [0.1, 0.4]])
A12 = np.array([[0.2, 0.1]]).T
A21 = np.array([[0.2, 0.2]])
A22 = np.array([0.5])

B1 = np.array([[0.8, 0.3], [1.0, 0.9]])
B2 = np.array([[0.3, 0]]).T

x1_0 = np.array([[1500, 1000]]).T


def c1(t): return np.array([500, 200]) * np.exp(0.1 * t)


c2 = 400

bounds = [0, 0.7]
t = np.linspace(*bounds, 100)

A22_neg_inv = np.linalg.inv(np.eye(1) - A22)
A1 = A11 + A12 @ A22_neg_inv @ A21
B = B1 + B2 @ A22_neg_inv @ A21


def c(t):
    return c1(t) - np.dot(A22_neg_inv.T, A12.T).reshape(A12.shape[0]) * c2


# general

B_inv = np.linalg.inv(B)
A1_neg_inv = np.linalg.inv(np.eye(A1.shape[0]) - A1)


def x_der(t, x):
    return B_inv @ x - B_inv @ A1 @ x - B_inv @ c(t)


x1 = solve_ivp(x_der, bounds, x1_0.reshape(2), t_eval=t).y
x2 = A22_neg_inv @ (A21 @ x1 - c2)

plt.plot(t, x1[0], t, x1[1], t, x2[0])
plt.legend(['x1_0', 'x1_1', 'x2'])
plt.show()


# closed

def y_der(t, y):
    return (np.eye(A1.shape[0]) - A1) @ B_inv @ y


y1 = solve_ivp(y_der, bounds, x1_0.reshape(2), t_eval=t).y
y2 = A22_neg_inv @ (A21 @ y1 - c2)

plt.plot(t, y1[0], t, y1[1], t, y2[0])
plt.legend(['y1_0', 'y1_1', 'y2'])
plt.show()

# tech growth factor

eigen_values, _ = np.linalg.eig(A1_neg_inv @ B)
tech_growth = 1.0 / np.max(eigen_values)
print('Technology growth factor: {:.2f}'.format(tech_growth))

st = np.exp(tech_growth * t)

plt.plot(x1[0], x1[1])
plt.plot(y1[0], y1[1])
plt.plot(st * 1500, st * 1000)
plt.legend(['general', 'closed', 'techno'])
plt.show()
