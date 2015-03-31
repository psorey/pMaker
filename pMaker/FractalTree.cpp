#include "stdafx.h"

#include "FractalTree.h"
#include "Influence.h"
#include "Extruder.h"

#include <WriteDXF.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoInfo.h>

#define INCLUDE_EXTRUSIONS

int FractalTreeMaker::sNumBranches = 0;

FractalTreeMaker::FractalTreeMaker(void)
{
    fSpiralInfluence = NULL;
    fInfluence = NULL;
}

FractalTreeMaker::~FractalTreeMaker()
{
}

void FractalTreeMaker::MakeTree(SoSeparator    *baseNode,  
                                SoCoordinate3  *_shapeCoords,           // shape or cross-section coords
                                SoCoordinate3  *_leftPathCoords,        // left branch path
                                SoCoordinate3  *_rightPathCoords,       // right branch path
                                SoCoordinate3  *_scaleCoords,           // assume vert = horiz, 0 to 1 scaling...
                                SoCoordinate3  *_twistCoords,
                                float          ABRatio,                 // actually, B:A scale of 80% means B is smaller than A
                                float *        centerline_scale,        // 1 means...
                                int            numLevels,               // levels of fractals
                                float          typical_rotation,        // from one level to the next, in radians
                                float          influence_intensity,     // zero to infinity
                                float          direction,               // in radians from y-vector(?));
                                CString        leftFilename,
                                CString        rightFilename,
                                bool           useLinearEffect,
                                bool           useExponentialEffect,
                                bool           useSpiralEffect,
                                float *        thickness,
                                bool           useThickness,
                                float *        rot,
                                bool           use_rot,
                                float *        infl,
                                bool           use_infl)
{
    fBaseNode             = baseNode;     
    fABRatio              = ABRatio;
    fNumLevels            = numLevels;
    fLeftCoords           = _leftPathCoords;
    fRightCoords          = _rightPathCoords;
    fScaleCoords          = _scaleCoords;
    fShapeCoords          = _shapeCoords;
    fTwistCoords          = _twistCoords;
    fInfluenceIntensity   = influence_intensity;
    fLeftFilename         = leftFilename;
    fRightFilename        = rightFilename;
    fUseLinearEffect      = useLinearEffect;
    fUseExponentialEffect = useExponentialEffect;
    fUseSpiralEffect      = useSpiralEffect;
    fModelNode            = new SoSeparator;
    SoInfo *info          = new SoInfo;
    fUseThickness         = useThickness;
    
    //
    //   centerline_scale  local, temporary variable;
    //   centerlineScale   a function parameter
    //   

    for(int i = 0; i < 7; i++) {
        fCenterlineScale[i] = centerline_scale[i];
        fThickness[i] = thickness[i];
    } 

    CString notes = "\n\nCreated using pMakerCoin\nCopyright 2012 Paul Sorey\nFractal tree parameters used:\n";
    char info_string[1000];
    sprintf(info_string,"\n    levels: %d\n    typ.rotation: %f\n    ABRatio: %f\n\n    influenceIntensity: %f\n    influenceDirection: %f\n\n    leftSideFile: %s\n    rightSideFile: %s\n\n", 
           numLevels, typical_rotation, ABRatio, influence_intensity, direction, leftFilename.GetString(), rightFilename.GetString() );
    
    notes += info_string;
    for(int i = 0; i < 7; i++) {
        char info_string[1000];
        sprintf(info_string, "    level %d   section_scale: %f   centerline_scale: %f   thickness: %f\n", i, (pow(fABRatio,i)), fCenterlineScale[i], fThickness[i]);
        notes += info_string;
    }
    notes += "\n    ";
    info->string.setValue(notes.GetBuffer());
    // info->string.setValue("fix: show scale and thickness values for all layers \nadd: 'haircut' ");
    fBaseNode->addChild(info);
    fBaseNode->addChild(fModelNode);
    
    // modify the default y-axis wind direction by 'direction' degrees around the x axis...
    SbVec3f i_direction(SbVec3f(0.0,1.0,0.0));
    SbMatrix direction_modifier;
    direction_modifier.makeIdentity();
    direction_modifier.setRotate(SbRotation(SbVec3f(1,0,0), direction));
    SbVec3f result;
    direction_modifier.multVecMatrix(i_direction, result);
    fInfluenceDirection = result;
    if(useSpiralEffect) {
        fSpiralInfluence = new SpiralInfluence(fInfluenceDirection, fInfluenceIntensity);
        fSpiralInfluence->setNumLayers(fNumLevels);
        fSpiralInfluence->fUseLinearEffect = fUseLinearEffect;
        fSpiralInfluence->fUseExponentialEffect = fUseExponentialEffect;
    } else {
        fInfluence = new Influence(fInfluenceDirection, fInfluenceIntensity);
        fInfluence->setNumLayers(fNumLevels);
        fInfluence->fUseLinearEffect = fUseLinearEffect;
        fInfluence->fUseExponentialEffect = fUseExponentialEffect;
    }
    fExtruder = new Extruder();
    sNumBranches = 0;
    fTypicalRotation = typical_rotation;
    int level = 0;
    // float scale = 1.0;
    SbMatrix mat;
    mat.makeIdentity();
    // mat.setScale(.74); // !!!! testing !!!!    
    double branchRotation = 0;
    
    sNumBranches = 0;

    ConstructTreeRecursively(fBaseNode, mat, branchRotation, level);
}



