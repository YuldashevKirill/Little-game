#include "class.hpp"
#include "func.hpp"
namespace m
{
    enum {
        spacing = 5,
        button_w = 200,
        button_h = 80,
        font_size = 30,
        action_spacing = 20,
        action_w = 600,
        action_h = 150
    };
};
using namespace m;
int main(int argc, char **argv)
{
    int N = atoi(argv[2]);
    srand(atoi(argv[1]));
    double mull = atof(argv[3]);
    controls *ctrl = new controls(N, mull);
    Movement *move = new Movement(mull*spacing, mull*(5*action_spacing + action_h*4), 
                                  mull*action_w, mull*action_h, "Следующий шаг", ctrl);
    ctrl->win->end();
    ctrl->win->show();
    ctrl->s->run();
    delete ctrl;
    delete move;
    return 0;
}

