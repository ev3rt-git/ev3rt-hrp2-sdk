APPL_COBJS += 

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/spp_master_test_api/Makefile

endif
