#include <iostream>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <vector>
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

using namespace std;

int  move = 0;
int  look = 0;

int angle;
float rotAng = 0;
float tileX = -0.68;
float tileZ = 0;
float attackersX = 4;
int multipleBullets = 0;
int pause = 0;
int GameOver = 0;
int cinematicView = 0;

float CVradius = 4;
float CVangle = 0;

int TotalResource = 15;
int chooseRow;
int chooseColumn;
int chooseCharacter;
int chooseSteps;

float attackerTimer = 5;

vector<float> attackingLanes = {0,-0.5,-1,-1.5,-2};

void DrawString (std::string s, float x, float y)
{
    unsigned int i;
    glRasterPos2f(x, y);
    
    for (i = 0; i < s.length(); i++)
        glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
}

/*---------------------------------------- Characters --------------------------------*/
class ResourceGatherer
{
private:
    float x;
    float z;
    int price;
public:
    int resources;

    ResourceGatherer () {x = 0; z = 0; price = 5;}
    float getX(){return x;}
    float getZ(){return z;}
    int getPrice(){return price;}
    void setX(float newX)
    {
        x = newX;
    }
    void setZ(float newZ)
    {
        z = newZ;
    }
    void drawResourceGatherer()
    {
        
        if (pause == 0) {
            resources++;
        }
        if (resources % 5 == 0) {
            TotalResource++;
        }

        glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x, 0.4, z);
            glScaled(0.08, 0.08, 0.08);
            glRotated(-rotAng, 0, 1, 0);
            
            for(angle = 0; angle < 3000; angle ++)
            {
                glPushMatrix();
                
                    glTranslatef(0  ,angle*0.002 , 0);
                    glRotated(angle, 0, 1, 0);
                    glTranslatef(1 ,-4 , 0);
                    glutSolidSphere(0.09, 5, 5);
                
                glPopMatrix();
            }
            
            
            glPushMatrix();
                glTranslatef(0, 3, 0);
                glutSolidDodecahedron();
            glPopMatrix();
        
        glPopMatrix();
    }

};

vector<ResourceGatherer> PlaceResourceGatherer;

class Bullet {

public:
    float x;
    float z;
    Bullet(float xx, float zz){ x = xx; z = zz;}
    void drawBullet()
    {
        if (pause == 0)
        {
            x += 0.1;
        }
        glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(x, 0.6, z);
        glScalef(0.03, 0.03, 0.03);
        
        glutSolidSphere(1, 5, 5);
        glPopMatrix();
    }
    
};
class Defender
{
private:
    float x;
    float z;
    int price;
public:
    
    Defender (){x = 0; z = 0; price = 10;}
    float getX()
    { return x; }
    float getZ()
    { return z;}
   
    int getPrice()
    { return price;}
    
    void setX(float newX)
    { x = newX;}
    void setZ(float newZ)
    { z = newZ;}
    
  
    
    void drawDefender()
    {
        glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x, 0.6, z);
            glRotated(90, 0, 1, 0);
            glScalef(0.8, 0.8, 0.8);
            glPushMatrix();
                glScalef(0.2, 0.2, 0.2);
                glutSolidSphere(0.7, 35, 35);
            glPopMatrix();
            
            glPushMatrix();
                glScalef(0.5, 0.5, 0.5);
                GLUquadricObj * qobj;
                qobj = gluNewQuadric();
                gluQuadricDrawStyle(qobj,GLU_LINE);
                gluCylinder(qobj, 0.2, 0.2, 0.4, 500,500);
            glPopMatrix();
            
            glPushMatrix();
                glScalef(0.5, 0.5, 0.5);
                glRotated(90, 1, 0, 0);
                GLUquadricObj * qobj2;
                qobj2 = gluNewQuadric();
                gluQuadricDrawStyle(qobj2,GLU_LINE);
                gluCylinder(qobj2, 0.2, 0.2, 1.5, 500,500);
            glPopMatrix();
        glPopMatrix();
        
    }
    
    void drawDefenderBullets()
    {
        drawDefender();
        Bullet b = *new Bullet(x,z);
        b.drawBullet();
        
    }

};

