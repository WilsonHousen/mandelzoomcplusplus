/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics      Wilson Housen */
/*--------------------------------------------------------*/
/*  MandelZoom                                            */
/*--------------------------------------------------------*/

#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <cfloat>
#include <iostream>

using namespace std;

// Defining default values for window size, shape and location.
#define INITIAL_WIN_W 800
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 150
#define INITIAL_WIN_Y 50

#define INIT_complexXONE -2.0
#define INIT_complexXTWO 0.5
#define INIT_complexYONE -1.25
#define INIT_complexYTWO 1.25

double complexXONE = INIT_complexXONE;
double complexXTWO = INIT_complexXTWO; 
double complexYONE = INIT_complexYONE;
double complexYTWO = INIT_complexYTWO;

int*** tableOfTables;

int** iterTable;

int globalPos;

void initTables(int w, int h) {
iterTable = new int*[w];
for (int i = 0; i < w; i++) iterTable[i] = new int[h];
}

void initTableSuper(int i) {
	tableOfTables = new int**[i];
}

//void deleteTables(numRows)
//{
//	for (int i = 0; i < numRows; i++) delete table[i];
//	delete table;
//}

//struct rectangle
//{
//	double xmin;
//	double ymin;
//	double xmax;
//	double ymax;
//	rectangle(double xmn, double ymn, double xmx, double ymx)
//		: xmin(xmn), ymin(ymn), xmax(xmx), ymax(ymx)
//	{}
//};

//list<rectangle*>  rectList;
//
//list<rectangle*>::iterator  rectListIter;

// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight, windowWidth;

void clearPicture()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

//void drawLine(int xOld, int yOld, int xNew, int yNew )
//// Draw a line from (xOld,yOld) to (xNew,yNew).
//{
//  glBegin( GL_LINES );
//  glVertex2i( xOld, yOld );
//  glVertex2i( xNew, yNew );
//  glEnd();
//  glFlush();
//}

//int mandelSNum(double x1, double x2, double y1, double y2, int w, int h, int u, int v) {
//
//	int s;
//	int leftSide;
//	int rightSide;
//
//	leftSide = (x1 + (u * ((x2 - x1) / (double) (w - 1))));
//	rightSide = (y1 + (v * ((y2 - y1) / (double) (h - 1))));
//	s = leftSide + rightSide;
//	return s;
//
//}

double mandelLeftSum(double x1, double x2, int w, int u) {

	//calculates the x side of the s sum
	double leftSide;
	//double DW = (double) w;
	//double DU = (double) u;
	leftSide = x1 + u * ((x2 - x1) /(double) (w - 1));
	return leftSide;

}

double mandelRightSum(double y1, double y2, int h, int v) {

	// calculates the y side of the s sum
	double rightSide;
	//double DH = (double) h;
	//double DV = (double) v;
	rightSide = y1 + v * ((y2 - y1) /(double) (h - 1));
	return rightSide;

}

double zFuncLeft(double x, double y, double sL) {

	//calculates the x cartesian coordinate for the complex number
	double sqrLeft = ((x*x) - (y*y));
	double addLeft = sqrLeft + sL;
	return addLeft;

}

double zFuncRight(double x, double y, double sR) {

	// calculates the y i cartesian coordinate for the complex number
	double sqrRight = ((2.0 * x)*y);
	double addRight = sqrRight + sR;
	return addRight;

}

int iterMandelBrot(double x1, double x2, double y1, double y2, int w, int h, int u, int v) {

	// calculates the mandelbrot-y-ness of the coordinates with 700 iterations

	double currZR = 0.0;
	double currZL = 0.0;
	double currSL = mandelLeftSum(x1, x2, w, u);
	double currSR = mandelRightSum(y1, y2, h, v);
	double currAbsZ = 0.0;
	int i = 1;

	while (currAbsZ < 4.0 && i < 1000) {

		double tempZL = zFuncLeft(currZL, currZR, currSL);
		double tempZR = zFuncRight(currZL, currZR, currSR);
		currAbsZ = (tempZL * tempZL) + (tempZR * tempZR);
		currZL = tempZL;
		currZR = tempZR;
		i++;

	}

	return i;

}

