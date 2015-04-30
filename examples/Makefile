ifndef CONFIG_SANDBOX

ifdef FTRACE
subdir-ccflags-y += -finstrument-functions -DFTRACE
endif

subdir-y += standalone
subdir-y += bist_test
subdir-y += hw_test
subdir-$(CONFIG_API) += api
endif
