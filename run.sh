#!/bin/sh

export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES="LIBFT_MALLOC.SO"
export DYLD_FORCE_FLAT_NAMESPACE=1 $@
