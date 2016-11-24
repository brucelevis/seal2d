.PHONY: mac shader build run clean

project_name=$(shell basename `pwd`)

BGFX_DIR=../../libs/bgfx/
RUNTIME_DIR=$(BGFX_DIR)/examples/runtime
BUILD_DIR=./Resources/custom

shader:
	include $(BGFX_DIR)/scripts/shader.mk

rebuild:
	@make -s --no-print-directory TARGET=0 clean all
	@make -s --no-print-directory TARGET=1 clean all
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

clean:
	rm -rf Resources/res
