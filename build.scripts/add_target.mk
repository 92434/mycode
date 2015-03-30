ifneq ($(MAKECMDGOALS),clean)
include $(d_files)
endif

local_targets : $(target_files)
