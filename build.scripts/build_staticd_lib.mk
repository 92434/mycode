include $(C_COMMON)

define gen-lib-name-from-user-defined
	$(patsubst %,lib%.a,$(1))
endef

define gen-lib-path-from-lib-name
	$(patsubst %,$(out_dir)/lib/%,$(1))
endef

define gen-lib-o-file-name-from-module-name-and-c-file-name
	$(patsubst %,$(out_dir)/obj/$(1)/%.o,$(basename $(2)))
endef

define transform-c-file-to-o-file
	$(quiet)mkdir -p $(dir $@)
	$(quiet)$(TOOLCHAIN_PREFIX)$(CC) -c $($@.local_cflags) $(CFLAGS) -o $@ $< 
endef

define transform-o-files-to-lib-file
	$(quiet)mkdir -p $(dir $@)
	$(quiet)$(TOOLCHAIN_PREFIX)$(AR) -cru $@ $($@.o_files)
endef

staticd_lib_name := $(strip $(call gen-lib-name-from-user-defined, $(staticd_lib_name)))
staticd_lib := $(call gen-lib-path-from-lib-name,$(staticd_lib_name))


$(out_dir)/obj/$(staticd_lib_name)/%.o : %.c
	$(call transform-c-file-to-o-file)
	$(call echo-why)
$(out_dir)/obj/$(staticd_lib_name)/%.o : %.cc
	$(call transform-c-file-to-o-file)
	$(call echo-why)
$(out_dir)/obj/$(staticd_lib_name)/%.o : %.cpp
	$(call transform-c-file-to-o-file)
	$(call echo-why)

staticd_lib_o_files := $(call gen-lib-o-file-name-from-module-name-and-c-file-name,$(staticd_lib_name),$(staticd_lib_c_files))

$(foreach o_file,$(staticd_lib_o_files),$(eval $(o_file).local_cflags := $(LOCAL_CFLAGS)))

$(eval $(staticd_lib).o_files := $(staticd_lib_o_files))
$(staticd_lib) : $(staticd_lib_o_files) $(LOCAL_DEPS)
	$(call transform-o-files-to-lib-file)
	$(call echo-why)

ifneq ($(LOCAL_PRECONDITION),)
$(eval target_files += $(LOCAL_PRECONDITION))
endif

$(eval target_files += $(staticd_lib))

##########################################################################################################################3

define gen-d-file-target-name-for-obj-file-from-d-file-name
	$(basename $(1))
endef

define gen-d-file-name-for-o-file-from-module-name-and-c-file-name
	$(patsubst %,$(out_dir)/obj/$(1)/%.o.d,$(basename $(2)))
endef

$(out_dir)/obj/$(staticd_lib_name)/%.o.d : %.c
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-obj-file-from-d-file-name,$@)))
	$(call echo-why)
$(out_dir)/obj/$(staticd_lib_name)/%.o.d : %.cc
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-obj-file-from-d-file-name,$@)))
	$(call echo-why)
$(out_dir)/obj/$(staticd_lib_name)/%.o.d : %.cpp
	$(call transform-c-file-to-d-file,$(strip $(call gen-d-file-target-name-for-obj-file-from-d-file-name,$@)))
	$(call echo-why)

staticd_lib_o_files_d_files := $(call gen-d-file-name-for-o-file-from-module-name-and-c-file-name,$(staticd_lib_name),$(staticd_lib_c_files))
$(foreach d_file,$(staticd_lib_o_files_d_files),$(eval $(d_file).local_cflags := $(LOCAL_CFLAGS)))

$(eval d_files += $(staticd_lib_o_files_d_files))

LOCAL_CFLAGS :=
LOCAL_LDFLAGS :=
LOCAL_DEPS :=
LOCAL_PRECONDITION :=