vector<Defender> PlaceDefenders;

class Attacker
{
    
    public:
    int health;
    float x;
    float z;

    Attacker () { x = 4; health = 100; }
    void drawAttacker ()
    {
        glScaled(0.7, 0.7, 0.7);
        glColor3f(0.4, 0.4, 0.4);
        glRotated(-90, 0, 1, 0);
        glTranslatef(0, 0.51, 0);
        glPushMatrix();
        glScaled(0.5, 0.5, 0.5);
        glutSolidCube(1);
        glPopMatrix();
        
        glPushMatrix();
        glScaled(0.13, 0.13, 0.13);
        glTranslatef(0, 3, 0);
        glutSolidDodecahedron();
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.15, 0, 0.23);
        glScaled(0.1, 0.1, 0.1);
        glutSolidCone(0.5, 1, 35, 35);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0, 0, 0.23);
        glScaled(0.1, 0.1, 0.1);
        glutSolidCone(0.5, 1, 35, 35);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.15, 0, 0.23);
        glScaled(0.1, 0.1, 0.1);
        glutSolidCone(0.5, 1, 35, 35);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.24, -0.3, 0.149);
        glScalef(0.5, 0.5, 0.5);
        glRotated(90, 0, 1, 0);
        GLUquadricObj * q1;
        q1 = gluNewQuadric();
        gluQuadricDrawStyle(q1,GLU_LINE);
        gluCylinder(q1, 0.2, 0.2, 0.2, 500,500);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.24, -0.3, -0.2);
        glScalef(0.5, 0.5, 0.5);
        glRotated(90, 0, 1, 0);
        GLUquadricObj * q2;
        q2 = gluNewQuadric();
        gluQuadricDrawStyle(q2,GLU_LINE);
        gluCylinder(q2, 0.2, 0.2, 0.2, 500,500);
        glPopMatrix();
        
        
        glPushMatrix();
        glTranslatef(-0.24, -0.3, 0.149);
        glScalef(0.5, 0.5, 0.5);
        glRotated(90, 0, 1, 0);
        GLUquadricObj * q3;
        q3 = gluNewQuadric();
        gluQuadricDrawStyle(q3,GLU_LINE);
        gluCylinder(q3, 0.2, 0.2, 0.2, 500,500);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.24, -0.3, -0.2);
        glScalef(0.5, 0.5, 0.5);
        glRotated(90, 0, 1, 0);
        GLUquadricObj * q4;
        q4 = gluNewQuadric();
        gluQuadricDrawStyle(q4,GLU_LINE);
        gluCylinder(q4, 0.2, 0.2, 0.2, 500,500);
        glPopMatrix();
        
    }
    void moveAttackers()
    {
        glPushMatrix();
        glTranslatef(x, 0, z);
        drawAttacker();
        glPopMatrix();
        
    }

};

vector<Attacker> Attackers;

/*----------------------------------------- Grid -------------------------------------*/
class GridCell
{
    float x;
    float z;
    
public:
    GridCell (){x = 0; z = 0;}
    float getX()
    { return x; }
    float getZ()
    { return z;}
    
    void setX(float newX)
    {
        x = newX;
    }
    void setZ(float newZ)
    {
        z = newZ;
    }
    
    
    
};
vector<GridCell> OccupiedCells;

