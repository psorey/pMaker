//
// Stuff to hilight parts of the line-set manipulator
//

#include "stdafx.h"
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/SoInput.h>

#include "LineManip.h"

void
LineManip2::setHilightSize(float size)
{
    hilightSize = size;
//
// This should really be figured out during event handling to give the
// hilights a uniform size in screen space (say, 5 or 10 pixels).
//
    hilightTransform->scaleFactor = SbVec3f(hilightSize, hilightSize,
					    hilightSize);
}

void
LineManip2::initHilightStuff()
{
    SoSeparator *sep = new SoSeparator;
    addChild(sep);

    SoPickStyle *ps = new SoPickStyle;
    sep->addChild(ps);
    ps->style = SoPickStyle::UNPICKABLE;

    hilightMaterial = new SoMaterial;
    sep->addChild(hilightMaterial);

    hilightSwitch = new SoSwitch;
    sep->addChild(hilightSwitch);
    hilightSwitch->whichChild = SO_SWITCH_NONE;

    hilightTransform = new SoTransform;
//
// This should really be figured out during event handling to give the
// hilights a uniform size in screen space (say, 5 or 10 pixels).
//
    hilightTransform->scaleFactor = SbVec3f(hilightSize, hilightSize,
					    hilightSize);

    {	// VERTEX HILIGHT MUST BE CHILD 0
	// The vertex hilight is a little cross-hair:
	//
	static float vhCoords[8][3] = {
	    {.25, 0, 0}, {1, 0, 0}, {0, .25, 0}, {0, 1, 0},
	    {-.25, 0, 0}, {-1, 0, 0}, {0, -.25, 0}, {0, -1, 0},
	};
	static int lsNV[4] = { 2, 2, 2, 2 };

	SoGroup *g = new SoGroup;
	hilightSwitch->addChild(g);
	g->addChild(hilightTransform);
	SoCoordinate3 *c = new SoCoordinate3;
	g->addChild(c);
	c->point.setValues(0, 8, (SbVec3f *)vhCoords);
	SoLineSet *ls = new SoLineSet;
	g->addChild(ls);
	ls->numVertices.setValues(0, 4, lsNV);
    }
    {	// LINE HILIGHT MUST BE CHILD 1
	// The line hilight is a line (or lines) to the new vertex and
	// an octagon surrounding the new vertex.
	//
	SoGroup *g = new SoGroup;
	hilightSwitch->addChild(g);

	// First, lines from previous to next vertex
	hilightCoord = new SoCoordinate3;
	g->addChild(hilightCoord);
	SoLineSet *ls = new SoLineSet;
	g->addChild(ls);

	// Second: octagon:
	static float lhCoords[9][3] = {
	    {1, 0, 0}, {M_SQRT1_2, M_SQRT1_2, 0},
	    {0, 1, 0}, {-M_SQRT1_2, M_SQRT1_2, 0},
	    {-1, 0, 0}, {-M_SQRT1_2, -M_SQRT1_2, 0},
	    {0, -1, 0}, {M_SQRT1_2, -M_SQRT1_2, 0},
	    {1, 0, 0}
	};
	g->addChild(hilightTransform);
	SoCoordinate3 *c = new SoCoordinate3;
	g->addChild(c);
	c->point.setValues(0, 9, (SbVec3f *)lhCoords);
	g->addChild(ls);
    }
}

void
LineManip2::hilightVertex(const SbVec3f &position, const SbColor &color)
{
    // Set switch to vertex (if necessary)
    if (hilightSwitch->whichChild.getValue() != 0)
    {
	hilightSwitch->whichChild.setValue(0);
    }
    // Set transformation to given position (if necessary)
    if (hilightTransform->translation.getValue() != position)
    {
	hilightTransform->translation.setValue(position);
    }
    // And set color, if necessary.
    if (hilightMaterial->diffuseColor[0] != color)
    {
	hilightMaterial->diffuseColor.setValue(color);
    }
}

void
LineManip2::hilightLine(int whichLine, const SbVec3f &position,
			const SbColor &color)
{
    if (coord == NULL)
	return;

    // Draw the line hilight
    if (hilightSwitch->whichChild.getValue() != 1)
    {
	hilightSwitch->whichChild.setValue(1);
    }

    // Set transformation to given position
    hilightTransform->translation.setValue(position);

    // And set color, if necessary.
    if (hilightMaterial->diffuseColor[0] != color)
    {
	hilightMaterial->diffuseColor.setValue(color);
    }

    int n = coord->point.getNum();
    // And set coordinates of the extra line segments:
    if (whichLine >= 0 && whichLine < n)
    {
	hilightCoord->point.set1Value(0, coord->point[whichLine]);
    }
    else
    {
	hilightCoord->point.set1Value(0, position);
    }
    if (whichLine+1 < coord->point.getNum() && whichLine+1 >= 0)
    {
	hilightCoord->point.set1Value(2, coord->point[whichLine+1]);
    }
    else
    {
	hilightCoord->point.set1Value(2, position);
    }
    hilightCoord->point.set1Value(1, position);
}

void
LineManip2::removeHilights()
{
    if (hilightSwitch->whichChild.getValue() != SO_SWITCH_NONE)
    {
	hilightSwitch->whichChild.setValue(SO_SWITCH_NONE);
    }
}
