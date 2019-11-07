#include "glutp.h"

void glutWireTetrahedronP(GLdouble dSize) {
    /* Magic Numbers:  r0 = ( 1, 0, 0 )
   *                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
   *                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
   *                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
   * |r0| = |r1| = |r2| = |r3| = 1
   * Distance between any two points is 2 sqrt(6) / 3
   *
   * Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
   */

    double r0[3] = {1.0*dSize, 0.0, 0.0};
    double r1[3] = {-0.333333333333*dSize, 0.942809041582*dSize, 0.0};
    double r2[3] = {-0.333333333333*dSize, -0.471404520791*dSize, 0.816496580928*dSize};
    double r3[3] = {-0.333333333333*dSize, -0.471404520791*dSize, -0.816496580928*dSize};

    glBegin(GL_LINE_LOOP);
    glNormal3d (           -1.0*dSize,             0.0,             0.0 ) ; glVertex3dv ( r1 ) ; glVertex3dv ( r3 ) ; glVertex3dv ( r2 ) ;
    glNormal3d ( 0.333333333333*dSize, -0.942809041582*dSize,             0.0 ) ; glVertex3dv ( r0 ) ; glVertex3dv ( r2 ) ; glVertex3dv ( r3 ) ;
    glNormal3d ( 0.333333333333*dSize,  0.471404520791*dSize, -0.816496580928*dSize ) ; glVertex3dv ( r0 ) ; glVertex3dv ( r3 ) ; glVertex3dv ( r1 ) ;
    glNormal3d ( 0.333333333333*dSize,  0.471404520791*dSize,  0.816496580928*dSize ) ; glVertex3dv ( r0 ) ; glVertex3dv ( r1 ) ; glVertex3dv ( r2 ) ;
  glEnd() ;
}


void glutSolidTetrahedronP(GLdouble dSize) {
    /* Magic Numbers:  r0 = ( 1, 0, 0 )
   *                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
   *                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
   *                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
   * |r0| = |r1| = |r2| = |r3| = 1
   * Distance between any two points is 2 sqrt(6) / 3
   *
   * Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
   */

    double r0[3] = {1.0 *dSize, 0.0, 0.0};
    double r1[3] = {-0.333333333333*dSize, 0.942809041582*dSize, 0.0};
    double r2[3] = {-0.333333333333*dSize, -0.471404520791*dSize, 0.816496580928*dSize};
    double r3[3] = {-0.333333333333*dSize, -0.471404520791*dSize, -0.816496580928*dSize};

    glBegin(GL_TRIANGLES);
    glNormal3d (           -1.0*dSize,             0.0,             0.0 ) ; glVertex3dv ( r1 ) ; glVertex3dv ( r3 ) ; glVertex3dv ( r2 ) ;
    glNormal3d ( 0.333333333333*dSize, -0.942809041582*dSize,             0.0 ) ; glVertex3dv ( r0 ) ; glVertex3dv ( r2 ) ; glVertex3dv ( r3 ) ;
    glNormal3d ( 0.333333333333*dSize,  0.471404520791*dSize, -0.816496580928*dSize ) ; glVertex3dv ( r0 ) ; glVertex3dv ( r3 ) ; glVertex3dv ( r1 ) ;
    glNormal3d ( 0.333333333333*dSize,  0.471404520791*dSize,  0.816496580928*dSize ) ; glVertex3dv ( r0 ) ; glVertex3dv ( r1 ) ; glVertex3dv ( r2 ) ;
  glEnd() ;
}