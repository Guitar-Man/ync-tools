.PHONY: clean All

All:
	@echo "----------Building project:[ yncapi - Debug ]----------"
	@cd "yncapi" && "$(MAKE)" -f  "yncapi.mk"
	@echo "----------Building project:[ yncc-gtk - Debug ]----------"
	@cd "yncc-gtk" && "$(MAKE)" -f  "yncc-gtk.mk"
clean:
	@echo "----------Cleaning project:[ yncapi - Debug ]----------"
	@cd "yncapi" && "$(MAKE)" -f  "yncapi.mk"  clean
	@echo "----------Cleaning project:[ yncc-gtk - Debug ]----------"
	@cd "yncc-gtk" && "$(MAKE)" -f  "yncc-gtk.mk" clean
