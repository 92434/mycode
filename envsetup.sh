#!/bin/sh

export top=$(pwd)
export utils=$top/build.scripts/utils.mk
export default_rules=$top/build.scripts/Makefile.mk
export C_COMMON=$top/build.scripts/c_common.mk
export BUILD_BINS=$top/build.scripts/build_bins.mk
export BUILD_APP=$top/build.scripts/build_app.mk
export BUILD_LIB=$top/build.scripts/build_staticd_lib.mk
export BUILD_SO=$top/build.scripts/build_shared_lib.mk
export BUILD_KO=$top/build.scripts/build_kernel_modules.mk
export BUILD_CLASS=$top/build.scripts/build_java_classes.mk
export ADD_TARGET=$top/build.scripts/add_target.mk
