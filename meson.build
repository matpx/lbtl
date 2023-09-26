project('lbtl', ['c', 'cpp'])
executable(
    'lbtl',
    [
        'src/main.cpp',
        'src/impl.cpp',
        'src/utils.hpp',
        'src/components.hpp',
        'src/renderer.cpp',
        'src/renderer.hpp',
        'src/player.cpp',
        'src/player.hpp',
        'src/world.cpp',
        'src/world.hpp',
        'src/flecs/flecs.h',
        'src/flecs/flecs.c'
        ],
    dependencies: [
        dependency('GL'), dependency('X11'), dependency('xi'), dependency('xcursor')
    ],
    cpp_args : [
        '-DHANDMADE_MATH_NO_SSE',
        '-DHANDMADE_MATH_USE_TURNS',
        '-DFLECS_CUSTOM_BUILD',
        '-DFLECS_CPP',
    ],
)
