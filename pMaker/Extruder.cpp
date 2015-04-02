#include "stdafx.h"
#include "FractalTreeMaker.h"
#include "Extruder.h"
#include "Flattener.h"
#include "Influence.h"
#include <Inspect.h>

#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/actions/SoWriteAction.h>


// for Fractal Tree making:
// pass Extruder a centerline path and typical section or shape, the h- and v-scale coords(unprocessed)
// centerline path is not modified (must be modified before passing)
// shape (section) will be scaled according to fractal scale, then according to h- and v-scale factors

// for normal extruding:
//
//


Extruder::Extruder(void)    // does no transformations on input data other than that necessary for 
                            // creating an extrusion as specified in the input
{
    fLoftCoords        = new SoCoordinate3;     // for output
    fLoftFaces         = new SoIndexedFaceSet;  // for output
    fLoftScaleCoords   = new SoCoordinate3;     // (horiz_scale, vert_scale, 0_not_used)
    fLoftTwistCoords   = new SoCoordinate3;     // (twist, 0_not_used, 0_not_used)
    fLoftCoords->ref(); //
    fLoftFaces->ref();
    fLoftScaleCoords->ref();
    fLoftTwistCoords->ref(); 
    fClosedShape       = TRUE;
    fFractalScale      = 1.0;
    fEqualScale        = TRUE;
    fThickness         = 0.0;
}

Extruder::~Extruder(){
    if(NULL != fLoftCoords) fLoftCoords->unref();
    if(NULL != fLoftFaces) fLoftFaces->unref();
    if(NULL != fLoftScaleCoords) fLoftScaleCoords->unref();
    if(NULL != fLoftTwistCoords) fLoftTwistCoords->unref();
}


SoSeparator * Extruder::extrude(SoCoordinate3 * shapeCoords, 
                                SoCoordinate3 * loftPathCoords, 
                                SoCoordinate3 * hScaleCoords, 
                                SoCoordinate3 * vScaleCoords,
                                SoCoordinate3 * twistCoords,
                                bool            flatten) 
{
    fShapeCoords     = shapeCoords;
    fLoftPathCoords  = loftPathCoords;
    fHScaleCoords    = hScaleCoords;
    fVScaleCoords    = vScaleCoords;
    fTwistCoords     = twistCoords;
    fFractalScale    = 1.0;   // already initialized in constructor...
    fThickness       = 0;       //

    this->makeLoftScaleCoords();        //
    this->makeLoftTwistCoords();        //
    this->makeLoftObject();             //
	TRACE("Extrude:\n");
	//Inspect::Coordinate3("fShapeCoords", fShapeCoords);
	//Inspect::Coordinate3("fVScaleCoords", fVScaleCoords);
    if(TRUE == flatten) 
         Flattener::flatten_polylines(this->fLoftCoords, this->fShapeCoords->point.getNum()-1, this->fLoftPathCoords->point.getNum());     

    SoSeparator        * returnNode = new SoSeparator;
    SoShapeHints       * hints = new SoShapeHints;

	hints->shapeType.setValue(SoShapeHints::UNKNOWN_SHAPE_TYPE);
	hints->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
    
    returnNode->addChild(hints);
    returnNode->addChild(this->createExtrusionNode()); // copy internal extrusion data create new scene subtree. 
    return returnNode;
}


