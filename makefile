CC = gcc
CFLAGS = -std=c99
#CFLAGS = -std=c11
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS +=-DUNITY_FIXTURE_NO_EXTRAS

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

LIBS = -lreadline

SRC_FILES := $(wildcard src/*.c)
SRC_TARGET = bin/app.out
UNITY_DIR  = test/test-framework
INC_DIRS   = -Isrc -I$(UNITY_DIR)
TEST_FILES:= $(filter-out src/main.c, \
	$(wildcard src/*.c test/*.c test/test_runners/*.c \
  $(UNITY_DIR)/unity.c $(UNITY_DIR)/unity_fixture.c))
TEST_TARGET = bin/test.out
MEM_CHECK_TARGET = bin/memcheck.out

.PHONY: all
all: memcheck test app

.PHONY: app
app: $(SRC_TARGET)
	@./$<

$(SRC_TARGET): $(SRC_FILES)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

.PHONY: test
test: $(TEST_TARGET)
	@./$< -v

$(TEST_TARGET): $(TEST_FILES)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) $(ASANFLAGS) $(INC_DIRS) $^ -o $@ $(LIBS)

.PHONY: memcheck
memcheck: $(MEM_CHECK_TARGET)
	@./$<
	@echo "😊 Memory check passed 😊"

$(MEM_CHECK_TARGET): $(SRC_FILES)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	@rm -f $(MEM_CHECK_TARGET) $(TEST_TARGET) $(SRC_TARGET)