void drawGrid()
{
    glPushMatrix();
    glScaled(0.5, 0.15, 0.5);
    
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j > -5; j--)
        {
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    glColor3f(0, 0.8, 0);
                } else
                {
                    glColor3f(0, 1.0, 0);
                }
            } else
            {
                if (j % 2 == 0)
                {
                   glColor3f(0.14, 0.7, 0.14);
                } else
                {
                    glColor3f(0.14, 0.9, 0.14);
                }
            }
            
            if (i == chooseColumn - 1 && chooseSteps <= 2 && chooseSteps > 0 )
            {
                if (i % 2 == 0)
                {
                    if (j % 2 == 0)
                    {
                        glColor3f(0, 0.4, 0);
                    } else
                    {
                        glColor3f(0, 0.6, 0);
                    }
                    
                } else
                {
                    if (j % 2 == 0)
                    {
                        glColor3f(0.08, 0.4, 0.08);
                    } else
                    {
                        glColor3f(0.1, 0.5, 0.1);
                    }
                }
            }
            if (j == 1 - chooseRow && chooseSteps <= 2 && chooseSteps > 0)
            {
                if (i % 2 == 0)
                {
                    if (j % 2 == 0)
                    {
                        glColor3f(0, 0.5, 0);
                    } else
                    {
                        glColor3f(0, 0.7, 0);
                    }
                    
                } else
                {
                    if (j % 2 == 0)
                    {
                        glColor3f(0.18, 0.5, 0.18);
                        
                    } else
                    {
                        
                        glColor3f(0.1, 0.6, 0.1);
                    }
                }
            }
            if (j == 1 - chooseRow && i == chooseColumn - 1 &&chooseSteps == 2)
            {
                glColor3f(0, 0.25, 0);
            }
            glPushMatrix();
            glTranslated(i, 0, j);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
    glPopMatrix();
}
void removeCell()
{
    for (int i = 0; i < OccupiedCells.size(); i++)
    {
        if ((OccupiedCells[i].getX() == (chooseColumn - 1)*0.5)
            && OccupiedCells[i].getZ() == -((chooseRow - 1)* 0.5))
        {
            OccupiedCells.erase(OccupiedCells.begin() + i);
        }
    }

}
void removeCharacter()
{
    for (int i = 0; i < PlaceResourceGatherer.size(); i++)
    {
        if ((PlaceResourceGatherer[i].getX() == (chooseColumn - 1)*0.5)
            && PlaceResourceGatherer[i].getZ() == -((chooseRow - 1)* 0.5))
        {
            PlaceResourceGatherer.erase(PlaceResourceGatherer.begin() + i);
            removeCell();
        }
    }
    for (int i = 0; i < PlaceDefenders.size(); i++)
    {
        if ((PlaceDefenders[i].getX() == (chooseColumn - 1)*0.5)
            && PlaceDefenders[i].getZ() == -((chooseRow - 1)* 0.5))
        {
            PlaceDefenders.erase(PlaceDefenders.begin() + i);
            removeCell();
        }
    }
    
}
bool cellEmpty(float x, float z)
{
    
    for (int i = 0; i < OccupiedCells.size(); i++)
    {
        if (OccupiedCells[i].getX() == x && OccupiedCells[i].getZ() == z)
        {
            return false;
        }

    }
    return true;
}

bool cellAvailable(float z)
{
    for (int i = 0; i < attackingLanes.size(); i++)
    {
        if (attackingLanes[i] == z)
        {
            return true;
        }
        
    }
    return false;
}

