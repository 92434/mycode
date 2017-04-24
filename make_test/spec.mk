local_m := $(module)
$(local_m)-name := $(name)
$(local_m)-type := $(type)

$(eval targets += $(local_m))
