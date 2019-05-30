PLUGIN_NAME = ss_ctrl

HELP_DIR=/home/adam/RTXI/module_help
PLDS_DIR=$(HELP_DIR)/plds_adam/plds_adamX/plds_adamX


HEADERS = ss_ctrl.h\
	$(PLDS_DIR)/plds_adam_funs.hpp\
	$(PLDS_DIR)/plds_ctrl_adam.hpp


SOURCES = ss_ctrl.cpp\
          moc_ss_ctrl.cpp\
	$(HELP_DIR)/StAC_rtxi/dataFuns.cpp\
	  $(PLDS_DIR)/plds_adam_funs.cpp\
	$(PLDS_DIR)/plds_ctrl_adam.cpp

LIBS = 

CXXFLAGS += -I$(HELP_DIR)
CXXFLAGS += -I$(PLDS_DIR)

# dynCtrlEst
include Makefile.include_plds

# armadillo
include Makefile.include_arm

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
