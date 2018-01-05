import sys

if len(sys.argv) < 2:
	print("Usage: python obj2scene.py OBJFILE")
	sys.exit(1)

filename = sys.argv[1]
file = open(filename, "r")

vertices = []
faces = []

for line in file:
	line = line.strip().split()
	if not line:
		continue
	if line[0] == 'v':
		vertex = [float(c) for c in line[1:4]]
		vertices.append(vertex)
	elif line[0] == 'f':
		face = [int(i) for i in line[1:4]]
		faces.append(face)

minc = [float('inf')] * 3
maxc = [-float('inf')] * 3

for vertex in vertices:
	for i in range(3):
		if vertex[i] < minc[i]:
			minc[i] = vertex[i]
		if vertex[i] > maxc[i]:
			maxc[i] = vertex[i]

avgc = [(minc[i] + maxc[i]) / 2 for i in range(3)]
width = abs(minc[0]) + abs(maxc[0])
height = abs(minc[1]) + abs(maxc[1])

print("[ Generated from %s by obj2dat.py ]\n" % filename)

print("image %s 360\n" % int(360 * width / height))

print("camera")
print("\tscreen %s %s" % (repr(width), repr(height)))
print("\teye %s %s %s" % (repr(avgc[0]), repr(avgc[1]), repr(avgc[2] - width - height)))
print("\tlook %s %s %s" % (repr(avgc[0]), repr(avgc[1]), repr(avgc[2])))
print("\tup 0 1 0")
print("\tfov 45")
print("end\n")

print("ambient .5 .5 .5\n")

print("light")
print("\tpos %s %s %s" % (repr(avgc[0]), repr(avgc[1] + height), repr(avgc[2] - width - height)))
print("\tcolor 1 1 1")
print("\tdiffuse 1")
print("\tspecular 1")
print("end\n")

material = ".".join(filename.replace(".", " ").split()[:-1])

print("material %s" % material)
print("\tambient .01 .01 .01")
print("\tdiffuse .75 .75 .75")
print("\tspecular .33 .33 .33")
print("\tshininess 10")
print("end\n")

print("plane")
print("\tnormal 0 1 0")
print("\tdistance %s" % repr(-minc[1]))
print("\tambient .5 .5 .5")
print("\tdiffuse .5 .5 .5")
print("end\n")

print("[")
print("x ranges from %s to %s" % (repr(minc[0]), repr(maxc[0])))
print("y ranges from %s to %s" % (repr(minc[1]), repr(maxc[1])))
print("z ranges from %s to %s" % (repr(minc[2]), repr(maxc[2])))
print("]")

for face in faces:
	v1 = vertices[face[0]]
	v2 = vertices[face[1]]
	v3 = vertices[face[2]]
	print("triangle use %s" % material)
	print("\tv1 %s %s %s" % (repr(v1[0]), repr(v1[1]), repr(v1[2])))
	print("\tv2 %s %s %s" % (repr(v2[0]), repr(v2[1]), repr(v2[2])))
	print("\tv3 %s %s %s" % (repr(v3[0]), repr(v3[1]), repr(v3[2])))
	print("end")

file.close()
