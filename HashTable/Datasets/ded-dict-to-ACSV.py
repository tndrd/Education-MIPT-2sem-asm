ALIGNMENT = 16

def utf8len(s):
    return len(s.encode("utf-8"))

def align_str(string):
    length   = utf8len(string)
    n_blocks = length // ALIGNMENT + 1

    dop = ALIGNMENT - (length + 1) % ALIGNMENT

    aligned =  str(chr(n_blocks)) + string
    aligned += str(chr(0)) * (dop * ((length %ALIGNMENT) != ALIGNMENT - 1))

    return aligned

def align(pair):
    return align_str(pair[0]) + align_str(pair[1])

with open("Raw/ded-dict.txt", "r") as txt:
    pairs = []
    lines = list(txt)

    for line in lines:
        pairs.append(line.split("="))
    
    keys   = open("Acsv/ded-dict.keys", "w")
    values = open("Acsv/ded-dict.values", "w")

    index = 0

    keys.write(str(chr(0)) * (ALIGNMENT-1))
    for pair in pairs:
        keys.write(align_str(pair[0].rstrip('\n')))
        values.write(pair[1].rstrip('\n')+"\n")
        index+=1

    keys.write(chr(0))
    keys.write(chr(0))

    keys.close()
    values.close()