SoSeparator * Extruder::extrude_fractal(SoCoordinate3 *shapeCoords, 
                                SoCoordinate3 *loftPathCoords, 
                                SoCoordinate3 * hScaleCoords, 
                                SoCoordinate3 * vScaleCoords,
                                SoCoordinate3 * twistCoords,
                                float fractal_scale, 
                                float thickness,
                                bool flatten,
                                char *file_extension ) 
{
    fShapeCoords     = shapeCoords;     //
    fLoftPathCoords  = loftPathCoords;  //
    fHScaleCoords    = hScaleCoords;    //
    fVScaleCoords    = vScaleCoords;    //
    fTwistCoords     = twistCoords;     //
    fFractalScale    = fractal_scale;   //
    fThickness       = thickness;       //
   // TRACE("Extrude::extrude_fractal\n");
	//TRACE("fFractal_scale = %f\n", fFractalScale);
	//Inspect::Coordinate3("fShapeCoords", fShapeCoords);
	//Inspect::Coordinate3("fHScaleCoords", fHScaleCoords);

    this->makeLoftScaleCoords();        //
    this->makeLoftTwistCoords();        //
    this->makeLoftObject();             //

    if(TRUE == flatten) 
         Flattener::flatten_polylines(this->fLoftCoords, this->fShapeCoords->point.getNum()-1, this->fLoftPathCoords->point.getNum());     

    SoShapeHints       * hints = new SoShapeHints;
	hints->shapeType.setValue(SoShapeHints::UNKNOWN_SHAPE_TYPE);
	hints->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
    SoSeparator       * returnNode = new SoSeparator;
    
    returnNode->addChild(hints);
    returnNode->addChild(this->createExtrusionNode()); // copy internal extrusion data to create new scene subtree. 
    return returnNode;
}

SoSeparator * Extruder::extrude_using_multiple_sections(SoCoordinate3 * section_1, SoCoordinate3 * section_2, int num_sides)
{
    fLoftCoords->point.deleteValues(0);
    fLoftFaces->coordIndex.deleteValues(0);
    int numShapeVertices = num_sides + 1;
    int numPathVertices  = 2;
    int fLoftCoordsCount = 0;

    // generate the lofted shape...
    for(int i = 0; i < numShapeVertices; i++) {
        SbVec3f point = section_1->point[i];
        fLoftCoords->point.set1Value(fLoftCoordsCount++, section_1->point[i]);
    }
    for(int i = 0; i < numShapeVertices; i++) {
        SbVec3f point = section_2->point[i];
        fLoftCoords->point.set1Value(fLoftCoordsCount++, section_2->point[i]);
    }
    // now generate IndexedFaceSet coordinate indices...
    int ii = 0;
    int jj = 0;
    int count = 0;    
    for( jj = 1; jj < numPathVertices + 1; jj++ ) {
      //  TRACE("jj = %d\n", jj);
        //int limit = 2 * numShapeVertices;
        int limit = numShapeVertices;
      //  TRACE("limit = %d\n", limit);

        for (ii = jj - 1; ii < limit; ii++) {
      //      TRACE("jj = %d   i = %d\n", jj, ii);
            fLoftFaces->coordIndex.set1Value(count++, ii);
            fLoftFaces->coordIndex.set1Value(count++, ii + 1);
            fLoftFaces->coordIndex.set1Value(count++, ii + numShapeVertices);
            fLoftFaces->coordIndex.set1Value(count++, -1);
            fLoftFaces->coordIndex.set1Value(count++, ii + 1);
            fLoftFaces->coordIndex.set1Value(count++, ii + numShapeVertices + 1);
            fLoftFaces->coordIndex.set1Value(count++, ii + numShapeVertices);
            fLoftFaces->coordIndex.set1Value(count++, -1);
            // ii += numShapeVertices - 1;
        }
    }
    Flattener::flatten_polylines(this->fLoftCoords, numShapeVertices - 1, numPathVertices); 

    SoWriteAction wa;
    wa.getOutput()->openFile("test.iv");
    wa.apply(fLoftCoords);
    
    SoLineSet* lineSet = new SoLineSet;
    lineSet->ref();
    wa.apply(lineSet);
    lineSet->unref();

    SoShapeHints       * hints = new SoShapeHints;
	hints->shapeType.setValue(SoShapeHints::UNKNOWN_SHAPE_TYPE);
	hints->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
    SoSeparator        * returnNode = new SoSeparator;
    returnNode->addChild(hints);
    returnNode->addChild(this->createExtrusionNode()); // copy internal extrusion data create new scene subtree. 
    returnNode->ref();
    wa.apply(returnNode);
    wa.getOutput()->closeFile();
    return returnNode;
}

