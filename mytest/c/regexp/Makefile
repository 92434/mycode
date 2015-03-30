include $(default_rules)

#CC := gcc

CURL_VER := 7.41.0
CURL_DIR := curl-$(CURL_VER)
CURL_TAR := curl-$(CURL_VER).tar.gz
CURL_LIB := $(CURL_DIR)/__install/lib/libcurl.a
$(eval target_files += $(CURL_LIB))

$(CURL_LIB) : $(CURL_DIR)
	cd $<; \
	./configure --prefix $(shell pwd)/$(CURL_DIR)/__install; \
	make; \
	make install
$(CURL_DIR) : $(CURL_TAR)
	tar xf $<
$(CURL_TAR) :
	wget http://curl.haxx.se/download/$(CURL_TAR)
clean : curl_clean
curl_clean :
	rm -rf $(CURL_DIR)

ICONV_VER := 1.14
ICONV_DIR := libiconv-$(ICONV_VER)
ICONV_TAR := libiconv-$(ICONV_VER).tar.gz
ICONV_LIB := $(ICONV_DIR)/__install/lib/libiconv.so
$(eval target_files += $(ICONV_LIB))

$(ICONV_LIB) : $(ICONV_DIR)
	cd $<; \
	./configure --prefix=$(shell pwd)/$(ICONV_DIR)/__install --enable-static=yes; \
	make install
$(ICONV_DIR) : $(ICONV_TAR)
	tar xf $<
$(ICONV_TAR) :
	wget http://ftp.gnu.org/gnu/libiconv/$(ICONV_TAR)
clean : iconv_clean
iconv_clean :
	rm -rf $(ICONV_DIR)

c_files := regexp_clib.cpp
c_files := fetch_page.cpp
$(c_files) : $(CURL_LIB) $(ICONV_LIB) 
fetch_page.cpp.LOCAL_CFLAGS := -I$(shell dirname $(shell dirname $(CURL_LIB)))/include
fetch_page.cpp.LOCAL_CFLAGS += -I$(shell dirname $(shell dirname $(ICONV_LIB)))/include
fetch_page.cpp.LOCAL_LDFLAGS := -L$(shell dirname $(CURL_LIB))
fetch_page.cpp.LOCAL_LDFLAGS += -lcurl
fetch_page.cpp.LOCAL_LDFLAGS += -L$(shell dirname $(ICONV_LIB))
fetch_page.cpp.LOCAL_LDFLAGS += -liconv
#fetch_page.cpp.LOCAL_LDFLAGS += -static
include $(BUILD_BINS)

include $(ADD_TARGET)

test :
	export LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$(dir $(CURL_LIB)):$(dir $(ICONV_LIB)); \
	./out/bin/fetch_page linux 0
