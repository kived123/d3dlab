/*
 * LabMesh.h
 *
 *  Created on: 16 авг. 2017 г.
 *      Author: kived
 */

#ifndef LABMESH_H_
#define LABMESH_H_


#include <vector>
#include "Map.h"


struct MeshBuilder
{
	std::vector<float> vertices;
	void addVertex(float x, float y, float z, float u, float v);

};

void BuildLabirintMesh(MeshBuilder & mb, Map& map, float StartX, float StartY );


#endif /* LABMESH_H_ */
