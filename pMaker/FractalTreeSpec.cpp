#include "stdafx.h"
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
#include "FractalTreeSpec.h"
#include <Inventor/SbLinear.h>

#include <Inspect.h>

#include <iostream>
#include <fstream>
#include <string>

//using namespace std;


IMPLEMENT_SERIAL( FractalTreeSpec, CObject, 1 )


FractalTreeSpec::FractalTreeSpec(void){
    fShapeCoords         = new SoCoordinate3;                   // cross-section shape	
    fLeftCentCoords      = new SoCoordinate3;     
    fRightCentCoords     = new SoCoordinate3;     
    fLeftHScaleCoords    = new SoCoordinate3;     
    fRightHScaleCoords   = new SoCoordinate3;     
    fLeftVScaleCoords    = new SoCoordinate3;     
    fRightVScaleCoords   = new SoCoordinate3;        
    fLeftTwistCoords     = new SoCoordinate3;       
    fRightTwistCoords    = new SoCoordinate3;
    fShapeCoords->ref();              
    fLeftCentCoords->ref();  
    fRightCentCoords->ref(); 
    fLeftHScaleCoords->ref(); 
    fRightHScaleCoords->ref(); 
    fLeftVScaleCoords->ref(); 
    fRightVScaleCoords->ref();   
    fLeftTwistCoords->ref(); 
    fRightTwistCoords->ref();
}


FractalTreeSpec::~FractalTreeSpec(void){
    fShapeCoords->unref();              
    fLeftCentCoords->unref();  
    fRightCentCoords->unref(); 
    fLeftHScaleCoords->unref(); 
    fRightHScaleCoords->unref(); 
    fLeftVScaleCoords->unref(); 
    fRightVScaleCoords->unref();   
    fLeftTwistCoords->unref(); 
    fRightTwistCoords->unref();
}


FractalTreeSpec::FractalTreeSpec(        
                        float		            leftABRatio,
                        float                   rightABRatio,   
                        int		                numLevels,
                        float                   influenceDirection,    
                        CString                 rightFilename,
                        CString                 leftFilename,
                        float                 * leftCenterlineScale,
                        float                 * rightCenterlineScale,
                        float                 * leftThickness,
                        float                 * rightThickness,
                        float                 * leftRotation,
                        float                 * rightRotation,
                        float                 * leftInfluence,
                        float                 * rightInfluence,
                        BOOL                    useCenterlineScale,
                        BOOL                    useThickness,
                        BOOL                    useRotation,
                        BOOL                    useInfluence,
                        int                     useInfluenceType ) 
{
    fShapeCoords           = new SoCoordinate3;                   // cross-section shape	
    fLeftCentCoords        = new SoCoordinate3;     
    fRightCentCoords       = new SoCoordinate3;     
    fLeftHScaleCoords      = new SoCoordinate3;     
    fRightHScaleCoords     = new SoCoordinate3;     
    fLeftVScaleCoords      = new SoCoordinate3;     
    fRightVScaleCoords     = new SoCoordinate3;        
    fLeftTwistCoords       = new SoCoordinate3;       
    fRightTwistCoords      = new SoCoordinate3;       
    fLeftABRatio           = leftABRatio;
    fRightABRatio          = rightABRatio;
    fNumLevels             = numLevels;
    fInfluenceDirection    = influenceDirection;    
    fRightFilename         = rightFilename;
    fLeftFilename          = leftFilename; 
    fUseCenterlineScale    = useCenterlineScale;
    fUseThickness          = useThickness;
    fUseRotation           = useRotation;
    fUseInfluence          = useInfluence;
    fUseInfluenceType      = useInfluenceType;   // for storing in FractalTreeSpec...can we display in dialog as list box?
    fUseSpiralEffect       = FALSE;              // for sending to dialog - redundant when we use list box?
    fUseExponentialEffect  = FALSE;
    fUseLinearEffect       = FALSE;

    for (int i = 0; i < 9; i++) {
        fLCentScale[i]     = leftCenterlineScale[i];
		//TRACE("fLCentScale[%d] = %f\n", i, fLCentScale[i]); 
        fRCentScale[i]     = rightCenterlineScale[i];
        fLThick[i]         = leftThickness[i];
        fRThick[i]         = rightThickness[i];
        fLRot[i]           = leftRotation[i];
        fRRot[i]           = rightRotation[i];
        fLInfl[i]          = leftInfluence[i];
        fRInfl[i]          = rightInfluence[i];
    }

    fShapeCoords->ref();              
    fLeftCentCoords->ref();  
    fRightCentCoords->ref(); 
    fLeftHScaleCoords->ref(); 
    fRightHScaleCoords->ref(); 
    fLeftVScaleCoords->ref(); 
    fRightVScaleCoords->ref();   
    fLeftTwistCoords->ref(); 
    fRightTwistCoords->ref(); 

    this->ReadLDF(true,  fLeftFilename  );  // fill in the coordinates
    this->ReadLDF(false, fRightFilename );
    //this->listSpecs();
}

