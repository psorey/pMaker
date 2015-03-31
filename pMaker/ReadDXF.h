#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>

class ReadDXF {
public:
    char                line[100];
    FILE                *ifp;
    int                 lnct;
    int                 count;
    int                 coordIndexNum;
    int                 vertexNum;
    SoSeparator         *parent;
    double              x,y,z,trash, x1,y1,z1,x2,y2,z2;
    //SoVertexProperty    *theVP;
    SoCoordinate3       *theCoords;
    SoIndexedLineSet    *theLines;
    //SoPointSet          *thePoints;

            ReadDXF( SoSeparator *_parent);
    CString    readFile();
    void    get_group ( char group_number[5] );
    void    get_xyz( void );
    void    getLINE(void);
    void    getPOLYLINE(void);
    void    getLWPOLYLINE(void);
    SbVec3f getWorld(SbVec3f pointVector /*point to transform*/, SbVec3f az /*arbitrary z-axis*/);

};