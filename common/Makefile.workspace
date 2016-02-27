#
# Makefile for a workspace of EV3 Platform.
#

#
# Include configurations of EV3RT SDK
#
SDKDIR = ..
WSPDIR = $(basename $(PWD))
include $(SDKDIR)/Makefile.sdk.conf

# Configuration
SRCLANG := c
KERNEL := hrp2

#
# Functions
#
get_relpath = $(shell perl -MFile::Spec -e "print File::Spec->abs2rel(q($1),q($2))")

#
# Paths
#
KERNELDIR    := $(PWD)/$(SDKDIR)/$(EV3RT_BASE_DIR)
OBJDIR       := $(PWD)/$(SDKDIR)/$(EV3RT_PRJ_OBJ_DIR)
LIBKERNELDIR := $(PWD)/$(SDKDIR)/$(EV3RT_LIBKERNEL_DIR)
TARGETDIR    := $(PWD)/$(KERNELDIR)/target/ev3_gcc

# Object files
OBJFILENAME := $(KERNEL)
ifneq (, $(findstring CYGWIN, $(shell uname)))
    OBJFILENAME := $(OBJFILENAME).exe
endif
OBJBINARY := $(OBJDIR)/$(KERNEL).bin

#
# Determine Makefile for application
# OUTPUT:
# $(APPLDIR):         Absolute path of application folder
# $(MKFILENAME):      File name of Makefile
# $(MKFILE_DIR):      Absolute path of the folder holding Makefile
#
ifdef img
APPLDIR := $(PWD)/$(img)
MKFILENAME := Makefile.img
endif
ifdef app
APPLDIR := $(PWD)/$(app)
MKFILENAME := Makefile.app
endif
MKFILE_DIR := $(APPLDIR)
ifeq (,$(wildcard $(mkfile_dir)/$(MKFILENAME)))
MKFILE_DIR := $(SDKDIR)/common
endif



# Target for an application (static)
#
ifdef img

include $(APPLDIR)/Makefile.inc

ifeq (,$(wildcard $(LIBKERNELDIR)/libkernel.a))
# Build libkernel.a if not exist
img: $(APPLDIR) $(LIBKERNELDIR)/libkernel.a prepare-obj-folder
else
img: $(APPLDIR) prepare-obj-folder
endif
	@cd $(OBJDIR) && \
	make offset.h kernel_cfg.h && \
	make -j8 > /dev/null && \
	arm-none-eabi-objcopy -O binary \
	    $(OBJFILENAME) $(call get_relpath,$(OBJBINARY),$(OBJDIR))
	@mkimage -A arm -O linux -T kernel -C none -a 0xc0008000 -e 0xc0008000 \
        -n "TOPPERS/$(KERNEL) Kernel (EV3)" \
		-d $(call get_relpath,$(OBJBINARY),$(PWD)) uImage
	@chmod +x uImage
	@cp $(OBJDIR)/$(OBJFILENAME) $(PWD)

$(LIBKERNELDIR)/libkernel.a: prepare-obj-folder
	@cd $(OBJDIR) && \
	make clean && \
	make libkernel.a > /dev/null && \
	cp libkernel.a $(LIBKERNELDIR)/libkernel.a

endif

#
# Target for an application module (dynamic)
#
ifdef app

include $(APPLDIR)/Makefile.inc

app: $(APPLDIR) prepare-obj-folder
	@cd $(OBJDIR) && \
	make module_cfg.h && \
	make -j8 && \
	cp app $(PWD)/app # && cp app $(PWD)/app-$(subst /,,$(app))

endif

usage:
	@echo make img="<folder>"
	@echo make app="<folder>"

clean:
	rm -rf $(OBJDIR)

realclean: clean
	rm -rf $(notdir $(OBJFILENAME)) uImage app $(LIBKERNELDIR)/libkernel.a

#
# Phony target for preparing $(OBJDIR) folder
#
temp_mkfilename := .ev3rt_temp_Makefile
ifdef app
configure_copts := -DBUILD_MODULE
endif
prepare-obj-folder: clean
	@cp $(MKFILE_DIR)/$(MKFILENAME) $(APPLDIR)/$(temp_mkfilename)
	@mkdir -p $(OBJDIR)
	cd $(OBJDIR) && \
	$(KERNELDIR)/configure -T ev3_gcc -A app \
		-a $(call get_relpath,$(APPLDIR),$(OBJDIR)) \
		-t $(call get_relpath,$(APPLDIR),$(OBJDIR)) \
		-D $(call get_relpath,$(KERNELDIR),$(OBJDIR)) \
		-L $(call get_relpath,$(LIBKERNELDIR),$(OBJDIR)) \
		-l $(SRCLANG) \
		-m $(temp_mkfilename) -o "$(configure_copts)" \
		-U "$(APPLOBJS)" && \
	rm $(APPLDIR)/$(temp_mkfilename) && \
	mv $(temp_mkfilename) Makefile && \
	make clean


.PHONY: clean realclean prepare-obj-folder img app

