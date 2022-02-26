import re
import string
import os

from collections import Counter
from operator import itemgetter

#Function to read and count each items frequency
def readFileAndCountItems():
    with open('CS210_Project_Three_Input_File.txt', 'r') as file:
        lines = [line.rstrip('\n') for line in file]

    return Counter(lines)

#Function to display either all the items and it's frequency
#or a single item and it's frequency
def displayItem(itemToDisplay=''):
    items = readFileAndCountItems()

    if not itemToDisplay:
        for item in sorted(items.items(), key=itemgetter(1), reverse=True):
            print("{0}: {1}".format(item[0], item[1]))
        
        return 0
    else:
        return items[itemToDisplay] if items[itemToDisplay] else 0


#Function to write the items from the file to another file with the number of times
#each item was purchased
def writeItemsToFile():
    items = readFileAndCountItems()

    #Write each item to the file and it's frequency
    with open("frequency.dat", "w") as file:
        for item in items.items():
            file.write('{} {}\n'.format(item[0], str(item[1])))

    return 0