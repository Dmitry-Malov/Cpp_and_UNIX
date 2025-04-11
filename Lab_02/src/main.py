import random
import time


def iteration(x):
    return x ** 2 - x ** 2 + x * 4 - x * 5 + x + x


def run_experiment(n, x):
    start_time = time.time()
    for _ in range(n):
        iteration(x)

    sum_time = time.time() - start_time
    print(f"Время {n} итераций: {sum_time:.6f}")


def is_number(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


def main():
    while True:
        user_input = input("Введите количесто итераций (или не число для выхода из программы)")

        if not is_number(user_input):
            print("Работа программы завершена")
            break

        n = int(user_input)
        x = random.randint(1, 100000)
        run_experiment(n, x)


main()
