from sys import argv, exit
import csv

DNA_Sequence = str()
DNA_DB = dict()
STRs_List = list()
First_DB_row = list()
STRs_Sequences_Counting = str()

def main():

    verifyUsage()
    loadDNASequence()
    loadDataBase()
    countSTRs()
    compareDBwithSequence()

    exit(0)

def verifyUsage():

    if len(argv) < 3 or argv[1][-3::] != "csv" or argv[2][-3::] != "txt":
        
        print("Usage: dna.py path/tableFile.csv path/dnaFile.txt")
        
        exit(1)


def loadDataBase():

    populateSTRsList()
    populateFirstDBrow()
    populateDNADB()
    

def populateSTRsList():
    
    global STRs_List
    
    db_file = open(argv[1], 'r')
    db_reader_ls = csv.reader(db_file, delimiter=",")

    for row in db_reader_ls:
        
        for column in row:
            
            if "name" in column:
                
                STRs_List = row
                STRs_List.pop(0)
              
    db_file.close()
    
    
def populateFirstDBrow():
    
    global First_DB_row
    
    db_file = open(argv[1], 'r')
    db_reader_ls = csv.reader(db_file, delimiter=",")

    for row in db_reader_ls:
        for column in row:
            if "name" in column:
                First_DB_row = row

    db_file.close()
    
                
def populateDNADB():
    
    global DNA_DB
    
    db_file = open(argv[1], 'r')
    db_reader_dict = csv.DictReader(db_file, delimiter=",")
  
    for row in db_reader_dict:
        
        i = 0
        tmp = ""
        
        for column in row:
           
            if i != 0:
               
              tmp = tmp + "," + row[First_DB_row[i]]
              
            i += 1
        
        DNA_DB[row[First_DB_row[0]]] = tmp
    
    db_file.close()    
        

def loadDNASequence():

    global DNA_Sequence

    dna_file_name = argv[2]
    dna_file = open(dna_file_name, 'r')

    for line in dna_file:
        DNA_Sequence = line.rstrip("\n")
    dna_file.close()


def countSTRs():

    global DNA_Sequence
    global STRs_List
    global STRs_Sequences_Counting

    for STR in STRs_List:

        biggest = 0

        for i in range(len(STR)):

            count = 0
            bigger = 0

            for j in range(i, len(DNA_Sequence), len(STR)):

                k = j + len(STR)

                if DNA_Sequence[j:k] == STR:

                    count += 1

                    if count > bigger:

                        bigger = count

                if DNA_Sequence[j:k] != STR:

                    count = 0

            if bigger > biggest:

                biggest = bigger

        STRs_Sequences_Counting = STRs_Sequences_Counting + "," + str(biggest)


def compareDBwithSequence():

    match = False
    
    for person, STRs_Counting in DNA_DB.items():
        if STRs_Counting == STRs_Sequences_Counting:
            print(person)
            match = True
    
    if match == False:
        print("No match")


if __name__ == "__main__":
    main()
