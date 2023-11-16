MKC = ./mkclassv107
IMG = ./imgui

all:
	make -C $(MKC) all
	make -C $(MKC) install
	make -C $(IMG) all
	@echo "MKC_SCP build complete!"

clean:
	make -C $(MKC) clean
	make -C $(IMG) clean
