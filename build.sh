name=$1
echo $name
g++ $name.cpp -g -I ./include -I ./tools -L ./bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o $name.out -std=c++11
LD_LIBRARY_PATH=./bin ./$name
