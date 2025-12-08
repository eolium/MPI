from pprint import pprint
from tqdm import tqdm

with open("j6_input.txt", "r", encoding="utf8") as f:
    tab = [[m for m in e.split(' ') if m != ''] for e in f.read().split("\n")]



operations = tab[-1]
tab.pop(-1)

somme = 0

for i in tqdm(range(len(tab[0]))):
    # i parcourt chaque colonne
    ligne_calcul = ""
    for j in range(len(tab)-1):
        # j parcourt chaque ligne
        ligne_calcul += tab[j][i]
        ligne_calcul += operations[i]
    ligne_calcul += tab[-1][i]

    somme += eval(ligne_calcul)

print(somme)