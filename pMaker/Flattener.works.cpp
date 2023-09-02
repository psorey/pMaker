#include "stdafx.h"
#include "Flattener.h"
#include "Intersection.h"
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

#define THICKEN_INTERVAL 20


//Flattener::Flattener(void);

Flattener::Flattener(void) {
	fPlacedCoords = NULL;
	SoInput myInput;    
	myInput.openFile("c:\\junk\\placed_coords.iv");  // crashes if not found !!!  maybe open a file dialog to find the one
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


// flatten_polylines(zigzag_coords, 1, numPathCoords? )



///////// * Using this one  * ///////////

#define MARKER_SPACING 10
#define MARKER_WIDTH .1

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

	FILE * fp = fopen(m_strAddFile, "w");
    if(fp == NULL) return;
	WriteDXF * writeDXF = new WriteDXF(fp); // opens the file and writes the begin section
    writeDXF->WriteBeginDXF();

	for(int j = 0; j < numSides; j++) {
        int count = 0;
		newCoords->point.deleteValues(0);
		flatCoords->point.deleteValues(0);
		for ( int k = j; k < loftCoords->point.getNum() - 1; k += (numSides + 1)) {
			newCoords->point.set1Value(count++, loftCoords->point[k]);
			newCoords->point.set1Value(count++, loftCoords->point[k + 1]);
		}
		flatten(newCoords, flatCoords);
        TRACE("numNewCoords = %d   numFlatCoords = %d\n", newCoords->point.getNum(), flatCoords->point.getNum());   

		int numVertices = flatCoords->point.getNum();
		char layer[4];
		_itoa( j, layer, 10);
        int marker_spacing = MARKER_SPACING;
        float line_width = 0.0;
        int count2 = 0;
        writeDXF->WriteLWPOLYLINEHeader(layer, numVertices/2); 
		int i = 0;
		for ( i = 0; i < numVertices; i += 2) {
            
			SbVec3f point = flatCoords->point[i];
			if (count2 % (marker_spacing * 10) == 0)    // emphasize every tenth marker
				writeDXF->WriteLWPOLYLINEPoint(point, MARKER_WIDTH * 5);
			else if (count2 % (marker_spacing * 5) == 0)    // emphasize every fifth marker
				writeDXF->WriteLWPOLYLINEPoint(point, MARKER_WIDTH * 2);
			else if (count2 % marker_spacing == 0)
				writeDXF->WriteLWPOLYLINEPoint(point, MARKER_WIDTH);
            else
                writeDXF->WriteLWPOLYLINEPoint(point, 0.0);
			count2++;
		}
		writeDXF->WriteZero();
        count2 = 0;
		writeDXF->WriteLWPOLYLINEHeader(layer, numVertices / 2);
		for( i = 1; i < numVertices; i += 2) {
            
			SbVec3f point = flatCoords->point[i];
			if (count2 % (marker_spacing * 10) == 0)    // emphasize every tenth marker
				writeDXF->WriteLWPOLYLINEPoint(point, MARKER_WIDTH * 5);
			else if (count2 % (marker_spacing * 5) == 0)    // emphasize every fifth marker
				writeDXF->WriteLWPOLYLINEPoint(point, MARKER_WIDTH * 2);
			else if (count2 % marker_spacing == 0)
				writeDXF->WriteLWPOLYLINEPoint(point, MARKER_WIDTH);
			else
				writeDXF->WriteLWPOLYLINEPoint(point, 0.0);
			count2++;
		}
        writeDXF->WriteZero();


		int  fLastCoord = 0;
		SoMFVec3f  fScallopedLine;  // append output from insert_placed_coords() to fScallopedLine

#define INSERT_COORDS 1  // if TRUE objects placed_coords are placed along the polylines

		////////////////////////////////////////////////////////////
		// if an object is being placed on the flattened geometry...

		if (fPlacedCoords != NULL && INSERT_COORDS == true) {
			// place_scaled_objects(flatCoords);  // place the 
			// split the flattened polyline into two lines...
			SoCoordinate3 * line1 = new SoCoordinate3;
			SoCoordinate3 * line2 = new SoCoordinate3;
			line1->ref();
			line2->ref();
			int count = 0;
			int num = flatCoords->point.getNum();
			for (int i = 0; i < num; i+=2)  {
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
			// define variables and initialize with default value
		
			double begin_spacing; // the starting distance between placed objects
			double end_spacing = 0.0;
			double begin_scale = 0.0;
			double end_scale = 0.0;
			double current_distance = 0.0;   // 
			// place the first at distance 0
			double overall_scale = 1.0;


#define PADDLE_SPACING 0
#define NEW_PADDLE_SPACING 0  //  use this one

#define PROW_SPACING 0
#define PROW_SPACING_FINAL 0   // use this one

#define SPOKANE_SPACING 1

#define NEW_CHRISTIE_NARROWER 0
#define NEW_CHRISTIE_SPACING 0
#define CHRISTIE_SPACING 0
#define STALK_SPACING 0
#define SEPAL_SPACING 0
#define GUIDE_SPACING 0
#define ROCK_SPACING 0
#define PETAL_SPACING 0


			if (SPOKANE_SPACING) {
				overall_scale = 1.8;  // 3.0
				begin_spacing = 1.2 * overall_scale; // the starting distance between placed objects
				end_spacing = 1.2 * overall_scale;
				begin_scale = 1.0;
				end_scale = 1.0; 
			}

			if (NEW_CHRISTIE_NARROWER) {
				overall_scale = 1.2;  // 3.0
				begin_spacing = 1.1 * overall_scale; // the starting distance between placed objects
				end_spacing = 1.1 * overall_scale;
				begin_scale = .9;
				end_scale = .9;
			}

			if (NEW_CHRISTIE_SPACING) {
				overall_scale = 1.8;  // 3.0
				begin_spacing = 1.1 * overall_scale; // the starting distance between placed objects
				end_spacing = 1.1 * overall_scale;
				begin_scale = .9;
				end_scale = .9;
			}

			if (PROW_SPACING) {
				overall_scale = 4;
				end_spacing = 21.9; // the starting distance between placed objects
				begin_spacing = 3.8;
				end_scale = 8.6;
				begin_scale = 1.80;   
			}

			if (PROW_SPACING_FINAL) {
				overall_scale = 4;
				end_spacing = 15.9; // the starting distance between placed objects
				begin_spacing = 4.2;
				end_scale = 5.6;
				begin_scale = 1.60;
			}
/*
			if (PROW_SPACING_TEST) {
				overall_scale = 4;
				end_spacing = 21.9; // the starting distance between placed objects
				begin_spacing = 4.2;
				end_scale = 8.6;
				begin_scale = 1.60;
			}
*/

			if (NEW_PADDLE_SPACING) {
				overall_scale = 3;
				begin_spacing = 1.1 * overall_scale; // the starting distance between placed objects
				end_spacing = 1.1 * overall_scale;
				begin_scale = 1.2;
				end_scale = 1.2;
			}

			if (PADDLE_SPACING) {
				overall_scale = 2;
				begin_spacing = 1.1 * overall_scale; // the starting distance between placed objects
				end_spacing = 1.1 * overall_scale;
				begin_scale = .8;
				end_scale = .8;
			}


			if (CHRISTIE_SPACING) {
				overall_scale = 4;
				begin_spacing = 1 * overall_scale; // the starting distance between placed objects
				end_spacing = 1 * overall_scale;
				begin_scale = 1;
				end_scale = 1;
			}


			if (STALK_SPACING) {
				begin_spacing = 1.95; // the starting distance between placed objects
				end_spacing = 0.8;
				begin_scale = 1.5;
				end_scale = .50;
			}

			if (SEPAL_SPACING) {
				begin_spacing = 0.8; // the starting distance between placed objects
				end_spacing = 0.6;
				begin_scale = 0.5;
				end_scale = .30;
			}

			if (GUIDE_SPACING) {  // for stitching petals
				begin_spacing = 6; // the starting distance between placed objects
				end_spacing = 3;
				begin_scale = 1;
				end_scale = 1;
			}

			if (ROCK_SPACING) {
				begin_spacing = 0.45; // the starting distance between placed objects
				end_spacing = 0.35;
				begin_scale = 0.3;
				end_scale = .25;
			}

			if (PETAL_SPACING) {
				begin_spacing = 1.75; // the starting distance between placed objects
				end_spacing = 0.8;  //0.96
				begin_scale = 1.1;
				end_scale = .570;
			}

			SbMatrix mat;
			float distance = 0.0;
			// !!! below
			insertPlacedCoords(layer, 0, writeDXF, line1, line2, line1length, begin_spacing, end_spacing, begin_scale, end_scale);
			insertPlacedCoords(layer, 1, writeDXF, line2, line1, line2length, begin_spacing, end_spacing, begin_scale, end_scale);

			line1->unref();
			line2->unref();
		}
	}  // side
	////////////////////////////////////////////////////////////////////////////////
    writeDXF->WriteDXFEndsec();
    writeDXF->WriteEndDXF();
	fclose(fp);
	tempSep->unref();
	flatCoords->unref();
}	




int Flattener::get_point_at_length(const SoMFVec3f line, float length) {
	float distance = 0.0;
	int i;
	for (i = 0; i < line.getNum() && distance < length; i++) {
			distance += (line[i + 1] - line[i]).length();
	}
	return i;
}


void Flattener::insertPlacedCoords(     // called once per leg of the shape, to flip the insertion to the other side of the line
	char *			layer, 
	int				side, 
	WriteDXF *		writeDXF,			// 'fPlacedCoords' is a 2-D shape (the scallop), a series of line segments
    SoCoordinate3 * line,
	SoCoordinate3 * opposite_line,
	float			linelength,         // 'line' is a 2-D line which is the (left or right) outline of a flattened side
	float			begin_spacing, 
	float			end_spacing,        // 'spacing' is how far apart the placed coords are inserted onto 'line'
	float			begin_scale, 
	float			end_scale)          // 'scale' is the scale of the placed coords 
{

	SoMFVec3f		last_bit_plus_trimmed_insert;
	SoMFVec3f       accumulated_segments;
	//SoMFVec3f       connecting_coords; // coords copied from line that connect to a) beginning of line, or b) coords added to the last placed_coords
	
	int last_point_num = 0;
	int new_last_point_num = 0;
	SbVec3f new_last_insert_point = SbVec3f(0,0,0);  // value changed in function intersect...
	SbVec3f last_insert_point = SbVec3f(0, 0, 0);

	//SbVec3f last_point;

	// measure the line
	float distance = 0.0;
	float current_distance = 0.0;

	// move along the line points
	while (distance < linelength) {              // 
		float ratio = distance / linelength;
		float scale = begin_scale - ((begin_scale - end_scale) * ratio);
		float spacing = begin_spacing - ((begin_spacing - end_spacing) * ratio);

		current_distance = 0.0;
		// increment along points defining 'line'
		int point = 0;
		for (point = 0; current_distance <= distance; point++) {
			current_distance += (line->point[point + 1] - line->point[point]).length();
		}
		if (point > 0) point--;
	//	save_point = point;  // point = the last point BEFORE the insertion point of the placed_coords (not the trimmed line)
		SbVec3f vector = line->point[point + 1] - line->point[point];
		float rot = atan2(vector[1], vector[0]);
		// find 'insertion' point, rotation and scale
		SbVec3f insert_point = findPointOnLine(line, distance);
		SbRotation rotation;
		if (side == 0) {
			rotation = SbRotation(SbVec3f(0, 0, 1), rot + 1.5707);
		}
		else {
			rotation = SbRotation(SbVec3f(0, 0, 1), rot - 1.5707);
			// also need to flip order of vertices .. see below
		}
		SbMatrix mat;  // matrix 
		mat.setTransform(insert_point, rotation, SbVec3f(scale, scale, scale));
		
		// generate placed(insert)_coords that are placed, scaled and rotated
		// 
		SoCoordinate3 * insert_coords = new SoCoordinate3;
		insert_coords->ref();
		for (int i = 0; i < fPlacedCoords->point.getNum(); i++) {    // fPlacedCoords is a 2-D shape made of line segments, currently at c:\\junk
			SbVec3f transformed_coord;
			mat.multVecMatrix(fPlacedCoords->point[i], transformed_coord);
			insert_coords->point.set1Value(i, transformed_coord);
			//TRACE("new_insert_coords  %d\n", i);
		}
		if(side != 0) {   // reverse order}
		    SoMFVec3f copy; /// = insert_coords->point.copy
		    copy.copyFrom(insert_coords->point);
		    int num = insert_coords->point.getNum();
			for (int i = 0; i < num; i++) {
				insert_coords->point.set1Value(num -1 - i, copy[i]);
			}
		}
		
		// modify to return the list of points from the input point (last point?) # to the point number after the intersection
		// place the actual coords over the actual line and get intersection points
		// int coord_before_intersection;
		// int coord_after_intersection;    SbVec3f last_insert_point, SbVec3f & new_last_insert_point,
		if (distance > 0.00001) {      // don't do the first insert
			TRACE("distance = %f\n", distance);
			intersect_placed_coords_with_line(last_bit_plus_trimmed_insert, last_insert_point, new_last_insert_point, last_point_num, new_last_point_num, insert_coords, line);
			
			last_point_num = new_last_point_num;
			last_insert_point = new_last_insert_point;

			TRACE("last_point_num = %d\n", last_point_num);
			int num_exist_pts = accumulated_segments.getNum();
			int num_pts_to_add = last_bit_plus_trimmed_insert.getNum();

			for (int i = 0; i < num_pts_to_add; i++) {
				TRACE("last_bit_plus_trimmed_insert  =  %f\t\t%f\n", last_bit_plus_trimmed_insert[i][0], last_bit_plus_trimmed_insert[i][1]);
			}

			// save the new points to the end of the array
			for (int i = 0; i < num_pts_to_add; i++) {
				accumulated_segments.set1Value(i + num_exist_pts, last_bit_plus_trimmed_insert[i]);
			}
			insert_coords->unref();
		}
		
		distance += spacing;
	}
	SoCoordinate3 * scalloped_side = new SoCoordinate3;
	scalloped_side->ref();
	scalloped_side->point.copyFrom(accumulated_segments);

	writeDXF->WriteLWPOLYLINE(scalloped_side, layer, -1);
	/*
	// for testing...
	SoWriteAction wa;
	wa.getOutput()->openFile("scallop_test.iv");
	wa.apply(scalloped_side);
	wa.getOutput()->closeFile();
	*/
	scalloped_side->unref();
}


// find points where insert_coords lines intersect with the flatten line
// trim insert_coords to line trim_to_line(SoCoordinate3 * line, SoCoordinate3 * placed_coords)
// create line from last insert_coords trimmed last-point to current insert_coords trimmed first-point
// void Flattener::intersect_placed_coords_with_line(
//                                SbVec3f &coords_to_append,
//                                int &coord_before, 
//                                it  &coord_after, 
//                                SoCoordinate3 * insert_coords, 
//                                SoCoordinate3 * line)
// {

void Flattener::intersect_placed_coords_with_line(  
	                                        SoMFVec3f & last_bits_plus_trimmed_insert, 
							                SbVec3f last_insert_point,    // last vertex of last inserted item,
	                                        SbVec3f & new_last_insert_point, 
	                                        int last_point_num,          // line->point[last_point_num]
	                                        int &new_last_point_num, 
	                                        SoCoordinate3 * insert_coords, // these are trimmed at line
	                                        SoCoordinate3 * line )
{

	// test each segment of line with each segment of insert_coords? or assume 1st and last segment of insert_coords only...
	// return trimmed first, middle coords, trimmed last
	//SoMFVec3f return_coords;  // line[last_point_num], // then add points from line until an intersection found --
	int num_line_coords = line->point.getNum();
	int count_two = 0;
	int save_pts[2];
	int first_pt_num = 0;
	last_bits_plus_trimmed_insert.deleteValues(0);

	// steps through the line until an intersection is found
	for (int i = 0; i < num_line_coords - 1; i++) {
		SbVec2f  S1[2];  // first line segment from insert_coords
		SbVec2f  S2[2];  // line segment from line
		SbVec2f  I0;
		SbVec2f  I1;

		// segment of line we are testing
		S2[0] = SbVec2f(line->point[i][0], line->point[i][1]);
		S2[1] = SbVec2f(line->point[i + 1][0], line->point[i + 1][1]);

		// better to not care if first or second hit is the first or last -- 
		// test first segment of insert...................................
		S1[0] = SbVec2f(insert_coords->point[0][0], insert_coords->point[0][1]);
		S1[1] = SbVec2f(insert_coords->point[1][0], insert_coords->point[1][1]);
		int test = Intersection::intersect2D_2Segments(S1, S2, I0, I1);
		if (test == 1) {
			insert_coords->point.set1Value(0, SbVec3f(I0[0], I0[1], 0.0));  // replace the first point (assumes only the first segment crosses the line
			save_pts[0] = i; // saves the point_num on line before splice count = 0
			first_pt_num = i;
			TRACE("last_point_num =  %d     first_pt_num = %d\n", last_point_num, first_pt_num);
			count_two++;
		}


		// test last segment of insert.....................................
		int num = insert_coords->point.getNum();
		S1[0] = SbVec2f(insert_coords->point[num - 2][0], insert_coords->point[num - 2][1]);
		S1[1] = SbVec2f(insert_coords->point[num - 1][0], insert_coords->point[num - 1][1]);
		test = Intersection::intersect2D_2Segments(S1, S2, I0, I1);
		if (test == 1) {
			insert_coords->point.set1Value(num - 1, SbVec3f(I0[0], I0[1], 0.0));  // replace the last point (assumes only the last segment crosses the line
			save_pts[1] = i;  // saves the point_num on line after splice
			new_last_insert_point = SbVec3f(I0[0], I0[1], 0.0);
			TRACE("new_last_insert_point =  %f\t\t%f\n", I0[0], I0[1]);
			new_last_point_num = i +1;
			count_two++;
		}

		if (count_two >= 2) {
			TRACE("found two\n");
			i = num_line_coords; // end loop
		}
	}
	// we've trimmed the insert_coords, now insert [last_point from previous insertion, line[saved_pts[1]] from last time, through line[saved_pts[0]] this time, 
	int num_insert_coords = insert_coords->point.getNum();
	int num_line_coords_before_splice = first_pt_num - last_point_num + 1;  // maybe should be counting from line->point[0]?
	int num_to_insert = 2 + (first_pt_num -last_point_num);   // == point #
	TRACE("num before splice = %d\n", num_line_coords_before_splice);
	int i;
	last_bits_plus_trimmed_insert.set1Value(0, last_insert_point);

	for(i = 1; i <= num_line_coords_before_splice; i++) {  //// !!!!!!!!
		//// 
	    last_bits_plus_trimmed_insert.set1Value(i, line->point[i - 1 + last_point_num]); //  the final fix that made it work
		// TRACE("added a point!     %f      %f\n", line->point[i + last_point_num][0], line->point[i + last_point_num][1]);
	}
	 // now add the trimmed insert_coords
	for(int j = 0; j < num_insert_coords; j++) {
		last_bits_plus_trimmed_insert.set1Value(i, insert_coords->point[j]);
		i++;
	}
}


void Flattener::intersect_placed_coords_with_line(
								SoCoordinate3 * insert_coords, 
								SoCoordinate3 * line)
{
    // test each segment of line with each segment of insert_coords? or assume 1st and last segment of insert_coords only...
    // return trimmed first, middle coords, trimmed last
	
	int num_line_coords = line->point.getNum();

	int count_two = 0;

	// coords_to_append
	// coord_before = 0;
	// coord_after = 0;

	for (int i = 0; i < num_line_coords - 1; i++) {
		SbVec2f  S1[2];  // first line segment from insert_csoords
		SbVec2f  S2[2];  // line segment from line
		SbVec2f  I0;
		SbVec2f  I1;
		S2[0] = SbVec2f(line->point[i][0], line->point[i][1]);
		S2[1] = SbVec2f(line->point[i+1][0], line->point[i+1][1]);

		// test for first and last segment of insert_coords crossing line ( only works if these segments cross the line consistently )
		S1[0] = SbVec2f(insert_coords->point[0][0], insert_coords->point[0][1]);
		S1[1] = SbVec2f(insert_coords->point[1][0], insert_coords->point[1][1]);
		int test = Intersection::intersect2D_2Segments(S1, S2, I0, I1);
		if (test == 1) {
			insert_coords->point.set1Value(0, SbVec3f(I0[0], I0[1], 0.0));  // replaces old value with new intersection point
			count_two++;
		}
		int num = insert_coords->point.getNum();
		S1[0] = SbVec2f(insert_coords->point[num-2][0], insert_coords->point[num-2][1]);
		S1[1] = SbVec2f(insert_coords->point[num-1][0], insert_coords->point[num-1][1]);
		test = Intersection::intersect2D_2Segments(S1, S2, I0, I1);
		if (test == 1) {
			insert_coords->point.set1Value(num-1, SbVec3f(I0[0], I0[1], 0.0));
			count_two++;
		}
		if (count_two >= 2) {
			TRACE("found two\n");
			i = num_line_coords; // end loop
		}
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


// this one derives the current scale from current distance between the same point on line 1 and 2
/* not working yet
void Flattener::insertPlacedCoords(char * layer, int side, WriteDXF * writeDXF,    // 'fPlacedCoords' is a 2-D shape (the scallop), a series of line segments
SoCoordinate3 * line,
SoCoordinate3 * opposite_line,
float linelength,                               // 'line' is a 2-D line which is the (left or right) outline of a flattened side
float begin_spacing, float end_spacing,         // 'spacing' is how far apart the placed coords are inserted onto 'line'
float begin_scale, float end_scale)             // 'scale' is the scale of the placed coords
{
float distance = 0.0;
float current_distance = 0.0;
float ratio;
float scale;
float spacing;

while (distance < linelength) {              //

// find the point # closest to the current distance:
int current_point = get_point_at_length(line->point, distance);
SbVec3f line_point = line->point[current_point];
SbVec3f opposite_line_point = opposite_line->point[current_point];

//float ratio = distance / linelength;
float scale = begin_scale - ((begin_scale - end_scale) * ratio);
float spacing = begin_spacing - ((begin_spacing - end_spacing) * ratio);

current_distance = 0.0;
// increment along points defining 'line'l
int point = 0;
for (point = 0; current_distance <= distance; point++) {
current_distance += (line->point[point + 1] - line->point[point]).length();
}
if (point > 0) point--;
save_point = point;
SbVec3f vector = line->point[point + 1] - line->point[point];
float rot = atan2(vector[1], vector[0]);
// find 'insertion' point, rotation and scale
SbVec3f insert_point = findPointOnLine(line, distance);
SbRotation rotation;
if (side == 0) {
rotation = SbRotation(SbVec3f(0, 0, 1), rot + 1.5707);
}
else {
rotation = SbRotation(SbVec3f(0, 0, 1), rot - 1.5707);
}
SbMatrix mat;  // matrix
mat.setTransform(insert_point, rotation, SbVec3f(scale, scale, scale));
SoCoordinate3 * insert_coords = new SoCoordinate3;
insert_coords->ref();
for (int i = 0; i < fPlacedCoords->point.getNum(); i++) {    // fPlacedCoords is a 2-D shape made of line segments
SbVec3f transformed_coord;
mat.multVecMatrix(fPlacedCoords->point[i], transformed_coord);
insert_coords->point.set1Value(i, transformed_coord);
}
intersect_placed_coords_with_line(insert_coords, line);  // modifies insert_coords (trims to line)
SoCoordinate3 * new_coords = new SoCoordinate3;
new_coords->ref();
new_coords->point.setValue(last_point);
if (side == 0) {
for (int i = 0; i < insert_coords->point.getNum(); i++) {
new_coords->point.set1Value(i + 1, insert_coords->point[i]);
}
last_point = new_coords->point[new_coords->point.getNum() - 1];
} else {
int count = 1;
for(int i = insert_coords->point.getNum() - 1; i >= 0; i--) {
new_coords->point.set1Value(count++, insert_coords->point[i]);
}
last_point = new_coords->point[new_coords->point.getNum() - 1];
//last_point = new_coords->point[1];
}
writeDXF->WriteLWPOLYLINE(new_coords, layer, -1);
new_coords->unref();
insert_coords->unref();
distance += spacing;
}
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