void placeCharacters()
{
    if (chooseCharacter != 0)
    {
        if (cellEmpty((chooseColumn - 1)*0.5, -((chooseRow - 1)* 0.5)) == true && cellAvailable(-((chooseRow - 1)* 0.5)) )
        {
            if (chooseCharacter == 2)
            {
                if (TotalResource >= 5)
                {
                    ResourceGatherer g = *new ResourceGatherer();
                    g.setX((chooseColumn - 1)*0.5);
                    g.setZ(-((chooseRow - 1)* 0.5));
                    PlaceResourceGatherer.push_back(g);
                    TotalResource -= g.getPrice();
                }
                GridCell c = *new GridCell();
                c.setX((chooseColumn - 1)*0.5);
                c.setZ(-((chooseRow - 1)* 0.5));
                OccupiedCells.push_back(c);
            }

        
            if (chooseCharacter == 1)
            {
                if (TotalResource >= 10)
                {
                    
                    Defender d = *new Defender();
                    d.setX((chooseColumn - 1)*0.5);
                    d.setZ(-((chooseRow - 1)* 0.5));
                    PlaceDefenders.push_back(d);
                    TotalResource -= d.getPrice();
                }
                
                GridCell c = *new GridCell();
                c.setX((chooseColumn - 1)*0.5);
                c.setZ(-((chooseRow - 1)* 0.5));
                OccupiedCells.push_back(c);
            }
                    }
        else if (chooseCharacter == 3)
        {
            removeCharacter();
        }
        
    }
}
void reDisplayCharacters()
{
    
    for (int i = 0; i < PlaceResourceGatherer.size(); i++) {

        for (int j = 0; j < Attackers.size(); j++)
        {
            if (Attackers[j].x >= PlaceResourceGatherer[i].getX() && Attackers[j].z != PlaceResourceGatherer[i].getZ() ) {
                PlaceResourceGatherer[i].drawResourceGatherer();
            }
        }
        
    }
    for (int i = 0; i < PlaceDefenders.size(); i++) {
        for (int j = 0; j < Attackers.size(); j++)
        {
            if (Attackers[j].x >= PlaceDefenders[i].getX() && Attackers[j].z != PlaceDefenders[i].getZ() ) {
                    PlaceDefenders[i].drawDefenderBullets();
            }
        }
    }
}
void removeLane(float z)
{
    int lane = - (z*2);
    
    if (attackingLanes.size() > 0)
    {
        for (int i = 0; i < attackingLanes.size(); i++)
        {
            if (i == lane)
            {
                attackingLanes.erase(attackingLanes.begin() + i);
            }
        }
    }
    for (int i=0; i < Attackers.size(); i++)
    {
        if (Attackers[i].z == z)
        {
            cout << endl <<"Z:"<< Attackers[i].z << endl;
            Attackers.erase(Attackers.begin() + i);
        }
    }
}

 void addAttackers()
{
    
    if (attackerTimer <= 0)
    {
        
        int random = rand() % attackingLanes.size();
        
        Attacker a = *new Attacker();
        a.z = attackingLanes[random];
        
        Attackers.push_back(a);
        
        attackerTimer = 5;
        
        cout << random << endl;
    }
    
}
void DrawAttackers()
{
    for (int i = 0; i < Attackers.size(); i++)
    {
        Attackers[i].moveAttackers();
    }
}

