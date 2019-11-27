GCC=			/usr/bin/gcc
EXEC=			fakedns
S=			./src/


CSOURCE=        	$(S)main.c		\
			$(S)server.c		\
			$(S)command.c		\
			$(S)list.c		\
			$(S)hashtab.c		\
			$(S)utils.c


OBJS=			$(CSOURCE:.c=.o)

CFLAGS=         	-g -O0 -I $(S) -I/usr/include/libxml2 -Wall -W -Werror
LDFLAGS=		-lpthread

all:			$(OBJS) $(EXEC)

$(EXEC):
			$(GCC) -o $@ $(OBJS) $(LDFLAGS) 

.c.o:
			$(GCC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

.PHONY: clean  distclean install uninstall

install:
			@echo "create dir /etc/hstats/ & /usr/bin/hstats/..."
			@mkdir /etc/$(EXEC)
			@echo "copy files and binary in dir ..."
			@cp file.config /etc/$(EXEC)/
			@cp $(EXEC) /usr/sbin/
			@echo "create entry systemd ..."
			@cp ./$(SYSTEMD_SERVICE) /etc/systemd/system/
			@systemctl daemon-reload
			@echo "enable domstats on boot"
			@systemctl enable $(SYSTEMD_SERVICE)
			@systemctl start $(SYSTEMD_SERVICE)
			@echo "install success"

uninstall:
			@echo "remove dir/config and binary"
			@rm -rf /etc/$(EXEC)
			@rm -rf /usr/sbin/$(EXEC)
			@rm /etc/systemd/system/$(SYSTEMD_SERVICE)
			@echo "uninstall success"

clean:
			@rm $(S)*.o

distclean:
			@rm $(EXEC)
