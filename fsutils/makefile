all: cleanall fsutils clean

fsutils.o: fsutils.c lib/ext2.h lib/fat16.h
	gcc -Wall -Wextra -c fsutils.c -o fsutils.o

ext2.o: modules/ext2.c lib/ext2.h
	gcc -Wall -Wextra -c modules/ext2.c -o ext2.o

fat16.o: modules/fat16.c lib/fat16.h
	gcc -Wall -Wextra -c modules/fat16.c -o fat16.o

fsutils: fsutils.o ext2.o fat16.o
	gcc -o fsutils fsutils.o ext2.o fat16.o -Wall -Wextra

clean:
	rm -f *.o

cleanall:
	rm -f *.o fsutils
