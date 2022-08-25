#ifndef CL
#define CL

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Button.H>
class Scene;
struct controls;
class Balloon;

class Actions : public Fl_Radio_Button
{
public:
    Actions(int x, int y, int h, int w, const char *name);
};

class Movement : public Fl_Button
{
public:
    Movement(int x, int y, int h, int w, const char *name, controls *ctrl);
    static void next_step(Fl_Widget *w, void *user);
};

class Cell : public Fl_Button
{
public:
    static void OnPress(Fl_Widget *w, void *user);
    Cell(int x, int y, int h, int w) : Fl_Button(x, y, h, w, nullptr) {}
    void do_call(controls *ctrl) { callback(OnPress, ctrl);}
    //virtual int get_type() const;
};



class Round : public Cell
{
public:
    Round(int x, int y, controls *ctrl);
    static void Press(Fl_Widget *w, void *user);
};


class Balloon : public Round
{
public:
    Balloon(int x, int y, controls *ctrl) : Round(x, y, ctrl) { color(FL_RED); }
    //virtual int get_type() const;
};

class Bubble : public Round
{
public:
    Bubble(int x, int y, controls *ctrl) : Round(x, y, ctrl) { color(FL_BLUE); }
    //virtual int get_type() const;
};

class Scene
{
    Cell ***cells;
public:
    Scene();
    void put_call(controls *ctrl);
    void cont(Fl_Widget *w, controls *ctrl, int q);
    void change_scene(Fl_Widget *w, controls *ctrl);
    void swap(controls *ctrl, int **count);
    int run() const { return Fl::run(); }
    ~Scene();
};

struct controls
{
    Actions *act[4];
    Scene *s;
    Fl_Window *win;
    controls(int N1, double mull);
    ~controls();
};

#endif
