import matplotlib.image as img
import matplotlib.pyplot as plt
import numpy as np


def grevill(x):
    m = len(x)
    a = np.vstack(x[0])
    A = np.vstack(np.transpose(a))

    if (np.dot(np.transpose(a), a) == 0):
        pinv = a
    else:
        pinv = a / (np.dot(np.transpose(a), a))

    for k in range(1, m):
        a = np.vstack(x[k])
        pAA = np.dot(pinv, A)
        Z = np.eye(len(pAA[0])) - pAA
        atZa = np.dot(np.dot(np.transpose(a), Z), a)
        A = np.vstack((A, np.transpose(a)))
        if (np.abs(atZa) < 1e-5):
            R = np.dot(pinv, np.transpose(pinv))
            atRa = np.dot(np.dot(np.transpose(a), R), a)
            pinv = pinv - (np.dot(np.dot(np.dot(R, a), np.transpose(a)), pinv)) / (1 + atRa)
            pinv = np.hstack((pinv, np.dot(R, a) / (1 + atRa)))
        else:
            pinv = pinv - (np.dot(np.dot(np.dot(Z, a), np.transpose(a)), pinv)) / atZa
            pinv = np.hstack((pinv, np.dot(Z, a) / atZa))

    return pinv


def moore_penrouse(x):
    rows = len(x)
    cols = len(x[0])

    delta = 64
    eps = 0.00002
    diff = 10000
    while diff > eps:
        if cols > rows:
            x1 = np.dot(np.transpose(x),
                        np.linalg.inv(np.dot(x, np.transpose(x)) + np.dot(np.dot(delta, delta), np.eye(rows))))
        else:
            x1 = np.dot(np.linalg.inv(np.dot(np.transpose(x), x) + np.dot(np.dot(delta, delta), np.eye(cols))),
                        np.transpose(x))

        delta = delta / 2

        if cols > rows:
            x2 = np.dot(np.transpose(x),
                        np.linalg.inv(np.dot(x, np.transpose(x)) + np.dot(np.dot(delta, delta), np.eye(rows))))
        else:
            x2 = np.dot(np.linalg.inv(np.dot(np.transpose(x), x) + np.dot(np.dot(delta, delta), np.eye(cols))),
                        np.transpose(x))

        diff = np.linalg.norm(x1 - x2)
    return x1


x = img.imread('x1.bmp')
y = img.imread('y9.bmp')

n = len(x[0])

d = np.ones(n)
x = np.vstack((x, d))

m = len(x)


def check(xp):
    assert np.linalg.norm(np.dot(np.dot(x, xp), x) - x) < 1e-5
    assert np.linalg.norm(np.dot(np.dot(xp, x), xp) - xp) < 1e-5
    assert np.linalg.norm(np.dot(xp, x) - np.transpose(np.dot(xp, x))) < 1e-5
    assert np.linalg.norm(np.dot(x, xp) - np.transpose(np.dot(x, xp))) < 1e-5

def find_a(xp):
    check(xp)

    x_xp = np.dot(x, xp)
    z = np.eye(m) - x_xp

    v = np.random.rand(len(y), m)

    a = np.dot(y, xp) + np.dot(v, np.transpose(z))

    res = np.dot(a, x)

    res.astype(np.uint8)
    plt.imshow(res, cmap='gray', vmin=0, vmax=255)
    plt.show()


find_a(grevill(x))
find_a(moore_penrouse(x))
