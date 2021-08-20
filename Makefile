TARGET=getaddrinfo
SRC=getaddrinfo.c

%: %.c
	clang -o $@ $^

run: $(TARGET)
	echo \# heise.de:80; ./$(TARGET) heise.de 80; \
	echo \# unavailable.network:80; ./$(TARGET) unavailable.network 80; \
	echo \# zoidberg.inwx.net:53; ./$(TARGET) zoidberg.inwx.net 53;

all: $(TARGET)
