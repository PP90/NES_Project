CONTIKI=/home/user/contiki
include $(CONTIKI)/Makefile.include

ifndef TARGET
TARGET=sky
endif

all: general-sensing-data-structure energy-data-structureperiodic-sensing-telosb
 
%.tgz: %.ihex
	mkdir $(basename $<) ; \
	mv $< $(basename $<) ; \
	echo $(basename $<)/$(basename $<).ihex 600 > $(basename $<)/runfile ; \
	tar czf $@ $(basename $<)
