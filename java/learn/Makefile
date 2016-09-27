include $(default_rules)

#LOCAL_PRECONDITION := resource

java_files := com/xiaofei/test/ProcessStream.java

jar_name := ProcessStream

#package_inc := -C resources .

#include_class_path := out/bin/jieba.jar

entry_point := com.xiaofei.test.ProcessStream

include $(BUILD_CLASS)

ProcessStream:
	java -jar out/bin/ProcessStream.jar ls


#LOCAL_PRECONDITION := resource

java_files := com/xiaofei/test/StreamReader.java
java_files += com/xiaofei/test/ExecuteScript.java

jar_name := ExecuteScript

#package_inc := -C resources .

#include_class_path := out/bin/jieba.jar

entry_point := com.xiaofei.test.ExecuteScript

include $(BUILD_CLASS)


ExecuteScript:
	java -jar out/bin/ExecuteScript.jar

#LOCAL_PRECONDITION := resource

java_files := com/xiaofei/test/StaticClassMember.java

jar_name := StaticClassMember

#package_inc := -C resources .

#include_class_path := out/bin/StaticClassMember.jar

entry_point := com.xiaofei.test.StaticClassMember

include $(BUILD_CLASS)

StaticClassMember:
	java -jar out/bin/StaticClassMember.jar

include $(ADD_TARGET)
