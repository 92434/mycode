include $(default_rules)
include /home/xiaofei/workspace/code/mycode/make_test/inc.mk

c_files := 
include $(BUILD_BINS)

include $(ADD_TARGET)

local_targets: xiaofei
	gcc -print-search-dirs
	echo $$\(CURDIR\) is $(CURDIR)
	echo $$\(MAKEFLAGS\) is $(MAKEFLAGS)

#define get-device-major
#	$(strip $(shell awk '/$(1)/ {print $$1}' /proc/devices))
#endef
#define get-device-major
#	$(shell awk '/$(1)/ {print $$1}' /proc/devices)
#endef

get-device-major = $(shell awk '/$(1)/ {print $$1}' /proc/devices)

# Cancel implicit rules on top Makefile
$(CURDIR)/Makefile Makefile:

1:
2:
3:
.PHONY: 1 2 3

xiaofei : 2
xiaofei : 1
	echo xiaofei1
	echo $^
xiaofei : 3

%.t2 : %.t1
	echo "$@----t1"
	touch $@

%.t2 :
	echo "$@----t2"
	touch $@

test : 1.t2 2.t2 3.t2
	echo $(call get-device-major,rtc)
	echo $(get-device-major)


