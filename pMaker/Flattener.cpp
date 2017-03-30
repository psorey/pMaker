#include "stdafx.h"
#include "Flattener.h"
//#include "Influence.h"
#include <WriteDXF.h>


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
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>

//Flattener::Flattener(void);

Flattener::Flattener(void) {
	fPlacedCoords = NULL;
	SoInput myInput;
	myInput.openFile("c:\\placed_coords.iv");
	if (!myInput.isValidFile()) {
		AfxMessageBox("no placed_coords.iv file...");
		return;
	}
	SoSeparator *tempSep = new SoSeparator;
	tempSep->ref();
	tempSep->addChild(SoDB::readAll(&myInput));
	myInput.closeFile();

	SoCoordinate3 *tempCoords = (SoCoordinate3 *)findChildByType(tempSep, SoCoordinate3::getClassTypeId());
	if(tempCoords != NULL){
		if (fPlacedCoords == NULL) fPlacedCoords = new SoCoordinate3;
		fPlacedCoords->ref();
		fPlacedCoords->copyFieldValues(tempCoords);
	}
	tempSep->unref();
}


Flattener::~Flattener(void) {
	if (fPlacedCoords != NULL) fPlacedCoords->unref();
}


SbVec3f Flattener::GetVectorPoint(SbVec3f pt, float length, float theta)
{
	SbVec3f temp;
	temp[0] = pt[0] + length * cos(theta);
	temp[1] = pt[1] + length * sin(theta);
	temp[2] = 0.0;
	return temp;
}




