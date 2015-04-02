#include "stdafx.h"

#include "FractalTreeMaker.h"
#include "Influence.h"
#include "Extruder.h"
#include <Inspect.h>

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



FractalTreeMaker::FractalTreeMaker(SoSeparator * treeRoot, FractalTreeSpec * tree_spec)
{
    fExtruder        = NULL;
    fSpiralInfluence = NULL;
    fInfluence       = NULL;
    fts              = tree_spec;         // pointer to FractalTreeSpec pMakerView::fFractalTreeSpec
    fTreeRoot        = treeRoot;  
    test_root        = new SoSeparator;
    test_root->ref();  
}

FractalTreeMaker::~FractalTreeMaker()
{
    if(fInfluence != NULL)        delete fInfluence;
    if(fSpiralInfluence != NULL)  delete fSpiralInfluence;
    test_root->unref();
    test_root = NULL;
    fts = NULL;
    if(fExtruder != NULL)         delete fExtruder;
}

void FractalTreeMaker::MakeTree(SoSeparator * baseNode, FractalTreeSpec * fts)  
{
    SoInfo *info          = new SoInfo;
    fTreeRoot             = baseNode; 
    // make annotation for SoInfo node...
    CString notes = "\n\nCreated using pMakerCoin\nCopyright 2013 Paul Sorey\nFractal tree parameters used:\n";
    CString info_string = fts->listSpecs("test_specs");    
    notes += info_string;
    notes += "\n    ";
    info->string.setValue(notes.GetBuffer());
    fTreeRoot->addChild(info);
        
    // pass the FractalTreeSpec to Influence
    // fInfluence = new Influence(fts);
    fExtruder = new Extruder();
    int level = 0;
    SbMatrix placement;
    placement.makeIdentity();
    //fLastPlacement = placement;
    sNumBranches = 0;
    float scale = 1.0;

    ConstructTreeRecursively(fTreeRoot, placement, scale, level);  // start the ball rolling...
    
    char * filename_test = "C:\\3000 ArtProjects\\048 Austin\\tree\\test_.iv";
    Inspect::WriteTree(filename_test, test_root);
    delete fExtruder;
    fExtruder = NULL;
    delete fInfluence;
    fInfluence = NULL;
}


void FractalTreeMaker::ConstructTreeRecursively(SoSeparator * branchRoot, SbMatrix placement, float scale, int level)
{
    //  SbMatrix placement
    //     tells us how to position the branch we are making:
    //     put it at the tip of the last branch (rotation + translation)
    //     (are calculated from the last two points in the previous branch.
    //     but it may be better to multiply the incoming matrix?...
	//TRACE("ctr: level = %d       scale = %f\n", level, scale);
    sNumBranches++;
    SoSeparator * branch_sep = new SoSeparator;
    char node_name[1000];
    sprintf(node_name, "level_%d", level);
    branch_sep->setName(node_name);
    branchRoot->addChild(branch_sep);
    this->MakeBranch(true  /* left side  */, branch_sep, placement, scale, level);
    this->MakeBranch(false /* right side */, branch_sep, placement, scale, level);
    return;
}

