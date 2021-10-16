test_sources = ['test-main.cpp',
                'test-color.cpp',
                'test-fonts.cpp',
                'test-graphics.cpp',
                'test-image.cpp',
                'test-math.cpp',
                'test-noise.cpp',
                'test-pyro.cpp',
                'test-vector.cpp',
                'test-string.cpp']

testexe = executable('unit-tests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib,
                     dependencies: [catch2.get_variable('catch2_dep')],
                     install: false)


test('Unittests', testexe, workdir: meson.source_root())
