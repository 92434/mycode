include $(C_COMMON)

#define gen-app-name-from-user-defined
#	$(patsubst %,%,$(1))
#endef

define gen-app-path-from-app-name
	$(patsubst %,$(out_dir)/bin/%,$(1))
endef

define transform-c-files-to-app-file
	$(quiet)mkdir -p $(dir $@)
	$(quiet)$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -o $@ $($@.c_files) $($@.local_ldflags) $(LDFLAGS)
endef

#app_name := $(strip $(call gen-app-name-from-user-defined,$(app_name)))
app_bin_file := $(call gen-app-path-from-app-name,$(app_name))

$(eval $(app_bin_file).c_files := $(app_c_files))
$(eval $(app_bin_file).local_cflags := $(LOCAL_CFLAGS))
$(eval $(app_bin_file).local_ldflags := $(LOCAL_LDFLAGS))
$(app_bin_file) : $(LOCAL_DEPS)
	$(call transform-c-files-to-app-file)

ifneq ($(LOCAL_PRECONDITION),)
$(eval target_files += $(LOCAL_PRECONDITION))
endif

$(eval target_files += $(app_bin_file))

##########################################################################################################################3

define gen-d-file-target-name-for-app-file-from-d-file-name
	$(patsubst $(out_dir)/obj/%,$(out_dir)/bin/%,$(basename $(1)))
endef

$(out_dir)/obj/$(app_name)/%.tmp.d : %.c
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-app-file-from-d-file-name,$@)))
$(out_dir)/obj/$(app_name)/%.tmp.d : %.cc
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-app-file-from-d-file-name,$@)))
$(out_dir)/obj/$(app_name)/%.tmp.d : %.cpp
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-app-file-from-d-file-name,$@)))


module_tmp_d_files := $(call gen-d-file-names-for-module-tmp-from-module-name-and-c-file-names,$(app_name),$(app_c_files))
app_d_file := $(call gen-d-file-name-for-target-file-from-target-file-name,$(app_name))

$(foreach d_file,$(module_tmp_d_files),$(eval $(d_file).local_cflags := $(LOCAL_CFLAGS)))

$(eval $(app_d_file).target_file := $(strip $(app_bin_file)))
$(app_d_file) : $(module_tmp_d_files)
	$(call transform-d-files-to-target-d-file)

$(eval d_files += $(app_d_file))

LOCAL_CFLAGS :=
LOCAL_LDFLAGS :=
LOCAL_DEPS :=
LOCAL_PRECONDITION :=