float FractalTreeSpec::getLevelRotationAmount(bool isLeft, int level) 
{
    float total_rot = 0.0;                 //
    if(fUseRotation) {                     //
        for(int i = 0; i <= level; i++) {  //
            if(isLeft)                     //
                total_rot += fLRot[i];     //
            else                           //
                total_rot += fRRot[i];     //
        }                                  //
        return total_rot  * .0174532925;   //
    } else {
        return 0.0;
    }
}

void  FractalTreeSpec::getCenterlineCoords(bool isLeft, SoCoordinate3 * coords, int level)
{
    if(isLeft) 
        coords->copyFieldValues(fLeftCentCoords);
    else       
        coords->copyFieldValues(fRightCentCoords);
}


SbMatrix  FractalTreeSpec::getCenterlineScaleMatrix(bool isLeft, int level)
{
    SbMatrix centerline_scale_matrix;
    centerline_scale_matrix.makeIdentity();
    if(isLeft)
        centerline_scale_matrix.setScale(fLCentScale[level]);
    else
        centerline_scale_matrix.setScale(fRCentScale[level]);
    return centerline_scale_matrix;
}


float  FractalTreeSpec::getFractalScale(bool isLeft, int level)
{
    if(isLeft){
        return (pow(fLeftABRatio, level));
       // return fLeftABRatio;
	}else{
        return (pow(fRightABRatio, level));
       // return fRightABRatio;
	}
}


SbMatrix  FractalTreeSpec::getFractalScaleMatrix(bool isLeft, int level)
{
    SbMatrix fractal_scale_matrix;
    fractal_scale_matrix.makeIdentity();
	//TRACE("ratio of level[%d] = %f\n", level, pow(fLeftABRatio, level));
    //Inspect::Matrix("before set scale", fractal_scale_matrix);

    if(isLeft) {

        fractal_scale_matrix.setScale(pow(fLeftABRatio, level));
	}else{
        fractal_scale_matrix.setScale(pow(fRightABRatio, level));
	}

    //Inspect::Matrix("after set scale", fractal_scale_matrix);

    return fractal_scale_matrix;
}


float  FractalTreeSpec::getCenterlineScale(bool isLeft, int level)
{
    if(isLeft)
        return fLCentScale[level];
    else
        return fRCentScale[level];
}




float  FractalTreeSpec::getEndTwist(bool isLeft, int level)
{
    float twist = 0.0;
    if (TRUE == isLeft) 
        return fLeftTwistCoords->point[fLeftTwistCoords->point.getNum()-1][1];
    else 
        return fRightTwistCoords->point[fRightTwistCoords->point.getNum()-1][1];
}

int FractalTreeSpec::getNumCenterlineCoords(bool isLeft)
{
    if(isLeft)
        return fLeftCentCoords->point.getNum();
    else
        return fRightCentCoords->point.getNum();
}


int FractalTreeSpec::getTotalCenterlineCoords(bool isLeft)
{
    if(isLeft)
        return fNumLevels * fLeftCentCoords->point.getNum();
    else
        return fNumLevels * fRightCentCoords->point.getNum();
}


float  FractalTreeSpec::getInfluence(bool isLeft, int level)
{
    if (TRUE == isLeft) 
        return fLInfl[level];
    else 
        return fRInfl[level];
}


SbVec3f FractalTreeSpec::getInfluenceVector(void)
{
    // create the desired influence direction vector from the influence direction...
    SbVec3f default_direction_vector(SbVec3f(0,1,0));  // default direction is along y-axis
    SbMatrix rot_matrix;
    rot_matrix.makeIdentity();                         
    rot_matrix.setRotate(SbRotation(SbVec3f(1,0,0), this->getInfluenceDirection())); // to get desired vector we rotate around the x-axis by radians
    SbVec3f desired_direction_vector;
    rot_matrix.multVecMatrix(default_direction_vector, desired_direction_vector);
    return desired_direction_vector;
}


float FractalTreeSpec::getInfluenceDirection()
{
    return fInfluenceDirection * .0174532925; 
}


