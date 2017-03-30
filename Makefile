CC := gcc
CFLAGS := -g -W -Wall
PROGNAME := tcpsend tcprecv tcpselectrecv

all : $(PROGNAME)

tcpsend: tcpsend.c
	$(CC) $(CFLAGS) -o $@ $<

tcprecv: tcprecv.c
	$(CC) $(CFLAGS) -o $@ $<

tcpselectrecv: tcpselectrecv.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -rf $(PROGNAME)

