include $(utils)

#CFLAGS := -pg
#CFLAGS += -rdynamic
#CFLAGS += -Wall
CFLAGS :=
LDFLAGS :=
DEPS :=

LOCAL_CFLAGS :=
LOCAL_LDFLAGS :=
LOCAL_DEPS :=

TOOLCHAIN_PREFIX :=
CC := g++
AR := ar
JAVAC := javac
JAVA := java
JAR := jar

out_dir := out

SUB_DIRS :=

target : sub_dirs_make local_targets
	$(call target-echo-why)

sub_dirs_make :
	$(call sub-dirs-make,$(SUB_DIRS))
	$(call target-echo-why)

clean : sub_dirs_clean
	$(silent)-rm -rf out cscope e_cs dep_files
	$(call target-echo-why)

sub_dirs_clean : 
	$(call sub-dirs-make,$(SUB_DIRS),clean)
	$(call target-echo-why)

clean_bin : sub_dirs_clean_bin
	$(silent)if [ -n "$(strip $(target_files))" ]; then rm $(strip $(target_files)); fi
	$(call target-echo-why)
sub_dirs_clean_bin : 
	$(call sub-dirs-make,$(SUB_DIRS),clean_bin)
	$(call target-echo-why)

clean_dep : sub_dirs_clean_dep
	$(silent)if [ -n "$(strip $(d_files))" ]; then rm $(strip $(d_files)); fi
	$(call target-echo-why)

sub_dirs_clean_dep : 
	$(call sub-dirs-make,$(SUB_DIRS),clean_dep)
	$(call target-echo-why)

cscope: target
	$(silent)tags.sh prepare;
	$(silent)touch dep_files;
	$(silent)for f in $$(find . -type f -name "*.d" 2>/dev/null); do \
		for i in $$(cat $$f | sed 's/^.*: //g'); do \
			if test $${i:0:1} = /;then echo "$$i" >>dep_files;fi; \
		done; \
	done;
	$(silent)cat dep_files | sort | uniq | sed 's/^\(.*\)$$/\"\1\"/g' >> cscope/cscope.files;
	$(silent)cat dep_files | sort | uniq >> cscope/ctags.files;
	$(silent)tags.sh cscope;
	$(silent)tags.sh tags;
	$(silent)tags.sh env;
	$(call target-echo-why)
