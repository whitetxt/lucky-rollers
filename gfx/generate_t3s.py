import os, re

def atoi(text):
	return int(text) if text.isdigit() else text
def natural_keys(text):
	return [ atoi(c) for c in re.split('(\d+)',text) ]

cards = []

for root, dirs, files in os.walk("./cards"):
	cards = [os.path.join(root, file) for file in files]

cards.sort(key = natural_keys)

open("sprites.t3s", "w")

with open("sprites.t3s", "a") as f:
	f.write("--atlas -f rgba8888 -z auto\n./fade.png\n")
	for file in cards:
		f.write(file + "\n")