#pragma once

class SoCoordinate3;
class SoIndexedFaceSet;
class SoNode;
class SoType;
class SoSeparator;
class SbVec3f;
class WriteDXF;
class SoMFVec3f;

class Flattener
{
public:
    // input...
    //SoCoordinate3       * fCoords;               
    //SoIndexedFaceSet    * fFaces;
    // output
    //SoCoordinate3       * fFlatCoords;
	SoCoordinate3         * fPlacedCoords;

// member functions:
    Flattener();
    ~Flattener();

public:  
    SbVec3f               GetVectorPoint(SbVec3f pt, float length, float theta);
	SbVec3f               findPointOnLine(SoCoordinate3 * line, float desired_distance);
	//void				  flatten_polylines(SoCoordinate3 * loftCoords, int numSides, int numPathCoords, CString filename);  // no
	//void                place_scaled_objects(SoCoordinate3 * placed_object_coords, SoCoordinate3* flat_coords);
    void				  flatten_polylines(SoCoordinate3 * loftCoords, int numSides, int numPathCoords);

    void                  flatten_quadrilaterals(SoCoordinate3 * loftCoords, int numSides, int numPathCoords);
    void                  flatten(SoCoordinate3 *inCoords, SoCoordinate3 * flatCoords);
	//SbVec3f               findPointOnLine(SoCoordinate3 * line1, float distance);
	double                GetVectorAngle(double a1, double a2, double a3, double b1, double b2, double b3);
	SoNode *              findChildByType(SoSeparator *parent, SoType typeId);
	void                  insertPlacedCoords(char * layer, int side, WriteDXF *writeDXF, SoCoordinate3 * line, float line1length, float begin_spacing, float end_spacing, float begin_scale, float end_scale);
	void                  intersect_placed_coords_with_line(SoCoordinate3 * insert_coords, SoCoordinate3 * line); // modifies insert_coords !

};

