#include "core/game.h"


i32 main(i32 argc, char **argv)
{
    Game *game = game_create(argc, argv);
    game_run(game);
    game_close(game);
    return 0;
}