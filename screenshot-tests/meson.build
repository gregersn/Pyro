test_sources = ['tests-main.cpp',
                'test-image.cpp',
                'test-image-composition.cpp',
                'test-image-blend.cpp',
                'test-image-placements.cpp',
                'test-image-resize.cpp',
                'test-lines.cpp',
                'test-noise.cpp',
                'test-shapes.cpp',
                'test-typography.cpp',
                'screenshotmatcher.cpp']

testexe = executable('screenshot-tests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib,
                     dependencies: [freetype_dep, catch2.get_variable('catch2_dep')],
                     install: false)

test('Screenshots', testexe)

run_target('update_screenshots',
        command: './update_screenshots.sh')