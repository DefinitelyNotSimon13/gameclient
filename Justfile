all: clean
    bear -- make
    make
    make run

clean:
    make clean

