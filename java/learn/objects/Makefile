include $(default_rules)

LOCAL_PRECONDITION :=

java_files := Bits.java

include_class_path :=

jar_name := Bits

package_inc :=

entry_point := objects.Bits

include $(BUILD_CLASS)

$(eval all_jars := $(filter %.jar,$(target_files)))
$(info "all_jars:$(all_jars)")

include $(ADD_TARGET)


LOCAL_PRECONDITION :=

java_files := ArraySize.java

include_class_path :=

jar_name := ArraySize

package_inc :=

entry_point := objects.ArraySize

include $(BUILD_CLASS)

$(eval all_jars := $(filter %.jar,$(target_files)))
$(info "all_jars:$(all_jars)")

include $(ADD_TARGET)

run:
	#java -Xbootclasspath/a:out/bin/jieba.jar -jar out/bin/JiebaSegmenterTest.jar
	for i in $(all_jars); do \
		java -jar $$i; \
	done
