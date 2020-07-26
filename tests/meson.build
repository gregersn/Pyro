test_sources = ['tests-main.cpp',
                'test-color.cpp',
                'test-fonts.cpp',
                'test-graphics.cpp',
                'tests-image.cpp',
                'test-math.cpp',
                'test-noise.cpp',
                'test-pyro.cpp',
                'tests-vector.cpp',
                'test-string.cpp']

testexe = executable('unit-tests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib,
                     dependencies: [catch2.get_variable('catch2_dep')],
                     install: false)


test('Unittests', testexe)
