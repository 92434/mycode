include $(default_rules)
include $(shell pwd)/inc.mk

# Cancel implicit rules on top Makefile
$(CURDIR)/Makefile Makefile:;

env: 1 *.mk
	echo $$\(CURDIR\) is $(CURDIR)
	echo $$\(MAKEFLAGS\) is $(MAKEFLAGS)
	echo $$\? is $?
	echo $$\(wildcard $$^\) is $(wildcard $^)

1 2 3:
	echo $@

overriding_commands: 1
	echo $@ dep $<
overriding_commands: 2
	echo $@ dep $<
overriding_commands: 3
	echo $@ dep $<
overriding_commands:
	echo $@ dep $<

#define get-device-major
#	$(shell awk '$$2 ~ /$(1)/ && $$1 !~ /xiaofei/ {print $$1}' /proc/devices)
#endef
get-device-major = $(shell awk '$$2 ~ /$(1)/ && $$1 !~ /xiaofei/ {print $$1}' /proc/devices)
%_major:
	echo ${@:_major=} is $(call get-device-major,${@:_major=})

.PHONY: gcc-search-path
gcc-search-path :
	gcc -print-search-dirs

