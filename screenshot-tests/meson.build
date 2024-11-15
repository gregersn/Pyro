test_sources = [
                'test-main.cpp',
                'test-curves.cpp',
                'test-graphics.cpp',
                'test-image.cpp',
                'test-image-composition.cpp',
                'test-image-blend.cpp',
                'test-image-placements.cpp',
                'test-image-resize.cpp',
                'test-lines.cpp',
                'test-noise.cpp',
                'test-shapes.cpp',
                'test-style.cpp',
                'test-typography.cpp',
                'test-resolution.cpp',
                'test-colors.cpp',
                'test-units.cpp',
                'test-pyro.cpp',
                'screenshotmatcher.cpp']

catch2 = subproject('catch2', default_options: 'tests=false')

testexe = executable('screenshot-tests', test_sources,
                     include_directories: inc,
                     link_with: pyro,
                     dependencies: [catch2.get_variable('catch2_dep'), eigen_dep],
                     install: false)

test('Screenshots', testexe, workdir: meson.project_source_root())

run_target('update_screenshots',
        command: './update_screenshots.sh')
