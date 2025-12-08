with open("j5_inputj.txt", "r", encoding="utf8") as f:
    a, b = f.read().split("\n\n")

lst_ok = [[int(i) for i in e.split("-")] for e in a.split("\n")]
lst_tester = [int(e) for e in b.split("\n")]

def est_frais(n):
    for a, b in lst_ok:
        if a <= n and n <= b:
            return True
    return False


s = sum(map(est_frais, lst_tester))

print(s)