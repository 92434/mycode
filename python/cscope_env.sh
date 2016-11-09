#!/bin/sh

export tags_src_dir=$(python -c "import sys; print ':'.join([i for i in sys.path if i != ''])")
