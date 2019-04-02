import json
import sys
import re
import math
import numpy as np


def main():
    bot_name = "Чарлі"
    user_greeting = f"Вітаємо! Веселих Вам балачок із нашим ботом {bot_name}!"
    default_user_name = "Користувач"

    print(user_greeting)
    print("Яке ваше імя?")
    print(f"> ", end='')
    user_name = input()
    if not user_name:
        user_name = default_user_name

    with open("responses.json") as file:
        dataset = json.load(file)

    while True:
        print(f"{user_name}: ", end='')

        # зчитуємо нову репліку від користувача
        sentence = input()
        sentence = sentence.strip().lower()
        if not sentence:
            continue

        # добуваємо відповідний список можливих реплік бота і обираємо якийсь із них
        options = []

        for option in dataset:
            if re.fullmatch(option['request'], sentence, flags=re.IGNORECASE):
                options.append(option)

        assert (len(options) > 0)

        pos = min(len(options) - 1, int(math.fabs(np.random.randn()) * 2))
        final_option = options[pos]

        print(f"{bot_name}: {final_option['response']}")


if __name__ == "__main__":
    main()
