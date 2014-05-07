.PHONY: default
default:
	$(MAKE) --directory=examples

.PHONY: clean
clean:
	$(MAKE) --directory=examples clean
	$(MAKE) --directory=sbase clean
	$(MAKE) --directory=sani clean
	$(MAKE) --directory=testdriver clean
