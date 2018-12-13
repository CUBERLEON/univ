from collections import defaultdict
import re

# допоміжний клас для зчитування навчального тексту і
# обчислення для нього уніграм, біграм
class CountDict(object):
    def __init__(self, filename):
        self.filename = filename

        # ініціалізуємо словники уніграм та біграм
        self.unigrams = defaultdict(int)
        self.bigrams = defaultdict(int)

    def populate(self):
        # регулярний вираз символів, які будуть вилучаютися
        to_remove = re.compile("(,)|(;)|(:)|(!)|(\?)|(\")|(\()|(\))|(\[)|(\])|(\s-)")

        # зчитуємо навчальний файл порядково
        with open(self.filename) as f:
            for line in f:
                line = line.lower()
                sentences = line.split('.')

                # виділяємо окремо речення, і структурні елементи речень,
                # щоб уникнути помилкових рекомендацій
                for sentence in sentences:
                    # переводимо рядок до нижнього регістру
                    # і вилучаємо символи за допомогою попередньої регулярки
                    words = to_remove.sub("", sentence)

                    # розділяємо рядок на токени (слова)
                    tokens = words.split()

                    # заповнюємо словник уніграм,
                    # підраховуючи кільки кожної
                    for word in tokens:
                        self.unigrams[word] += 1

                    # заповнюємо словник біграм,
                    # підраховуючи кільки кожної
                    for i in range(len(tokens) - 1):
                        bigram = (tokens[i], tokens[i + 1])
                        self.bigrams[bigram] += 1

        # к-ть унікальних уніграм та біграм
        self.unique_unigrams = len(self.unigrams.keys())
        self.unique_bigrams = len(self.bigrams.keys())

        # заповнюємо словник відповідностей між словом
        # там можливими доповненнями згідно з навчальним файлом
        self.bigram_profile = defaultdict(set)

        for key in self.bigrams:
            w1, w2 = key
            self.bigram_profile[w1].add(key)
