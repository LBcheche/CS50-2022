# TODO
from cs50 import SQL
from sys import argv, exit


def main():

    verifyUsage()
    queryDB()

    exit(0)


def verifyUsage():  # Verify if arguments have correct syntax

    if len(argv) < 2 or (argv[1] != "Gryffindor" and argv[1] != "Hufflepuff" and argv[1] != "Ravenclaw" and argv[1] != "Slytherin"):

        print("Usage: python roster.py [House Name]")
        exit(1)


def queryDB():  # Make Data Base query using house argument

    db = SQL("sqlite:///students.db")

    house = argv[1]

    studentsHouse = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

    if len(studentsHouse) > 1:
        for student in studentsHouse:

            middle = " "

            if student["middle"] != None:  # verifies if there is a middle name
                
                middle = " " + student["middle"] + " "  # joins middle name with spaces

            fullName = student["first"] + middle + student["last"] + ","  # makes full name string

            print(fullName, "born", student["birth"], )  # prints results


if __name__ == "__main__":
    main()
