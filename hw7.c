/*
 * Lab 7: X-Wing with Lighting and Textures
 * Christopher Jordan
 * CSCI 4229 Fall 2014
 *
 * Utilizing code provided from William Schreuder
 * via the ex9 and ex13 programs
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include <time.h>

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=0;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=30.0;   //  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =  15;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light

// For idle function
unsigned long ot;
unsigned long dt;

// For initclock
int first=0;

//  Macro for sin & cos in degrees
double Ex, Ey, Ez;
double Vx = 0, Vy = 0, Vz =  0;
int strafe = 0;

// Mouse variables
int xOrigin = -1;
int yOrigin = -1;
int m_button = 0;

/*
 *  Determine normal for triangle using the first point as
 *  an origin for the vectors that go to points 2 and 3
 *     
 */
 static void normal(double x1, double y1, double z1,
                  double x2, double y2, double z2,
                  double x3, double y3, double z3){
   // Build first vector
   double v1x = x2 - x1;
   double v1y = y2 - y1;
   double v1z = z2 - z1;

   // Build second vector
   double v2x = x3 - x1;
   double v2y = y3 - y1;
   double v2z = z3 - z1;

   // Get Cross Product
   double nx = (v1y*v2z) - (v1z*v2y);
   double ny = (v1z*v2x) - (v1x*v2z);
   double nz = (v1x*v2y) - (v1y*v2x);

   // Set normal for trianlge plane
   glNormal3f(nx,ny,nz);
 }

/*
 *  Draw an x-wing wing laser cannon
 *     
 */
static void wingLaser(double x, double y, double z){
   int th = 0;

   glPushMatrix();
   glTranslated(x, y, z);

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 0);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,-1);
      glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 0);
      
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 0);
      glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 1);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 1);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,-1);
      glVertex3d(0.7 * Cos(th), 0.7 * Sin(th), 1);
      
   }
   glEnd();
   
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 8);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,1);
      glVertex3d(0.7 * Cos(th), 0.7 * Sin(th), 8);
      
   }
   glEnd();
   
   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(0.7 * Cos(th), 0.7 * Sin(th), 1);
      glVertex3d(0.7 * Cos(th), 0.7 * Sin(th), 8);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 16);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,1);
      glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), 16);
      
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), 8);
      glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), 16);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 24);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,1);
      glVertex3d(0.25 * Cos(th), 0.25 * Sin(th), 24);
      
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(0.25 * Cos(th), 0.25 * Sin(th), 16);
      glVertex3d(0.25 * Cos(th), 0.25 * Sin(th), 24);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 24.5);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,1);
      glVertex3d(0.8 * Cos(th), 0.8 * Sin(th), 24.5);
      
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), 24);
      glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), 24.5);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 26);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,1);
      glVertex3d(0.1 * Cos(th), 0.1 * Sin(th), 26);
      
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(0.1 * Cos(th), 0.1 * Sin(th), 24.5);
      glVertex3d(0.1 * Cos(th), 0.1 * Sin(th), 26);
   }
   glEnd();

   glPopMatrix();
}

/*
 *  Draw an x-wing wing engine
 *     
 */
static void wingEngine(double x, double y, double z){
   int th = 0;
   glPushMatrix();
   glTranslated(x, y, z);

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 0);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,-1);
      glVertex3d(1.5 * Cos(th), 1.5 * Sin(th), 0);
      
   }
   glEnd();
   
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, 8);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,1);
      glVertex3d(1.5 * Cos(th), 1.5 * Sin(th), 8);
      
   }
   glEnd();
   
   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(1.5 * Cos(th), 1.5 * Sin(th), 0);
      glVertex3d(1.5 * Cos(th), 1.5 * Sin(th), 8);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(0, 0, -9);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(0,0,-1);
      glVertex3d(0.8 * Cos(th), 0.8 * Sin(th), -9);
      
   }
   glEnd();

   glBegin(GL_QUAD_STRIP);
   for (th = 0; th <= 360; th += 1)
   {
      glNormal3d(Cos(th),Sin(th),0);
      glVertex3d(.8 * Cos(th), 0.8 * Sin(th), 0);
      glVertex3d(.8 * Cos(th), 0.8 * Sin(th), -9);
   }
   glEnd();
   glPopMatrix();
}

/*
 *  Draw the upper wing of the x-wing
 *     
 */
