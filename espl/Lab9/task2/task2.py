import csv
import collections
import matplotlib.pyplot as plt




if __name__ == '__main__':
    with open('IMDB.csv', 'r') as file:
        reader = csv.reader(file)
        next(reader, None)  # skip the titles
        data = list(reader)