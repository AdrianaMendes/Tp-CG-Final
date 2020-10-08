#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "CarregarArquivo.cpp"
#include<math.h>
#define PI 3.14159265

GLubyte lado[2048][2048][3];
GLuint textura_id2;

using namespace std;

static int handx = 0;
static int shoulderx = 0;
static int shouldery = 0;
static int fingerfinalz = 0;
static int elbow = 0;
static int shoulderz = 0;
static int angRoda = 0;

GLfloat angle = 60, fAspect;
CarregarArquivo carro;
CarregarArquivo roda;
CarregarArquivo frente;

int Rot_carro = 90;
float Trans_carro_x = 0.0;
float Trans_carro_z = 90.0;
float Velocidade = 0.0;
float scala = 1.0;
float camera_x = 0.0;
float camera_z = -13;

bool flag = false;

void Inicializa(void)
{
    glClearColor (0.0, 0.3, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    angle = 60;
    glEnable(GL_DEPTH_TEST);
    carro.Carregar("C:/Users/adria/Desktop/PLE -20.3/CG/Projetos/TP - Final/carro.obj");
    roda.Carregar("C:/Users/adria/Desktop/PLE -20.3/CG/Projetos/TP - Final/roda.obj");
    frente.Carregar("C:/Users/adria/Desktop/PLE -20.3/CG/Projetos/TP - Final/frente.obj");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, 1.0, 0.5, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,13,0,0,0,0,1,0); //especifica a posição do observador e do alvo.

    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);

    /* BEGIN */
    try
    {
        char c;
        ifstream arq2("C:/Users/adria/Desktop/PLE -20.3/CG/Projetos/TP - Final/texture/pista.bmp" ,ios::binary);

        if(!arq2)
            cout << "Erro ao abrir";

        for(int i = 0; i < 54 ; i++)
            c = arq2.get();
        for(int i = 0; i < 2048 ; i++)
            for(int j = 0; j < 2048 ; j++) {
                c = arq2.get();
                lado[i][j][2] = c;
                c =  arq2.get();
                lado[i][j][1] = c ;
                c =  arq2.get();
                lado[i][j][0] = c;
            }

        arq2.close();
        arq2.clear();
    }
    catch(...) {
        cout << "Erro ao ler imagem" << endl;
    }

    glGenTextures(1,&textura_id2);
    glBindTexture(GL_TEXTURE_2D, textura_id2);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,2048, 2048, 0, GL_RGB,GL_UNSIGNED_BYTE, lado);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    /* END */
}

void Roda (float scala, float tx, float ty, float tz)
{
    glPushMatrix();
    glTranslated(tx,ty,tz);
    glScalef(scala,scala,scala);
    glColor3f( 0, 0, 0 );
    for (unsigned int j = 0; j < (roda.faces).size(); ++j )
    {
        glBegin ( GL_POLYGON );

        for (unsigned int i = 0; i < (roda.faces[j]).size() ; ++i )
        {

            GLfloat vert[3] = {(roda.vertices[roda.faces[j][i][0]][0]),(roda.vertices[roda.faces[j][i][0]][1]),(roda.vertices[roda.faces[j][i][0]][2])};
            glVertex3fv ( vert );
        }

        glEnd( );
    }
    glPopMatrix();
}

void Frente (float scala, float tx, float ty, float tz)
{
    glPushMatrix();
    glTranslated(tx,ty,tz);
    glScalef(scala,scala,scala);
    glColor3f( 0.1, 0.1, 0.1);
    for (unsigned int j = 0; j < (frente.faces).size(); ++j )
    {
        glBegin ( GL_POLYGON );

        for (unsigned int i = 0; i < (frente.faces[j]).size() ; ++i )
        {

            GLfloat vert[3] = {(frente.vertices[frente.faces[j][i][0]][0]),(frente.vertices[frente.faces[j][i][0]][1]),(frente.vertices[frente.faces[j][i][0]][2])};
            glVertex3fv ( vert );
        }

        glEnd( );
    }
    glPopMatrix();

}

/*
int anguloRodaPositivo(int anguloCarro) {
    printf("%d\n", anguloCarro);
    anguloCarro = anguloCarro % 360;
    if(anguloCarro > 10)
        return 10;
    else
        return anguloCarro;
}
*/
void Carro (int rot_y, float trans_x, float trans_z)
{
    glPushMatrix();
    glTranslatef(trans_x,0.0,trans_z);

    glRotatef(rot_y + 90,0.0,1.0,0.0);
    glColor3f(1.0,0.0,0.0);
    for (unsigned int j = 0; j < (carro.faces).size(); ++j )
    {
        glBegin ( GL_POLYGON );
        for (unsigned int i = 0; i < (carro.faces[j]).size() ; ++i )
        {
            GLfloat vert[3] = {(carro.vertices[carro.faces[j][i][0]][0]),(carro.vertices[carro.faces[j][i][0]][1]),(carro.vertices[carro.faces[j][i][0]][2])};
            glVertex3fv ( vert );
        }
        glEnd( );
    }
    Roda (1.0, 2.0, 1.9, 1.0);  //roda traseira esquerda
    Roda (1.0, 2.0, 1.9, -0.8); //roda traseira direita   ARRUMAR

    glPushMatrix();
        glRotatef(angRoda, 0.0, 0.1, 0.0);
        Roda (1.0, -1.2, 1.9, 1.2); //roda dianteira esquerda
        Roda (1.0, -1.2, 1.9, -0.9); //roda dianteira direita  ARRUMAR
    glPopMatrix();

    //glClearColor (1.0, 1.0, 1.0, 1.0);
    Frente(0.889, 0.69, 1.700, 0.0689);


    glPopMatrix();

    //Roda (escala, frente, cima, lado direito);
}

