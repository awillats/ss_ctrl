PLUGIN_NAME = ss_ctrl

HEADERS = ss_ctrl.h

SOURCES = ss_ctrl.cpp\
          moc_ss_ctrl.cpp\
		  ../../../module_help/StAC_rtxi/dataFuns.cpp\

LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