void calcMandelPixels(double x1, double x2, double y1, double y2, int w, int h) {

	//rectangle tempRect = new rectangle(0.0, 0.0, w, h);

	for (int r = 0; r < w; r++) {

		for (int e = 0; e < h; e++) {

			iterTable[r][e] = iterMandelBrot(x1, x2, y1, y2, w, h, r, e);
			//cout << iterTable[r][e] << endl; 


		}


	}

	tableOfTables[globalPos] = iterTable;

}

void drawMandelBrot() {

	//clear image, draw new mandelbrot
	//based on rectangle at current position in rectlist
	cout << complexXONE << endl;
	cout << complexXTWO << endl;
	cout << complexYONE << endl;
	cout << complexYTWO << endl;
	calcMandelPixels(complexXONE, complexXTWO, complexYONE, complexYTWO, windowWidth, windowHeight);
	glBegin( GL_POINTS );

	int** localTable = tableOfTables[globalPos];

	for (int x = 0; x < windowWidth; x++) {

		for (int y = 0; y < windowHeight; y++) {

			if (localTable[x][y] == 1000) {

				glColor3f( 0.0, 0.0, 0.0 );
				glVertex2i(x, y);

			}

			if (localTable[x][y] < 1000 && localTable[x][y] > 800) {

				glColor3f(0.7, 0.1, 0.7);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 800 && localTable[x][y] > 600) {

				glColor3f(0.9, 0.0, 0.3);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 600 && localTable[x][y] > 400) {

				glColor3f(0.2, 0.0, 0.9);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 400  && localTable[x][y] > 100) {

				glColor3f(0.2, 0.1, 0.6);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 100 && localTable[x][y] > 50) {

				glColor3f(0.2, 0.0, 0.2);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 50 && localTable[x][y] > 4) {

				glColor3f(0.5, 0.0, 0.5);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 4 && localTable[x][y] > 3) {

				glColor3f(0.5, 0.0, 0.1);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 3 && localTable[x][y] > 2) {

				glColor3f(0.9, 0.3, 0.9);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 2 && localTable[x][y] > 1) {

				glColor3f(0.9, 0.0, 0.6);
				glVertex2i(x, y);

			}

			if (localTable[x][y] <= 1 && localTable[x][y] >= 0) {

				glColor3f(0.7, 0.0, 0.1);
				glVertex2i(x, y);

			}

		}

	}

	glEnd();
	glFlush();


};

//void drRB(int oldx, int newx, int oldy, int newy) {
//
//	glColor3f(1.0, 1.0, 1.0);
//	glEnable(GL_COLOR_LOGIC_OP);
//	glLogicOp(GL_XOR);
//	glBegin(GL_LINE_LOOP);
//
//	glVertex2i(oldx, oldy);
//	glVertex2i(newx, oldy);
//	glVertex2i(newx, newy);
//	glVertex2i(oldx, newy);
//
//	glEnd();
//	glDisable(GL_COLOR_LOGIC_OP);
//	glFlush();
//
//}

void drawRubberBand(int xA, int yA, int xS, int yS)
{
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    glBegin( GL_LINE_LOOP );
    glVertex2i( xA, yA );
	glVertex2i( xS, yA );
    glVertex2i( xS, yS );
	glVertex2i( xA, yS );
    glEnd();
    glDisable(GL_COLOR_LOGIC_OP);
    glFlush();
}

