#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

// http://fr.openclassrooms.com/informatique/cours/creez-des-programmes-en-3d-avec-opengl
//  g++ gtkgl.cpp -Werror -Wall -W -O2 `pkg-config --cflags --libs gtkmm-2.4 gtkglextmm-1.2`

void DrawCube(float xPos, float yPos, float zPos)
{
  glPushMatrix();
  glBegin(GL_POLYGON);

  // This is the top face
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, -1.0f);
  glVertex3f(-1.0f, 0.0f, -1.0f);
  glVertex3f(-1.0f, 0.0f, 0.0f);

  // This is the front face
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(0.0f, -1.0f, 0.0f);

  // This is the right face
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, -1.0f, 0.0f);
  glVertex3f(0.0f, -1.0f, -1.0f);
  glVertex3f(0.0f, 0.0f, -1.0f);

  // This is the left face
  glVertex3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 0.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);

  // This is the bottom face
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);

  // This is the back face
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 0.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(0.0f, -1.0f, -1.0f);

  glEnd();
  glPopMatrix();

  (void)xPos;
  (void)yPos;
  (void)zPos;

}

void Render()
{

  static float angle = 0.0f;
  std::cout<<angle<<std::endl;
  // Increase rotation angle counter
  angle += 1.0f;
  // Reset after we have completed a circle
  if (angle >= 360.0f)
      angle = 0.0f;

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  
  // Heres our rendering. Clears the screen
  // to black, clear the color and depth
  // buffers, and reset our modelview matrix.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt(5,5,5,0,0,0,0,0,1); //exemple

  // Move, rotate, draw and dispose of the current matrix.
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(0.1f, 0.0f, 0.0f);
  glRotatef(angle, 1.0f, 0.0f, 1.0f);
  glColor3ub(255,0,0); // rouge
  DrawCube(0.0f, 0.0f, 0.0f);
  glPopMatrix();

  glFlush();
}


class GLWidget : public Gtk::GL::DrawingArea {
public:
  GLWidget(Glib::RefPtr<Gdk::GL::Config> glconfig)
    : Gtk::GL::DrawingArea(glconfig) {}
  ~GLWidget() {}
  virtual bool on_expose_event(GdkEventExpose* event);
  void refresh();
};

bool GLWidget::on_expose_event(GdkEventExpose* event)
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70,(double)640/480,1,1000);
  std::cout<<"ini\n";

  
  refresh();

  (void)event;
  return true;
}

void GLWidget::refresh()
{
  Glib::RefPtr<Gdk::GL::Drawable> d = get_gl_drawable();
  d->gl_begin(get_gl_context());

 // make this as complex as you need
  Render();

  d->swap_buffers();
  d->gl_end();
}


class Fenetre : public Gtk::Window 
{
public:
  Fenetre();
  ~Fenetre();
  
  void on_button_clicked();
  bool on_key_pressed(GdkEventKey *evt);

private:
  Gtk::HBox* boite;
  GLWidget* drawingArea;
  Gtk::Button* bouton;
};
Fenetre::Fenetre()
{
  
    boite= new Gtk::HBox(true,10);
    boite->show();

    Glib::RefPtr<Gdk::GL::Config> glconfig;
    glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DOUBLE);
    if (!glconfig)
        glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB);
    if (!glconfig) {
        g_critical("Cannot create OpenGL-capable config\n");
	exit(1);
    }
    drawingArea = new GLWidget(glconfig);
    drawingArea->set_size_request(640, 480);
    boite->add(*drawingArea);
    drawingArea->show();

    bouton= new Gtk::Button("bouh");
    bouton->set_can_focus(false);
    boite->pack_start(*bouton);
    bouton->show();

    bouton->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &Fenetre::set_title), "Bonjour"));
    bouton->signal_clicked().connect(sigc::mem_fun(*this, &Fenetre::on_button_clicked));
    
    this->signal_key_press_event().connect(sigc::mem_fun(*this,&Fenetre::on_key_pressed),false);

    this->add(*boite);
}

bool Fenetre::on_key_pressed(GdkEventKey *evt)
{
      if (evt->keyval == GDK_a || evt->keyval == GDK_A)
        if (evt->state & GDK_CONTROL_MASK)
        {
            if (evt->type == GDK_KEY_PRESS)
	      std::cout << "Ctrl-A pressed"<<std::endl;
            else
	      std::cout << "Ctrl-A released"<<std::endl;

            // stop further processing of this event -
	    // we've handled it
            return true;
        }

    // this signal handler didn't do anything with the event
    // pass it on to the default (or other signal) handlers
    return false;
}




Fenetre::~Fenetre()
{
  delete boite;
  delete drawingArea;
  delete bouton;
}

void Fenetre::on_button_clicked()
{
  drawingArea->refresh();
}
  
int main(int argc, char **argv)
{
    Gtk::Main app(argc, argv);
    Gtk::GL::init(argc, argv);
 
    Fenetre  mainWindow;
    Gtk::Main::run(mainWindow);
    return 0;
}
