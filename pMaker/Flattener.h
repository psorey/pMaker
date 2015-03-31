#if !defined(FLATTENER__H)
#define FLATTENER__H

// #define DO_TRACE

class SoCoordinate3;
class SoIndexedFaceSet;

class Flattener
{
public:
    // input...
    SoCoordinate3       *fCoords;               
    SoIndexedFaceSet    *fFaces;
    // output
    SoCoordinate3       *fFlatCoords;

// member functions:
    Flattener(void);
    ~Flattener();

public:                  
    static void          flatten_polylines(SoCoordinate3 * loftCoords, int numSides, int numPathCoords, CString filename);
    static void          flatten_polylines(SoCoordinate3 * loftCoords, int numSides, int numPathCoords);
    static void          flatten_quadrilaterals(SoCoordinate3 * loftCoords, int numSides, int numPathCoords);
    static void          flatten(SoCoordinate3 *inCoords, SoCoordinate3 * flatCoords);
    static double        GetVectorAngle(double a1, double a2, double a3, double b1, double b2, double b3);
};

#endif