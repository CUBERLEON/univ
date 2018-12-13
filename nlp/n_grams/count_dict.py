from collections import defaultdict
import re

# допоміжний клас для зчитування навчального тексту і
# обчислення для нього N-1 та N-грам
class CountDict(object):
    def __init__(self, filename, n):
        self.filename = filename
        self.n1 = n-1
        self.n2 = n

        # ініціалізуємо словники N-1 та N-грам
        self.unigrams = defaultdict(int)
        self.n1_grams = defaultdict(int)
        self.n2_grams = defaultdict(int)

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
                        self.unigrams[(word,)] += 1

                    # заповнюємо словник N-1-грам,
                    # підраховуючи кільки кожної
                    for i in range(len(tokens) - self.n1 + 1):
                        n1_gram = tuple()
                        for j in range(self.n1):
                            n1_gram = n1_gram + (tokens[j], )
                        self.n1_grams[n1_gram] += 1

                    # заповнюємо словник N-грам,
                    # підраховуючи кільки кожної
                    for i in range(len(tokens) - self.n2 + 1):
                        n2_gram = tuple()
                        for j in range(self.n2):
                            n2_gram = n2_gram + (tokens[j], )
                        self.n2_grams[n2_gram] += 1

        # к-ть унікальних уніграм та біграм
        self.unique_unigrams = len(self.unigrams.keys())
        self.unique_n1_grams = len(self.n1_grams.keys())
        self.unique_n2_grams = len(self.n2_grams.keys())
