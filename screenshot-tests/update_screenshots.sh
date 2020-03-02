#!/bin/sh

cd "${MESON_SOURCE_ROOT}/screenshot-tests"
rsync -HP -vaxc ${MESON_BUILD_ROOT}/current/* actual/
