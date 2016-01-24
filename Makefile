
All:
	mkdir -p Debug
	mkdir -p Debug/data
	echo "this is our test text!\n hi there!" >> "Debug/data/serverfile"
	gcc -c  "util.c" -g -O0 -Wall  -o ./Debug/util.c.o -I. -I.
	gcc -c  "connection.c" -g -O0 -Wall  -o ./Debug/connection.c.o -I. -I.
	gcc -c  "server.c" -g -O0 -Wall  -o ./Debug/server.c.o -I. -I.
	gcc -c  "command.c" -g -O0 -Wall  -o ./Debug/command.c.o -I. -I.
#	g++ -o ./Debug/simple_ftp_server @"simple_ftp_server.txt" -L.

