project('lbtl', ['c', 'cpp'], default_options : ['cpp_std=c++20'])

cmake = import('cmake')

reactphysics = cmake.subproject('reactphysics3d').dependency('reactphysics3d').as_system()

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
        'src/alloc.cpp',
        'src/alloc.hpp',
        'src/types.hpp',
        'src/engine.hpp',
        'src/components.hpp',
        'src/input.cpp',
        'src/input.hpp',
        'src/physics.cpp',
        'src/physics.hpp',
        'src/renderer.cpp',
        'src/renderer.hpp',
        'src/player.cpp',
        'src/player.hpp',
        'src/world.cpp',
        'src/world.hpp',
        'src/prefab.cpp',
        'src/prefab.hpp',
        'src/assets.cpp',
        'src/assets.hpp',
        'src/thirdparty/flecs/flecs.h',
        'src/thirdparty/flecs/flecs.c'
        ],
    dependencies: deps,
    cpp_args : [
        '-DHANDMADE_MATH_NO_SSE',
        '-DHANDMADE_MATH_USE_TURNS',
        '-DFLECS_CUSTOM_BUILD',
        '-DFLECS_CPP',
    ],
)
