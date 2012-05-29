.PHONY: clean All

All:
	@echo ----------Building project:[ gateway - Debug ]----------
	@cd "gateway" && "$(MAKE)" -f "gateway.mk"
clean:
	@echo ----------Cleaning project:[ gateway - Debug ]----------
	@cd "gateway" && "$(MAKE)" -f "gateway.mk" clean
