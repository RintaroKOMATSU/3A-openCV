#pragma once

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "utility.h"
#include "vec3.h"

void hsv2rgb(double *hsv, double *rgb) {
	double h = hsv[0], s = hsv[1], v = hsv[2];
	double h0 = (fmod(h, 360))/60;
	double c = s;
	double x = c*(1-abs(fmod(h0, 2) -1));
	double r = v-c, g = v-c, b = v-c;
	switch (((int)h0))
	{
	case 0:
		r += c;g+=x;
		break;
	case 1:
		r +=x;g+=c;
		break;
	case 2:
		g +=c;b+=x;
		break;
	case 3:
		g+=x;b+=c;
		break;
	case 4:
		r+=x;b+=c;
		break;
	case 5:
		r+=c;b+=x;
		break;
	}
	rgb[0] = r; rgb[1] = g; rgb[2] = b;
}

void setColorRGB(double *rgb) {
	glColor3d(rgb[0], rgb[1], rgb[2]);
}

void setColorHSV(double *hsv) {
	double rgb[3];
	hsv2rgb(hsv, rgb);
	setColorRGB(rgb);
}

void draw_cube(double x, double y, double z, double x_size, double y_size , double z_size , double* hsv) {
    double x0 = x_size/2;
    double y0 = y_size/2;
    double z0 = z_size/2;
	GLdouble pointA[] = {x0, y0, z0};
	GLdouble pointB[] = {-1*x0, y0, z0};
	GLdouble pointC[] = {-1*x0, -1*y0, z0};
	GLdouble pointD[] = {x0, -1*y0, z0};
    GLdouble pointE[] = {x0, y0, -1*z0};
	GLdouble pointF[] = {-1*x0, y0, -1*z0};
	GLdouble pointG[] = {-1*x0, -1*y0, -1*z0};
	GLdouble pointH[] = {x0, -1*y0, -1*z0};

    glPushMatrix();
    glTranslatef(x, y, z);

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();

    glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointF);
	glVertex3dv(pointE);
	glEnd();

    glBegin(GL_POLYGON);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointG);
	glVertex3dv(pointF);
	glEnd();

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glVertex3dv(pointH);
	glVertex3dv(pointG);
	glEnd();

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointD);
	glVertex3dv(pointA);
	glVertex3dv(pointE);
	glVertex3dv(pointH);
	glEnd();

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointE);
	glVertex3dv(pointF);
	glVertex3dv(pointG);
	glVertex3dv(pointH);
	glEnd();
    glPopMatrix();
}


void draw_sphere(double x, double y, double z, double radius, double* hsv) {
    glPushMatrix();
	setColorHSV(hsv);
    glTranslatef(x, y, z);
    glutSolidSphere(radius,32,32);
    glPopMatrix();
}

void draw_plane(double x, double y, double z, double x_size, double y_size, double* hsv) {
	double x0 = x_size/2;
    double y0 = y_size/2;
	GLdouble pointA[] = {x0, y0, z};
	GLdouble pointB[] = {-1*x0, y0, z};
	GLdouble pointC[] = {-1*x0, -1*y0, z};
	GLdouble pointD[] = {x0, -1*y0, z};
	glPushMatrix();
	setColorHSV(hsv);
	glTranslatef(x, y, z);
	glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();
	glPopMatrix();

}	

void draw_circle(double x0, double y0, double z0, double radius, double* hsv) {
	glPushMatrix();
	setColorHSV(hsv);
    glTranslatef(x0, y0, z0);
 	glBegin(GL_POLYGON);
	int n = 32;
	double x = radius, y = 0.0;
    double rotation_mat[2][2] = {{cos(2*M_PI/n), -sin(2*M_PI/n)}, 
								{sin(2*M_PI/n),cos(2*M_PI/n)}};
    for (int i = 0 ; i < n; i++) {
        glVertex3d(x, y, 0);
		double tmp_x = x;
		double tmp_y = y;
		x = rotation_mat[0][0]*tmp_x+rotation_mat[0][1]*tmp_y;
		y = rotation_mat[1][0]*tmp_x+rotation_mat[1][1]*tmp_y;
    }
	glEnd();
    glPopMatrix();
}

void draw_fragent(vec3 point, double angle_x, double angle_y, double angle_z, double size, double* hsv) {
	glPushMatrix();
	setColorHSV(hsv);
	glTranslatef(point.x(), point.y(), point.z());
	glRotatef(angle_x, point.x(), 0, 0);
	glRotatef(angle_y, 0, point.y(), 0);
	glRotatef(angle_z, 0, 0, point.z());
 	glBegin(GL_POLYGON);
	glVertex3d(size*0.6, 0, 0);
	glVertex3d(-0.3*size, 0.4*size, 0);
	glVertex3d(-0.3*size, -0.4*size, 0);
	glEnd();
	glPopMatrix();
} 