#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include<vector>
#include<fstream>
#include<sstream>

#include<string>
using namespace std;

#include "./Library/SOIL/include/SOIL.h"
#include "./Library/glut/glut.h"

#pragma comment( lib, "./Library/glut/glut32.lib" )
#pragma comment( lib, "./Library/SOIL/lib/SOIL.lib" )

// Global texture ID
GLuint textures[2];
vector<float> vx,vy,vz;
vector<float> second_vx, second_vy, second_vz;
vector<float> vtx,vty;
vector<float> second_vtx, second_vty;
vector<int> vnx,vny,vnz;

vector<int> afv, bfv, cfv;
vector<int> second_afv, second_bfv, second_cfv;

vector<int> afvt, bfvt,cfvt;
vector<int> second_afvt, second_bfvt, second_cfvt;

int k = 0;
// Parameter for View angles
int angleX ;
int angleY ;
int lastX, lastY;
float vec[3];
float norm[3];
void normal(float norm[3]) {
    float length = sqrtf(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
    norm[0] /= length;
    norm[1] /= length;
    norm[2] /= length;
}
void cross(float* v1, float* v2, float* vec) {
    vec[0] = v1[1] * v2[2] - v1[2] * v2[1];
    vec[1] = v1[2] * v2[0] - v1[0] * v2[2];
    vec[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void get_normal(float* p1, float* p2, float* p3, float norm[3]) {
    float v1[3] = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
    float v2[3] = { p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2] };
    cross(v1, v2, norm);
    normal(norm);
}

void loadObj(string path,int k) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Failed to open the file!" << endl;
    }
    string line;
int j = 1;
    while (getline(file, line))
 {   
       //cout << "line: " << line;
        if (line.substr(0, 2) == "vt") // 頂點紋理坐標數據
        {
            istringstream s(line.substr(2));
            string temp;
            float x, y;
           
            s >> x >> y;
            if (y < 0) {
                y = -y;
            }
          
           //cout << x << " " << y << endl;
            if (k == 0) {
                vtx.push_back(x);
                vty.push_back(y);
            }
            else {
                second_vtx.push_back(x);
                second_vty.push_back(y);
            }
            
        }
        else if (line.substr(0, 1) == "v") // 頂點位置數據
        {
            istringstream s(line.substr(2));
            float x, y, z;
            s >> x >> y >> z;
          // cout << x << " " << y <<" "<<z<< endl;
            x /= 250;
           y /= 250;
           z /= 250;
           if (k == 0) {
               vx.push_back(x);
               vy.push_back(y);
               vz.push_back(z);
           }
           else {
                second_vx.push_back(x);
                second_vy.push_back(y);
                second_vz.push_back(z);
           }
            
        }
        else if (line.substr(0, 1) == "f") // 面數據
        {
            istringstream s(line.substr(2));
            
            int a, b, c, d, e, f, g, h, i;
            char slash;
            s >> a >> slash >> b >> slash >> c >> d >> slash >> e >> slash >> f >> g >> slash >> h >> slash >> i;
            if (k == 0) {
                afv.push_back(a);
                afvt.push_back(b);
                bfv.push_back(d);
                bfvt.push_back(e);
                cfv.push_back(g);
                cfvt.push_back(h);
            }
            else {
                second_afv.push_back(a);
                second_afvt.push_back(b);
                second_bfv.push_back(d);
                second_bfvt.push_back(e);
                second_cfv.push_back(g);
                second_cfvt.push_back(h);
            }
            
            if (j <0) {
                cout << a << " " << b <<" "<<c<<" " <<d<<endl;
                ++j;
            }
            
        }
        else if (line[0] == '#') // 註釋忽略
        {
        }
        else
        {
            //cout << "do not know" << line << endl;
        }
        
    }

    file.close();
    cout << "end of read"<<endl;
}
void init() {

    glDisable(GL_CULL_FACE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glEnable(GL_CULL_FACE);
    loadObj("./source/tiger.obj",0);
    loadObj("./source/buddha.obj",1);
    glEnable(GL_DEPTH_TEST);  // Enable depth testing

    // Enable shading
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Load textures
    textures[0] = SOIL_load_OGL_texture("./source/tiger-atlas.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    textures[1] = SOIL_load_OGL_texture("./source/buddha-atlas.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if (textures[0] == 0 || textures[1] == 0) {
        printf("Error loading texture\n");
    }
    else {
        //cout << "texture complete"<< endl;
    }

    // Enable 2D texture
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Material
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void reshape(int w, int h) {
    if (h == 0) h = 1; // Divide by zero
    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void draw(GLenum mode) {
    // Set texture mode
    
    //cout << "start draw"<<endl;
    //draw tiger
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
    for (int i = 0; i < afvt.size(); ++i) {

        int av=afv[i]-1, avt=afvt[i]-1;
        int bv=bfv[i]-1, bvt=bfvt[i]-1;
        int cv=cfv[i]-1, cvt=cfvt[i]-1;
        
         float temp1[3];
         temp1[0] = vx[av];
         temp1[1] = vy[av] ;
         temp1[2] = vz[av];
         float temp2[3];
         temp2[0] = vx[bv];
         temp2[1] = vy[bv];
         temp2[2] = vz[bv];
         float temp3[3];
         temp3[0] = vx[cv];
         temp3[1] = vy[cv];
         temp3[2] = vz[cv];
         float vec[3];
         get_normal(temp1, temp2, temp3, vec);
         if (i < 0) {
             cout<< "temp1 " << temp1[0] << " " << temp1[1] << " " << temp1[2] << endl;
             cout << "temp2 " << temp2[0] << " " << temp2[1] << " " << temp2[2] << endl;
             cout << "temp3 " << temp3[0] << " " << temp3[1] << " " << temp3[2] << endl;
             cout << "temp " << vec[0] << " " << vec[1] << " " << vec[2] << endl;
         }
         if (i < 0) {
             cout << "vtxa " << vtx[avt] << " " << vty[avt]<< endl;
             cout << "vtxb " << vtx[bvt] << " " << vty[bvt]<< endl;
             cout << "vtxc " << vtx[cvt] << " " << vty[cvt]<< endl;
         }

         
         glBindTexture(GL_TEXTURE_2D, textures[0]);

         glBegin(GL_TRIANGLES);
         glNormal3f(vec[0], vec[1],vec[2]);

         glTexCoord2f(vtx[avt], vty[avt]); 
         glVertex3f(temp1[0], temp1[1], temp1[2]);

         glTexCoord2f(vtx[bvt], vty[bvt]);
         glVertex3f(temp2[0], temp2[1], temp2[2]);

         glTexCoord2f(vtx[cvt], vty[cvt]);
         glVertex3f(temp3[0], temp3[1], temp3[2]);
         glEnd();
         
    }
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);

    for (int i = 0; i < second_afvt.size(); ++i) {

        int av = second_afv[i] - 1, avt = second_afvt[i] - 1;
        int bv = second_bfv[i] - 1, bvt = second_bfvt[i] - 1;
        int cv = second_cfv[i] - 1, cvt = second_cfvt[i] - 1;

        float temp1[3];
        temp1[0] = second_vx[av];
        temp1[1] = second_vy[av];
        temp1[2] = second_vz[av];
        float temp2[3];
        temp2[0] = second_vx[bv];
        temp2[1] = second_vy[bv];
        temp2[2] = second_vz[bv];
        float temp3[3];
        temp3[0] = second_vx[cv];
        temp3[1] = second_vy[cv];
        temp3[2] = second_vz[cv];
        float vec[3];
        get_normal(temp1, temp2, temp3, vec);
        if (i < 0) {
            cout << "temp1 " << temp1[0] << " " << temp1[1] << " " << temp1[2] << endl;
            cout << "temp2 " << temp2[0] << " " << temp2[1] << " " << temp2[2] << endl;
            cout << "temp3 " << temp3[0] << " " << temp3[1] << " " << temp3[2] << endl;
            cout << "temp " << vec[0] << " " << vec[1] << " " << vec[2] << endl;
        }
        if (i < 0) {
            cout << "vtxa " << vtx[avt] << " " << vty[avt] << endl;
            cout << "vtxb " << vtx[bvt] << " " << vty[bvt] << endl;
            cout << "vtxc " << vtx[cvt] << " " << vty[cvt] << endl;
        }


        glBindTexture(GL_TEXTURE_2D, textures[1]);

        glBegin(GL_TRIANGLES);
        glNormal3f(vec[0], vec[1], vec[2]);

        glTexCoord2f(second_vtx[avt], second_vty[avt]);
        glVertex3f(temp1[0], temp1[1], temp1[2]);

        glTexCoord2f(second_vtx[bvt], second_vty[bvt]);
        glVertex3f(temp2[0], temp2[1], temp2[2]);

        glTexCoord2f(second_vtx[cvt], second_vty[cvt]);
        glVertex3f(temp3[0], temp3[1], temp3[2]);
        glEnd();

    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // View setting
    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Draw content
      draw(GL_MODULATE); // Test GL_MODULATE, GL_REPLACE and GL_BLEND to check different results
     
   
    glutSwapBuffers();
}

// Change viewpoint
void mouseMove(int x, int y) {

    angleX += (y - lastY);
    angleY += (x - lastX);
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}


void mouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
    }
}

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Texture ");

    init();

    glutDisplayFunc(display);
    glutMotionFunc(mouseMove);
    glutMouseFunc(mouseButton);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
