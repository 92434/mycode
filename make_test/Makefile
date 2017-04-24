include $(default_rules)
include $(shell pwd)/inc.mk

# Cancel implicit rules on top Makefile
$(CURDIR)/Makefile Makefile:;

env: 1 *.mk gcc-search-path
	echo $$\(CURDIR\) is $(CURDIR)
	echo $$\(MAKEFLAGS\) is $(MAKEFLAGS)
	echo $$\? is $?
	echo $$^ is $^
	echo $$* is $*
	echo $$\(wildcard $$^\) is $(wildcard $^)
	touch env

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

module := xiaofei1
type := xiaofei-type1
name := xiaofei-name1
include spec.mk

module := xiaofei2
type := xiaofei-type2
name := xiaofei-name2
include spec.mk

define pinfo
	$(foreach target, $(targets), 
	$(info target $(target))
	$(info $(target)-type $($(target)-type))
	$(info $(target)-name $($(target)-name))
	)
endef

test1:
	$(call pinfo)

include $(ADD_TARGET)
