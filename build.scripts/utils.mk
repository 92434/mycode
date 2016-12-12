MAKE := make

comma:=,
quote:= "
squote:= '
dot:=.
semicolon:=:
star:=*
empty:=
space:=$(empty) $(empty)

escsq = $(subst $(squote),'\$(squote)',$1)
any-changed = $(filter $(wildcard $^), $?)
any-depends = $(filter-out $(wildcard $^),$^)

define target-why
$(if $(wildcard $@), 
	$(if $(strip $(any-changed)),
		- due to $(any-changed) changed,
		- due to depends $(any-depends)
	),
	- due to target missing
)
endef
target-why-string = $(call escsq, $(strip $(target-why)))
define target-echo-why
	@echo -e "$@:\033[31m"$(call target-why-string)"\033[0m"
endef

ifeq ("$(origin S)", "command line")
$(info "silent flag is $(S)")
endif
S ?= 0

is-verbose = $(S:1=)
silent := $(if $(call is-verbose),,@)

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
