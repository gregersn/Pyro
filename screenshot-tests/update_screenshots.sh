#!/bin/sh

cd "${MESON_SOURCE_ROOT}/screenshot-tests"
rsync -HP -vax ${MESON_BUILD_ROOT}/current/* actual/
