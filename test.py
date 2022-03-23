import time
from sys import stdout

for i in range(1000):
	print (f"\b\b\b\b\b{str(i)}", end="")
	stdout.flush()
	time.sleep(0.01)
