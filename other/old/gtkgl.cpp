#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>

typedef unsigned char uchar; // int between 0 and 255

// http://fr.openclassrooms.com/informatique/cours/creez-des-programmes-en-3d-avec-opengl
// http://www-evasion.imag.fr/Membres/Antoine.Bouthors/teaching/opengl/opengl3.html

//  g++ gtkgl.cpp -Werror -Wall -W -O2 `pkg-config --cflags --libs gtkmm-2.4 gtkglextmm-1.2`


// http://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Introduction.html

static float angle = 0.0f;

void DrawCube6(float,float,float,uchar,uchar,uchar);

class Tetrominos
{
private:
  std::vector<bool> shape;
  std::vector<unsigned char> color;
  std::vector<float> pos;
public:
  Tetrominos(int type=0);
  void gldisplay();
  void fall(float h){pos[2]+=h;}
  bool 
};
Tetrominos::Tetrominos(int type)
{
  shape.resize(16,false);
  color.resize(3,0);
  pos.resize(3,0.0f);
  if (type==1)
    {
      shape[1]=true;
      shape[5]=true;
      shape[9]=true;
      shape[10]=true;

      color[0]=255;
      color[1]=128;
      color[2]=0;
    }
  else
    {
      std::cerr<<"Type non reconnu"<<std::endl;
    }
}

void Tetrominos::gldisplay()
{
  for(int i=0;i<16;i++)
    {
      if (!shape[i])
	continue;
      float zi=(float)(i/4);
      float yi=(float)(i%4);
      
      DrawCube6(pos[0],pos[1]+yi,pos[2]+zi,color[0],color[1],color[2]);
    }
}

static Tetrominos T(1);

void DrawCube0()
{
  glPushMatrix();
  glBegin(GL_QUADS);

  glColor3ub(255,0,0); //face rouge
  glVertex3f(1.0f,1.0f,1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,1.0f);

  glColor3ub(0,255,0); //face verte
  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,1.0f);
 
  glColor3ub(0,0,255); //face bleue
  glVertex3f(-1.0f,-1.0f,1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(-1.0f,1.0f,1.0f);
    
  glColor3ub(255,0,255); //face magenta
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,1.0f);
  
  glColor3ub(255,255,0); //face jaune
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,1.0f,1.0f);

  glColor3ub(0,255,255); //face cyan
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
   
  glEnd();
  glPopMatrix();
}

void DrawCube6(float xPos, float yPos, float zPos,unsigned char cR,unsigned char cG, unsigned char cB)
{
  glPushMatrix();
  glTranslatef(xPos,yPos,zPos);
  glColor3ub(cR,cG,cB); //couleur
  glBegin(GL_QUADS);

  glVertex3f(1.0f,1.0f,1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,1.0f);

  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,1.0f);
 
  glVertex3f(-1.0f,-1.0f,1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(-1.0f,1.0f,1.0f);
    
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,1.0f);
  
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,1.0f,1.0f);

  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
   

  glEnd();
  glPopMatrix();

}

void Render()
{
  //static float angle = -1.0f;
  // Increase rotation angle counter
  angle += 1.0f;
  //std::cout<<angle<<std::endl;
  // Reset after we have completed a circle
  if (angle >= 360.0f)
      angle = 0.0f;

  glPushMatrix();

  
  // Move, rotate, draw
  DrawCube6(-1,-1,0,255,0,0);
  DrawCube6(-1,1,0,255,255,0);
  DrawCube6(1,-1,0,255,0,255);
  DrawCube6(1,1,0,255,120,120);
  glRotatef(angle,0.0f,0.0f,1.0f);
  T.gldisplay();

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
  gluPerspective(90,(double)640/480,1,1000);

 // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  std::cout<<"on_expose_event\n";
  
  refresh();

  (void)event;
  return true;
}

void GLWidget::refresh()
{
  Glib::RefPtr<Gdk::GL::Drawable> d = get_gl_drawable();

  // Clears the screen, the color and depth buffers, and reset our modelview matrix.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // and reset our modelview matrix.
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  // Place the camera
  gluLookAt(3,3,3,0,0,0,0,0,1);

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
  
  void refresh_area(){drawingArea->refresh();}
  bool on_key_pressed(GdkEventKey *evt);
  bool isactive;
private:
  Gtk::HBox* boite;
  GLWidget* drawingArea;
  Gtk::Button* bouton;
};
Fenetre::Fenetre()
{
  isactive=true;
  
  boite= new Gtk::HBox(true,10);
  boite->show();

  Glib::RefPtr<Gdk::GL::Config> glconfig;
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
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
  bouton->signal_clicked().connect(sigc::mem_fun(*this, &Fenetre::refresh_area));
    
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
  if ( (evt->keyval == GDK_z || evt->keyval == GDK_Z) & (evt->type == GDK_KEY_PRESS) )
    {
      drawingArea->refresh();
      return true;
    }
  // this signal handler didn't do anything with the event
  // pass it on to the default (or other signal) handlers
  return false;
}




Fenetre::~Fenetre()
{
  isactive=false;
  delete boite;
  delete drawingArea;
  delete bouton;
}

static gboolean time_handler(Fenetre *widget)
{
  if (!widget->isactive) return false; //widget->window==NULL ... pour terminer quand on a détruit le widget

  T.fall(0.01);
  widget->refresh_area();

  return true;
}



int main(int argc, char **argv)
{
    Gtk::Main app(argc, argv);
    Gtk::GL::init(argc, argv);
 
    Fenetre  mainWindow;

    g_timeout_add(40, (GSourceFunc)time_handler, &mainWindow); // 25fps

    Gtk::Main::run(mainWindow);
    return 0;
}
