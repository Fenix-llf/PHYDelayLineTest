CC = /home/fenix/workspace/x2000/linux/tools/toolchains/mips-gcc720-glibc229/bin/mips-linux-gnu-gcc
INCS = -I ./
LIBPATH = -L ./
LIBS = 
CFLAGS = -Wall -c

phy_delay_test:main.o mem.o  ping.o
	$(CC) -o $@ $^ ${LIBPATH} ${LIBS}
	adb push phy_delay_test /usr/data
%.o:%.c
	$(CC) $(INCS) -c $< $(CFLAGS) 

clean:
	rm *.o

.PHONY:phy_delay_test clean
