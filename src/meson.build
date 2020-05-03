pyro_sources = ['pyro.cpp',
                'pyro_sdl.cpp',
                'pyrocolor.cpp', 
                'pyrographics.cpp', 
                'pyrographics_cairo.cpp',
                'pyrographics_3d.cpp',
                'pyroimage.cpp',
                'pyromath.cpp',
                'pyroshape.cpp',
                'pyrovector.cpp',
                'pyronoise.cpp',
                'pyrostring.cpp',
                'opensimplexnoise.cpp']

pyrolib = shared_library('pyro', 
                         pyro_sources, 
                          include_directories: inc_dir,
                          dependencies: [libpng_dep, cairo_dep, sdl_dep, catch2.get_variable('catch2_dep')],
                          install: true,
                          
        cpp_args: '-Weffc++')
