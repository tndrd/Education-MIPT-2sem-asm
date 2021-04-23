data_400k = list(open("Raw/400k-words.txt"))
raw_ded  = list(open("Raw/ded-dict.txt"))

print("ded-dict   contains " + str(len(raw_ded)) +   " key-value pairs")
print("400k-words contains " + str(len(data_400k)) + " words")

data_ded = []

for ded in raw_ded:
    data_ded.append(ded.split("=")[0].strip())

for i in range (len(data_400k)):
    data_400k[i] = data_400k[i].strip()

data_400k = set(data_400k)
raw_ded = set(data_ded)

intersect = data_400k & raw_ded


print("Intersection between them is " + str(len(intersect)) + " words")


  