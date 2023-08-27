test_sources = [
                'test-color.cpp',
                'test-fonts.cpp',
                'test-graphics.cpp',
                'test-image.cpp',
                'test-math.cpp',
                'test-noise.cpp',
                'test-pyro.cpp',
                'test-vector.cpp',
                'test-string.cpp',
                'test-transformer2d.cpp',
                'test-shape.cpp']

catch2 = subproject('catch2', default_options: 'tests=false')

testexe = executable('unit-tests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib,
                     dependencies: [catch2.get_variable('catch2_with_main_dep'), eigen_dep],
                     install: false)


test('Unittests', testexe, workdir: meson.source_root())
