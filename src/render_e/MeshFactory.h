/* 
 * File:   MeshFactory.h
 * Author: morten
 *
 * Created on December 6, 2010, 12:16 PM
 */

#ifndef MESHFACTORY_H
#define	MESHFACTORY_H

#include "Mesh.h"

namespace render_e {
class MeshFactory {
public:
    // static Mesh *CreateCube(float size);
    static Mesh *Tetrahedron();
private:
    MeshFactory();

};
}
#endif	/* MESHFACTORY_H */
