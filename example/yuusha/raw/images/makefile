dirs=$(shell ls -l | grep ^d | awk '{print $$9}')
dst_dir=../../Resources/res/images
tp=/Applications/TexturePacker.app/Contents/MacOS/Texturepacker
.PHONY: all $(dirs)

$(dirs): $(dirs)
	mkdir -p $(dst_dir)
	$(tp) --data $(dst_dir)/$@.json --format json --sheet $(dst_dir)/$@.pvr $@/*.png --trim-mode None --disable-rotation --premultiply-alpha
	@echo making atlas in folder: $@

all:create_dir $(dirs)
