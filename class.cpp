#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include "class.hpp"
#include "func.hpp"

namespace FL1
{
    double mul;
    int N;
    enum {
        spacing = 5,
        button_w = 200,
        button_h = 80,
        font_size = 30,
        action_spacing = 20,
        action_w = 600,
        action_h = 150
    };
}
using namespace FL1;
Actions::Actions(int x, int y, int h, int w, const char *name) 
    : Fl_Radio_Button(x, y, h, w, name) 
{ 
    labelsize(font_size*mul);
}
Movement::Movement(int x, int y, int h, int w, const char *name, controls *ctrl) 
    : Fl_Button(x, y, h, w, name) 
{
    labelsize(font_size*mul); 
    color(FL_YELLOW);
    color2(FL_YELLOW);
    callback(next_step, ctrl);
}

void Movement::next_step(Fl_Widget *w, void *user)
{
    controls *ctrl = (controls*)user;
    ctrl->s->change_scene(w, ctrl);
}

void Scene::change_scene(Fl_Widget *w, controls *ctrl)
{
    int **a1, i, j;
    a1 = new int* [N];
    for (i = 0; i < N; i++)
    {
        a1[i] = new int [N];
        for (j = 0; j < N; j++)
        {
            //a1[i][j] = cells[i][j]->get_type();
            a1[i][j] = 0;
            if (dynamic_cast <Balloon*> (cells[i][j]))
				a1[i][j] = 1;
			if (dynamic_cast <Bubble*> (cells[i][j]))
				a1[i][j] = 2;
		}
    }
    for (i = 0; i < N; i++)
        for (j = 0 ; j < N; j++)
        {
            if (a1[i][j] == 1)
                a1[i][j] = 10 + rand()%8;
            if (a1[i][j] == 2)
                a1[i][j] = 20 + rand()%8;
        }
    int ***a2 = new int** [N];
    int **count = new int* [N];
    first_try(a1, a2, count, N);
    second_try(a1, a2, count, N);
    third_try(a1, a2, count, N);
    delete [] a1;
    delete [] a2;
    this->swap(ctrl, count);
}
void Scene::swap(controls *ctrl, int **count)
{
    int a1[N][N], i, j;
    ctrl->win->begin();
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
			int type = 0;
            if (dynamic_cast <Balloon*> (cells[i][j]))
				type = 1;
			if (dynamic_cast <Bubble*> (cells[i][j]))
				type = 2;
            if (type)
            {
                delete cells[i][j];
                cells[i][j] = new Cell (mul*(2*spacing*(i+1) + action_w + button_h*i), 
                                        mul*(2*spacing*(j+1) + button_h*j), button_h*mul, button_h*mul);
                cells[i][j]->do_call(ctrl);
            }
        }
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            if (count[i][j] == 10)
            {
                delete cells[i][j];
                cells[i][j] = new Balloon (mul*(2*spacing*(i+1) + action_w + button_h*i), 
                                           mul*(2*spacing*(j+1) + button_h*j), ctrl);
            }
            if (count[i][j] == 1)
            {
                delete cells[i][j];
                cells[i][j] = new Bubble (mul*(2*spacing*(i+1) + action_w + button_h*i), 
                                          mul*(2*spacing*(j+1) + button_h*j), ctrl);
            }
        }
    ctrl->win->redraw();
    ctrl->win->end();
    delete [] count;
}

void Cell::OnPress(Fl_Widget *w, void *user) 
{
    controls *ctrl = (controls*)user;
    if (ctrl->act[0]->value())
        ctrl->s->cont(w, ctrl, 0);
    if (ctrl->act[1]->value())
        ctrl->s->cont(w, ctrl, 1);
    if (ctrl->act[2]->value() || ctrl->act[3]->value())
        ctrl->s->cont(w, ctrl, -1);
}

/*
int Cell::get_type() const
{
    return 0;
}
*/
Round::Round(int x, int y, controls *ctrl) 
    : Cell(x, y, mul*button_h, mul*button_h)
{ 
    labelsize(font_size*mul); 
    callback(Press, ctrl);
}

