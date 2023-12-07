MKC = ./mkclassv107
IMG = ./imgui
BUILD = ./exes
EXE = mkc_scp

all: build
	mv $(IMG)/mkc_scp $(BUILD)/mkc_scp
	mv $(IMG)/mkclass $(BUILD)/mkclass
	mv $(IMG)/mkprelim $(BUILD)/mkprelim
	mv $(IMG)/srebin0 $(BUILD)/srebin0
	mv $(IMG)/smooth2 $(BUILD)/smooth2
	@echo "$(EXE) executable in $(BUILD)"

build:
	make -C $(MKC) all
	make -C $(MKC) install
	make -C $(IMG) all
	@echo "MKC_SCP build complete!"

clean:
	make -C $(MKC) clean
	make -C $(IMG) clean
	rm -f $(BUILD)/*.exe
