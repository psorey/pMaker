#ifndef __FRACTALTREESPEC_
#define __FRACTALTREESPEC_


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
class SoCoordinate3;


class FractalTreeSpec : public CObject
{
public: 

    DECLARE_SERIAL( FractalTreeSpec )
    
    enum {
        LINEAR,
        EXPONENTIAL,
        SPIRAL
    };

    SoCoordinate3 *	   fShapeCoords;                  // cross-section shape
    SoCoordinate3 *    fLeftCentCoords;               // centerline coords
    SoCoordinate3 *	   fRightCentCoords;              // 
    SoCoordinate3 *    fLeftHScaleCoords;             // 
    SoCoordinate3 *    fRightHScaleCoords;            // from pMaker x = 0-1  y = hori
    SoCoordinate3 *    fLeftVScaleCoords;             // 
    SoCoordinate3 *    fRightVScaleCoords;            // from pMaker x = 0-1  y = hori
    SoCoordinate3 *    fLeftTwistCoords;              // stored as x = 0-1  y = twist
    SoCoordinate3 *    fRightTwistCoords;             // 
    float		       fLeftABRatio;                  // ratio of scale from one level to the next 
    float              fRightABRatio;                 
    int		           fNumLevels;
    float              fInfluenceDirection;          
    CString            fRightFilename;
    CString            fLeftFilename;
    float              fLCentScale[9];
    float              fRCentScale[9];
    float              fLThick[9];
    float              fRThick[9];
    double             fLRot[9];
    double             fRRot[9];
    float              fLInfl[9];
    float              fRInfl[9];  
    BOOL               fUseCenterlineScale;
    BOOL               fUseThickness;
    BOOL               fUseRotation;
    BOOL               fUseInfluence;
    BOOL               fUseSpiralEffect;
    BOOL               fUseExponentialEffect;
    BOOL               fUseLinearEffect;
    int                fUseInfluenceType;



    FractalTreeSpec(    float	   leftABRatio,
                        float      rightABRatio,
                        int		   numLevels,
                        float      influenceDirection,   
                        CString    rightFilename,
                        CString    leftFilename,
                        float    * LCentScale,   // pointers to arrays
                        float    * RCentScale,
                        float    * LThick,
                        float    * RThick,
                        float    * LRot,
                        float    * RRot,
                        float    * LInfl,
                        float    * RInfl,
                        BOOL       useCenterlineScale,
                        BOOL       useThickness,
                        BOOL       useRotation,
                        BOOL       useInfluence,
                        int        useInfluenceType );

           FractalTreeSpec(void);
  virtual ~FractalTreeSpec(void);

  void      Serialize( CArchive& archive );
  CString   listSpecs(CString filename);
  float     getLevelRotationAmount(bool isLeft, int level);       // in radians
  float     getCenterlineScale(bool isLeft, int level);
  float     getFractalScale(bool isLeft, int level);
  SbMatrix  getCenterlineScaleMatrix(bool isLeft, int level);
  SbMatrix  getFractalScaleMatrix(bool isLeft, int level);
  float     getEndTwist(bool isLeft, int level);
  void      getCenterlineCoords(bool isLeft, SoCoordinate3 *coords, int level);
  float     getInfluence(bool isLeft, int level);
  float     getInfluenceDirection(void);                         // returns radians
  SbVec3f   getInfluenceVector(void); 
  int       getTotalCenterlineCoords(bool isLeft);
  int       getNumCenterlineCoords(bool isLeft);
  void      copyValues(FractalTreeSpec * source);
  void      ReadLDF(bool isLeft, CString filename);
  SoNode *  findNodeByName(SoGroup *parent, char *name);
};

#endif  // __FRACTALTREESPEC_