void FractalTreeMaker::MakeBranch(SoSeparator * branchRoot, SoCoordinate3 * inputCoords, SbMatrix placement, float branchRotation, int level) // bool isLeft, int level)
{
    //branchRotation += fTypicalRotation; // (60.0 * .0174532925);  // 1.047;
    //TRACE("branch rotation = %f\n", branchRotation);
    branchRotation = fTypicalRotation;  // ! just get rid of 'branchRotation'

    float fractal_scale = (pow(fABRatio,level));
    float centerline_scale = fractal_scale * fCenterlineScale[level];
  
    SbMatrix        centerline_scale_matrix;
    SbMatrix        fractal_scale_matrix;

    fractal_scale_matrix.makeIdentity();    
    fractal_scale_matrix.setScale(pow(fABRatio,level));
    centerline_scale_matrix.makeIdentity();
    centerline_scale_matrix.setScale(fractal_scale * fCenterlineScale[level]);
    SoCoordinate3  * branchCoords = new SoCoordinate3;
    SoLineSet      * branchLine   = new SoLineSet;
    int num_coords = inputCoords->point.getNum();
    
    SbVec3f result;
    for (int j = 0; j < num_coords; j++) {       
        centerline_scale_matrix.multVecMatrix(inputCoords->point[j], result);
        branchCoords->point.set1Value(j, result);
    } 
    
    // need to rotate first for this to work correctly !!!
    // if(fInfluence != NULL)
    //    fInfluence->influence_coords(leftBranchCoords, level);
    // if(fSpiralInfluence != NULL)
    //    fSpiralInfluence->influence_coords(leftBranchCoords, level);
    
    /* let extruder do this...
    int num_shape_coords = fShapeCoords->point.getNum();
    for(int j=0; j<num_shape_coords; j++) {
        centerline_scale_matrix.multVecMatrix(fLeftCoords->point[j], result);
        leftBranchCoords->point.set1Value(j, result);
    } */

    SoSeparator *branch_sep = new SoSeparator;
    SoTransform * placement_transform = new SoTransform;
    branch_sep->addChild(placement_transform);
    placement_transform->setMatrix(placement);

    // alternating colors for extrusions...
    SoMaterial    * levelMaterial = new SoMaterial;
    levelMaterial->shininess.setValue(.9);
    levelMaterial->diffuseColor.setValue(SbColor(.82, .82, .82));
    if ( level % 2 == 0 ) {
        levelMaterial->diffuseColor.setValue(SbColor(.72, .72, .72));
    }
    branch_sep->addChild(levelMaterial);

    float thickness = 0.0;
    if (fUseThickness)
        thickness = fThickness[level];

    // EXTRUDE 

    bool flatten = FALSE;
    branch_sep->addChild(fExtruder->extrude(fShapeCoords, branchCoords, fScaleCoords, fScaleCoords, fTwistCoords, fractal_scale, thickness, flatten ));
    branchRoot->addChild(branch_sep);

    // now make a placement_matrix for the next branch...
    if(level <= fNumLevels)  {
        SbVec3f to = branchCoords->point[num_coords-1] - branchCoords->point[num_coords-2];
        SbVec3f from(1, 0, 0);
        float ftwist = (fTwistCoords->point[fTwistCoords->point.getNum()-1][1] - fTwistCoords->point[0][1]);
        TRACE("ftwist = %f\n", ftwist);

        SbRotation twist = SbRotation(to, branchRotation /* + ftwist */ );  // rotate along the rotated x-axis
        SbRotation branchRot = SbRotation(from, to);
        branchRot *= twist; 

        SbMatrix branchMatrix;
        if(level < fNumLevels)  {
            branchMatrix.setTransform(branchCoords->point[num_coords-1], branchRot, SbVec3f(1, 1, 1));   
            // branchMatrix *= placement;  // accumulate the transformation matrix...
            // for the centerline...
            SoSeparator * newSep = new SoSeparator;
            branch_sep->addChild(newSep);
            ConstructTreeRecursively(newSep, branchMatrix, fTypicalRotation, level+1);
        }
    }
}