CString FractalTreeSpec::listSpecs(CString filename)
{
    FILE * fp = fopen(filename,"w");
	fUseInfluenceType = 0; // !!! this is not being initiated elsewhere...
    CString returnString = "";
    char str[10000];
    sprintf(str,   "FractalTreeSpec: \n\n\
                    rightFilename:   %s\n\
                    leftFilename:    %s\n\
                    leftABRatio:     %f\n\
                    rightABRatio:    %f\n\
                    numLevels:       %d\n\
                    infl-dir:        %f\n\
                    use-cent-scale:  %d\n\
                    use-thk:         %d\n\
                    use-rot:         %d\n\
                    use-infl:        %d\n\
                    use-infl-type:   %d\n",  fRightFilename, fLeftFilename, fLeftABRatio, fRightABRatio, fNumLevels, 
                                             fInfluenceDirection, fUseCenterlineScale, fUseThickness, fUseRotation,
                                             fUseInfluence, fUseInfluenceType);
 
    fprintf(fp,   "rightFilename:   %s\n\
                    leftFilename:    %s\n\
                    leftABRatio:     %f\n\
                    rightABRatio:    %f\n\
                    numLevels:       %d\n\
                    infl-dir:        %f\n\
                    use-cent-scale:  %d\n\
                    use-thk:         %d\n\
                    use-rot:         %d\n\
                    use-infl:        %d\n\
                    use-infl-type:   %d\n",  fRightFilename, fLeftFilename, fLeftABRatio, fRightABRatio, fNumLevels, 
                                             fInfluenceDirection, fUseCenterlineScale, fUseThickness, fUseRotation,
                                             fUseInfluence, fUseInfluenceType);
    returnString += str;
    for(int i = 0; i < 9; i++) {
        char str[9000];
        sprintf(str, "Level %d\tLCentSc: %f\tRCentSc: %f\tLRot: %f\t\tRRot: %f \t\tLThick: %f \t\tRThick: %f \t\tLInfl: %f \t\tRInfl: %f\n",
            i, fLCentScale[i], fRCentScale[i], fLRot[i], fRRot[i], fLThick[i], fRThick[i], fLInfl[i], fRInfl[i]);
        returnString += str;
        fprintf(fp, "Level %d\tLCentSc: %f\tRCentSc: %f\tLRot: %f\t\tRRot: %f \t\tLThick: %f \t\tRThick: %f \t\tLInfl: %f \t\tRInfl: %f\n",
            i, fLCentScale[i], fRCentScale[i], fLRot[i], fRRot[i], fLThick[i], fRThick[i], fLInfl[i], fRInfl[i]);
    }
    fclose(fp);
    return returnString;
  }


void FractalTreeSpec::copyValues(FractalTreeSpec * fts)
{
	// TRACE("FractalTreeSpec::copy values\n");
    fLeftABRatio          = fts->fLeftABRatio;
    fRightABRatio         = fts->fRightABRatio;
    fNumLevels            = fts->fNumLevels;
    fInfluenceDirection   = fts->fInfluenceDirection;    
    fRightFilename        = fts->fRightFilename;
    fLeftFilename         = fts->fLeftFilename; 
    fUseCenterlineScale   = fts->fUseCenterlineScale;
    fUseThickness         = fts->fUseThickness;
    fUseRotation          = fts->fUseRotation;
    fUseInfluence         = fts->fUseInfluence;
    fUseInfluenceType     = fts->fUseInfluenceType;   
    fUseSpiralEffect      = fts->fUseSpiralEffect;          
    fUseExponentialEffect = fts->fUseExponentialEffect;
    fUseLinearEffect      = fts->fUseLinearEffect;

    for (int i = 0; i < 9; i++) {
        fLCentScale[i]     = fts->fLCentScale[i];
		//TRACE("FractalTreeSpec::fLCentScale[%d] = %f\n", i, fLCentScale[i]);
        fRCentScale[i]     = fts->fRCentScale[i];
        fLThick[i]         = fts->fLThick[i];
        fRThick[i]         = fts->fRThick[i];
        fLRot[i]           = fts->fLRot[i];
        fRRot[i]           = fts->fRRot[i];
        fLInfl[i]          = fts->fLInfl[i];
        fRInfl[i]          = fts->fRInfl[i];
    }

    fShapeCoords->copyFieldValues(fts->fShapeCoords);                   // cross-section shape	
    fLeftCentCoords->copyFieldValues(fts->fLeftCentCoords);     
    fRightCentCoords->copyFieldValues(fts->fRightCentCoords);     
    fLeftHScaleCoords->copyFieldValues(fts->fLeftHScaleCoords);     
    fRightHScaleCoords->copyFieldValues(fts->fRightHScaleCoords);     
    fLeftVScaleCoords->copyFieldValues(fts->fLeftVScaleCoords);
    fRightVScaleCoords->copyFieldValues(fts->fRightVScaleCoords);
    fLeftTwistCoords->copyFieldValues(fts->fLeftTwistCoords);
    fRightTwistCoords->copyFieldValues(fts->fRightTwistCoords);
}


