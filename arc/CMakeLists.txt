add_executable(arc
    src/main.c
    src/core/app.c
    src/core/darray.c
    src/core/window.c
    src/renderer/texture.c
    src/scene/game_object.c
    src/scene/components.c
    src/scene/scene.c
)

target_include_directories(arc PRIVATE
    ${THIRD_PARTY_DIR}/raylib/src/
    ${ROOT_DIR}/arc/src
)

add_dependencies(arc
    raylib
)

target_link_libraries(arc PRIVATE
    raylib
)