// this only works for regular polygons centered on the origin !!!
int Extruder::scaleCoordsForThickness(SoMFVec3f & sectionCoords)
{
    // the input coords are scaled to the 'outer skin' of the design;
    // here, we are calculating the scale change needed to achieve the offset
    // from the outer skin to the inner surface.

    SbMatrix begin_matrix;
    begin_matrix.makeIdentity();

    SbLine line(sectionCoords[0], sectionCoords[1]);
    SbVec3f origin(0,0,0);
    SbVec3f close_point = line.getClosestPoint(origin);

    //Inspect::Vec3f("sectionCoords[0]", sectionCoords[0]);
    //Inspect::Vec3f("sectionCoords[1]", sectionCoords[1]);

    float dist = (close_point - origin).length();
    float ratio = (dist - fThickness) / dist;

    begin_matrix.setScale(ratio);

    // get two consecutive coords and scale the section shape..
    for(int i = 0; i < sectionCoords.getNum(); i++) {
        SbVec3f point = sectionCoords[i];
        SbVec3f scaled_point;
        begin_matrix.multVecMatrix(point, scaled_point);
        sectionCoords.set1Value(i, scaled_point);
    }   //(should be 'life size' now)
    return(1);
}


void Extruder::makeLoftScaleCoords()  // this only needs to be called when the loft changes...
{
    int numLoftPathCoords = fLoftPathCoords->point.getNum();
    // get total length of horiz line...
    int num_vscale_coords = fVScaleCoords->point.getNum();
    int num_hscale_coords = fHScaleCoords->point.getNum();

    double totalLength = 0.0;
    for (int i = 1; i < numLoftPathCoords; i++)
        totalLength += dist(fLoftPathCoords->point[i], fLoftPathCoords->point[i - 1]);
    double currentLength = 0.0;
    fLoftScaleCoords->point.deleteValues(0);
    fLoftScaleCoords->point.set1Value(0, fHScaleCoords->point[0][1], fVScaleCoords->point[0][1], 1.0); 
    for (int i = 1; i < numLoftPathCoords -1; i++) {
        currentLength += dist(fLoftPathCoords->point[i], fLoftPathCoords->point[i - 1]);
        SbVec3f sc = interpolateScale(currentLength / totalLength);
        fLoftScaleCoords->point.set1Value(i, sc);
        //TRACE("sc scales = %f\n", sc);
    }
    fLoftScaleCoords->point.set1Value(numLoftPathCoords - 1, 
        SbVec3f(fHScaleCoords->point[num_hscale_coords - 1][1], 
        fVScaleCoords->point[num_vscale_coords - 1][1], 1.0)); 
    return;
}


void Extruder::makeLoftTwistCoords()  // this only needs to be called when the loft changes...
{
    int numLoftPathCoords = fLoftPathCoords->point.getNum();
    // get total length of horiz line...
    int num_twist_coords = fTwistCoords->point.getNum();
    double totalLength = 0.0;
   // for (int i = 0; i < numLoftPathCoords - 1; i++)
    for (int i = 0; i < numLoftPathCoords; i++)
        totalLength += dist(fLoftPathCoords->point[i], fLoftPathCoords->point[i - 1]);
    double currentLength = 0.0;
    fLoftTwistCoords->point.deleteValues(0);
    fLoftTwistCoords->point.setValue(fTwistCoords->point[0][1], fTwistCoords->point[0][1], 1.0); 
    for (int i = 1; i < numLoftPathCoords -1; i++) {
        currentLength += dist(fLoftPathCoords->point[i], fLoftPathCoords->point[i - 1]);
        SbVec3f sc = interpolateTwist(currentLength / totalLength);
        fLoftTwistCoords->point.set1Value(i, sc);
    }
    fLoftTwistCoords->point.set1Value(numLoftPathCoords - 1, 
        SbVec3f(fTwistCoords->point[num_twist_coords - 1][1], 
        fTwistCoords->point[num_twist_coords - 1][1], 1.0)); 
    for (int i=0; i<fLoftTwistCoords->point.getNum(); i++) 
        SbVec3f pt = fLoftTwistCoords->point[i];
    //Inspect::WriteCoordinate3("test_twist", fLoftTwistCoords);
    return;
}


