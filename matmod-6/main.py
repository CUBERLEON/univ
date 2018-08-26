import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import math


def blurmat(m, n, r):
    H = np.zeros((m, n))
    for y in range(m):
        for x in range(n):
            if x ** 2 + y ** 2 <= r ** 2:
                H[y, x] = 1 / (math.pi * (r ** 2))
            else:
                H[y, x] = 0
    return H


def unfocus(X, r):
    m, n = np.shape(X)
    Y = blurmat(m, n, r)
    Z = np.fft.irfft2(np.multiply(np.fft.rfft2(X), np.fft.rfft2(Y)))
    return Z


def focus(Z, r):
    m, n = np.shape(Z)
    X = blurmat(m, n, r)
    Y = np.fft.irfft2(np.divide(np.fft.rfft2(Z), np.fft.rfft2(X)))
    return Y


def scale2im(X):
    T = X - X.min()
    Y = np.uint8(T / T.max() * 255)
    return Y


def evalfocus(X):
    m, n = np.shape(X)
    err = 0
    for y in range(m):
        for x in range(n):
            e = 0
            c = 0
            if y > 0:
                e = e + np.abs(X[y, x] - X[y - 1, x])
                c = c + 1
            if y < m - 1:
                e = e + np.abs(X[y, x] - X[y + 1, x])
                c = c + 1
            if x > 0:
                e = e + np.abs(X[y, x] - X[y, x - 1])
                c = c + 1
            if x < n - 1:
                e = e + np.abs(X[y, x] - X[y, x + 1])
                c = c + 1
            err = err + e / c
    return err


def autofocus(U, lb, ub):
    r = lb
    errs = np.zeros(ub + 1)
    for i in range(lb, ub + 1):
        curY = focus(U, i / 8)
        errs[i] = evalfocus(curY)

        if errs[i] < errs[r]:
            r = i

    Y = focus(U, r / 8)
    return Y, r


img_unfocused = np.double(mpimg.imread('y_blured_9.bmp'))
plt.imshow(img_unfocused, cmap='gray')
plt.title('Before focusing')
plt.show()

# print(evalfocus(img))

F, r = autofocus(img_unfocused, 30, 50)

print("err =", evalfocus(F))
print("r =", r / 8)
plt.imshow(scale2im(F), cmap='gray')
plt.title('Focused')
plt.show()

img_focused = np.double(mpimg.imread('y1.bmp'))
plt.imshow(img_focused, cmap='gray')
plt.title('Before unfocusing')
plt.show()

r = 7
U = unfocus(img_focused, r)
plt.imshow(scale2im(U), cmap='gray')
plt.title('Unfocused')
plt.show()