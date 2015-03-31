#ifndef __FRACTALTREEMAKER_
#define __FRACTALTREEMAKER_

#define MAX_LEVELS 9

#include "FractalTreeSpec.h"

#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoShapeHints.h>

class Influence;
class SpiralInfluence;
class Extruder;

#define MAX_LEVELS 9

class FractalTreeMaker
{
public:
	SoSeparator           * fTreeRoot;
    static double           sCurrentRotation;
    static int              sNumBranches;
    Influence             * fInfluence;
    SpiralInfluence       * fSpiralInfluence;
    Extruder              * fExtruder;
    FractalTreeSpec       * fts;
    SoSeparator           * test_root;
    //SbMatrix                fLastPlacement;

    FractalTreeMaker(SoSeparator * fTreeRoot, FractalTreeSpec * tree_spec);
	~FractalTreeMaker();

    void MakeTree(SoSeparator * treeRoot, FractalTreeSpec * fractalTreeSpec);

	void        ConstructTreeRecursively(SoSeparator * branchRoot, SbMatrix mat, float scale, int level);
    void        MakeBranch(bool isLeft, SoSeparator * branchRoot, SbMatrix placement, float scale, int level);
    void        FlattenAllLevels(CString filename_ext);
    SoNode*     findNodeByName(SoGroup *parent, char *name);
    SbMatrix    getNodeMatrix(SoNode *mynode);
    void        transformCoords(SoCoordinate3 * branch_centerline_coords, SbMatrix centerline_scale_matrix);
    SbMatrix    getAccumulatedTransforms(SoSeparator *root_sep, SoSeparator *current_node);
};



#endif  // __FRACTALTREE_