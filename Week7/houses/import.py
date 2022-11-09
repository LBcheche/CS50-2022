from cs50 import SQL
from sys import argv, exit
import csv


def main():

    verifyUsage()
    loadCSVToDB()

    exit(0)


def verifyUsage():  # Verify if arguments have correct syntax

    if len(argv) < 2 or argv[1][-3::] != "csv":

        print("Usage: python import.py characters.csv")
        exit(1)


def loadCSVToDB():  # Load data base from .csv file

    db = SQL("sqlite:///students.db")

    with open(argv[1], "r") as students:
        
        reader = csv.DictReader(students)
        
        for row in reader:
            
            fullName = getFullName(row["name"])
            
            birth = int(row["birth"])
            
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES( ?, ?, ?, ?, ?)",
                       fullName[0], fullName[1], fullName[2], row["house"], birth)

            
def getFullName(name):  # separates each name part into a list and if there is no middle name, add None
    
    fullName = name.split()
    
    if len(fullName) == 2:
        
        fullName.insert(1, None)
    
    return fullName


if __name__ == "__main__":
    main()
