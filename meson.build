project('lbtl', ['c', 'cpp'], default_options : ['cpp_std=c++20'])

cmake = import('cmake')

reactphysics = cmake.subproject('reactphysics3d').dependency('reactphysics3d')

deps = [reactphysics]

if host_machine.system() == 'windows'
    # deps = []
else
    deps += [dependency('GL'), dependency('X11'), dependency('xi'), dependency('xcursor')]
endif

executable(
    'lbtl',
    [
        'src/main.cpp',
        'src/impl.cpp',
        'src/engine.hpp',
        'src/components.hpp',
        'src/renderer.cpp',
        'src/renderer.hpp',
        'src/player.cpp',
        'src/player.hpp',
        'src/world.cpp',
        'src/world.hpp',
        'src/assets.cpp',
        'src/assets.hpp',
        'src/flecs/flecs.h',
        'src/flecs/flecs.c'
        ],
    dependencies: deps,
    cpp_args : [
        '-DHANDMADE_MATH_NO_SSE',
        '-DHANDMADE_MATH_USE_TURNS',
        '-DFLECS_CUSTOM_BUILD',
        '-DFLECS_CPP',
    ],
)
