import random as rnd
import sys

if __name__ == "__main__":
    data = []
    for i in range(0, int(sys.argv[1])):
        data.append([])
        for j in range(0, int(sys.argv[2])):
            data[i].append([
                round(rnd.uniform(0, 5.00), 2), 
                rnd.randrange(0, 5), 
                #1,
                round(rnd.uniform(0, 500.00), 2)
            ])

    print(len(data), len(data[0]))

    for i in range(0, len(data)):
        for j in range(0, len(data[0])):
            print(' '.join([str(elem) for elem in data[i][j]]))