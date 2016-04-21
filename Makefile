CC=gcc
CFLAGS=-Wall
CSRC=src/main.c src/bot/bot.c src/bot/parser.c src/field/field.c src/util.c src/bot/bot_state.c

debug:	clean
	$(CC) $(CFLAGS) -g $(CSRC) -o build/gostarter
stable:	clean
	$(CC) $(CFLAGS) $(CSRC) -o  build/gostarter
clean:
	rm -vfr *~ build/gostarter
