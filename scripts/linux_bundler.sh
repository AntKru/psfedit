#!/bin/sh -e

readlineFile=`ldd ${MESON_INSTALL_PREFIX}/bin/psfedit | grep libreadline | cut -d ' ' -f 3`
ncursesFile=`ldd ${MESON_INSTALL_PREFIX}/bin/psfedit | grep libncurses | cut -d ' ' -f 3`
panelFile=`ldd ${MESON_INSTALL_PREFIX}/bin/psfedit | grep libpanel | cut -d ' ' -f 3`
menuFile=`ldd ${MESON_INSTALL_PREFIX}/bin/psfedit | grep libmenu | cut -d ' ' -f 3`
mkdir ${MESON_INSTALL_PREFIX}/lib -p
cp $readlineFile ${MESON_INSTALL_PREFIX}/lib/
cp $ncursesFile ${MESON_INSTALL_PREFIX}/lib/
cp $panelFile ${MESON_INSTALL_PREFIX}/lib/
cp $menuFile ${MESON_INSTALL_PREFIX}/lib/
strip ${MESON_INSTALL_PREFIX}/lib/*
cp ${MESON_SOURCE_ROOT}/scripts/psfedit.sh ${MESON_INSTALL_PREFIX}

