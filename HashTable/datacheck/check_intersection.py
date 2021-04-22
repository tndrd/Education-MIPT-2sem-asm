data_400k = list(open("400k-words.txt"))
raw_ded  = list(open("ded-dict.txt"))

data_ded = []

for ded in raw_ded:
    data_ded.append(ded.split("=")[0].strip())

for i in range (len(data_400k)):
    data_400k[i] = data_400k[i].strip()

data_400k = set(data_400k)
raw_ded = set(data_ded)

intersect = data_400k & raw_ded
print(len(intersect))


  