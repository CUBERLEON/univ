from collections import defaultdict


# допоміжний клас, який у разі відсутності ключа
# заповнює певним значенням, яке видає фабрика
class CustomDict(dict):
    def __init__(self, factory):
        self.factory = factory

    def __missing__(self, key):
        self[key] = self.factory(key)
        return self[key]

# допоміжний клас для знаходження достовірностей біграм
class ProbabilityDict(object):
    def __init__(self, count_dict):
        self.counts = count_dict

        # знаходиться кількість унікальних слів та загальна кількість вжитих слів
        # та виводиться на екран
        self.vocab_size_total = sum(self.counts.unigrams.values())
        self.vocab_size_unique = len(self.counts.unigrams.keys())
        print("Vocab size: total={}, unique={}".format(self.vocab_size_total, self.vocab_size_unique))

    # Maximum Likelihood Estimate
    # методом максимальної достовірності знаходяться достовірності  біграм
    def bigram_MLE(self):
        mle = defaultdict(float)
        for key, value in self.counts.bigrams.items():
            w1, w2 = key
            mle[key] = value / self.counts.unigrams[w1]

        return mle

    # Additive or Laplase smoothing
    # методом Лапласового згладжування знаходяться достовірності біграм
    def bigram_laplace(self):
        laplace_dict = defaultdict()

        # коефіцієнт згладжування
        alpha = 1

        for key, value in self.counts.bigrams.items():
            w1, w2 = key
            laplace_dict[key] = (value + alpha) / (self.counts.unigrams[w1] + self.vocab_size_unique * alpha)
        return laplace_dict