void Extruder::print_matrix(char * label, SbMatrix mat)
{
     FILE * fp = fopen("c:\matrix.txt", "a");
     mat.print(fp);
     fclose(fp);
}

// generate the loft and store it locally...
void Extruder::makeLoftObject() 
{
    fLoftCoords->point.deleteValues(0);
    fLoftFaces->coordIndex.deleteValues(0);
    // get number of shape vertices
    int numShapeVertices = fShapeCoords->point.getNum();  
    // get number of path vertices
    int numPathVertices  = fLoftPathCoords->point.getNum();  //loft path usually starts at 0,0,0
    int fLoftCoordsCount = 0;

    for (int i = 0; i < numPathVertices; i++) {
        SbMatrix mat;
        mat.makeIdentity();
        // apply twist...
        float twist = fLoftTwistCoords->point[i][1];
        SbRotation twistRotation(SbVec3f(0,0,1),twist);
        mat.setRotate(twistRotation);   // !!!! best place to twist!

        // rescale the section to give true corners...
        // this seems to work only in the x direction !!!
        if (/*fAntiSquish && */ i != 0 && i != numPathVertices - 1 ) {
            float deflection1 = atan2 (( fLoftPathCoords->point[i][1] - fLoftPathCoords->point[i-1][1]) , 
                (  fLoftPathCoords->point[i][0] - fLoftPathCoords->point[i-1][0] ));
            float deflection2 = atan2 (( fLoftPathCoords->point[i+1][1] - fLoftPathCoords->point[i][1]) , 
                (  fLoftPathCoords->point[i+1][0] - fLoftPathCoords->point[i][0] ));
            deflection1 -= deflection2;
            deflection1 /= 2;
            //if(abs(deflection1) < .0001) 
            //{} 
            SbMatrix mat00;
            float scaleFactor = fabs(1/cos(deflection1));
            mat00.setScale(SbVec3f(scaleFactor, 1, 1));
            mat *= mat00;
        }
        // apply the fractal scale factor (normally 1.0)
        SbMatrix fractal_scale_matrix;
        fractal_scale_matrix.setScale(fFractalScale);
        mat *= fractal_scale_matrix; 

        // apply the loft scale...
        int count = 0;
        SbMatrix mat0;
        mat0.setScale(fLoftScaleCoords->point[i]);
        mat *= mat0;
        SoMFVec3f scaledCoords;
        scaledCoords.deleteValues(0, -1);
        int scaledCoordsCount = 0;
        for (int j = 0; j < numShapeVertices; j++) {
            SbVec3f result;
            mat.multVecMatrix(fShapeCoords->point[j], result);
            scaledCoords.set1Value(j, result);			
        }
        if(fThickness > .0001)
            scaleCoordsForThickness(scaledCoords);

        // finished scaling and offsetting, now continue matrix operations...
        SbMatrix   mat1;
        SbMatrix   mat2;
        SbRotation rot;
        SbRotation rot3(SbVec3f(1,0,0), 1.5707963267);
        mat1.setRotate(rot3);
        SbVec3f translate = fLoftPathCoords->point[i];
        mat2.setTranslate(translate);
        // rot = pitch, roll, yaw
        if (0 == i)
            rot = this->getPRY(fLoftPathCoords->point[i], fLoftPathCoords->point[i+1]);
        else if (i == numPathVertices - 1)
            rot = this->getPRY(fLoftPathCoords->point[i-1], fLoftPathCoords->point[i]);
        else
            rot = this->interpolatePRY2(fLoftPathCoords->point[i - 1], 
                                        fLoftPathCoords->point[i],
                                        fLoftPathCoords->point[i + 1]);
        SbMatrix mat3;
        mat3.setRotate(rot);
        SbMatrix finalMat;
        finalMat.makeIdentity();
        finalMat *= mat1;  // rotate 90 degrees in the x axis
        finalMat *= mat3;  // rotate half the interior vector angle
        finalMat *= mat2;  // translate the shape to the loft path coordinates
        
        // generate the lofted shape at the current path vertex...
        for (int j = 0; j < numShapeVertices; j++) {
            SbVec3f result;
            finalMat.multVecMatrix(scaledCoords[j], result);
            fLoftCoords->point.set1Value(fLoftCoordsCount++, result);
        }
    }

                        // AND generate coord3 that follows triangles
    // now generate IndexedFaceSet coordinate indices...
    SoCoordinate3 * new_coords = new SoCoordinate3;
    new_coords -> ref();

    int count = 0;
    fUpperLimit = fLoftPathCoords->point.getNum()-1;
    fLowerLimit = 0;
    for( int jj = 0; jj < numShapeVertices - 1; jj++) {
        for (int i = jj + fLowerLimit * numShapeVertices; i < jj + fUpperLimit * numShapeVertices; i++) {
            fLoftFaces->coordIndex.set1Value(count++, i);
            fLoftFaces->coordIndex.set1Value(count++, i + 1);
            fLoftFaces->coordIndex.set1Value(count++, i + numShapeVertices);
            fLoftFaces->coordIndex.set1Value(count++, -1);
            fLoftFaces->coordIndex.set1Value(count++, i + 1);
            fLoftFaces->coordIndex.set1Value(count++, i + numShapeVertices + 1);
            fLoftFaces->coordIndex.set1Value(count++, i + numShapeVertices);
            fLoftFaces->coordIndex.set1Value(count++, -1);
            i += numShapeVertices - 1;
        }
    }
    SoIndexedFaceSet * aFaces = new SoIndexedFaceSet;
    aFaces->ref();
	SoIndexedLineSet * aLines = new SoIndexedLineSet;
	aLines->ref();
    count = 0;
	int count_lines = 0;
    // for( int jj = 0; jj < numShapeVertices - 1; jj++) {    
    for( int jj = 0; jj < 1; jj++) {
        for (int i = jj + fLowerLimit * numShapeVertices; i < jj + fUpperLimit * numShapeVertices; i++){
            aFaces->coordIndex.set1Value(count++, i);
            aFaces->coordIndex.set1Value(count++, i + 1);
            aFaces->coordIndex.set1Value(count++, i + 2);
            aFaces->coordIndex.set1Value(count++, -1);
            aFaces->coordIndex.set1Value(count++, i );
            aFaces->coordIndex.set1Value(count++, i + 2);
            aFaces->coordIndex.set1Value(count++, i + 3);
            aFaces->coordIndex.set1Value(count++, -1);
            if(i > 0) {
				aLines->coordIndex.set1Value(count_lines++, i-4);
				aLines->coordIndex.set1Value(count_lines++, i);
				aLines->coordIndex.set1Value(count_lines++, -1);

				aLines->coordIndex.set1Value(count_lines++, i-4);
				aLines->coordIndex.set1Value(count_lines++, i+1);
				aLines->coordIndex.set1Value(count_lines++, -1);
				
				aLines->coordIndex.set1Value(count_lines++, i-3);
				aLines->coordIndex.set1Value(count_lines++, i+2);
				aLines->coordIndex.set1Value(count_lines++, -1);

				aLines->coordIndex.set1Value(count_lines++, i -2);
				aLines->coordIndex.set1Value(count_lines++, i+3);
				aLines->coordIndex.set1Value(count_lines++, -1);
			}
			aLines->coordIndex.set1Value(count_lines++, i);
			aLines->coordIndex.set1Value(count_lines++, i+1);
			aLines->coordIndex.set1Value(count_lines++, i+2);
			aLines->coordIndex.set1Value(count_lines++, i+3);
			aLines->coordIndex.set1Value(count_lines++, i+4);
			aLines->coordIndex.set1Value(count_lines++, -1);
			i += numShapeVertices - 1;
        }
    }

    SoWriteAction wa;
    wa.getOutput()->openFile("sails.iv");
    wa.apply(fLoftCoords);
    wa.apply(aFaces);
	wa.apply(aLines);
    wa.getOutput()->closeFile();
    aFaces->unref();
	aLines->unref();
}