///////// * Using this one  * ///////////
void Flattener::flatten_polylines(SoCoordinate3 * loftCoords, int numSides, int numPathCoords) 
{
	// create new DXF file for flattened sides...
	BOOL    bAddFileDialog = FALSE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = "dxf";
	LPCTSTR lpszFileName = "*.dxf";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;

	CFileDialog addFileDialog(bAddFileDialog,
	  lpszDefExt, lpszFileName, dwFlags,
	  lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("create a flattened-sides DXF file...");
	int nModal = addFileDialog.DoModal();
	CString m_strAddFile;
	if (nModal != IDOK) return;
	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

	SoCoordinate3* flatCoords = new SoCoordinate3;
	flatCoords->ref();
	
	SoSeparator* tempSep = new SoSeparator;
	tempSep->ref();
    SoCoordinate3 *newCoords = new SoCoordinate3;
	tempSep->addChild(newCoords);
	SoLineSet *newLines = new SoLineSet;
	tempSep->addChild(newLines);
    
	int indexCount = 0;
	int index;

	FILE* fp = fopen(m_strAddFile, "w");
    if(fp == NULL) return;
	WriteDXF * writeDXF = new WriteDXF(fp); // opens the file and writes the begin section
    writeDXF->WriteBeginDXF();

	for(int j = 0; j < numSides; j++) {
        int count = 0;
		newCoords->point.deleteValues(0);
		flatCoords->point.deleteValues(0);
		for ( int k = j; k < loftCoords->point.getNum() - 1; k += (numSides + 1)) {
            //TRACE("k =  %d\n", k);
			newCoords->point.set1Value(count++, loftCoords->point[k]);
			newCoords->point.set1Value(count++, loftCoords->point[k + 1]);
		}
		flatten(newCoords, flatCoords);
        TRACE("numNewCoords = %d   numFlatCoords = %d\n", newCoords->point.getNum(), flatCoords->point.getNum());   

		int numVertices = flatCoords->point.getNum();
		char layer[4];
		_itoa( j, layer, 10);
        int marker_spacing = 10;
        float line_width = 0.0;
        int count2 = 0;
        writeDXF->WriteLWPOLYLINEHeader(layer, numVertices); 
		for (int i = 0; i < numVertices; i += 2) {
            count2++;
			SbVec3f point = flatCoords->point[i];
            if(count2 % marker_spacing == 0) 
                writeDXF->WriteLWPOLYLINEPoint(point, 0.2);
            else
                writeDXF->WriteLWPOLYLINEPoint(point, 0.0);
		}
        count2 = 0;
		for (int i = numVertices - 1; i > 0; i -= 2) {
            count2++;
			SbVec3f point = flatCoords->point[i];
            if(count2 % marker_spacing == 0) 
                writeDXF->WriteLWPOLYLINEPoint(point, 0.2);
            else
                writeDXF->WriteLWPOLYLINEPoint(point, 0.0);
		}
        writeDXF->WriteZero();
	}
	//////////////////////////////////////////////////////////////////////////////
	// if an object is being placed on the flattened geometry...
	if (fPlacedCoords != NULL) {
		// place_scaled_objects(flatCoords);  // place the 
		// split the flattened polyline into two lines...
		SoCoordinate3 * line1 = new SoCoordinate3;
		SoCoordinate3 * line2 = new SoCoordinate3;
		line1->ref();
		line2->ref();
		int count = 0;
		int num = flatCoords->point.getNum();
		for (int i = 0; i < num; i+=2 )  {
			line1->point.set1Value(count++, flatCoords->point[i]);
		}
		count = 0;
		for (int i = 1; i < num; i+=2) {
			line2->point.set1Value(count++ , flatCoords->point[i]);
		}
		// calculate total lengths of lines
		double line1length = 0.0;
		double line2length = 0.0;
		for (int i = 0; i < line1->point.getNum() - 1; i++) {
			line1length += (line1->point[i + 1] - line1->point[i]).length();
		}
	    for (int i = 0; i < line2->point.getNum() - 1; i++) {
		    line2length += (line2->point[i + 1] - line2->point[i]).length();
	    }
		TRACE("line1length = %f\n", line1length);
		TRACE("line2length = %f\n", line2length);
		writeDXF->WriteLWPOLYLINE(line1, "line1", -1);
		writeDXF->WriteLWPOLYLINE(line2, "line2", -1);
 
		float begin_spacing = 1.5; // the starting distance between placed objects
		float end_spacing = 1.1;
		float begin_scale = 1;
		float end_scale = .68;
		float current_distance = 0.0;   // 
		// place the first at distance 0

		//SoMFVec3f placed_polyline;
		SbMatrix mat;
		float distance = 0.0;
		//TRACE("line1length = %f\n", line1length);
		insertPlacedCoords(0, writeDXF, line1, line1length, begin_spacing, end_spacing, begin_scale, end_scale);
		insertPlacedCoords(1,  writeDXF, line2, line2length, begin_spacing, end_spacing, begin_scale, end_scale);

		line1->unref();
		line2->unref();
	}
	////////////////////////////////////////////////////////////////////////////////
    writeDXF->WriteDXFEndsec();
    writeDXF->WriteEndDXF();
	fclose(fp);
	tempSep->unref();
	flatCoords->unref();
}	


void Flattener::insertPlacedCoords(int side, WriteDXF * writeDXF, SoCoordinate3 *line, float linelength, float begin_spacing, float end_spacing, float begin_scale, float end_scale) {
	float distance = 0.0;
	float current_distance = 0.0;
	while (distance < linelength) {
		float ratio = distance / linelength;
		float scale = begin_scale - ((begin_scale - end_scale) * ratio);
		float spacing = begin_spacing - ((begin_spacing - end_spacing) * ratio);

		current_distance = 0.0;
		// which line segment are we looking at
		int point = 0;
		for (point = 0; current_distance < distance; point++) {
			current_distance += (line->point[point + 1] - line->point[point]).length();
		}
		TRACE("current point = %d     numPoints = %d    \n", point, line->point.getNum());
		SbVec3f vector = line->point[point + 1] - line->point[point];
		float rot = atan2(vector[1], vector[0]);
		// find 'insertion' point, rotation and scale
		SbVec3f insert_point = findPointOnLine(line, distance);
		SbRotation rotation;
		if (side == 0) {
			rotation = SbRotation(SbVec3f(0, 0, 1), rot + 1.507);
		}
		else {
			rotation = SbRotation(SbVec3f(0, 0, 1), rot - 1.507);
		}
		SbMatrix mat;
		mat.setTransform(insert_point, rotation, SbVec3f(scale, scale, scale));
		SoCoordinate3 * insert_coords = new SoCoordinate3;
		insert_coords->ref();
		for (int i = 0; i < fPlacedCoords->point.getNum(); i++) {
			SbVec3f transformed_coord;
			mat.multVecMatrix(fPlacedCoords->point[i], transformed_coord);
			insert_coords->point.set1Value(i, transformed_coord);
		}
		writeDXF->WriteLWPOLYLINE(insert_coords, "insert", -1);
		insert_coords->unref();
		distance += spacing;
		//TRACE("distance = %f\n", distance);
	}
}


SbVec3f Flattener::findPointOnLine(SoCoordinate3 * line, float desired_distance) {
	SbVec3f point;
	float current_distance = 0.0;
	for (int i = 0; i < line->point.getNum() - 1; i++) {
		float current_segment_length = (line->point[i + 1] - line->point[i]).length();
		current_distance += current_segment_length;
		if (current_distance > desired_distance) {
			float portion = current_segment_length - (current_distance - desired_distance);
			SbVec3f vector = line->point[i + 1] - line->point[i];
			float theta = atan2(vector[1], vector[0]);
			point = GetVectorPoint(line->point[i], portion, theta);
			return point;
		}
	}
}



// UNUSED..............................................................................
// not using this one:
/*void Flattener::flatten_polylines(SoCoordinate3 * loftCoords, int numSides, int numPathCoords, CString filename) 
{
	SoCoordinate3* flatCoords = new SoCoordinate3;
	flatCoords->ref();
	SoSeparator* tempSep = new SoSeparator;
	tempSep->ref();
    SoCoordinate3 *newCoords = new SoCoordinate3;
	tempSep->addChild(newCoords);
	SoLineSet *newLines = new SoLineSet;
	tempSep->addChild(newLines);
	
    int indexCount = 0;
	int index;
    // bool fClosedShape = false; // we treat it as a n-sided extrusion, not necessarily closed

	FILE* fp = fopen(filename, "w");
	WriteDXF * writeDXF = new WriteDXF(fp); // opens the file and writes the begin section
    writeDXF->WriteBeginDXF();

	for(int j = 0; j < numSides; j++) {
        int count = 0;
        TRACE("count = %d\n", count);

		newCoords->point.deleteValues(0);
		flatCoords->point.deleteValues(0);
		for ( int k = j; k < loftCoords->point.getNum() - 1; k += (numSides + 1)) {
            //TRACE("k =  %d\n", k);
			newCoords->point.set1Value(count++, loftCoords->point[k]);
			newCoords->point.set1Value(count++, loftCoords->point[k + 1]);
		}
		flatten(newCoords, flatCoords);  

		int numVertices = flatCoords->point.getNum();
		char layer[10];
		_itoa( j, layer, 10);

        writeDXF->WriteLWPOLYLINEHeader(layer, numVertices); 
		for (int i = 0; i < numVertices; i+= 2) {		
			SbVec3f point = flatCoords->point[i];
            writeDXF->WriteLWPOLYLINEPoint(point);
		}
		// for (int i = numVertices - 2; i > 0; i -= 2) {
		for (int i = numVertices - 1; i > 0; i -= 2) {
			SbVec3f point = flatCoords->point[i];
            writeDXF->WriteLWPOLYLINEPoint(point);
		}
        writeDXF->WriteZero();
	}
    writeDXF->WriteDXFEndsec();
    writeDXF->WriteEndDXF();
	fclose(fp);
	tempSep->unref();
	flatCoords->unref();
}
*/



void Flattener::flatten_quadrilaterals(SoCoordinate3 * loftCoords, int numSides, int numPathCoords)  // quadrilaterals
{
	// create new DXF file for flattened sides...
	BOOL    bAddFileDialog = FALSE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = "dxf";
	LPCTSTR lpszFileName = "*.dxf";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;

	CFileDialog addFileDialog(bAddFileDialog,
	  lpszDefExt, lpszFileName, dwFlags,
	  lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("create a flattened-quadrilaterals DXF file...");
	int nModal = addFileDialog.DoModal();
	CString m_strAddFile;

	if (nModal != IDOK) return;
	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

    TRACE("just before fileopen\n");
	FILE* fp = fopen(m_strAddFile, "w");
	//fprintf(fp,"  0\nSECTION\n  2\nENTITIES\n");

	SoCoordinate3 *newCoords = new SoCoordinate3;
	newCoords->ref();
	SoCoordinate3* flatCoords = new SoCoordinate3;
	flatCoords->ref();
	
	SoSeparator* tempSep = new SoSeparator;
	tempSep->ref();
	tempSep->addChild(newCoords);
	SoLineSet *newLines = new SoLineSet;
	tempSep->addChild(newLines);
	int indexCount = 0;
	int index;
    bool fClosedShape = true;

    int entity_count = 444;
	for(int j = 0; j < numSides; j++) {
		int count = 0;
		newCoords->point.deleteValues(0, -1);
		flatCoords->point.deleteValues(0, -1);

		for ( int k = j; k < loftCoords->point.getNum() - 1; k += numSides + 1)
		{
			if (j < numSides) {
				newCoords->point.set1Value(count++, loftCoords->point[k]);
				newCoords->point.set1Value(count++, loftCoords->point[k + 1]);
			}
			else if (fClosedShape == TRUE) {
				newCoords->point.set1Value(count++, loftCoords->point[k]);
				newCoords->point.set1Value(count++, loftCoords->point[k - numSides + 1]);
			}
		}
		flatten(newCoords, flatCoords);
		int numVertices = flatCoords->point.getNum();
		char layer[4];
		_itoa( j, layer, 10);
		for(int i = 0; i < numVertices - 3; i += 2) {
			fprintf(fp,"LWPOLYLINE\n  5\n%d\n100\nAcDbEntity\n  8\n%s\n100\n", entity_count++, layer);
            // thicken every tenth rectangle...
            // if(i % 20 != 0) 
			    fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n     1\n 43\n0.0\n", 4);
		   // else 
           // fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n     1\n 43\n0.2\n", 4);
			SbVec3f point = flatCoords->point[i];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);

			point = flatCoords->point[i + 2];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);

			point = flatCoords->point[i + 3];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);

			point = flatCoords->point[i +1];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);

			//point = flatCoords->point[i];
			//fprintf(fp," 10\n");
			//fprintf(fp,"%f\n", point[0]);
			//fprintf(fp," 20\n");
			//fprintf(fp,"%f\n", point[1]);
            fprintf(fp, "  0\n");  // !!!
		}
	}
	// fprintf(fp,"  0\nENDSEC\n  0\nEOF\n");
	fclose(fp);
	tempSep->unref();
	flatCoords->unref();
	newCoords->unref();

	char commandString[100];
    system("cd \"C:\\3000 ArtProjects\\046 Maryland\\tree\"");
    // chdir("c:\\3000 ArtProjects\\046 Maryland\\tree");
    sprintf(commandString, "ruby join_dxf.rb\n");
    //TRACE("%s\n", system(commandString));
    //char ret[200];
    system(commandString);
    //TRACE("%s\n", ret);
}

