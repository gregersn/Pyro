#!/bin/sh

cd "${MESON_SOURCE_ROOT}/screenshots"
rsync -HP -vaxc ./current/* ./expected/
