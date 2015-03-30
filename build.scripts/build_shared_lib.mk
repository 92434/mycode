include $(C_COMMON)

define gen-so-name-from-user-defined
	$(patsubst %,lib%.so,$(1))
endef

define gen-so-path-from-so-name
	$(patsubst %,$(out_dir)/lib/%,$(1))
endef

define transform-c-files-to-so-file
	$(quiet)mkdir -p $(dir $@)
	$(quiet)$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -shared -o $@ $($@.c_files) $($@.local_ldflags) $(LDFLAGS)
endef

shared_lib_name := $(strip $(call gen-so-name-from-user-defined,$(shared_lib_name)))
shared_lib := $(call gen-so-path-from-so-name,$(shared_lib_name))

$(eval $(shared_lib).c_files := $(shared_lib_c_files))
$(eval $(shared_lib).local_ldflags := $(LOCAL_LDFLAGS))
$(eval $(shared_lib).local_cflags := $(LOCAL_CFLAGS))
$(shared_lib) : $(shared_lib_c_files) $(LOCAL_DEPS)
	$(call transform-c-files-to-so-file)

$(eval target_files += $(shared_lib))

##########################################################################################################################3

define gen-d-file-target-name-for-so-file-from-d-file-name
	$(patsubst $(out_dir)/obj/%,$(out_dir)/lib/%,$(basename $(1)))
endef

$(out_dir)/obj/$(shared_lib_name)/%.tmp.d : %.c
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-so-file-from-d-file-name,$@)))
$(out_dir)/obj/$(shared_lib_name)/%.tmp.d : %.cc
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-so-file-from-d-file-name,$@)))
$(out_dir)/obj/$(shared_lib_name)/%.tmp.d : %.cpp
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-so-file-from-d-file-name,$@)))

module_tmp_d_files := $(call gen-d-file-names-for-module-tmp-from-module-name-and-c-file-names,$(shared_lib_name),$(shared_lib_c_files))
shared_lib_d_file := $(call gen-d-file-name-for-target-file-from-target-file-name,$(shared_lib_name))

$(foreach d_file,$(module_tmp_d_files),$(eval $(d_file).local_cflags := $(LOCAL_CFLAGS)))

$(eval $(shared_lib_d_file).target_file := $(strip $(shared_lib)))
$(shared_lib_d_file) : $(module_tmp_d_files)
	$(call transform-d-files-to-target-d-file)

$(eval d_files += $(shared_lib_d_file))

LOCAL_LDFLAGS :=
LOCAL_CFLAGS :=
LOCAL_DEPS :=
