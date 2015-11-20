include $(default_rules)

LOCAL_PRECONDITION :=

java_files := Question.java

include_class_path :=

jar_name := Question

package_inc :=

entry_point := xiaofei.Question

include $(BUILD_CLASS)

$(eval all_jars := $(filter %.jar,$(target_files)))
$(info "all_jars:$(all_jars)")

include $(ADD_TARGET)

run:
	#java -Xbootclasspath/a:out/bin/jieba.jar -jar out/bin/JiebaSegmenterTest.jar
	for i in $(all_jars); do \
		java -jar $$i; \
	done
