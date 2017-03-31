CC := gcc
CFLAGS := -g -W -Wall
PROGNAME := tcpsend tcprecv tcpselectrecv tcpooblinerecv tcpinterruptrecv

all : $(PROGNAME)

tcpsend: tcpsend.c
	$(CC) $(CFLAGS) -o $@ $<

tcprecv: tcprecv.c
	$(CC) $(CFLAGS) -o $@ $<

tcpselectrecv: tcpselectrecv.c
	$(CC) $(CFLAGS) -o $@ $<

tcpooblinerecv: tcpooblinerecv.c
	$(CC) $(CFLAGS) -o $@ $<

tcpinterruptrecv: tcpinterruptrecv.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -rf $(PROGNAME)

