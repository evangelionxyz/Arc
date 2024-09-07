#include "core/app.h"

int main(int argc, char **argv)
{
    Application *app = app_create(argc, argv);
    app_run(app);
    app_close(app);
    return 0;
}