PYTHON := /usr/bin/env python

all:
	@$(PYTHON) ./setup.py build_ext --inplace

clean:
	@find . -depth -regex '.*/build\|.*/dist\|.*/__pycache__\|.*\.egg-info' -type d -exec rm -rf '{}' \;
	@find . -regex '.*\.py[co]\|.*\.so' -type f -delete

.PHONY: all clean

