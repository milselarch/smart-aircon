# RAWLEN [1, 1, 7, 5, 1, 3, 5, 1, 3, 1, 5, 5, 35, 5, 3, 1, 1, 113, 1, 1, 1, 3, 1]
a = '0000030200010200010002021102010000380000000100'
vals = []
blobs = []
globs = []
dlobs = []

# 0000030200010200010002021102010000380000000100
# ['00', '00', '03', '02', '00', '01', '02', '00', '01', '00', '02', '02', '11', '02', '01', '00', '00', '38', '00', '00', '00', '01', '00']
# ['00', '00', '03', '02', '00', '01', '02', '00', '01', '00', '02', '02', '11', '02', '01', '00', '00', '38', '00', '00', '00', '01', '00']

for k in range(0, len(a), 2):
    c1 = a[k]
    c2 = a[k+1]

    print(c1 + c2)
    val = int(c1+c2, 16)
    globs.append(c1+c2)
    dlobs.append(val)
    print(val)

    b = 1
    vals.append(1)
    for i in range(val):
        vals.append(0)
        vals.append(1)
        b += 2
    
    if k < len(a) - 2:
        vals.append(-1)
        b += 1
    
    blobs.append(b)

print(blobs, sum(blobs))
print(dlobs)
print(globs)
print(blobs)
print(len(vals))