void FractalTreeSpec::ReadLDF(bool isLeft, CString filename)
{
	SoInput myInput;
	myInput.openFile(filename);
    if(!myInput.isValidFile()) {
        AfxMessageBox("invalid Inventor file or file not found");
        return;
    }
    SoSeparator *tempSep = new SoSeparator;
	tempSep->ref();
    tempSep->addChild(SoDB::readAll(&myInput));
	myInput.closeFile();

    SoCoordinate3 *tSectionCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "sectionCoords");
 	SoCoordinate3 *tVScaleCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "vScaleCoords");
	SoCoordinate3 *tHScaleCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "hScaleCoords");
	SoCoordinate3 *tTwistCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "twistCoords");
	SoCoordinate3* tThreeDCoords = (SoCoordinate3*)findNodeByName(
		tempSep, "threeDCoords");    

    if(tSectionCoords != NULL) 
	    fShapeCoords->copyFieldValues(tSectionCoords);
    if(isLeft) {
	    if(tHScaleCoords != NULL)
		    fLeftHScaleCoords->copyFieldValues(tHScaleCoords);
	    if(tVScaleCoords != NULL)
		    fLeftVScaleCoords->copyFieldValues(tVScaleCoords);
	    if(tTwistCoords != NULL)
		    fLeftTwistCoords->copyFieldValues(tTwistCoords);
	    if(tThreeDCoords != NULL)
		    fLeftCentCoords->copyFieldValues(tThreeDCoords);
    } else {
	    if(tHScaleCoords != NULL)
		    fRightHScaleCoords->copyFieldValues(tHScaleCoords);
	    if(tVScaleCoords != NULL)
		    fRightVScaleCoords->copyFieldValues(tVScaleCoords);
	    if(tTwistCoords != NULL)
		    fRightTwistCoords->copyFieldValues(tTwistCoords);
	    if(tThreeDCoords != NULL)
		    fRightCentCoords->copyFieldValues(tThreeDCoords);
    }
    tempSep->unref();
}

SoNode * FractalTreeSpec::findNodeByName(SoGroup *parent, char *name)
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


void FractalTreeSpec::Serialize( CArchive& archive )
{
    // call base class function first 
    // base class is CObject in this case
    CObject::Serialize( archive );
	//TRACE("archive is storing = %d\n", archive.IsStoring());

    // now do the stuff for our specific class 
    if( archive.IsStoring() ) {
        archive << fLeftABRatio <<  fRightABRatio  <<    fNumLevels <<  fInfluenceDirection << fRightFilename \
                << fLeftFilename  << fUseCenterlineScale << fUseThickness << fUseRotation  << fUseInfluence  <<  fUseInfluenceType \
                << fUseSpiralEffect  <<  fUseExponentialEffect  <<  fUseLinearEffect;
        for(int i = 0; i < 9; i++)  {
            archive << fLCentScale[i]  << fRCentScale[i] << fLThick[i] \
                << fRThick[i]  <<  fLRot[i] <<  fRRot[i]  << fLInfl[i]  << fRInfl[i];
        }
    } else  {
        archive >> fLeftABRatio >>  fRightABRatio  >>    fNumLevels >>  fInfluenceDirection >> fRightFilename \
                >> fLeftFilename  >> fUseCenterlineScale >> fUseThickness >> fUseRotation  >> fUseInfluence  >>  fUseInfluenceType \
                >> fUseSpiralEffect  >>  fUseExponentialEffect  >>  fUseLinearEffect;
        for(int i = 0; i < 9; i++)  {
            archive >> fLCentScale[i]  >> fRCentScale[i] >> fLThick[i] \
                >> fRThick[i]  >>  fLRot[i] >>  fRRot[i]  >> fLInfl[i]  >> fRInfl[i];
        }
    }
	//TRACE("fLCentScale[0] = %f\n", fLCentScale[0]);
}

 