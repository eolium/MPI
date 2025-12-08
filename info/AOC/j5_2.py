from tqdm import tqdm

with open("j5_input.txt", "r", encoding="utf8") as f:
    a, b = f.read().split("\n\n")

lst_ok = [[int(i) for i in e.split("-")] for e in a.split("\n")]
lst_tester = [int(e) for e in b.split("\n")]

out = set()

def is_disjoint(u, v):
    return u[1] < v[0] or v[1] < u[0]

def is_included(u, v):
    return v[0] <= u[0] and u[1] <= v[1]


def simplifier(u, v):
    # Simplifie 2 intervalles non-disjoints
    return [min(u[0], v[0]), max(u[1], v[1])]


def simplifier_ensemble(tab):
    for iu, u in enumerate(lst_ok):
        for iv, v in enumerate(lst_ok):
            if iu == iv: continue

            if not is_disjoint(u, v):
                tab[iu] = simplifier(u, v)
                tab.pop(iv)
                return True

    return False

def taille_disjoints(tab):
    return sum([b-a+1 for a, b in tab])


def admissible(n):
    if n <= 280332570045118: return False
    if n >= 352190824482134: return False
    return True


while simplifier_ensemble(lst_ok):
    print(len(lst_ok))

print(taille_disjoints(lst_ok))

print(admissible(taille_disjoints(lst_ok)))

# > 280332570045118