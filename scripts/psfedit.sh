#!/bin/bash

export LD_LIBRARY_PATH="${0%/*}/lib"
${0%/*}/bin/psfedit "$@"

