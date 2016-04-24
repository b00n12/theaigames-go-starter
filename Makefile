CC=gcc
CFLAGS=-Wall
CSRC=src/main.c src/bot/bot.c src/bot/parser.c src/field/field.c src/util.c
CSRC+=src/bot/bot_state.c src/field/scoring.c

TEST_SRC=src/field/field.c src/field/scoring.c test/test_scoring.c src/util.c
TEST_SRC+=src/bot/bot_state.c

debug:	clean
	$(CC) $(CFLAGS) -g $(CSRC) -o build/gostarter
stable:	clean
	$(CC) $(CFLAGS) $(CSRC) -o  build/gostarter
test:	clean
	$(CC) $(CFLAGS) $(TEST_SRC) -o  test/test_scoring
clean:
	rm -vfr *~ build/gostarter test/test_scoring
