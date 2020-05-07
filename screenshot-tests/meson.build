test_sources = ['tests-main.cpp',
                'test-shapes.cpp',
                'test-image-placements.cpp',
                'test-image.cpp',
                'test-typography.cpp',
                'test-noise.cpp',
                'test-lines.cpp',
                'screenshotmatcher.cpp']

testexe = executable('screenshot-tests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib,
                     dependencies: catch2.get_variable('catch2_dep'),
                     install: false)

test('Screenshots', testexe)

run_target('update_screenshots',
        command: './update_screenshots.sh')