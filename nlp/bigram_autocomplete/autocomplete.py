import sys

from bigram_autocomplete.count_dict import CountDict
from bigram_autocomplete.probabilities import ProbabilityDict


def main():
    train_path = "../data/bible.txt"
    smoothing = "laplace"

    # читаємо навчальний текст і
    # обчислюємо для нього уніграми, біграми
    counts = CountDict(train_path)
    counts.populate()

    # обчислюємо достовірності кожної біграми
    # обраним методом
    probs = ProbabilityDict(counts)

    if smoothing == "mle":
        choice = probs.bigram_MLE()
    if smoothing == "laplace":
        choice = probs.bigram_laplace()
    else:
        pass

    # реалізація консольного автодоповнення
    str = ""
    print("> ", end='')
    while True:
        # зчитуємо нове слово від користувача
        word = input()
        str = str + word
        word = str.split(' ')[-1]
        word = word.strip().lower()

        # добуваємо відповідний список можливих наступних слів,
        # з їхніми достовірностями, потім упорядковуємо у порядку спадання,
        # і виводимо найкращі 15 користувачу
        choices = [(key[1], choice[key]) for key in counts.bigram_profile[word]]
        choices = sorted(choices, key=lambda x: x[1], reverse=True)
        filtered = list(filter(lambda x: "<unk>" not in x, choices))
        filtered = [x[0] for x in filtered]
        for word in filtered[:15]:
            print("* {}".format(word), end=' ')
        print()

        print("> {}".format(str), end='')
        sys.stdout.flush()


if __name__ == '__main__':
    main()
