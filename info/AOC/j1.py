with open("j1_input.txt", "r", encoding="utf8") as f:
    code = f.read().strip().split("\n")

cadran = 50
out = 0
for e in code:
    n = int(e[1:])

    if e[0] == "L":
        if cadran == 0:
            cadran = 100

        cadran -= n

        if cadran == 0:
            out += 1
            print("click")

        while cadran < 0:
            cadran += 100
            out += 1
            print("click")
    else:
        cadran += n

        while cadran > 99:
            cadran -= 100
            out += 1
            print("clack")

    print(cadran)

print(f"Total passages = {out}")

# < 6367