void rubberBand( int x, int y )
// Callback for processing mouse motion.
{
  if ( rubberBanding )
  {
    drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
    y = windowHeight - y;
    xStretch = x;
    yStretch = y;
    drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
    glFlush();
  }
}

  void reshape( int w, int h )
  // Callback for processing reshape events.
  {
    windowWidth = w;
    windowHeight = h;
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, w, 0.0, h );
  }


  void escExit( GLubyte key, int, int )
  // Callback for processing keyboard events.
  {
    if ( key == 27 /* ESC */ ) std::exit( 0 );
  }


  void mainMenu( int item )
  // Callback for processing main menu.
  {
    switch ( item )
    {
      case 1: clearPicture(); break;
	  case 2: std::exit( 0 );
      case 3: std::exit( 0 );
    }
  }



  void setMenus()
  // Function for creating menus.
  {
    glutCreateMenu( mainMenu );
    glutAddMenuEntry( "Push (currently clear)", 1 );
	glutAddMenuEntry( "Pop (currently exit)", 2 );
    glutAddMenuEntry( "Exit", 3 );
    glutAttachMenu( GLUT_MIDDLE_BUTTON );
  }


  void processLeftDown( int x, int y )
  // Function for processing mouse left botton down events.
  {
    if (!rubberBanding)
    {
      int xNew = x;
      int yNew = windowHeight - y;
      xAnchor = xNew;
      yAnchor = yNew;
      xStretch = xNew;
      yStretch = yNew;
      drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
      rubberBanding = true;
    }
  }


  void processLeftUp( int x, int y )
  // Function for processing mouse left botton up events.
  {
    if (rubberBanding)
    {
      int xNew, yNew;
	  /*int ar, aw;
	  double xOld, yOld, Xn, Yn;*/
	  double tempx1, tempx2, tempy1, tempy2;


      drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
      rubberBanding = false;
      xNew = x;
      yNew = windowHeight - y;
	  drawRubberBand(xAnchor, yAnchor, xNew, yNew);
      glFlush();

	  //xOld = complexXTWO - complexXONE;
	  //yOld = complexYTWO - complexYONE;
	  //aw = yOld / xOld;

	  cout << xAnchor << endl;
	  cout << xStretch << endl;
	  cout << yAnchor << endl;
	  cout << yStretch << endl;
	  tempx1 = mandelLeftSum(complexXONE, complexXTWO, windowWidth, xAnchor);
	  cout << complexXONE << endl;
	  tempy1 = mandelRightSum(complexYONE, complexYTWO, windowHeight, yAnchor);
	  cout << complexYONE << endl;
	  tempx2 = mandelLeftSum(complexXTWO, complexXONE, windowWidth, xNew);
	  cout << complexXTWO << endl;
	  tempy2 = mandelRightSum(complexYTWO, complexYONE, windowHeight, yNew);
	  cout << complexYTWO << endl;

	  complexXONE = tempx1;
	  complexXTWO = tempx2;
	  complexYTWO = tempy2;
	  complexYONE = tempy1;

	  //Xn = complexXTWO - complexXONE;
	  //Yn = complexYTWO - complexYONE;
	  //ar = Yn / Xn;

	/*  if (ar > aw) {

		  complexXONE = complexXONE;
		  complexXTWO = complexXTWO;

	  }

	  if (ar > aw) {

		  complexXONE = complexYONE;
		  complexXTWO = complexYTWO;

	  }*/


	  globalPos++;
    }

	//clearPicture();
	//drawMandelBrot();
	glutPostRedisplay();
  }


  void mouse( int button, int state, int x, int y )
  // Function for processing mouse events.
  {
    if ( button == GLUT_LEFT_BUTTON )
      switch ( state )
      {
        case GLUT_DOWN: processLeftDown( x, y ); break;
        case GLUT_UP: processLeftUp( x, y ); break;
      }
  }


  int main( int argc, char * argv[] )
  {
    // Mask floating point exceptions.
    _control87( MCW_EM, MCW_EM );

    // Initialize glut with command line parameters.
    glutInit( & argc, argv );

    // Choose RGB display mode for normal screen window.
    glutInitDisplayMode( GLUT_RGB );

    // Set initial window size, position, and title.
    glutInitWindowSize( INITIAL_WIN_W, INITIAL_WIN_H );
    glutInitWindowPosition( INITIAL_WIN_X, INITIAL_WIN_Y );
    windowWidth = INITIAL_WIN_W;
    windowHeight = INITIAL_WIN_H;
    glutCreateWindow( "Mandel Zoom Temp Demo" );

    // You don't (yet) want to know what this does.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, ( double )INITIAL_WIN_W, 0.0, ( double )INITIAL_WIN_H ), glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.375, 0.375, 0.0 );

	globalPos = 0;
	initTableSuper(20);
	initTables(INITIAL_WIN_W, INITIAL_WIN_H);

    // Set the callbacks for the normal screen window.
    glutDisplayFunc( drawMandelBrot );
    glutMouseFunc( mouse );
	glutReshapeFunc(reshape);
    glutKeyboardFunc( escExit );
    glutMotionFunc( rubberBand );
    glutPassiveMotionFunc( rubberBand );

    // Set the drawing color.
    //glColor3f(0.5,0.5,0.5);

    // Set the color for clearing the window.
     glClearColor(0.0, 0.0, 0.0, 1.0);

    // Set up the menus.
    setMenus();

    glutMainLoop();
    return 0;
  }


