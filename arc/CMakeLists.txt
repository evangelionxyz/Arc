add_executable(arc
    src/main.c
    src/core/app.c
    src/core/app.h
    src/core/darray.c
    src/core/darray.h
    src/core/string_utils.h
    src/core/types.h
    src/core/window.c
    src/core/window.h
    src/scene/game_object.c
    src/scene/game_object.h
    src/scene/components.c
    src/scene/components.h
    src/scene/scene.c
    src/scene/scene.h
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
