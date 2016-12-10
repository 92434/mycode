include $(C_COMMON)

define gen-bin-path-from-c-file-name
	$(patsubst %,$(out_dir)/bin/%,$(basename $(1)))
endef

define transform-c-file-to-bin-file
	$(quiet)mkdir -p $(dir $@)
	$(quiet)$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -o $@ $($@.c_file) $($@.local_libs) $(LIBS) $($@.local_ldflags) $(LDFLAGS)
endef

$(out_dir)/bin/% : %.c
	$(call transform-c-file-to-bin-file)
	$(call echo-why)
$(out_dir)/bin/% : %.cc
	$(call transform-c-file-to-bin-file)
	$(call echo-why)
$(out_dir)/bin/% : %.cpp
	$(call transform-c-file-to-bin-file)
	$(call echo-why)

$(foreach c_file,$(c_files),$(eval $(call gen-bin-path-from-c-file-name,$(c_file)).c_file := $(c_file)))
bin_files := $(call gen-bin-path-from-c-file-name,$(c_files))

$(foreach c_file,$(c_files),$(eval $(call gen-bin-path-from-c-file-name,$(c_file)).local_cflags := $($(c_file).LOCAL_CFLAGS) $(LOCAL_CFLAGS)))
$(foreach c_file,$(c_files),$(eval $(call gen-bin-path-from-c-file-name,$(c_file)).local_libs := $($(c_file).LOCAL_LIBS) $(LOCAL_LIBS)))
$(foreach c_file,$(c_files),$(eval $(call gen-bin-path-from-c-file-name,$(c_file)).local_ldflags := $($(c_file).LOCAL_LDFLAGS) $(LOCAL_LDFLAGS)))
$(foreach c_file,$(c_files),$(eval $(strip $(call gen-bin-path-from-c-file-name,$(c_file))) : $($(c_file).LOCAL_DEPS) $(LOCAL_DEPS)))

ifneq ($(LOCAL_PRECONDITION),)
$(eval target_files += $(LOCAL_PRECONDITION))
endif

$(eval target_files += $(bin_files))

##########################################################################################################################3
define gen-d-file-name-for-bin-file-from-c-file-name
	$(patsubst %,$(out_dir)/obj/%.bin.d,$(basename $(1)))
endef

define gen-d-file-target-name-for-bin-file-from-d-file-name
	$(patsubst $(out_dir)/obj/%,$(out_dir)/bin/%,$(basename $(basename $(1))))
endef

$(out_dir)/obj/%.bin.d : %.c
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-bin-file-from-d-file-name,$@)))
	$(call echo-why)
$(out_dir)/obj/%.bin.d : %.cc
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-bin-file-from-d-file-name,$@)))
	$(call echo-why)
$(out_dir)/obj/%.bin.d : %.cpp
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-bin-file-from-d-file-name,$@)))
	$(call echo-why)


bins_d_files := $(call gen-d-file-name-for-bin-file-from-c-file-name,$(c_files))
$(foreach c_file,$(c_files),$(eval $(call gen-d-file-name-for-bin-file-from-c-file-name,$(c_file)).local_cflags := $($(c_file).LOCAL_CFLAGS) $(LOCAL_CFLAGS)))
$(eval d_files += $(bins_d_files))

$(foreach c_file,$(c_files),$(eval $(c_file).LOCAL_CFLAGS :=))
$(foreach c_file,$(c_files),$(eval $(c_file).LOCAL_LIBS :=))
$(foreach c_file,$(c_files),$(eval $(c_file).LOCAL_LDFLAGS :=))
$(foreach c_file,$(c_files),$(eval $(c_file).LOCAL_DEPS :=))

LOCAL_CFLAGS :=
LOCAL_LIBS :=
LOCAL_LDFLAGS :=
LOCAL_DEPS :=
LOCAL_PRECONDITION :=