SbVec3f Extruder::CalcVectorPoint(SbVec3f pt, float length, float theta)
{
	SbVec3f temp;
	temp[0] = pt[0] + length * cos(theta);
	temp[1] = pt[1] + length * sin(theta);
	temp[2] = 0.0;
	return temp;
}

SbVec3f Extruder::GetIntersection(SbVec3f ptA1, SbVec3f ptA2, SbVec3f ptB1, SbVec3f ptB2)
{
	// Segment intersection formulae; see the Antonio reference...
	float tx = ptA2[0] - ptA1[0];
	float ty = ptA2[1] - ptA1[1];
	float ex = ptB2[0] - ptB1[0];
	float ey = ptB2[1] - ptB1[1];
	float etx = ptA1[0] - ptB2[0];
	float ety = ptA1[1] - ptB2[1];
  
	float denom = ty * ex - tx * ey;
	ASSERT(denom != 0.0); 

	float split = (ey * etx - ex * ety) / denom;
	// Create the new point...
	SbVec3f newpoint;
	// Interpolate its coordinate and attributes... 
	//newpoint[i] = torg[i] + split * (tdest[i] - torg[i]);
	newpoint[0] = ptA1[0] + split * (ptA2[0] - ptA1[0]);
	newpoint[1] = ptA1[1] + split * (ptA2[1] - ptA1[1]);
	return newpoint;
}

