import os

for root, dirs, files in os.walk("./cards"):
	for file in files:
		if "_white" in file:
			os.rename(os.path.join(root, file), os.path.join(root, file.replace("_white", "")))
		if "ace" in file:
			os.rename(os.path.join(root, file), os.path.join(root, file.replace("ace", "1")))
		if "jack" in file:
			os.rename(os.path.join(root, file), os.path.join(root, file.replace("jack", "11")))
		if "queen" in file:
			os.rename(os.path.join(root, file), os.path.join(root, file.replace("queen", "12")))
		if "king" in file:
			os.rename(os.path.join(root, file), os.path.join(root, file.replace("king", "13")))