void FractalTreeMaker::ConstructTreeRecursively(SoSeparator * branchRoot, SbMatrix placement, double branchRotation, int level)
{
    //  SbMatrix placement
    //     tells us where to put the extrusion we are making:
    //     put it at the tip of the last branch (rotation + translation)
    //     (both can be calculated from the last two points in the previous branch.
    //     but it may be better to multiply the incoming matrix...

    sNumBranches++;

    branchRotation += fTypicalRotation; // (60.0 * .0174532925);  // 1.047;
    float fractal_scale = (pow(fABRatio, level));
    float centerline_scale = fractal_scale * fCenterlineScale[level];
  
    SbMatrix        centerline_scale_matrix;
    SbMatrix        fractal_scale_matrix;

    fractal_scale_matrix.makeIdentity();    
    fractal_scale_matrix.setScale(pow(fABRatio,level));
    centerline_scale_matrix.makeIdentity();
    centerline_scale_matrix.setScale(fractal_scale * fCenterlineScale[level]);
    SoSeparator * branchSep = new SoSeparator;
    branchRoot->addChild(branchSep);
    
    this->MakeBranch(branchSep, fLeftCoords, placement, branchRotation, level);
    this->MakeBranch(branchSep, fRightCoords, placement, branchRotation, level);

    return;
}

SoNode * FractalTreeMaker::findNodeByName(SoGroup *parent, char *name)
{
    SoSearchAction search;
    SoPath *path;
    SoNode *node;    
    search.setName(SbName(name));
    search.setInterest(SoSearchAction::FIRST);
    search.apply(parent);    
    path = search.getPath();
    if (path == NULL) return NULL;
    node = path->getTail();
    return node;
}

SbMatrix FractalTreeMaker::getNodeMatrix(SoNode *mynode) 
{
  // First get hold of an SoPath through the scenegraph down to the
  // node ("mynode") you want to query about its current world space
  // transformation(s).

  SoSearchAction * searchaction = new SoSearchAction;
  searchaction->setNode(mynode);
  searchaction->apply(fBaseNode);

  SoPath * path = searchaction->getPath();
  assert(path != NULL);

  // Then apply the SoGetMatrixAction to get the full transformation
  // matrix from world space.

  const SbViewportRegion vpr = NULL;
  SoGetMatrixAction * getmatrixaction = new SoGetMatrixAction(vpr);
  getmatrixaction->apply(path);

  SbMatrix transformation = getmatrixaction->getMatrix();

  // And if you want to access the individual transformation
  // components of the matrix:

  SbVec3f translation;
  SbRotation rotation;
  SbVec3f scalevector;
  SbRotation scaleorientation;

  transformation.getTransform(translation, rotation, scalevector, scaleorientation);
  return transformation;
}

void FractalTreeMaker::FlattenAllLevels(CString filename_ext)
{

}
