import sys
import matplotlib.pyplot as plt


coordinates = sys.argv[1]
lines = sys.argv[2]

d = dict()

with open(coordinates) as f:
    for line in f:
        (key, x, y) = line.split()
        d.setdefault(key,[])
        d[key].append(float(x))
        d[key].append(float(y))

fig, ax = plt.subplots()
for item in d:
    # print("{}:{},{}".format(item,d[item][0],d[item][1]))
    ax.scatter(d[item][0],d[item][1])
    ax.annotate(str(item), xy=(d[item][0],d[item][1]))


with open(lines) as f:
    for line in f:
        start, end = line.split()
        ax.plot((d[start][0],d[end][0]),(d[start][1],d[end][1]),"b-")

try:
    highways = sys.argv[3]
    option = sys.argv[4]
    with open(highways) as f:
        for line in f:
            start, end = line.split()
            if(option == "1"): #highway
                ax.plot((d[start][0],d[end][0]),(d[start][1],d[end][1]),"r-")
            else: #new points
                ax.plot((d[start][0],d[end][0]),(d[start][1],d[end][1]),"r:")
except:
    pass
plt.show()



# if __name__ == "__main__":
# syste