include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf "HOLA DON PEPITO HOLA DON JOSE AA" >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	rm -rf *.bin
	cd SampleCodeModule; make clean
	rm -rf *.bin
	 

.PHONY: sampleCodeModule all clean
