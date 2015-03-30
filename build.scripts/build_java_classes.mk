define gen-name-for-class-files
	$(patsubst java/src/%,$(out_dir)/class/%.class,$(basename $(1)))
endef

define transform-java-file-to-class
	$(quiet)mkdir -p $(out_dir)/class
	$(quiet)$(JAVAC) -d $(out_dir)/class $<
endef

$(out_dir)/class/%.class : java/src/%.java
	$(call transform-java-file-to-class)

class_files := $(call gen-name-for-class-files, $(java_files))
$(eval target_files += $(class_files))
