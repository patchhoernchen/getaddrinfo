TARGET=getaddrinfo
SRC=getaddrinfo.c

%: %.c
	clang -o $@ $^

run: $(TARGET)
	./$(TARGET) heise.de 80;
	./$(TARGET) unavailable.network;
	./$(TARGET) zoidberg.inwx.net 53 -4;
	./$(TARGET) zoidberg.inwx.net 53 -6;
	./$(TARGET) zoidberg.inwx.net 53 -8;
	./$(TARGET) zoidberg.inwx.net 53 -t;
	./$(TARGET) zoidberg.inwx.net 53 -u;

all: $(TARGET)
