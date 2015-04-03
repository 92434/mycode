include $(default_rules)

CC := g++

extract_src_files := C/7zTypes.h
extract_src_files += C/Precomp.h
extract_src_files += C/Compiler.h
extract_src_files += C/LzHash.h
extract_src_files += C/Alloc.c
extract_src_files += C/Alloc.h
extract_src_files += C/LzFind.h
extract_src_files += C/LzFind.c
extract_src_files += C/LzmaEnc.c
extract_src_files += C/LzmaEnc.h
extract_src_files += C/LzmaDec.c
extract_src_files += C/LzmaDec.h
extract_src_files += C/LzmaLib.c
extract_src_files += C/LzmaLib.h

staticd_lib_c_files := C/Alloc.c
staticd_lib_c_files += C/LzFind.c
staticd_lib_c_files += C/LzmaEnc.c
staticd_lib_c_files += C/LzmaDec.c
staticd_lib_c_files += C/LzmaLib.c
$(staticd_lib_c_files) : C
staticd_lib_name := lzma
LOCAL_CFLAGS := -D_7ZIP_ST
#LOCAL_CFLAGS += -g
include $(BUILD_LIB)

shared_lib_c_files := C/Alloc.c
shared_lib_c_files += C/LzFind.c
shared_lib_c_files += C/LzmaEnc.c
shared_lib_c_files += C/LzmaDec.c
shared_lib_c_files += C/LzmaLib.c
$(shared_lib_c_files) : C
shared_lib_name := lzma
LOCAL_CFLAGS := -D_7ZIP_ST
#LOCAL_CFLAGS += -g
include $(BUILD_SO)

c_files := test_lzma.c
test_lzma.c.LOCAL_LDFLAGS := -L$(out_dir)/lib
test_lzma.c.LOCAL_LDFLAGS += -static
test_lzma.c.LOCAL_LDFLAGS += -llzma
include $(BUILD_BINS)

app_c_files := test_lzma.c
LOCAL_LDFLAGS := -L$(out_dir)/lib
LOCAL_LDFLAGS += -static
LOCAL_LDFLAGS += -llzma
app_name := test_lzma_app
include $(BUILD_APP)

app_c_files := test_lzma.c
app_c_files += C/LzmaEnc.c
app_c_files += C/LzmaDec.c
app_c_files += C/LzmaLib.c
app_c_files += C/Alloc.c
app_c_files += C/LzFind.c
$(app_c_files) : C
app_name := test_lzma_app.raw
LOCAL_CFLAGS := -D_7ZIP_ST
#LOCAL_CFLAGS += -g
include $(BUILD_APP)

local_lib_src := $(top)/utils

app_c_files := mylzma.cpp
$(app_c_files) : C
LOCAL_CFLAGS := -I$(local_lib_src)
#LOCAL_CFLAGS += -g
LOCAL_LDFLAGS := -L$(local_lib_src)/$(out_dir)/lib
LOCAL_LDFLAGS += -lxiaofei
LOCAL_LDFLAGS += -L$(out_dir)/lib
LOCAL_LDFLAGS += -llzma
LOCAL_LDFLAGS += -static
app_name := mylzmaapp
LOCAL_DEPS := $(local_lib_src)/$(out_dir)/lib/libxiaofei.so
LOCAL_DEPS += $(local_lib_src)/$(out_dir)/lib/libxiaofei.a
include $(BUILD_APP)

c_files := mylzma.cpp
$(c_files) : C
mylzma.cpp.LOCAL_CFLAGS := -I$(local_lib_src)
#mylzma.cpp.LOCAL_CFLAGS += -g
mylzma.cpp.LOCAL_LDFLAGS := -L$(local_lib_src)/$(out_dir)/lib
mylzma.cpp.LOCAL_LDFLAGS += -lxiaofei
mylzma.cpp.LOCAL_LDFLAGS += -L$(out_dir)/lib
mylzma.cpp.LOCAL_LDFLAGS += -llzma
mylzma.cpp.LOCAL_LDFLAGS += -static
mylzma.cpp.LOCAL_DEPS := $(local_lib_src)/$(out_dir)/lib/libxiaofei.a
mylzma.cpp.LOCAL_DEPS += $(local_lib_src)/$(out_dir)/lib/libxiaofei.so
include $(BUILD_BINS)

include $(ADD_TARGET)

C : lzma.7z
	7z x $< $(patsubst lzma-src/%,%,$(extract_src_files))

lzma.7z :
	wget -O $@ http://7-zip.org/a/7z938-src.7z

clean : src_clean
src_clean :
	rm -rf C
