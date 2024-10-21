LDLIBS = -luserchannel-arm -lchannel_arm
LDFLAGS = -L. -L../../vep_0/libchannel/lib -L../../vep_0/libuserchannel-arm/lib

CFLAGS = -Wall -Wextra -pedantic -O3 -std=c99 -I. -I../../vep_0/libuserchannel-arm/include

TOOLS:=vcd

all: ${TOOLS}

clean:
	rm -f ${TOOLS} bus.vcd

.PHONY: all clean