void Flattener::flatten (SoCoordinate3* inCoords, SoCoordinate3* flatCoords)
{
    // flattens a zig-zag line connecting two edges of an extrusion.
	const double kPI = 3.1415926535897932384626433832795;
	double xplot1, xplot2, xplot3;
	double yplot1, yplot2, yplot3, yplot4;
	double side3;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	double ang1, ang2, angA, angB;
	double r;

	xplot1 = 0.0;
	yplot1 = 0.0;
	int flatCount = 0;
	double angle1;
	double angle2;

	x1 = inCoords->point[0][0];
	y1 = inCoords->point[0][1];
	z1 = inCoords->point[0][2];
	x2 = inCoords->point[1][0];
	y2 = inCoords->point[1][1];
	z2 = inCoords->point[1][2];
	x3 = inCoords->point[2][0];
	y3 = inCoords->point[2][1];
	z3 = inCoords->point[2][2];
	x4 = inCoords->point[3][0];
	y4 = inCoords->point[3][1];
	z4 = inCoords->point[3][2];
	
	double a1 = x1 - x2;
	double a2 = y1 - y2;
	double a3 = z1 - z2;
	double b1 = x3 - x2;
	double b2 = y3 - y2;
	double b3 = z3 - z2;
	
	angle2 = GetVectorAngle(a1, a2, a3, b1, b2, b3);

	double lengthA  = sqrt(a1*a1 + a2*a2 + a3*a3);
	double lengthB  = sqrt(b1*b1 + b2*b2 + b3*b3);

	double plotVectorADirection = kPI;
	double plotVectorBDirection = angle2;

	xplot2 = xplot1 - lengthA;
	yplot2 = yplot1;

    flatCoords->point.deleteValues(0);
	flatCoords->point.set1Value(flatCount++, SbVec3f(xplot1, yplot1, 0.0000));
	flatCoords->point.set1Value(flatCount++, SbVec3f(xplot2, yplot2, 0.0000));

	xplot3 = cos(angle2) * lengthB + xplot2;
	yplot3 = sin(angle2) * lengthB + yplot2;
	
	flatCoords->point.set1Value(flatCount++, SbVec3f(xplot3, yplot3, 0.0000));
	
	for (int i = 2; i < inCoords->point.getNum()-1; i += 2) {
		a1 = x2 - x3;
		a2 = y2 - y3;
		a3 = z2 - z3;
		b1 = x4 - x3;
		b2 = y4 - y3;
		b3 = z4 - z3;
		angle1 = GetVectorAngle(a1,a2,a3,b1,b2,b3);
		plotVectorADirection = plotVectorBDirection + kPI - angle1;
		xplot1 = xplot3;
		yplot1 = yplot3;
		x1 = x3;
		y1 = y3;
		z1 = z3;
		x2 = x4;
		y2 = y4;
		z2 = z4;
		x3 = inCoords->point[i+2][0];
		y3 = inCoords->point[i+2][1];
		z3 = inCoords->point[i+2][2];
		x4 = inCoords->point[i+3][0];
		y4 = inCoords->point[i+3][1];
		z4 = inCoords->point[i+3][2];
      		
		// make vectors out of A and B
		double a1 = x1 - x2;
		double a2 = y1 - y2;
		double a3 = z1 - z2;
		double b1 = x3 - x2;
		double b2 = y3 - y2;
		double b3 = z3 - z2;
		
		lengthA = sqrt(a1*a1 + a2*a2 + a3*a3);
		xplot2 = cos(plotVectorADirection) * lengthA + xplot1;
		yplot2 = sin(plotVectorADirection) * lengthA + yplot1;
		flatCoords->point.set1Value(flatCount++, SbVec3f(xplot2, yplot2, 0.0000));

		a1 = x1 - x2;
		a2 = y1 - y2;
		a3 = z1 - z2;
		b1 = x3 - x2;
		b2 = y3 - y2;
		b3 = z3 - z2;

		angle2 = GetVectorAngle(a1, a2, a3, b1, b2, b3);
		plotVectorBDirection = plotVectorADirection - kPI + angle2; 
		lengthB  = sqrt(b1* b1 + b2 * b2 + b3 * b3);
		xplot3 = cos(plotVectorBDirection) * lengthB + xplot2;
		yplot3 = sin(plotVectorBDirection) * lengthB + yplot2;
		flatCoords->point.set1Value(flatCount++, SbVec3f(xplot3, yplot3, 0.0000));
	}
    // hack workaround -- why does it make an extra value?
    int start = flatCoords->point.getNum() -1;
    flatCoords->point.deleteValues(start, -1);
    return;
}


double Flattener::GetVectorAngle(double a1, double a2, double a3, double b1, double b2, double b3)
{
	double lengthA  = sqrt( (a1 * a1) + (a2 * a2) + (a3 * a3) ); 	
	double lengthB  = sqrt( (b1 * b1) + (b2 * b2) + (b3 * b3) ); 	
	double dotAB    = a1 * b1 + a2 * b2 + a3 * b3;
	double cosAngle = dotAB / (lengthA * lengthB);
	double angle    = acos(cosAngle);
	return angle;
}


SoNode *
Flattener::findChildByType(SoSeparator * parent, SoType typeId)
{
	SoSearchAction search;
	SoPath *path;
	SoNode *node;

	search.setType(typeId, FALSE);
	search.setInterest(SoSearchAction::FIRST);
	search.apply(parent);

	path = search.getPath();
	if (path == NULL) return NULL;
	node = path->getTail();
	return node;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// #define DO_TRACE