void FractalTreeMaker::MakeBranch(bool isLeft, SoSeparator * branchRoot, SbMatrix placement, float scale, int level) 
{
    // get the branch scale for left or right...
    SbMatrix  centerline_scale_matrix  =  fts->getCenterlineScaleMatrix(isLeft, level);
    SbMatrix  fractal_scale_matrix     =  fts->getFractalScaleMatrix(isLeft, level); 
    //Inspect::Matrix("FTM::MakeBranch -- centerline scale matrix\n", centerline_scale_matrix);
	//Inspect::Matrix("FTM::MakeBranch -- fractal_scale_matrix\n", fractal_scale_matrix);
    // create some new persistent objects with unique names...
    SoCoordinate3  * branch_centerline_coords = new SoCoordinate3;  // the centerline that gets modified
    SoLineSet      * branch_line              = new SoLineSet;
    SoSeparator    * branch_sep               = new SoSeparator;

    char node_name[1000];
    if(isLeft)
        sprintf(node_name, "level_%d_left", level);
    else
        sprintf(node_name, "level_%d_right", level);
    branch_sep->setName(node_name);
    fts->getCenterlineCoords(isLeft, branch_centerline_coords, level);
    int num_coords = branch_centerline_coords->point.getNum();
    this->transformCoords(branch_centerline_coords, fractal_scale_matrix);
    this->transformCoords(branch_centerline_coords, centerline_scale_matrix);
    this->transformCoords(branch_centerline_coords, placement);  

    // now apply the Influence...
    // if(fts->fUseInfluence == true && fInfluence != NULL)
    //     fInfluence->influence_coords(isLeft, branch_centerline_coords, level);

    // because the coords are currently at their ultimate positions:
    // find the transform describing the beginning position of the next branch...
    float ftwist = fts->getEndTwist(isLeft, level);        // if there was twist involved in the extrusion...
    // first we get the rotation at the branch tip...
    SbVec3f to = branch_centerline_coords->point[num_coords-1] - branch_centerline_coords->point[num_coords-2];
    SbVec3f from(1, 0, 0);
    SbRotation branchRot = SbRotation(from, to);
    // then we add the fractal rotation, plus any twist in the extrusion...
    SbRotation twist = SbRotation(to, fts->getLevelRotationAmount(isLeft, level) + ftwist );  // rotate along the rotated x-axis
    branchRot *= twist; 

    SbMatrix next_branch_matrix;  // position & rotation but not scale...
    next_branch_matrix.setTransform(branch_centerline_coords->point[num_coords-1], branchRot, SbVec3f(1, 1, 1));

    // this makes a copy of just the centerlines for testing...
    SoSeparator   * test_sep     = new SoSeparator;
    SoCoordinate3 * test_coords  = new SoCoordinate3;
    SoLineSet     * test_lines   = new SoLineSet;
    test_sep->addChild(test_coords);
    test_coords->copyFieldValues(branch_centerline_coords);
    test_sep->addChild(test_lines);
    test_root->addChild(test_sep);

    // now apply the placement matrix in reverse to return the centerline to its original location...
    SbMatrix inverse = placement.inverse();
    this->transformCoords(branch_centerline_coords, inverse);
    // now centerline is at 0,0,  beginning direction is 1,0,0
  
    // set up scene graph for the extrusion...
    // because the transforms accumulate in the scene graph we get the transform relative to the current node transform...
    branchRoot->addChild(branch_sep);
    SbMatrix accumulated_matrix = this->getAccumulatedTransforms(fTreeRoot, branch_sep);
    SbMatrix transform_matrix = placement * accumulated_matrix.inverse();
    // fLastPlacement = placement;
    SoTransform *transform = new SoTransform; 
    branch_sep->addChild(transform);
    transform->setMatrix(transform_matrix);
    
    // alternating colors for extrusions...
    SoMaterial * levelMaterial = new SoMaterial;
    levelMaterial->shininess.setValue(.9);
    levelMaterial->diffuseColor.setValue(SbColor(.95, .62, .62));
    if ( level % 2 == 0 )
        levelMaterial->diffuseColor.setValue(SbColor(.50, .52, .72));
    branch_sep->addChild(levelMaterial);
    float thickness = 0.0;
    if (fts->fUseThickness)
        thickness = fts->fLThick[level];
    
    // EXTRUDE   Extruder takes shapeCoords, centerlineCoords (already scaled by fractal scale and centerline scale)
    bool flatten = FALSE;
    if (TRUE == isLeft) {
        branch_sep->addChild(fExtruder->extrude_fractal(fts->fShapeCoords, branch_centerline_coords, fts->fLeftHScaleCoords, fts->fLeftVScaleCoords, fts->fLeftTwistCoords, fts->getFractalScale(isLeft, level), thickness, false ));
        //branch_sep->addChild(fExtruder->extrude(fts->fShapeCoords, branch_centerline_coords, fts->fLeftHScaleCoords, fts->fLeftVScaleCoords, fts->fLeftTwistCoords,false ));
	} else {
        branch_sep->addChild(fExtruder->extrude_fractal(fts->fShapeCoords, branch_centerline_coords, fts->fRightHScaleCoords, fts->fRightVScaleCoords, fts->fRightTwistCoords, fts->getFractalScale(isLeft, level), thickness, false ));
    }
    // now make the next branch...
    if(level <= fts->fNumLevels)  {
        if(level < fts->fNumLevels)  {
            float newScale;
            if(isLeft) newScale = scale * fts->fLeftABRatio;
            else newScale = scale * fts->fRightABRatio;
            SoSeparator * newSep = new SoSeparator;
            branch_sep->addChild(newSep);
            ConstructTreeRecursively(newSep, next_branch_matrix, newScale, level+1); 
        }
    }
}


/////////////////////////////////////////////////////////////
//  UTILITIES  -- put somewhere else!!!                    //
/////////////////////////////////////////////////////////////

SbMatrix FractalTreeMaker::getAccumulatedTransforms(SoSeparator *root_sep, SoSeparator *current_node)
{
    // First get hold of an SoPath through the scenegraph down to the
    // node ("mynode") you want to query about its current world space
    // transformation(s).

    SoSearchAction * searchaction = new SoSearchAction;
    searchaction->setNode(current_node);
    searchaction->apply(root_sep);

    SoPath * path = searchaction->getPath();
    assert(path != NULL);

    // Then apply the SoGetMatrixAction to get the full transformation
    // matrix from world space.

    const SbViewportRegion vpr = SbViewportRegion();  // 
    SoGetMatrixAction * getmatrixaction = new SoGetMatrixAction(vpr);
    getmatrixaction->apply(path);

    SbMatrix transformation = getmatrixaction->getMatrix();
    // And if you want to access the individual transformation
    // components of the matrix:

    SbVec3f       translation;
    SbRotation    rotation;
    SbVec3f       scalevector;
    SbRotation    scaleorientation;
    transformation.getTransform(translation, rotation, scalevector, scaleorientation);
    
    return transformation;
}



void FractalTreeMaker::transformCoords(SoCoordinate3 * coords, SbMatrix matrix)
{
    int num_coords = coords->point.getNum();
    SbVec3f result;
    for (int j = 0; j < num_coords; j++) {
        matrix.multVecMatrix(coords->point[j], result);
        coords->point.set1Value(j, result);   // replace the old value with new one...
    } 
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
  searchaction->apply(fTreeRoot);

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
    // yes, this is the place to do it!
}
