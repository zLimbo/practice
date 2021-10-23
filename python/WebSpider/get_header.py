
with open('out.txt', 'w') as fout:
    with open('header.txt', 'r') as fin:
        for line in fin:
            pos = line.index(':')
            left = line[: pos].strip()
            right = line[pos+1:].strip()
            fout.write("'" + left + "': '" + right + "',\n") 
        