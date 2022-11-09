from cs50 import get_int


def main():

    cardNumber = get_int("Type credit card number:")

    while cardNumber < 0:  # prompt an non-negative number input for user

        cardNumber = get_int("Type credit card number:")

    cardLenght = countCardLenght(cardNumber)

    if checkCardLenght(cardLenght) == True:

        if checkSum(cardNumber, cardLenght) == True:

            checkAndprintlag(cardNumber, cardLenght)

        else:

            print("INVALID\n")

    else:

        print("INVALID\n")


def countCardLenght(cN):  # counts card number's lenght

    for i in range(13, 17, 1):

        powerX = (10 ** i)
        rest = cN / powerX

        if int(rest) == 0:

            return i

    return 0


def checkCardLenght(cL):  # checks if card number's lenght iguals 13, 15 ou 16

    if cL == 13 or cL == 15 or cL == 16:

        return True

    else:

        return False


def checkSum(cN, cL):  # calculates and verify checkSum Luhn's Algorithm

    firstSum = 0      # first Sum for checkSum Luhn's Algorithm
    secondSum = 0     # second Sum for checkSum Luhn's Algorithm
    toggle = False    # boolean to alternate between sums.

    for i in range(cL):

        digit = cN % 10
        cN = cN / 10

        if toggle == True:  # calculate first sum

            digit = int(digit) * 2   # each single digit from card Number

            if digit >= 10:

                secondDigit = int(digit)  # second digit if first digit > 9 after times 2
                secondDigit = int(secondDigit) / 10
                digit = int(digit) % 10
                digit += int(secondDigit)

            firstSum += int(digit)
            toggle = False

        else:  # calculate second sum

            secondSum += int(digit)
            toggle = True
    
    secondSum += int(firstSum)  # final sum for second sum

    if secondSum % 10 == 0:

        return True

    else:

        return False


def checkAndprintlag(cN, cL):  # checks first digits and prints flag for AMEX, MASTERCARD and VISA

    checkAndprintlagLenght13(cN, cL)
    checkAndprintlagLenght15(cN, cL)
    checkAndprintlagLenght16(cN, cL)


def checkAndprintlagLenght13(cN, cL):  # checks first digits and prints flag for leght 13 card's size

    if cL == 13:

        fD = cN / (10 ** 12)
        if int(fD) == 4:

            print("VISA\n")

        else:

            print("INVALID\n")


def checkAndprintlagLenght15(cN, cL):  # checks first digits and prints flag for leght 15 card's size

    if cL == 15:

        fD = cN / (10 ** 13)  # first digits

        if int(fD) == 34 or int(fD) == 37:

            print("AMEX\n")

        else:

            print("INVALID\n")


def checkAndprintlagLenght16(cN, cL):  # checks first digits and prints flag for leght 16 card's size

    if cL == 16:

        fD = cN / (10 ** 14)  # first digits

        if int(fD) == 51 or int(fD) == 52 or int(fD) == 53 or int(fD) == 54 or int(fD) == 55:

            print("MASTERCARD\n")

        else:

            fD = cN / (10 ** 15)
            if int(fD) == 4:

                print("VISA\n")

            else:

                print("INVALID5\n")


if __name__ == "__main__":
    main()