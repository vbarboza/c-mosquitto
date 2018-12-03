.PHONY: all clean

all: pub install

pub: pub_example.c
	gcc -o pub pub_example.c -lmosquitto

install: pub
	install -d bin
	mv pub bin

clean:
	rm -f pub *~