SoSeparator * Extruder::createExtrusionNode(void)
{
    SoSeparator       *brRoot = new SoSeparator;         
    SoCoordinate3     *loftCoords = new SoCoordinate3;
    SoIndexedFaceSet  *loftFaces  = new SoIndexedFaceSet;
    loftCoords->copyFieldValues(fLoftCoords);    
    loftFaces->copyFieldValues(fLoftFaces);
    brRoot->addChild(loftCoords);
    brRoot->addChild(loftFaces);
    return brRoot;
}

SbVec3f Extruder::interpolateScale(float length /* really a ratio of length : full-length  */)
{
    // we take the y-value and interpolate a z-value...
    int num_hscale_coords = fHScaleCoords->point.getNum();
    float hLength = length * fHScaleCoords->point[num_hscale_coords -1][0];
    float hScale = 1.0;
    float vScale = 1.0;
    if (num_hscale_coords == 1) {
        hScale = fHScaleCoords->point[0][1];
    } else {
        for (int i = 0; i < num_hscale_coords; i++){
            if (fHScaleCoords->point[i][0] == hLength){
                hScale = fHScaleCoords->point[i][1];
                TRACE("case 3\n");
                i = num_hscale_coords;
            }
            else if (fHScaleCoords->point[i][0] > hLength){
                if (i == 0) {
                    hScale = fHScaleCoords->point[i][1];
                    i = num_hscale_coords;
                }
                else {
                    float x2 = fHScaleCoords->point[i][0];
                    float x1 = fHScaleCoords->point[i - 1][0];
                    float ratio = (length - x1) / (x2 - x1);
                    float z2 = fHScaleCoords->point[i][1];
                    float z1 = fHScaleCoords->point[i - 1][1];
                    float z = (z2 - z1) * ratio + z1;
                    hScale = z;
                    i = num_hscale_coords;
                }
            }
        }
    }
    if (NULL == fVScaleCoords || ((fabs(fVScaleCoords->point[0][1]) - 1.0) < .001 && (fabs(fVScaleCoords->point[1][1]) - 1.0) < .001)) {      // we're done...
        vScale = hScale;
        return SbVec3f(hScale, vScale, 1);
    }
    int num_vscale_coords = fVScaleCoords->point.getNum();
    float vLength = length * fVScaleCoords->point[num_vscale_coords -1][0];

    if (num_vscale_coords == 1) {
        vScale = fVScaleCoords->point[0][1];
    } else if (length >= 1) { 
        vScale = fVScaleCoords->point[num_vscale_coords - 1][1];
    } else {
        for (int i = 0; i < num_vscale_coords; i++) {
            if (fVScaleCoords->point[i][0] == vLength) {
                vScale = fVScaleCoords->point[i][1];
                TRACE("case 3!\n");
                i = num_vscale_coords;
            }
            else if (fVScaleCoords->point[i][0] > vLength) {
                if (i == 0){
                    vScale = fVScaleCoords->point[i][1];
                    i = num_vscale_coords;
                }
                else {
                    float x2 = fVScaleCoords->point[i][0];
                    float x1 = fVScaleCoords->point[i - 1][0];
                    float ratio = (length - x1) / (x2 - x1);
                    float z2 = fVScaleCoords->point[i][1];
                    float z1 = fVScaleCoords->point[i - 1][1];
                    float z = (z2 - z1) * ratio + z1;
                    vScale = z;
                    i = num_vscale_coords;
                }
            }
        }
    }
    return SbVec3f(hScale, vScale, 1);
}


