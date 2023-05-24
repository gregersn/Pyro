test_sources = [
                'test-main.cpp',
                'test-image.cpp',
                'test-image-composition.cpp',
                'test-image-blend.cpp',
                'test-image-placements.cpp',
                'test-image-resize.cpp',
                'test-lines.cpp',
                'test-noise.cpp',
                'test-shapes.cpp',
                'test-typography.cpp',
                'test-resolution.cpp',
                'test-colors.cpp',
                'screenshotmatcher.cpp']

catch2 = subproject('catch2')

testexe = executable('screenshot-tests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib,
                     dependencies: [catch2.get_variable('catch2_dep'), eigen_dep],
                     install: false)

test('Screenshots', testexe, workdir: meson.source_root())

run_target('update_screenshots',
        command: './update_screenshots.sh')
