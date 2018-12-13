import math
from collections import defaultdict

import numpy
from matplotlib import pyplot

from n_grams.count_dict import CountDict


def main():
    # train_path = "../data/bible.txt"
    train_path = "../data/alice_in_wonderland.txt"

    # читаємо навчальний текст і
    # обчислюємо для нього N-1 та N-грами
    counts = CountDict(train_path, n=2)
    counts.populate()
    print("Populated dicts")

    # оцінюємо параметри згладжування для алгоритму Віттена-Белла:
    # знаходимо для кожної N-1-грами кількість типів N-грам,
    # які можна утворити з даної N-1-грами у даному корпусі
    extend_freqs = defaultdict(int)
    for n1_gram in counts.n1_grams.keys():
        extend_freqs[n1_gram] = 0
    for n2_gram in counts.n2_grams.keys():
        n1_gram = n2_gram[:-1]
        if n1_gram in counts.n1_grams:
            extend_freqs[n1_gram] += 1

    extend_freqs = [[key, count] for key, count in extend_freqs.items()]
    extend_freqs = sorted(extend_freqs, key=lambda x: x[1], reverse=True)
    print("Extend freqs: ", extend_freqs[:50])

    # оцінюємо параметри згладжування для алгоритму Гуда-Тюрінга
    # будуємо частотний словник частот N-грам (тобто частота і скільки є типів N-грам, що мають цю частоту)
    freq_freqs = defaultdict(int)
    for freq in counts.n2_grams.values():
        freq_freqs[freq] += 1
    freq_freqs[0] = counts.unique_n1_grams * counts.unique_unigrams - counts.unique_n2_grams
    print("Freq freq: ", freq_freqs)

    # поріг Катца (Katz threshold)
    # k = 5
    # for i in range(0, k):
    #     old_freq = freq_freqs[i]
    #     freq_freqs[i] = (((i + 1) * (freq_freqs[i + 1]) / (freq_freqs[i])) -
    #                      ((freq_freqs[k + 1] * i * (k + 1)) / freq_freqs[1])) /\
    #                     (1 - (freq_freqs[k + 1] * (k + 1)) / (freq_freqs[1]))
    #     print('{}: {} -> {}'.format(i, old_freq, freq_freqs[i]))

    bounds = [0, 100]
    x = numpy.arange(*bounds, 1)
    y = [math.log(freq_freqs[x]+1) for x in x]
    # y = [freq_freqs[x] for x in x]

    pyplot.plot(x, y)
    pyplot.xlabel('freq')
    pyplot.ylabel('freq freq')
    pyplot.show()


if __name__ == '__main__':
    main()
