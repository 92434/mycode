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
