CONTIKI=/home/user/contiki
include $(CONTIKI)/Makefile.include

ifndef TARGET
TARGET=sky
endif

all: pollution-data-structure test-ds1000 
 
%.tgz: %.ihex
	mkdir $(basename $<) ; \
	mv $< $(basename $<) ; \
	echo $(basename $<)/$(basename $<).ihex 600 > $(basename $<)/runfile ; \
	tar czf $@ $(basename $<)

