from cs50 import get_string


def main():

    text = get_string("Text: ")

    nLetters = countLetters(text)
    nWords = countWords(text)
    nSentences = countSenteces(text)
    avLetters = countAverage(nLetters, nWords)
    avSentences = countAverage(nSentences, nWords)

    printColemanLiauIndex(avLetters, avSentences)


def countLetters(t):  # count number of letters in text

    counter = 0

    for i in range(0, len(t), 1):

        if str.isalnum(t[i]):

            counter += 1

    return counter


def countWords(t):  # count number of words in text

    counter = 0
    isPreviousAlphaNum = False
    isCurrentAlphaNum = False

    for i in range(0, len(t), 1):

        if str.isalnum(t[i]) or t[i] == "'" or t[i] == "-":

            isCurrentAlphaNum = True

        else:

            isCurrentAlphaNum = False

        if isPreviousAlphaNum == False and isCurrentAlphaNum == True:

            counter += 1

        isPreviousAlphaNum = isCurrentAlphaNum

    return counter


def countSenteces(t):  # count number of senteces in text

    counter = 0
    isPreviousPunctuation = True
    isCurrentPunctuation = True

    for i in range(0, len(t), 1):

        if t[i] == "!" or t[i] == "." or t[i] == "?":

            isCurrentPunctuation = False

        else:

            isCurrentPunctuation = True

        if isPreviousPunctuation == True and isCurrentPunctuation == False:

            counter += 1

        isPreviousPunctuation = isCurrentPunctuation

    return counter


def countAverage(n1, n2):  # count average per 100

    return 100 * n1 / n2


def printColemanLiauIndex(L, S):  # print Coleman Liau Index

    f = 0.0588 * L - 0.296 * S - 15.8
    i = 0

    if f < 1:

        print("Before Grade 1")

    elif f > 16:

        print("Grade 16+")

    else:

        i = int(round(f))
        print("Grade " + str(i))


if __name__ == "__main__":
    main()