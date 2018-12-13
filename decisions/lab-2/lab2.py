import numpy as np
from scipy.stats import rankdata

votes = np.array([[5, 3, 1, 2, 8, 4, 6, 7],
                  [5, 4, 3, 1, 8, 2, 6, 7],
                  [1, 7, 5, 4, 8, 2, 3, 6],
                  [6, 4, 2.5, 2.5, 8, 1, 7, 5],
                  [8, 2, 4, 6, 3, 5, 1, 7],
                  [5, 6, 4, 3, 2, 1, 7, 8],
                  [6, 1, 2, 3, 5, 4, 8, 7],
                  [5, 1, 3, 2, 7, 4, 6, 8],
                  [6, 1, 3, 2, 5, 4, 7, 8],
                  [5, 3, 2, 1, 8, 4, 6, 7],
                  [7, 1, 3, 2, 6, 4, 5, 8],
                  [1, 6, 5, 3, 8, 4, 2, 7]])

print("experts options")
N = votes.shape[0]
M = votes.shape[1]
print(N, M)

print("average")
avg = np.average(votes, axis=0)
print(avg)

print("average rank")
avg_rank = rankdata(avg)
print(avg_rank)

print("median")
median = np.median(votes, axis=0)
print(median)

print("median rank")
median_rank = rankdata(median)
print(median_rank)

print("ядро суперечностей")
conflicts = np.zeros(shape=(M, M))
for i in range(M):
    for j in range(M):
        conflicts[i, j] = (avg_rank[i] - avg_rank[j]) * (median_rank[i] - median_rank[j]) < 0
        if conflicts[i, j] and i < j:
            print("({},{})".format(i + 1, j + 1))
print(conflicts)

print("кластеризоване ранжування")
print("avg:\t4 < 3 < {2,6} < 1 < 7 < 5 < 8")
print("median:\t4 < {2,3} < 6 < 1 < 7 < 8 < 5")
print("clst:\t4 < 3 < 2 < 6 < 1 < 7 < {5, 8}")