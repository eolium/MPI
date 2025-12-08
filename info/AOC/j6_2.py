from pprint import pprint
from tqdm import tqdm

with open("j6_input.txt", "r", encoding="utf8") as f:
    data = f.read()
    tab = [list(e) for e in data.split("\n")]

somme = 0

operation = ""


pprint(tab)

ligne_calcul = ""
ajout = ""

for j in tqdm(range(len(tab[0]))):

    zero = 0

    for i in range(len(tab)-1):
        if tab[-1][j] != " ":
            operation = tab[-1][j]

        if tab[i][j] != " " and tab[i][j] != "":
            ajout += tab[i][j]
        else:
            zero += 1
    
    if zero == len(tab)-1:

        if len(ajout) > 0:
            if ajout[0] in ["+", "*"]:
                ajout = ajout[1:]
            if ajout[-1] in ["+", "*"]:
                ajout = ajout[:-1]

        ligne_calcul += ajout + " + "
        ajout = ""

    ajout += operation



if len(ajout) > 0:
    if ajout[0] in ["+", "*"]:
        ajout = ajout[1:]
    if ajout[-1] in ["+", "*"]:
        ajout = ajout[:-1]

ligne_calcul += ajout
ajout = ""


print(eval(ligne_calcul))