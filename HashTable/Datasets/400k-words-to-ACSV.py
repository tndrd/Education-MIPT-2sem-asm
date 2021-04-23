def utf8len(s):
    return len(s.encode("utf-8"))

def align_str(string):
    length   = utf8len(string)
    n_blocks = length // 16 + 1

    dop = 16 - (length + 1) % 16

    aligned =  str(chr(n_blocks)) + string
    aligned += str(chr(0)) * (dop * ((length %16) != 15))

    return aligned

def align(pair):
    return align_str(pair[0]) + align_str(pair[1])

with open("Raw/400k-words.txt", "r") as txt:
    pairs = []
    lines = list(txt)

    for line in lines:
        pairs.append(line.split("="))
    
    keys   = open("Acsv/400k-words.keys", "w")
    values = open("Acsv/400k-words.values", "w")

    index = 0

    keys.write(str(chr(0)) * 15)

    for pair in pairs:
        keys.write(align_str(pair[0].rstrip('\n')))
        index+=1

    keys.write(chr(0))
    keys.write(chr(0))

    keys.close()
    values.close()