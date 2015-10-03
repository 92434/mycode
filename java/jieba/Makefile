include $(default_rules)

LOCAL_PRECONDITION := resources

java_files := jieba/DictSegment.java
java_files += jieba/Hit.java
java_files += jieba/CharacterUtil.java
java_files += jieba/JiebaSegmenter.java
java_files += jieba/Node.java
java_files += jieba/Pair.java
java_files += jieba/SegToken.java
java_files += jieba/WordDictionary.java
java_files += jieba/viterbi/FinalSeg.java

jar_name := jieba

package_inc := -C resources .

#entry_point := com.huaban.analysis.jieba.DictSegment

include $(BUILD_CLASS)

java_files := JiebaSegmenterTest.java

include_class_path := out/bin/jieba.jar

jar_name := JiebaSegmenterTest

entry_point := com.huaban.analysis.jieba.JiebaSegmenterTest

include $(BUILD_CLASS)

include $(ADD_TARGET)

resources:
	7z x resources.7z

clean: local_clean
local_clean:
	rm resources -rf
run:
	java -Xbootclasspath/a:out/bin/jieba.jar -jar out/bin/JiebaSegmenterTest.jar
