pyro_sources = ['pyro.cpp',
                'pyrocolor.cpp', 
                'pyrofont.cpp',
                'pyrographics.cpp', 
                'pyrographics_cairo.cpp',
                'pyrofont_cairo.cpp',
                'pyroimage.cpp',
                'pyromath.cpp',
                'pyroshape.cpp',
                'pyrovector.cpp',
                'pyronoise.cpp',
                'opensimplexnoise.cpp']

pyrolib = shared_library('pyro', 
                         pyro_sources, 
                          include_directories: inc_dir,
                          dependencies: [libpng_dep, cairo_dep, sdl_dep, freetype2_dep, catch2.get_variable('catch2_dep')],
                          install: true,
                          
        cpp_args: '-Weffc++')
