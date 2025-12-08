from collections import Counter
from tqdm import tqdm
from itertools import combinations
from sys import stdout
from time import sleep

def admissible(N):
    if N > 17144: return True
    return False

def maxi(tab):
    assert len(tab) > 0

    m = tab[0]
    im = 0

    for i in range(1, len(tab)):
        if tab[i] > m:
            m = tab[i]
            im = i

    return (im, m)


def get_val_max(banque: list, n: int) -> int:
    assert n <= len(banque)

    if len(banque) == 1:
        return [banque[0]]

    if n == 1:
        return [max(banque)]

    ia, a = maxi(banque[:-n+1])

    return [a] + get_val_max(banque[ia+1:], n-1)


def str_to_list(chaine: str) -> list:
    return [int(e) for e in chaine]


with open("j3_input.txt", "r", encoding="utf8") as f:
    entree = [str_to_list(e) for e in f.read().strip().split("\n")]


somme = 0

for i_banque, banque in enumerate(entree):
    t = get_val_max(banque, 2)
    n = int("".join([str(e) for e in t]))
    somme += n

    stdout.write(f"\rBanque {i_banque+1}/{len(entree)}, N = {n}")
    stdout.flush()

print("\n")
print(somme)
print(admissible(somme))