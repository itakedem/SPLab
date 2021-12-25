import csv
import collections
import matplotlib.pyplot as plt
import sys


def task1a(data):
    countries = []
    for row in data:
        for country in row[5].split(','):
            countries.append(country.strip())
    countryCounters = collections.Counter(countries)
    print(f"Country : Number of Movies:")
    originalSTD = sys.stdout
    with open("movies.stats", "w") as file:
        for country in countryCounters:
            sys.stdout = file
            print(f"{country} | {countryCounters[country]}")
    sys.stdout = originalSTD




def task1b(data: list, date, country):
    cleanData = list(filter(lambda row:  country in map(lambda x: x.strip(), row[5].split(','))  and int(row[2]) >= date, data))
    print(f"The number of movies in {country} since the year {date} are {len(cleanData)}")


def task1c(data):
    min = 1894
    max = 2020
    numBins = max - min
    listOfYears = list(map(lambda x: int(x[2]), data))
    plt.hist(listOfYears, numBins)
    plt.title("Movies' Release Year Histogram")
    plt.xlabel("Year")
    plt.ylabel("Number of Movies")
    plt.show()



if __name__ == '__main__':
    with open('IMDB.csv', 'r') as file:
        reader = csv.reader(file)
        next(reader, None)  # skip the titles
        data = list(reader)
        task1a(data)
        task1b(data, 2009,'USA')
        task1c(data)