void DefineIluminacao (void)
{
    GLfloat luzAmbiente[4]= {3,3,3,3};
    GLfloat luzDifusa[4]= {0.5,0.5,0.5,0.5}; // "cor"
    GLfloat luzEspecular[4]= {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz[4]= {0.0, 5.0, 5.0, 1.0};

    /*
    GLfloat luzAmbiente[4]= {0.2,0.2,0.2,1.0};
    GLfloat luzDifusa[4]= {0.7,0.7,0.7,1.0}; // "cor"
    GLfloat luzEspecular[4]= {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz[4]= {0.0, 5.0, 5.0, 1.0};
    */

    // Capacidade de brilho do material
    GLfloat especularidade[4]= {1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
    // Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void Piso(float scale, float altura)
{
    glPushMatrix();
        glTranslatef(0.0, altura, 0.0);
        //glScaled(scale,scale,scale);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura_id2);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        /*
        glBegin(GL_POLYGON); // Face posterior
            glNormal3f(0,0,1); // Normal da face
            glTexCoord2f(0,0);
            glVertex3f(5.0, 5.0, 5.0);
            glTexCoord2f(1,0);
            glVertex3f(-5.0, 5.0, 5.0);
            glTexCoord2f(1,1);
            glVertex3f(-5.0, -5.0, 5.0);
            glTexCoord2f(0,1);
            glVertex3f(5.0, -5.0, 5.0);
        glEnd();
        */
        glBegin(GL_POLYGON); // Face inferior
            glNormal3f(0,-1,0); // Normal da face
            glVertex3f(100.0, 0.0, 100.0);
            glTexCoord2f(0,1);
            glVertex3f(-100.0, 0.0, 100.0);
            glTexCoord2f(1,1);
            glVertex3f(-100.0, 0.0, -100.0);
            glTexCoord2f(1,0);
            glVertex3f(100.0, 0.0, -100.0);
            glTexCoord2f(0,0);
        glEnd();

    glPopMatrix();

    /*
    glPushMatrix();
        glTranslatef(0.0, altura, 0.0);
        glScalef(scale,scale,scale);
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_POLYGON);
            glVertex3f(100.0, 0.0, 100.0);
            glVertex3f(-100.0, 0.0, 100.0);
            glVertex3f(-100.0, 0.0, -100.0);
            glVertex3f(100.0, 0.0, -100.0);
        glEnd();
    glPopMatrix();
    */
}


void Desenha(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DefineIluminacao();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(Trans_carro_x,3, Trans_carro_z + 13, Trans_carro_x,0,Trans_carro_z, 0,1,0); // Camera
    //gluLookAt(camera_x,3,camera_z, Trans_carro_x,0,Trans_carro_z,0,1,0); // Camera atras do carro
    gluLookAt(camera_x,15,camera_z, Trans_carro_x,0,Trans_carro_z,0,1,0); // Camera atras do carro

    glPushMatrix();

        Piso(1.0,-2.0);

        Carro(Rot_carro, Trans_carro_x,Trans_carro_z);
        Carro(0, 0, 0);
    /* origem volta para o sistema de coordenadas original */
    glPopMatrix();

    glutSwapBuffers();
}


void AlteraTamanhoJanela (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    if(h == 0) h = 1;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(65, (GLfloat) w/(GLfloat) h, 0.5, 500);
}

void idle()
{
    Trans_carro_x = Trans_carro_x + Velocidade*sin(Rot_carro*PI/180);
    Trans_carro_z = Trans_carro_z + Velocidade*cos(Rot_carro*PI/180);
    camera_x = Trans_carro_x + 13.0*sin((Rot_carro+180)*PI/180);
    camera_z = Trans_carro_z + 13.0*cos((Rot_carro+180)*PI/180);
    glutPostRedisplay();
}

void Teclado(unsigned char key, int x, int y) {
    switch (key) {
    case 'd':
        if(Velocidade == 0.0) {
            printf("[ERROR] - O carro nao pode esta parado para virar.\n");
            if(angRoda > -20)
                angRoda = angRoda - 5;
            break;
        }
        if(Velocidade > 0) {
            Rot_carro = (Rot_carro - 5) % 360;
            if(angRoda > -20)
                angRoda = angRoda - 5;
        }
        else {
            Rot_carro = (Rot_carro + 5) % 360;
            if(angRoda < 20)
                angRoda = angRoda + 5;
        }
        glutPostRedisplay();
        break;

    case 'a':
        if(Velocidade == 0.0) {
            printf("[ERROR] - Para virar o carro precisar esta em movimento.\n");
            if(angRoda < 20)
                angRoda = angRoda + 5;
            break;
        }

        if(Velocidade > 0) {
            Rot_carro = (Rot_carro + 5) % 360;
            if(angRoda < 20)
                angRoda = angRoda + 5;
        }
        else {
            Rot_carro = (Rot_carro - 5) % 360;
            if(angRoda > -20)
                angRoda = angRoda - 5;
        }
        glutPostRedisplay();
        break;

    case 'r':
        if(Velocidade == 0.00)
            Velocidade = -0.02;
        else
            Velocidade = 0.00;
        glutPostRedisplay();
        break;

    case 'e':
        if(Velocidade == 0.0)
            Velocidade = 0.05;
        else
            Velocidade = 0.00;
        glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;

    default:
        printf("[ERROR] - Entrada: %c invalida.\n", key);
        break;
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

    glutInitWindowSize (800, 800);

    glutInitWindowPosition ((1280 - 800)/2, (800 - 800)/2);

    glutCreateWindow ("Trabalho Prático Final");

    Inicializa();

    glutDisplayFunc(Desenha);

    glutReshapeFunc(AlteraTamanhoJanela);

    glutKeyboardFunc(Teclado);

    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
