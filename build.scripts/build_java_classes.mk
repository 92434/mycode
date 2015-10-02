define gen-class-path-from-jar-name
	$(patsubst %,$(out_dir)/class/%,$(1))
endef

define gen-jar-path-from-jar-name
	$(patsubst %,$(out_dir)/bin/%.jar,$(1))
endef

define transform-java-file-to-jar
	$(quiet)mkdir -p $($@.class_path)
	$(quiet)mkdir -p $($@.bin_path);
	$(quiet)$(JAVAC) $(subst $(space)$(semicolon),$(semicolon),$($@.cp)) -d $($@.class_path) $^;
	$(quiet)$(JAR) $($@.option) $@ $($@.entry_point) -C $($@.class_path) . $($@.package_inc);
endef

class_path := $(call gen-class-path-from-jar-name,$(jar_name))
jar_file := $(call gen-jar-path-from-jar-name,$(jar_name))

$(jar_file) : $(java_files)
	$(call transform-java-file-to-jar)

$(eval $(jar_file).class_path := $(class_path))
$(eval $(jar_file).bin_path := $(dir $(jar_file)))
$(eval $(jar_file).package_inc += $(package_inc))
ifneq ($(include_class_path),)
$(eval $(jar_file).cp := -cp $(dot))
endif
$(foreach cp,$(include_class_path),$(eval $(jar_file).cp += $(semicolon)$(strip $(cp))))
ifeq ($(entry_point),)
$(eval $(jar_file).option := cvf)
else
$(eval $(jar_file).option := cvfe)
endif
$(eval $(jar_file).entry_point := $(entry_point))

$(eval target_files += $(jar_file))

include_class_path :=
jar_name := 
package_inc :=
entry_point :=
