comma:=,
quote:= "
squote:= '
dot:=.
semicolon:=:
star:=*
empty:=
space:=$(empty) $(empty)

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

MAKE := make

out_dir := out

SUB_DIRS :=

escsq = $(subst $(squote),'\$(squote)',$1)
any-changed = $(filter $(wildcard $^), $?)
any-deponds = $(filter-out $(wildcard $^),$^)

define why
$(if $(wildcard $@), 
	$(if $(strip $(any-changed)),
		- due to $(any-changed) changed,
		- due to depends $(any-deponds)
	),
	- due to target missing
)
endef
why_string = $(call escsq, $(strip $(why)))
define echo-why
	@echo -e "$@:\033[31m"$(call why_string)"\033[0m"
endef

ifeq ("$(origin S)", "command line")
$(info "silent flag is $(S)")
endif
S ?= 0

is-verbose = $(S:1=)
quiet := $(if $(call is-verbose),,@)

define all-dirs-under
	$(patsubst ./%/,%,\
		$(dir \
			$(shell find . -name Makefile -and -not -name ".*") \
		 ) \
	)
endef

#all_dirs := $(strip $(call all-dirs-under))

define sub-dirs-make
	@for dir in $(1) ; do \
		if test -d $$dir ; then \
			echo "$$dir: $(2)" ; \
			if ($(MAKE) -C $$dir $(2)) ; then \
				true; \
			else \
				exit 1; \
			fi; \
		fi; \
	done
endef

target : sub_dirs_make local_targets
	$(call echo-why)

sub_dirs_make :
	$(call sub-dirs-make,$(SUB_DIRS))
	$(call echo-why)

clean : sub_dirs_clean
	$(quiet)-rm -rf out cscope e_cs dep_files
	$(call echo-why)

sub_dirs_clean : 
	$(call sub-dirs-make,$(SUB_DIRS),clean)
	$(call echo-why)

clean_bin : sub_dirs_clean_bin
	$(quiet)if [ -n "$(strip $(target_files))" ]; then rm $(strip $(target_files)); fi
	$(call echo-why)
sub_dirs_clean_bin : 
	$(call sub-dirs-make,$(SUB_DIRS),clean_bin)
	$(call echo-why)

clean_dep : sub_dirs_clean_dep
	$(quiet)if [ -n "$(strip $(d_files))" ]; then rm $(strip $(d_files)); fi
	$(call echo-why)

sub_dirs_clean_dep : 
	$(call sub-dirs-make,$(SUB_DIRS),clean_dep)
	$(call echo-why)

cscope: target
	$(quiet)tags.sh prepare;
	$(quiet)touch dep_files;
	$(quiet)for f in $$(find . -type f -name "*.d" 2>/dev/null); do \
		for i in $$(cat $$f | sed 's/^.*: //g'); do \
			if test $${i:0:1} = /;then echo "$$i" >>dep_files;fi; \
		done; \
	done;
	$(quiet)cat dep_files | sort | uniq | sed 's/^\(.*\)$$/\"\1\"/g' >> cscope/cscope.files;
	$(quiet)cat dep_files | sort | uniq >> cscope/ctags.files;
	$(quiet)tags.sh cscope;
	$(quiet)tags.sh tags;
	$(quiet)tags.sh env;
	$(call echo-why)
