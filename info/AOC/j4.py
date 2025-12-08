with open("j4_input.txt", "r", encoding="utf8") as f:
    entree = f.read().strip()


tab = [list(e) for e in entree.split("\n")]


def is_accessible(x, y):
    nb_adj = -1
    for xp in range(max(0, x-1), min(x+2, WIDTH)):
        for yp in range(max(0, y-1), min(y+2, HEIGHT)):
            if tab[xp][yp] == "@":
                nb_adj += 1

    return nb_adj < 4


def simplifier():
    nb_accessibles = 0

    lst = []

    for x in range(WIDTH):
        for y in range(HEIGHT):
            if tab[x][y] == "@" and is_accessible(x, y):
                nb_accessibles += 1
                lst.append((x, y))


    for x, y in lst:
        tab[x][y] = "."        

    return nb_accessibles


WIDTH = len(tab)
HEIGHT = len(tab[0])


out = 0

while True:
    n = simplifier()
    if n == 0:
        break
    out += n


print(out)