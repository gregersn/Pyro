test_sources = ['tests-main.cpp',
                'test-math.cpp',
                'test-color.cpp',
                'test-pyro.cpp',
                'tests-image.cpp',
                'tests-vector.cpp']

testexe = executable('alltests', test_sources,
                     include_directories: inc_dir,
                     link_with: pyrolib)

test('Pyro test', testexe)
