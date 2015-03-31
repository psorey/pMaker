#ifndef __FRACTALTREE_
#define __FRACTALTREE_

#define MAX_LEVELS 9
#define NUM_LEVELS 7


#define INCLUDE_RIGHT_SIDE
#define INCLUDE_BRANCH_ROTATION

// #define INCLUDE_PROTOTYPE_EXTRUSION
// #define OUTPUT_DXF_CENTERLINES

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

class FractalTreeMaker
{
public:
    SpiralInfluence *       fSpiralInfluence;
    Influence *             fInfluence;
    Extruder *              fExtruder;
    bool                    fUseSpiralEffect;
    bool                    fUseLinearEffect;
    bool                    fUseExponentialEffect;

    int                     fNumPrototypeCoords;    // can calculate this if we pass a SoCoordinate3
	SoSeparator *           fBaseNode;
	SoSeparator *		    fModelNode;
	SoCoordinate3 *		    fLeftCoords;
	SoCoordinate3 *		    fRightCoords;
	SoCoordinate3 *		    fShapeCoords;            // cross-section shape	
    SoCoordinate3 *         fScaleCoords;            // from pMaker x = 0-1
    SoCoordinate3 *         fTwistCoords;
	float				    fABRatio;
	int					    fNumLevels;
    float                   fTypicalRotation;
    SbVec3f                 fInfluenceDirection;
    float                   fInfluenceIntensity;     
    // float                   fCenterlineScale;
    CString                 fRightFilename;
    CString                 fLeftFilename;
    static double           sCurrentRotation;
    static int              sNumBranches;
    //static int *            sLevelTaskComplete;
    float                   fCenterlineScale[9];
    float                   fThickness[9];
    bool                    fUseThickness;
    bool                    fUseRot;
    bool                    fUseInfl;

	FractalTreeMaker(void);

	~FractalTreeMaker();

    void MakeTree(   SoSeparator    * treeRoot,  
                     SoCoordinate3  * _shapeCoords,           // shape or cross-section coords
                     SoCoordinate3  * _leftPathCoords,        // left branch path
                     SoCoordinate3  * _rightPathCoords,       // right branch path
                     SoCoordinate3  * _scaleCoords,           // assume vert = horiz, 0 to 1 scaling...
                     SoCoordinate3  * _twistCoords,
                     float          ABScale,                 // actually, B:A scale of 80% means B is smaller than A
                     float          * centerline_scale,
                     int            numLevels,              // levels of fractals
                     float          typical_rotation,       // from one level to the next, in radians
                     float          influence_intensity,    // zero to infinity
                     float          influence_direction,    // in radians from y-vector(?));
                     CString        leftFilename,
                     CString        rightFilename,
                     bool           useLinearEffect,
                     bool           useExponentialEffect,
                     bool           useSpiralEffect,
                     float *        thickness,
                     bool           use_thickness,
                     float *        rot,
                     bool           use_rot,
                     float *        infl,
                     bool           use_infl);


    // float       GetOffsetRatio(float offset, int level);
	void        ConstructTreeRecursively(SoSeparator * branchRoot, 
                                         SbMatrix mat, 
                                         double branchRotation, 
                                         int level);
    void        MakeBranch(SoSeparator * branchRoot, SoCoordinate3 * inputCoords, SbMatrix placement, float branchRotation, int level);
    void        FlattenAllLevels(CString filename_ext);
    SoNode*     findNodeByName(SoGroup *parent, char *name);
    SbMatrix    getNodeMatrix(SoNode *mynode);

};



#endif  // __FRACTALTREE_