import csv
import sys

def task2a(data):
    genresDurations = {}
    for row in data:
        for genre in row[3].split(","):
            currDuration = genresDurations.get(genre.strip())
            if currDuration is None:
                currDuration = []
            currDuration.append(int(row[4]))
            genresDurations[genre.strip()] = currDuration
    originalSTD = sys.stdout
    with open("genres.stats", "w") as file:
        for genre in genresDurations:
            sys.stdout = file
            print(f"{genre} | {calcAvg(genresDurations[genre])}")
    sys.stdout = originalSTD


def calcAvg(list: list):
    return sum(list)/len(list)


def task2b(data, country):
    genres = {}
    cleanData = list(filter(lambda row:  country in map(lambda x: x.strip(), row[5].split(',')), data))
    for row in cleanData:
        for genre in row[3].split(","):
            currAmount = genres.get(genre.strip())
            if currAmount is None:
                currAmount = 0
            genres[genre.strip()] = currAmount + 1
    print("list of genres and the amount of movies:")
    for genre in genres:
        print(f"in {country} there are {genres[genre.strip()]} movies from the genre {genre}")



if __name__ == '__main__':
    with open('IMDB.csv', 'r') as file:
        reader = csv.reader(file)
        next(reader, None)  # skip the titles
        data = list(reader)
        task2a(data)
        task2b(data, "USA")