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
    src/scene/physics_2d.h
    src/scene/physics_2d.c
 "src/scene/sprite_sheet.h" "src/scene/sprite_sheet.c")

target_include_directories(arc PRIVATE
    ${THIRD_PARTY_DIR}/raylib/src/
    ${THIRD_PARTY_DIR}/box2d/include/
    ${ROOT_DIR}/arc/src
)

add_dependencies(arc
    raylib
    box2d
)

target_link_libraries(arc PRIVATE
    raylib
    box2d
)
