#pragma once

// #define DO_TRACE

// interpolate scale coordinates (a set of scalars meant to be applied by percent_length of loftPathCoords->getTotalLength() defining the desired curve...but not related to 
// create a set of scale factors pertaining to their respective values along the loft centerline/path coordinates 
// otherwise, keep it dumb: do all transformations of parameters beforehand...

class SoCoordinate3;
class SoIndexedFaceSet;
class Flattener;
class SbVec3d;


class Extruder 
{
public:
    // member variables...    
    // input...
    SoSeparator         * fRoot;                    // pointer to the model loft root separator
    SoCoordinate3       * fShapeCoords;
    SoCoordinate3       * fLoftPathCoords;
    SoCoordinate3       * fHScaleCoords;            //
    SoCoordinate3       * fVScaleCoords;            // if fVScaleCoords is NULL we use fHScaleCoords for vertical scale
    SoCoordinate3       * fTwistCoords;             // can be NULL...
    bool                fClosedShape;
    //bool                fEqualScale;
    // processed...
    SoCoordinate3       * fLoftScaleCoords;
    SoCoordinate3       * fLoftTwistCoords;

    // output...
    SoCoordinate3       * fLoftCoords;               // we instantiate copies of these to add to model
    SoIndexedFaceSet    * fLoftFaces;

    float               fThickness;                  // maintain this constant thickness along extrusion
    int                 fUpperLimit;
    int                 fLowerLimit;
    float               fFractalScale;               // overall scale
	Flattener           * fFlattener;

// member functions:

    Extruder();
    ~Extruder();

// exposed functions...
public: 

    SoSeparator * extrude(SoCoordinate3 * shapeCoords, 
                          SoCoordinate3 * pathCoords, 
                          SoCoordinate3 * hScaleCoords, 
                          SoCoordinate3 * vScaleCoords,   // if NULL then hScaleCoords will be substituted
                          SoCoordinate3 * twistCoords,
		                  float           thickness,
                          bool flatten);

    SoSeparator * extrude_fractal(SoCoordinate3 * shapeCoords, 
                          SoCoordinate3 * pathCoords, 
                          SoCoordinate3 * hScaleCoords, 
                          SoCoordinate3 * vScaleCoords,   // if NULL then hScaleCoords will be substituted
                          SoCoordinate3 * twistCoords,
                          float fractal_scale, 
                          float thickness, 
                          bool flatten,
                          char * file_extension = NULL);

    SoSeparator * extrude_using_multiple_sections(SoCoordinate3 *section_1, SoCoordinate3 * section_2, int num_sides);

    void          makeLoftScaleCoords(void);
    void          makeLoftTwistCoords(void);
    void          makeLoftObject(void);
//  void          flatten(void);
    int           scaleCoordsForThickness(SoMFVec3f & sectionCoords);

    SoSeparator * createExtrusionNode(void);
 // private:
 // utilities...
 //   double      dist(SbVec3f pt1, SbVec3f pt2);
    SbVec3f     interpolateScale(double length /* really a ratio of length : full-length  */);
    SbVec3f     interpolateTwist(float length);
    double      GetTheta(SbVec3f pt1, SbVec3f pt2);
    double      GetTheta(double ptAx, double ptAy, double ptBx, double ptBy);
    double      GetTheta(double sideX, double sideY);
   // SbRotation  interpolatePRY(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3);
    SbRotation  interpolatePRY2(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3);
    SbRotation  getPRY(SbVec3f pt1, SbVec3f pt2);
    //SbVec3f     GetIntersection(SbVec3f ptA1, SbVec3f ptA2, SbVec3f ptB1, SbVec3f ptB2);

    // temporary functions for testing...
    void        print_matrix(char * label, SbMatrix mat);
    //void        trace_SoCoordinate3(char * label, SoCoordinate3 * coords);
   // void        trace_SbVec3f(char * label, SbVec3f coords);
   //  void        trace_SbMatrix(char * label, SbMatrix mat); // by observing the matrix's effect on a vector...
};
