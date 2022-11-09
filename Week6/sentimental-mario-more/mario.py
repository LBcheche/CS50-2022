from cs50 import get_int


def printPyramids(h):

    for i in range(h):  # loop of rows

        n = h - i - 1

        for j in range(n):  # loop of spaces in a row (first pyramid)

            print(" ", end='')  # print spaces in a row

        for j in range(i + 1):  # loop of # in a row (first pyramid)

            print("#", end='')  # print # in a row

        print("  ", end='')  # print space between pyramids

        for j in range(i + 1):  # loop of # in a row (second pyramid)

            print("#", end='')  # print # in a row

        print("")  # go next row
        

def main():

    minHeight = 1
    maxHeight = 8

    height = get_int("Type pyramid height:")

    while height < minHeight or height > maxHeight:  # reject wrong inputs and asks again.

        height = get_int("Type pyramid height:")

    printPyramids(height)


if __name__ == "__main__":
    main()