from cs50 import get_int


def printPyramid(h):

    for i in range(h):  # loop of rows
        
        n = h - i - 1
        
        for j in range(n):
            
            print(" ", end='')  # print spaces in a row

        for j in range(i+1):  # loop of # in a row

            print("#", end='')  # print # in a row

        print("")  # go next row
        

def main():

    minHeight = 1
    maxHeight = 8

    height = get_int("Type pyramid height:")

    while height < minHeight or height > maxHeight:  # reject wrong inputs and asks again.

        height = get_int("Type pyramid height:")

    printPyramid(height)


if __name__ == "__main__":
    main()