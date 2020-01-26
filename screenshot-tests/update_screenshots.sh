#!/bin/sh

cd "${MESON_SOURCE_ROOT}/screenshot-tests"
rsync -HP -vax current/* actual/
