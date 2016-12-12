define all-c-files-under
	$(patsubst ./%,%,\
		$(shell find . \( -name "*.c" -o -name "*.cpp" -o -name "*.cc" \) -and -not -name ".*") \
	)
endef

#all_c_files := $(strip $(call all-c-files-under))

define gen-d-file-names-for-module-tmp-from-module-name-and-c-file-names
	$(patsubst %,$(out_dir)/obj/$(1)/%.tmp.d,$(basename $(2)))
endef

define gen-d-file-name-for-target-file-from-target-file-name
	$(patsubst %,$(out_dir)/obj/%.target.d,$(1))
endef


#define transform-c-file-to-d-file
#	$(silent)mkdir -p $(dir $@)
#	$(silent)#$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -MM $< | sed 's:^.*\::$(1) \::' >$@
#	$(silent)$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -M $< | sed 's:^.*\::$(1) \::' >$@
#	$(silent)#echo -e "\t$$""(""call $(2)"")" >>$@
#endef
define transform-c-file-to-d-file
	$(silent)mkdir -p $(dir $@)
	$(silent)#$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -MM $< | sed 's:^.*\::$(1) \::' >$@
	$(silent)$(TOOLCHAIN_PREFIX)$(CC) $($@.local_cflags) $(CFLAGS) -M $< | sed 's:^.*\::$(1) \::' >$@
endef

#define transform-o-files-to-bin-file
#	$(silent)mkdir -p $(dir $@)
#	$(silent)$(TOOLCHAIN_PREFIX)$(CC) -o $@ $(1) $($@.local_ldflags) $(LDFLAGS)
#endef
#
#define transform-c-file-to-s-file
#	$(silent)mkdir -p $(dir $@)
#	$(silent)$(TOOLCHAIN_PREFIX)$(CC) -S $($@.local_cflags) $(CFLAGS) -o $@ $<
#endef

#define transform-d-files-to-target-d-file
#	$(silent)mkdir -p $(dir $@);
#	$(silent)echo "#auto generate!!!---xiaofei">$@;
#	$(silent)for d in $^ ; do \
#		if (cat $$d | sed '$$d' | sed 's:^.*\::$($@.target_file) \::' >>$@) ; then \
#			true; \
#		else \
#			exit 1; \
#		fi; \
#	done;
#endef
define transform-d-files-to-target-d-file
	$(silent)mkdir -p $(dir $@);
	$(silent)echo "#auto generate!!!---xiaofei">$@;
	$(silent)for d in $^ ; do \
		if (cat $$d | sed 's:^.*\::$($@.target_file) \::' >>$@) ; then \
			true; \
		else \
			exit 1; \
		fi; \
	done;
endef

