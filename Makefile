cc=gcc
flags=-Wall
clientLibraries=-pthread
flagsFormat=-sob -bad -bap -br -nce -cli4 -npcs -cs -nsaw -nsai -nsaf -nbc -di1 -cdb -sc -brs -brf -i4 -lp -ppi 4 -l100 --ignore-newlines -nbbo -nut -ncdw -nbc -npsl
files=server.c client.c utils.h
serverOut=server
clientOut=client

all: compile

compile: client server

client: client.c
	$(cc) $(flags) $(clientLibraries) client.c -o $(clientOut)

server: server.c
	$(cc) $(flags) server.c -o $(serverOut)

clean:
	rm $(clientOut) $(serverOut)

format:
	indent $(files) $(flagsFormat)
