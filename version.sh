#!/bin/sh
if [ "$1" = "get_vcs" ]; then
  git -C "$MESON_SOURCE_ROOT" describe --always --dirty
elif [ "$1" = "set_dist" ]; then
  $MESONREWRITE --sourcedir="$MESON_PROJECT_DIST_ROOT" kwargs set project / version "$2"
else
  exit 1
fi