void Round::Press(Fl_Widget *w, void *user)
{
    controls *ctrl = (controls*)user;
    if (ctrl->act[2]->value())
        ctrl->s->cont(w, ctrl, 2);
    if (ctrl->act[3]->value())
        ctrl->s->cont(w, ctrl, 3);
    if (ctrl->act[0]->value() || ctrl->act[1]->value())
        ctrl->s->cont(w, ctrl, -1);
}


/*
int Balloon::get_type() const
{
    return 1;
}
*/

/*
int Bubble::get_type() const
{
    return 2;
}
*/
Scene::Scene()
{
    cells = new Cell** [N];
    for (int i = 0; i < N; i++)
    {
        cells[i] = new Cell* [N];
        int x = 2*spacing*(i+1) + action_w + button_h*i;
        for (int j = 0; j < N; j++)
        {
            int y = 2*spacing*(j+1) + button_h*j;
            cells[i][j] = new Cell(mul*x, mul*y, mul*button_h, mul*button_h);
        }
    }
}

void Scene::put_call(controls *ctrl)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cells[i][j]->do_call(ctrl);
}

void Scene::cont(Fl_Widget *w, controls *ctrl, int q)
{
    int k1 = 0, k2 = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (w->contains(cells[i][j]))
            {
                k1 = i;
                k2 = j;
            }
    //int type = cells[k1][k2]->get_type();
    int type = 0;
    if (dynamic_cast <Balloon*> (cells[k1][k2]))
		type = 1;
	if (dynamic_cast <Bubble*> (cells[k1][k2]))
		type = 2;
    if (type > 0 && !ctrl->act[type+1]->value() || q < 0)
    {
        Fl_Window *w = new Fl_Window(mul*action_w, mul*action_h*2, "ERROR");
        Fl_Box *b = new Fl_Box(0, 0, mul*action_w, mul*action_h*2, "ERROR");
        b->labelsize(3*font_size*mul);
        b->labelcolor(FL_GREEN);
        w->color(FL_RED);
        w->end();
        w->show();
        return;
    }
    ctrl->win->begin();
    delete cells[k1][k2];
    if (!q)
        cells[k1][k2] = new Balloon(mul*(2*spacing*(k1+1) + action_w + button_h*k1), 
                                    mul*(2*spacing*(k2+1) + button_h*k2), ctrl);
    if (q == 1)
        cells[k1][k2] = new Bubble (mul*(2*spacing*(k1+1) + action_w + button_h*k1),
                                    mul*(2*spacing*(k2+1) + button_h*k2), ctrl);
    if (q > 1)
    {
        cells[k1][k2] = new Cell (mul*(2*spacing*(k1+1) + action_w + button_h*k1), 
                                  mul*(2*spacing*(k2+1) + button_h*k2), button_h*mul, button_h*mul);
        cells[k1][k2]->do_call(ctrl);
    }
    ctrl->win->redraw();
    ctrl->win->end();
}

Scene::~Scene()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            delete cells[i][j];
        delete [] cells[i];
    }
    delete [] cells;
}
controls::controls(int N1, double mull)
{
    N = N1;
    mul = mull;
    int win_w = (spacing*2*(N+1) + action_w + button_h*N);
    int win_h = std::max(spacing*(2*N + 1) + button_h*N, action_spacing*6 + action_h*5);
    win = new Fl_Window(mul*win_w, mul*win_h, "buttons");
    act[0] = new Actions(mul*spacing, mul*action_spacing, mul*action_w, mul*action_h, "Добавить шарик");
    act[0]->labelcolor(FL_RED);
    act[1] = new Actions(mul*spacing, mul*(2*action_spacing + action_h), mul*action_w, mul*action_h, "Добавить пузырь");
    act[1]->labelcolor(FL_BLUE);
    act[2] = new Actions(mul*spacing, mul*(3*action_spacing + action_h*2), mul*action_w, mul*action_h, "Забрать шарик");
    act[3] = new Actions(mul*spacing, mul*(4*action_spacing + action_h*3), mul*action_w, mul*action_h, "Лопнуть пузырь");
    for (int i = 0; i < 4; i++)
        act[i]->labelsize(mul*font_size);
    s = new Scene();
    s->put_call(this);
}
controls::~controls()
{
    for (int i = 0; i < 4; i++)
        delete act[i];
    delete s;
    //delete win;
}

