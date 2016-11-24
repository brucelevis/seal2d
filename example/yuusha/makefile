.PHONY: mac shader build run clean

project_name=$(shell basename `pwd`)

BGFX_DIR=../../libs/bgfx/
RUNTIME_DIR=$(BGFX_DIR)/examples/runtime
BUILD_DIR=./Resources/
SHADERS_DIR=../../seal/
include $(BGFX_DIR)/scripts/shader.mk

# NOTICE 1: BUILD the shaderc for the first time, in $(BGFX_DIR), make shaderc. :)
# NOTICE 2: Uncomment line 14/15 to build shader on windows.
shader:
	#@make -s --no-print-directory TARGET=0 clean all
	#@make -s --no-print-directory TARGET=1 clean all
	@make -s --no-print-directory TARGET=2 clean all
	@make -s --no-print-directory TARGET=3 clean all
	@make -s --no-print-directory TARGET=4 clean all
	@make -s --no-print-directory TARGET=5 clean all

mac:
	make -C raw

build:
	xcodebuild -project proj.mac/$(project_name).xcodeproj -target $(project_name)

run:
	./proj.mac/build/Release/$(project_name)

