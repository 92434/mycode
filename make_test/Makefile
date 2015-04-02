include $(default_rules)

c_files := 
include $(BUILD_BINS)

include $(ADD_TARGET)

local_targets: xiaofei
	gcc -print-search-dirs
	echo $$\(CURDIR\) is $(CURDIR)
	echo $$\(MAKEFLAGS\) is $(MAKEFLAGS)

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
