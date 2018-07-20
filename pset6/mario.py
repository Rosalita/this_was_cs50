import cs50

while True:
    print('Height:', end="")
    height = cs50.get_int()
    if height >= 0 and height <= 23:
        break

stringlen = int(height) + 1
hashes = 2
spaces = stringlen - hashes

for i in range(height):

    print(spaces * ' ', end="")
    print(hashes * '#', end="")
    print()

    hashes += 1
    spaces -= 1
