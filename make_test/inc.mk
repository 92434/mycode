$(info MAKEFILE_LIST: $(MAKEFILE_LIST))

define my-dir
$(strip \
  $(eval LOCAL_MODULE_MAKEFILE := $(lastword $(MAKEFILE_LIST))) \
  $(info LOCAL_MODULE_MAKEFILE: $(LOCAL_MODULE_MAKEFILE)) \
  $(patsubst %/,%,$(dir $(LOCAL_MODULE_MAKEFILE))))
endef

$(info my-dir: $(call my-dir))
