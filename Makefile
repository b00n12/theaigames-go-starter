CC=gcc
CFLAGS="-Wall"
CSRC="src/main.c"

debug:clean
	$(CC) $(CFLAGS) -g -o build/gostarter $(CSRC)
stable:clean
	$(CC) $(CFLAGS) -o build/gostarter $(CSRC)
clean:
	rm -vfr *~ build/gostarter
