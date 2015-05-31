/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include<vector>
#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<time.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include<math.h>
#define BLACK 0, 0, 0
#include<math.h>
#define pi 2*acos(0)

using namespace std;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int n;
int scale =  6;
int len =1;
struct point
{
	double x,y;
	double z =0;
};
std::vector<int> val;

point myPoint[4];

point p[10000];

int  turn(point p, point r, point q)
{
    int val =  (q.y- p.y) * (r.x - q.x) - (r.y -q.y) * (q.x- p.x);

    return val;

}

void convex_hull()
{
    if(n <3)
        return;
    int l = 0;
    for(int i=0; i<n;i++)
    {
        if(p[i].x < p[l].x )
            l = i;
    }
    val.push_back(l);
    int s = l;
    while(1)
    {

        int q = (s+1)%n;

        for(int i=0; i<n; i++)
        {
            if( turn(p[s], p[i], p[q]) >0)
                q = i;

        }
        if(q == l)
            break;
        val.push_back(q);
        s = q;


    }


}



double MaxArea()
{
    double max=0;
    for(int i =0; i< val.size(); i++ )
    {
        for( int  j = (i+2) %val.size();  j !=  (i-1+val.size())%val.size() ; j = (j+1)%val.size() )
        {

            double max1 =0;
            double max2 =0;
            for(int k = (j+1)% val.size();  k!=i ; k = (k+1)% val.size() )
            {
                double temp = .5 * ( ( p[val[i]].x * ( p[val[j] ].y - p[ val[k] ].y   ) ) +
                                    ( p[val[j]].x * ( p[val[k] ].y - p[ val[i] ].y   ) ) +
                                 ( p[val[k]].x * ( p[val[i] ].y - p[ val[j] ].y   ) )
                                 );
                if( abs(temp) > max1)
                    max1 = abs(temp);


            }

              for(int k = (j-1 + val.size() )%val.size();  k !=i; k = (k-1+val.size()) % val.size() )
            {

                double temp = .5 * ( ( p[val[i]].x * ( p[val[j] ].y - p[ val[k] ].y   ) ) +
                                    ( p[val[j]].x * ( p[val[k] ].y - p[ val[i] ].y   ) ) +
                                 ( p[val[k]].x * ( p[val[i] ].y - p[ val[j] ].y   ) )
                                 );
                if( abs(temp) > max2)
                    max2 = abs(temp);

            }

            if( (max1+ max2) > max )
                max = max1 + max2;

        }
    }
    return max;
}
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(255.0, 0.0, 0.0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			glColor3f(0.0, 255.0, 0.0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			glColor3f(0.0, 0.0, 255.0);
	//		glVertex3f(0,0, 100);
	//		glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void drawSquare(float a)
{
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void drawss()
{
	glPushMatrix();{
		glRotatef(angle,0,0,1);
		glTranslatef(75,0,0);
		glRotatef(2*angle,0,0,1);


		glPushMatrix();{
			glRotatef(angle,0,0,1);
			glTranslatef(25,0,0);
			glRotatef(3*angle,0,0,1);
			glColor3f(0.0, 0.0, 1.0);
			drawSquare(5);

		}glPopMatrix();



		glColor3f(1.0, 0.0, 0.0);
		drawSquare(10.0);
	}glPopMatrix();

}

//draws half sphere
void drawsphere(float radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=sqrt(radius*radius-h*h);
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}

	}
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}

	}
}


void drawPoints()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int i=0; i<n;i++)
    {

        glPushMatrix();{

                glTranslatef(p[i].x * scale, p[i].y * scale,0);
         drawsphere(2,20,20);

       }glPopMatrix();

    }

}

void drawHull()
{
    int temp = val.size();
    for(int i=0; i<=val.size();i++)
    {
        glColor3f(255,215,0);
        glBegin(GL_LINES);              // Each set of 4 vertices form a quad

                glVertex2f(p[val[i%temp]].x * scale, p[val[i%temp]].y * scale );
                 glVertex2f(p[val[(i+1)%temp ]].x * scale, p[val[(i+1)%temp]].y * scale);

        glEnd();

    }
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();


	gluLookAt(0,0.1,cameraHeight,	0,0,0,	0,0,-1);
	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);


	glMatrixMode(GL_MODELVIEW);


//	drawAxes();
	drawGrid();
	glColor3f(1,0,0);
	drawPoints();
	drawHull();


	glutSwapBuffers();
}

void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){

  //  freopen("input.txt","r",stdin);
   // freopen("output.txt","w",stdout);

/*
    std::cin>>n;
    for(int i=0; i<n;i++)
    {
        std::cin>>p[i].x>>p[i].y;
      //  scanf("%d %d",p[i].x,p[i].y);

    }
    */
    std::cin>>n;
    if(n<3)
        return 0;

    for(int i=0; i<n;i++)
    {
        int posx = rand()%2;
        int posy = rand() %2;
        if(posx == 0)
            posx = -1;
        if(posy == 0)
            posy = -1;




        p[i].x = posx * rand() %20;
        p[i].y =  posy* rand() %20;
    }



  /*  for(int i=0; i<n; i++)
        cin>>p[i].x>>p[i].y;

*/
    convex_hull();

    for(int i=0; i<val.size();i++)
        std::cout<<p[ val[i] ].x<<" "<< p[ val[i] ].y<<std::endl;

        cout<<"MAX AREA: "<<MaxArea()<<endl;



	glutInit(&argc,argv);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
