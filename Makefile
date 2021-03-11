rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,MinecraftC/,*.c *.h)
OBJS = $(patsubst %.c, %.o, $(SRC))

# since we use clang-specific extensions, this should not be changed
CC := clang
# flags -Og -g are currently here for debug, replace with -O2 or -O3 for release
CFLAGS = -I$(CURDIR)/Include -Og -g -std=c11

LDFLAGS = -L$(CURDIR)/Libraries -lm -lGL -lSDL2 -lGLU

.PHONY: all clean objs copy_resources

all: ./bin ./bin/MinecraftC 

%.o: %.c
	cd bin/obj; $(CC) $(CFLAGS) -c ../../$^

./bin/MinecraftC: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ ./bin/obj/*.o
	cp -vr Resources/* bin/

./bin:
	mkdir -p bin/obj

clean:
	rm -rf bin
