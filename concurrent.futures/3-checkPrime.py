from concurrent.futures import ThreadPoolExecutor
import math

PREMS = [
    112272535095293,
    112582705942171,
    112272535095293,
    115280095190773,
    115797848077099,
    1099726899285419]


def est_prem(nb):
    if nb % 2 == 0:
        return False
    racine = int(math.floor(math.sqrt(nb)))
    for i in range(3, racine + 1, 2):
        if nb % i == 0:
            return False
    return True


def main():
    with ThreadPoolExecutor(6) as gr:
        res = gr.map(est_prem, PREMS)
        for nb, sont_prem in zip(PREMS, res):
            print('%d est premier? %s' % (nb, sont_prem))


if __name__ == '__main__':
    main()
