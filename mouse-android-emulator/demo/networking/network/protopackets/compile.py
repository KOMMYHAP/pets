import sys

f = open("test.txt", "w");
f.write('Argument list: ' + str(sys.argv))
f.close();
