with open("Raw/ded-dict.txt", "r") as txt:
    pairs = []
    lines = list(txt)

    for line in lines:
        pairs.append(tuple(line.split("=")))
    
    csv = open("Csv/ded-dict.csv", "w")
    
    for pair in pairs:
        csv.write(pair[0] + "=" + pair[1])

    csv.close()