/*------------------------------ Camera + Lighting Setup ----------------------------*/
class Vector3f
{
public:
    float x, y, z;
    
    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        x = _x;
        y = _y;
        z = _z;
    }
    
    Vector3f operator+(const Vector3f &v) {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3f operator-(Vector3f &v) {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3f operator*(float n) {
        return Vector3f(x * n, y * n, z * n);
    }
    
    Vector3f operator/(float n) {
        return Vector3f(x / n, y / n, z / n);
    }
    
    Vector3f unit() {
        return *this / sqrt(x * x + y * y + z * z);
    }
    
    Vector3f cross(Vector3f v) {
        return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

class Camera
{
public:
    Vector3f eye, center, up;
    
    Camera(float eyeX = 2.0f, float eyeY = 3.0f, float eyeZ = 3.5f, float centerX = 2.0f, float centerY = -1.0f, float centerZ = -7.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
        eye = Vector3f(eyeX, eyeY, eyeZ);
        center = Vector3f(centerX, centerY, centerZ);
        up = Vector3f(upX, upY, upZ);
    }
    void reset()
    {
        eye.x = 2.0;
        eye.y = 3.0;
        eye.z = 3.5;
        center.x = 2;
        center.y = -1;
        center.z = -7;
        up.x = 0;
        up.y = 1;
        up.z = 0;
    }
    void moveX(float d) {
        Vector3f right = up.cross(center - eye).unit();
        eye = eye + right * d;
        center = center + right * d;
    }
    
    void moveY(float d) {
        eye = eye + up.unit() * d;
        center = center + up.unit() * d;
    }
    
    void moveZ(float d) {
        Vector3f view = (center - eye).unit();
        eye = eye + view * d;
        center = center + view * d;
    }
    
    void rotateX(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
        up = view.cross(right);
        center = eye + view;
    }
    
    void rotateY(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
        right = view.cross(up);
        center = eye + view;
    }
    
    void look() {
        gluLookAt(
                  eye.x, eye.y, eye.z,
                  center.x, center.y, center.z,
                  up.x, up.y, up.z
                  );
    }
    
    void cinematicView(float r, float a)
    {
        center.x = 1;
        center.z = -1;
        eye.x = r * cos(a);
        eye.z = r * sin(a);
    }
};

Camera camera;

void setupLights()
{
    GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
    GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat shininess[] = { 50 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    
    GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
    GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 640 / 480, 0.001, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();
    if (cinematicView == 1) {
        camera.cinematicView(CVradius, CVangle);
    }
}
void cinematicView1()
{
    
}
/*--------------------------------------------------------------------------------------*/
void Key(unsigned char key, int x, int y)
{
    float d = 0.5;
    
    switch (key) {
        case 'w':
            camera.moveY(d);
            break;
        case 's':
            camera.moveY(-d);
            break;
        case 'a':
            camera.moveX(d);
            break;
        case 'x':
            camera.moveX(-d);
            break;
        case 'q':
            camera.rotateX(d);
            break;
        case 'e':
            camera.rotateX(-d);
            break;
            
        case GLUT_KEY_ESCAPE:
            exit(EXIT_SUCCESS);
    }
    
    if (key == 'p') {
        if (pause == 0) {
            pause = 1;
        }
        else pause = 0;
    }
    if (key == 'v') {
        if (cinematicView == 0) {
            //camera.cinematicView(CVradius, CVangle);
            cinematicView = 1;
        }
        else
        {
            camera.reset();
            cinematicView = 0;
        }
    }
    if (chooseSteps == 0 && key >= '1' && key <= '5')
    {
        chooseRow = key - '0';
        chooseSteps = 1;
        chooseCharacter = 0;
        
    }
    else if (chooseSteps == 1 && key >= '1' && key <= '9')
    {
        chooseColumn = key - '0';
        chooseSteps = 2;
    }
    else if (chooseSteps == 2)
    {
        switch (key) {
            case 'd': //Defender
                chooseCharacter = 1;
                break;
            case 'r': //Resource gatherer
                chooseCharacter = 2;
                break;
            case 'c': //Remove character
                chooseCharacter = 3;
                break;
                
            //If wrong character is pressed, player must re-choose the cell
            default: chooseSteps = 0;
                break;
        }
        
        chooseSteps = 0;
    }
    else chooseSteps = 0;
    
    //printf("chooseSteps: %d, chooseRow: %d, chooseColumn: %d, chooseCharacter: %d, key: %d\n, AttackersX: %d",chooseSteps, chooseRow, chooseColumn, chooseCharacter, key - '0', attackersX - '0');
    
    placeCharacters();
    
    glutPostRedisplay();
}


void Display()
{
    setupCamera();
    setupLights();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawGrid();
    
    reDisplayCharacters();
    
    addAttackers();
    
    DrawAttackers();
    if (attackingLanes.size() <= 3) {
        GameOver = 1;
        DrawString("GAME OVER!", 1, 2);
    }
    if (GameOver == 0)
    {
        char s[100] = "Resources: ";
        char seconds[100];
        sprintf(seconds, "%d", TotalResource);
        strcat(s, seconds);
        DrawString(s , 3, 2);
        DrawString("Resource Gatherer: 5 coins", 0, 2);
        DrawString("Defender: 10 coins", 1.5, 2);

    }
    
    
    
    glFlush();
}

void Anim()
{
    if (pause == 0 && GameOver == 0)
    {
        rotAng += 4;
        attackersX -= 0.008;
        attackerTimer -= 0.1;
        multipleBullets += 0.1;
        CVangle += 0.03;
        
        for (int i = 0; i < Attackers.size(); i++)
        {


            if (Attackers[i].x <= 0)
            {
                removeLane(Attackers[i].z);

            } else
            {
                Attackers[i].x -= 0.02;
            }
        }
    }
    glutPostRedisplay();
}
/*--------------------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitWindowSize(900, 550);
    glutInitWindowPosition(50, 50);
    
    glutCreateWindow("Plants VS Zombies");
    glutDisplayFunc(Display);
    glutIdleFunc(Anim);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    if (GameOver ==0)
    {
        glClearColor(0.05f, 0.15f, 0.4f, 0.0f);
    } else {
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glutKeyboardFunc(Key);
    glShadeModel(GL_SMOOTH);
    
    glutMainLoop();
}
