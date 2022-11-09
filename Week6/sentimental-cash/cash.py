from cs50 import get_float

cents = 0
counter = 0


def countCoins():

    global cents
    
    while cents > 0:

        verifyCoin(25)  # verify how many $0,25 coins need for change.
        verifyCoin(10)  # verify how many $0,10 coins need for change.
        verifyCoin(5)  # verify how many $0,05 coins need for change.
        verifyCoin(1)  # verify how many $0,01 coins need for change.
        

def verifyCoin(coin):  # function that add amount of coins to counter.

    global cents
    global counter

    if cents >= coin:

        cents -= coin
        counter = counter + 1

        while cents >= coin:

            cents -= coin
            counter = counter + 1
            

def main():

    global counter
    global cents

    dollars = get_float("Change owed:")  # ask user for change value.

    while dollars < 0:

        dollars = get_float("Change owed:")  # ask user for change value.

    cents = round(dollars * 100)  # round float to int for greater precision.

    countCoins()  # function that count minimun coins for change.

    print(counter)  # print number of coins.


if __name__ == "__main__":
    main()