all: prepare

install:
	sudo apt-get install gcc g++ cmake make

prepare:
	rm -rf build
	mkdir build