SbVec3f Extruder::interpolateTwist(float length /* really a ratio of length : full-length  */)
{
    // we take the y-value and interpolate a z-value...
    int num_twist_coords = fTwistCoords->point.getNum();
    float tTwist = 1.0;

    if (num_twist_coords == 1) {
        tTwist = fTwistCoords->point[0][1];
    } else {
        for (int i = 0; i < num_twist_coords; i++){
             if (fTwistCoords->point[i][0] > length) {
                if (i == 0) {
                    tTwist = fTwistCoords->point[i][1];
                    i = num_twist_coords;
                }
                else {
                    float x2 = fTwistCoords->point[i][0];
                    float x1 = fTwistCoords->point[i - 1][0];
                    float ratio = (length - x1) / (x2 - x1);
                    float z2 = fTwistCoords->point[i][1];
                    float z1 = fTwistCoords->point[i - 1][1];
                    float z = (z2 - z1) * ratio + z1;
                    tTwist = z;
                    i = num_twist_coords;
                }
            }
        }
    }
    return SbVec3f(tTwist, tTwist, 0.0);
}


float Extruder::dist(SbVec3f pt1, SbVec3f pt2)
{
    float   distance;
    distance = sqrt((pt2[0] - pt1[0])*(pt2[0] - pt1[0]) \
        +(pt2[1] - pt1[1])*(pt2[1] - pt1[1]));
    return distance;
}

double Extruder::GetTheta(SbVec3f pt1, SbVec3f pt2)
{
    //TRACE("GetTheta1\n");
    return  atan2 (( pt2[1] - pt1[1]) , ( pt2[0] - pt1[0] ));   
}

double Extruder::GetTheta(double ptAx, double ptAy, double ptBx, double ptBy)
{
   // TRACE("GetTheta2\n");
    return atan2 (( ptBy - ptAy ) , ( ptBx - ptAx));
}

double Extruder::GetTheta(double sideX, double sideY)
{
    //TRACE("GetTheta3\n");
    return atan2 (sideY, sideX);
}

SbRotation Extruder::interpolatePRY2(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3)
{
    //TRACE("interpPRY 2\n");
    // doing it this way eliminates any roll...
    SbRotation horizRot1(SbVec3f(0,1,0), SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0));
    SbRotation horizRot2(SbVec3f(0,1,0), SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0));

    SbRotation vertRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
    SbRotation vertRot2(SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0), pt3 - pt2);

    //SbRotation rollRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
    //SbRotation rollRot2(SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0), pt3 - pt2);

    horizRot1 *= vertRot1;
    horizRot2 *= vertRot2;
    SbRotation result = SbRotation::slerp(horizRot1, horizRot2, .5);   //  !!!!! Oct 2004
    //SbRotation result = horizRot1;   // !!!!!  
    return result;
}

SbRotation Extruder::getPRY(SbVec3f pt1, SbVec3f pt2)
{
    //TRACE("getPRY\n");
    SbRotation horizRot1(SbVec3f(0,1,0), SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0));
    SbRotation vertRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
    horizRot1 *= vertRot1;
    return horizRot1;
}

