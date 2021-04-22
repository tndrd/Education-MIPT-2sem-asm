with open("400k-words.txt", "r") as txt:
    lines = list(txt)
    csv = open("400k-words.csv", "w")
    
    for line in lines:
        csv.write(line.strip() +"=0\n")

    csv.close()