static void createWings(){
   // ------------------- Left Top Wing -----------------------------------
   glBegin(GL_POLYGON);
   normal(-3,0.5,-17, -3,0.5,-7, -25,8,-14);
   glVertex3d(-3,+0.5,-7);
   glVertex3d(-3,+0.5,-17);
   glVertex3d(-25,+8,-14);
   glVertex3d(-25,+8,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-3,1.5,-17, -3,1.5,-7, -25,9,-14);
   glVertex3d(-3,+1.5,-7);
   glVertex3d(-3,+1.5,-17);
   glVertex3d(-25,+9,-14);
   glVertex3d(-25,+9,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-25,8,-10, -3,0.5,-7, -25,9,-10);
   glVertex3d(-3,+0.5,-7);
   glVertex3d(-25,+8,-10);
   glVertex3d(-25,+9,-10);
   glVertex3d(-3,+1.5,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-25,8,-14, -3,0.5,-17, -25,9,-14);
   glVertex3d(-3,+0.5,-17);
   glVertex3d(-25,+8,-14);
   glVertex3d(-25,+9,-14);
   glVertex3d(-3,+1.5,-17);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-25,8,-14, -25,9,-14, -25,8,-10);
   glVertex3d(-25,+9,-14);
   glVertex3d(-25,+8,-14);
   glVertex3d(-25,+8,-10);
   glVertex3d(-25,+9,-10);
   glEnd();

   // Upper Left Engine and Laser
   wingEngine(-5.4, 3.75, -13);
   wingLaser(-24, 9, -16);

   // ------------------- Left Bottom Wing -----------------------------------
   glBegin(GL_POLYGON);
   normal(-3,0.5,-17, -3,0.5,-7, -25,-7,-14);
   glVertex3d(-3,+0.5,-7);
   glVertex3d(-3,+0.5,-17);
   glVertex3d(-25,-7,-14);
   glVertex3d(-25,-7,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-3,-0.5,-17, -3,-0.5,-7, -25,-8,-14);
   glVertex3d(-3,-0.5,-7);
   glVertex3d(-3,-0.5,-17);
   glVertex3d(-25,-8,-14);
   glVertex3d(-25,-8,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-25,-7,-10, -3,0.5,-7, -25,-8,-10);
   glVertex3d(-3,+0.5,-7);
   glVertex3d(-25,-7,-10);
   glVertex3d(-25,-8,-10);
   glVertex3d(-3,-0.5,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-25,-7,-14, -3,0.5,-17, -25,-8,-14);
   glVertex3d(-3,+0.5,-17);
   glVertex3d(-25,-7,-14);
   glVertex3d(-25,-8,-14);
   glVertex3d(-3,-0.5,-17);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-25,-7,-14, -25,-8,-14, -25,-7,-10);
   glVertex3d(-25,-8,-14);
   glVertex3d(-25,-7,-14);
   glVertex3d(-25,-7,-10);
   glVertex3d(-25,-8,-10);
   glEnd();

   // Bottom Left Engine and Laser
   wingEngine(-5.4, -2.3, -13);
   wingLaser(-24, -8, -16);

   // ------------------- Right Top Wing -----------------------------------
   glBegin(GL_POLYGON);
   normal(3,0.5,-17, 3,0.5,-7, 25,8,-14);
   glVertex3d(+3,+0.5,-7);
   glVertex3d(+3,+0.5,-17);
   glVertex3d(+25,+8,-14);
   glVertex3d(+25,+8,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(3,1.5,-17, 3,1.5,-7, 25,9,-14);
   glVertex3d(+3,+1.5,-7);
   glVertex3d(+3,+1.5,-17);
   glVertex3d(+25,+9,-14);
   glVertex3d(+25,+9,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(25,8,-10, 3,0.5,-7, 25,9,-10);
   glVertex3d(+3,+0.5,-7);
   glVertex3d(+25,+8,-10);
   glVertex3d(+25,+9,-10);
   glVertex3d(+3,+1.5,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(+25,8,-14, +3,0.5,-17, +25,9,-14);
   glVertex3d(-+3,+0.5,-17);
   glVertex3d(+25,+8,-14);
   glVertex3d(+25,+9,-14);
   glVertex3d(+3,+1.5,-17);
   glEnd();

   glBegin(GL_POLYGON);
   normal(25,8,-14, 25,9,-14, 25,8,-10);
   glVertex3d(+25,+9,-14);
   glVertex3d(+25,+8,-14);
   glVertex3d(+25,+8,-10);
   glVertex3d(+25,+9,-10);
   glEnd();

   // Upper Right Engine and Laser
   wingEngine(5.4, 3.75, -13);
   wingLaser(24, 9, -16);

   // ------------------- Right Bottom Wing -----------------------------------
   glBegin(GL_POLYGON);
   normal(3,0.5,-17, 3,0.5,-7, 25,-7,-14);
   glVertex3d(+3,+0.5,-7);
   glVertex3d(+3,+0.5,-17);
   glVertex3d(+25,-7,-14);
   glVertex3d(+25,-7,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(3,-0.5,-17, 3,-0.5,-7, 25,-8,-14);
   glVertex3d(+3,-0.5,-7);
   glVertex3d(+3,-0.5,-17);
   glVertex3d(+25,-8,-14);
   glVertex3d(+25,-8,-10);
   glEnd();

   glBegin(GL_POLYGON);
   normal(25,-7,-10, 3,0.5,-7, 25,-8,-10);
   glVertex3d(+3,+0.5,-7);
   glVertex3d(+25,-7,-10);
   glVertex3d(+25,-8,-10);
   glVertex3d(+3,-0.5,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(25,-7,-14, 3,0.5,-17, 25,-8,-14);
   glVertex3d(+3,+0.5,-17);
   glVertex3d(+25,-7,-14);
   glVertex3d(+25,-8,-14);
   glVertex3d(+3,-0.5,-17);
   glEnd();

   glBegin(GL_POLYGON);
   normal(25,-7,-14, 25,-8,-14, 25,-7,-10);
   glVertex3d(+25,-8,-14);
   glVertex3d(+25,-7,-14);
   glVertex3d(+25,-7,-10);
   glVertex3d(+25,-8,-10);
   glEnd();

   // Bottom Right Engine and Laser
   wingEngine(5.4, -2.3, -13);
   wingLaser(24, -8, -16);
}

/*
 *  Draw an x-wing
 *     
 */
static void xWing(double x, double y, double z,
                 double dx, double dy, double dz,
                 double rx, double ry, double rz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glRotated(th,rx,ry,rz);

   // Nose left top panel
   glBegin(GL_POLYGON);
   glColor3f(0.75,0.75,0.75);
   normal(-2,0,25, -5,0,-3, -1,1.5,30);
   glVertex3d(-5,+0,-3);
   glVertex3d(-2,+0,+30);
   glVertex3d(-1,+1.5,+30);
   glVertex3d(-2,+2.5,+7);
   glVertex3d(-3,+5,+0);
   glVertex3d(-3,+5,-2);
   glVertex3d(-3,+4,-3);
   glEnd();

   // Nose right top panel
   glBegin(GL_POLYGON);
   normal(2,0,25, 5,0,-3, 1,1.5,30);
   glVertex3d(+5,+0,-3);
   glVertex3d(+2,+0,+30);
   glVertex3d(+1,+1.5,+30);
   glVertex3d(+2,+2.5,+7);
   glVertex3d(+3,+5,+0);
   glVertex3d(+3,+5,-2);
   glVertex3d(+3,+4,-3);
   glEnd();

   // Cockpit window panel
   glBegin(GL_POLYGON);
   normal(-3,5,0, 3,5,0, -2,2.5,7);
   glVertex3d(+3,+5,+0);
   glVertex3d(-3,+5,+0);
   glVertex3d(-2,+2.5,+7);
   glVertex3d(+2,+2.5,+7);
   glEnd();

   // Cockpit top panel
   glBegin(GL_POLYGON);
   normal(3,5,0, -3,5,0, -3,+5,-2);
   glVertex3d(+3,+5,+0);
   glVertex3d(-3,+5,+0);
   glVertex3d(-3,+5,-2);
   glVertex3d(+3,+5,-2);
   glEnd();

   // Cockpit back panel
   glBegin(GL_POLYGON);
   normal(-3,5,-2, 3,5,-2, -3,4,-3);
   glVertex3d(+3,+5,-2);
   glVertex3d(-3,+5,-2);
   glVertex3d(-3,+4,-3);
   glVertex3d(+3,+4,-3);
   glEnd();

   // ---------------------------- Nose panels  ---------------------------
   // Nose top panel
   glBegin(GL_POLYGON);
   normal(2,2.5,7, -2,2.5,7, 1,1.5,30);
   glVertex3d(-2,+2.5,+7);
   glVertex3d(+2,+2.5,+7);
   glVertex3d(+1,+1.5,+30);
   glVertex3d(-1,+1.5,+30);
   glEnd();

   // Nose left bottom panel
   glBegin(GL_POLYGON);
   normal(-2,0,30, -5,0,-3, -1,-1,30);
   glVertex3d(-5,+0,-3);
   glVertex3d(-2,+0,+30);
   glVertex3d(-1,-1,+30);
   glVertex3d(-3,-3,-3);
   glEnd();

   // Nose right bottom panel
   glBegin(GL_POLYGON);
   normal(2,0,30, 5,0,-3, +1,-1,30);
   glVertex3d(+5,+0,-3);
   glVertex3d(+2,+0,+30);
   glVertex3d(+1,-1,+30);
   glVertex3d(+3,-3,-3);
   glEnd();

   // Nose bottom panel
   glBegin(GL_POLYGON);
   normal(3,-3,-3, -3,-3,-3, 1,-1,30);
   glVertex3d(-3,-3,-3);
   glVertex3d(+3,-3,-3);
   glVertex3d(+1,-1,+30);
   glVertex3d(-1,-1,+30);
   glEnd();

   // Nose tip panel
   glBegin(GL_POLYGON);
   normal(2,0,30, 1,-1,30, 1,1.5,30);
   glVertex3d(+1,-1,+30);
   glVertex3d(+2,+0,+30);
   glVertex3d(+1,+1.5,+30);
   glVertex3d(-1,+1.5,+30);
   glVertex3d(-2,+0,+30);
   glVertex3d(-1,-1,+30);
   glEnd();

   // --------------- Body top and bottom panels -----------------------
   glBegin(GL_POLYGON);
   normal(3,4,-3, -3,4,-3, 4,4,-4);
   glVertex3d(-3,+4,-3);  
   glVertex3d(+3,+4,-3);
   glVertex3d(+4,+4,-4);
   glVertex3d(+4,+4,-19);
   glVertex3d(-4,+4,-19);
   glVertex3d(-4,+4,-4);
   glEnd();

   glBegin(GL_POLYGON);
   normal(3,-3,-3, -3,-3,-3, 3,-3,-4);
   glVertex3d(-3,-3,-3);  
   glVertex3d(+3,-3,-3);
   glVertex3d(+3,-3,-4);
   glVertex3d(+3,-3,-19);
   glVertex3d(-3,-3,-19);
   glVertex3d(-3,-3,-4);
   glEnd();

   // --------------- Body left panels - behind cockpit to wings -------
   glBegin(GL_POLYGON);
   normal(-4,4,-4, -3,4,-3, -5,0,-4);
   glVertex3d(-3,+4,-3);
   glVertex3d(-4,+4,-4);
   glVertex3d(-5,+0,-4);
   glVertex3d(-5,+0,-3);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-5,0,-4, -4,4,-4, -5,0,-7);
   glVertex3d(-4,+4,-4);
   glVertex3d(-5,+0,-4);
   glVertex3d(-5,+0,-7);
   glVertex3d(-4,+4,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-5,0,-7, -5,0,-3, -3,-3,-7);
   glVertex3d(-5,+0,-3);
   glVertex3d(-5,+0,-7);
   glVertex3d(-3,-3,-7);
   glVertex3d(-3,-3,-3);
   glEnd();

   // --------------- Body right panels - behind cockpit to wings -------
   glBegin(GL_POLYGON);
   normal(+4,4,-4, +3,4,-3, +5,0,-4);
   glVertex3d(+3,+4,-3);
   glVertex3d(+4,+4,-4);
   glVertex3d(+5,+0,-4);
   glVertex3d(+5,+0,-3);
   glEnd();

   glBegin(GL_POLYGON);
   normal(+5,0,-4, +4,4,-4, +5,0,-7);
   glVertex3d(+4,+4,-4);
   glVertex3d(+5,+0,-4);
   glVertex3d(+5,+0,-7);
   glVertex3d(+4,+4,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(+5,0,-7, +5,0,-3, +3,-3,-7);
   glVertex3d(+5,+0,-3);
   glVertex3d(+5,+0,-7);
   glVertex3d(+3,-3,-7);
   glVertex3d(+3,-3,-3);
   glEnd();

   // ---------------- Body inner panels - to hide internals ------------
   glBegin(GL_POLYGON);
   normal(5,0,-7, 4,4,-7, 3,-3,-7);
   glVertex3d(+4,+4,-7);
   glVertex3d(+5,+0,-7);
   glVertex3d(+3,-3,-7);
   glVertex3d(-3,-3,-7);
   glVertex3d(-5,+0,-7);
   glVertex3d(-4,+4,-7);
   glEnd();

   glBegin(GL_POLYGON);
   normal(3,-3,-17, 4,4,-17, 3,-3,-19);
   glVertex3d(+4,+4,-17);
   glVertex3d(+3,-3,-17);
   glVertex3d(+3,-3,-19);
   glVertex3d(+4,+4,-19);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-3,-3,-17, -4,4,-17, -3,-3,-19);
   glVertex3d(-4,+4,-17);
   glVertex3d(-3,-3,-17);
   glVertex3d(-3,-3,-19);
   glVertex3d(-4,+4,-19);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-3,4,-17, -3,4,0, -3,-3,-17);
   glVertex3d(-3,+4,+0);
   glVertex3d(-3,+4,-17);
   glVertex3d(-3,-3,-17);
   glVertex3d(-3,-3,+0);
   glEnd();

   glBegin(GL_POLYGON);
   normal(+3,4,-17, +3,4,0, +3,-3,-17);
   glVertex3d(+3,+4,+0);
   glVertex3d(+3,+4,-17);
   glVertex3d(+3,-3,-17);
   glVertex3d(+3,-3,+0);
   glEnd();

   // ------------------- Body back panels --------------------------------
   glBegin(GL_POLYGON);
   normal(-3,-3,-19, -4,4,-19, 3,-3,-19);
   glVertex3d(-4,+4,-19);
   glVertex3d(-3,-3,-19);
   glVertex3d(+3,-3,-19);
   glVertex3d(+4,+4,-19);
   glEnd();

   glBegin(GL_POLYGON);
   normal(-3,-3,-17, -4,4,-17, 3,-3,-17);
   glVertex3d(-4,+4,-17);
   glVertex3d(-3,-3,-17);
   glVertex3d(+3,-3,-17);
   glVertex3d(+4,+4,-17);
   glEnd();

   // Build wings
   createWings();

   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   
   //  Perspective - set eye position
   if (mode)
   {
      double Ex2 = -2*dim*Sin(th)*Cos(ph);
      double Ey2 = +2*dim        *Sin(ph);
      double Ez2 = +2*dim*Cos(th)*Cos(ph);
      if ( strafe ){
         // Change vision point based on change in eye
         Vx += Ex2 - Ex;
         Vx += Ex2 - Ex;
         Vx += Ex2 - Ex;
      }

      // Set eye variables
      Ex = Ex2;
      Ey = Ey2;
      Ez = Ez2;

      // Call gluLookAt
      gluLookAt(Ex,Ey,Ez, Vx,Vy,Vz , 0,Cos(ph),0);
   }

   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        
        //  Enable lighting
        glEnable(GL_LIGHTING);
        
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        
        //  Enable light 0
        glEnable(GL_LIGHT0);
        
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   // Create XWING
   xWing(0,0,0, 1,1,1, 0,0,0, 0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("p/P: View Mode | Mouse LButton: Rotate Camera | Mouse RButton: Fwd/Bkwd and R/L Strafe");

   //  Display parameters
   glWindowPos2i(5,25);
   Print("f/F: Frozen/Falling Snow | PgUp: Move Fwd | PdDn: Move Bkwd | Arrows: Rotate Camera");
   
   //  Display parameters
   glWindowPos2i(5,45);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");

   // If light is on display lighting
   if (light)
   {
      glWindowPos2i(5,65);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,85);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  Idle function for falling snow
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim -= 0.1;
   
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim += 0.1;
   
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   
   //  Update projection
   Project(mode?fov:0,asp,dim);
   
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;

   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the mouse button pressed
 */
void mouseButton (int button, int state, int x, int y) {
   xOrigin = x; 
   yOrigin = y;
   if( button == GLUT_LEFT_BUTTON){
      m_button = 0;
   }
   if( button == GLUT_RIGHT_BUTTON){
      m_button = 1;
   }
}

/*
 *  GLUT calls this routine when the mouse moves
 */
void mouseMove(int x, int y) {
   if ( m_button == 0){
      //  Sideways angle view
      strafe = 0;
      th += (xOrigin - x)/2;
      th %= 360;
      xOrigin = x;

      // Up and down angle view
      ph += (y - yOrigin)/2;
      ph %= 360;
      yOrigin = y;
   }
   if ( m_button == 1){
      //  Right and left movement changes
      strafe = 1;
      th += (xOrigin - x)/2;
      th %= 360;
      xOrigin = x;

      // Forward and backward movement
      if ( mode ){
         fov += (y - yOrigin)/2;
      }
      else{
         dim += (y - yOrigin)/2;
      }
      yOrigin = y;
   }
   //  Reproject
   Project(mode?fov:0,asp,dim);

   //  Redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(800,800);
   glutCreateWindow("Christopher Jordan Lab5: Snow with Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   
   // Set keyboard functions
   glutSpecialFunc(special);
   glutKeyboardFunc(key);

   // Set idle functions
   glutIdleFunc(idle);

   //  Mouse handler functions
   glutMouseFunc(mouseButton);
   glutMotionFunc(mouseMove);

   // Set clock
   ot = clock();

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
