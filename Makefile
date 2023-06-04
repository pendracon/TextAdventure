TARGET = bin/crawler
CC = g++
ODIR = build
SDIR = src
INC = -Isrc

_OBJS = zobject.o config.o character.o item.o room.o player.o dungeon.o main.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

.PHONY: all buildir clean install

all : buildir $(TARGET)

buildir:
	if test -n $(ODIR); then mkdir -p $(ODIR); fi

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(LIBS) $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

install : $(TARGET)
	install $(TARGET) ~/bin

clean :
	rm -f $(TARGET) 2> /dev/null
	rm -rf $(ODIR) 2> /dev/null
