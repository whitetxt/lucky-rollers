echo --atlas -f rgba8888 -z auto > sprites.t3s
echo ./fade.png >> sprites.t3s;

for file in ./cards/*
do
	if [ $file != fade.png ]; then
		echo $file >> sprites.t3s;
	fi
done
