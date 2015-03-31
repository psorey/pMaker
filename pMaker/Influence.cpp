#include "stdafx.h"
#include "FractalTreeMaker.h"
#include "Influence.h"
#include <Inspect.h>
#include "FractalTreeSpec.h"

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/SbLine.h>

Influence::Influence(void)
{
    // don't use this one. needed because Influence is subclassed.
}

Influence::Influence(FractalTreeSpec * fractalTreeSpec)
{
    fFractalTreeSpec = fractalTreeSpec;
}

Influence::~Influence()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// these two work together for a translation..

void Influence::influence_coords(bool isLeft, SoCoordinate3 * centerline_coords, int level )  {
    TRACE("isLeft = %d\n", isLeft);
    //Inspect::WriteCoordinate3("before_influence.iv", centerline_coords);
    int num_coords = centerline_coords->point.getNum();
    SbVec3f influence_vector = fFractalTreeSpec->getInfluenceVector();
    //Inspect::Vec3f("influence vector from fractal tree spec", influence_vector);
    for(int h = 1; h < num_coords; h++) {

        // second coordinate...last coordinate
        SbVec3f moving_coords              = centerline_coords->point[h];
        SbVec3f segment_direction_vector   = moving_coords - centerline_coords->point[h-1];  // non-unit-length vector from origin...
        float   influence_amount           = apply_incident_angle_factor(isLeft, SbLine(SbVec3f(0,0,0), segment_direction_vector)); // (0-1)
        Inspect::Vec3f("influence vector from fractal tree spec", influence_vector);

        // default wind direction is z = 1



        SbRotation segment_to_influence_rotation(segment_direction_vector, influence_vector); // supposed to return shortest, but doesn't
        SbVec3f axis;
        float rotation_angle;
        segment_to_influence_rotation.getValue(axis,rotation_angle);
        TRACE("rotation_angle = %f        axis = %f  %f  %f\n", rotation_angle, axis[0], axis[1], axis[2]);
        influence_amount *= fFractalTreeSpec->getInfluence(isLeft, level);
        influence_amount *= .01;  // arbitrary scale factor...
        // add other modifiers of influence here...

        if (isLeft)
            TRACE("left influence amount = %f\n", influence_amount);
        else
            TRACE("right influence amount =                       %f\n", influence_amount);
        
        SbRotation rotation(axis, rotation_angle * influence_amount);
        
        SbMatrix   rot_matrix;
        SbVec3f    translation(0,0,0);
        SbVec3f    scaleFactor(1,1,1);
        
        SbVec3f    center(centerline_coords->point[h-1]);
        SbRotation scaleOrientation(SbVec3f(1,0,0), 0);
        
        //rot_matrix.setTransform (translation, deflection_rotation, scaleFactor, scaleOrientation, center);
        rot_matrix.setTransform (translation, rotation, scaleFactor, scaleOrientation, center);
        
        // rotate the remaining coords similarly...
        for(int n = h; n < num_coords; n++) {
            SbVec3f result;
            rot_matrix.multVecMatrix(centerline_coords->point[n], result);
            centerline_coords->point.set1Value(n, result);
        }
    }
    Inspect::WriteCoordinate3("after_influence.iv", centerline_coords);
    int blank = 1;
}

float Influence::apply_incident_angle_factor(bool isLeft, SbLine segment)
{
    SbVec3f desired_direction_vector = fFractalTreeSpec->getInfluenceVector();
    
    // if the segment is perpendicular to the wind it gets the most influence, as sin() of the angle.
    // find the angle between the segment and the desired_direction by rotation...
    SbVec3f segment_direction_vector = segment.getDirection();             // normalized vector
    Inspect::Vec3f("seg_dir", segment_direction_vector);
    Inspect::Vec3f("influence_dir", desired_direction_vector);
    float dot = (segment_direction_vector.dot(desired_direction_vector));  // dot product is the angle between the vectors

    TRACE("dot = %f\n\n", dot);
    float influence_amount = abs(cos(dot));  // 0 to 1, centerline segment perpendicular to influence returns 1.0, parallel = 0.0
    return influence_amount;
}


float Influence::apply_linear_incremental_factor(bool isLeft, int which_segment, int which_level)
{
    int total_num_coords = fFractalTreeSpec->getTotalCenterlineCoords(isLeft);
    int coord_number = which_segment + 1 + (fFractalTreeSpec->getNumCenterlineCoords(isLeft) * (which_level - 1));
    float ret_val = ((float)coord_number / (float)total_num_coords);
    float new_val = (ret_val + 1) / 2;
    return new_val;
}

float Influence::apply_non_linear_incremental_factor(bool isLeft, int which_segment, int which_level)
{
    int total_num_coords = fFractalTreeSpec->getTotalCenterlineCoords(isLeft);
    total_num_coords *= total_num_coords;
    int coord_number = which_segment + 1 + (fFractalTreeSpec->getNumCenterlineCoords(isLeft) * (which_level - 1));
    coord_number *= coord_number;
    float ret_val = ((float)coord_number / (float)total_num_coords);
    return ret_val;
}

/*
float Influence::apply_fractal_level_factor(SbLine segment, int level)
{
    return 1.0;
}
*/

float Influence::apply_blended_matrices(SbPList *matrix_list)
{
    return 1.0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpiralInfluence::SpiralInfluence(FractalTreeSpec * fts)
{
    fFractalTreeSpec = fts;
}

SpiralInfluence::~SpiralInfluence(void){}




SbVec3f SpiralInfluence::get_spiral_direction(SbVec3f location)
{
    SbVec3f dir = SbVec3f(0, location[1], location[2]);
    SbVec3f direction;
    SbRotation rot(SbVec3f(1,0,0), 3.1415926 / 2); // wind is 90 degrees from...
    rot.multVec(dir, direction);
    return direction;
}

void SpiralInfluence::influence_coords(bool isLeft, SoCoordinate3 *centerline_coords, int level )  
{  
    /*
    int num_coords = centerline_coords->point.getNum();
    for(int h = 1; h < num_coords; h++) {
        float segment_ratio = float(h + (level * num_coords)) / float (fFractalTreeSpec->fNumLevels * num_coords); 
        // second coordinate...last coordinate
        SbVec3f moving_coords = centerline_coords->point[h];
        SbVec3f direction_vector = moving_coords - centerline_coords->point[h-1];
        float influence = abs(apply_incident_angle_factor(isLeft, SbLine(centerline_coords->point[h],centerline_coords->point[h-1])));
        
        float effect = 1.0;
        if(fFractalTreeSpec->fUseLinearEffect)
            effect = apply_linear_incremental_factor(isLeft, h, level);
        else if(fFractalTreeSpec->fUseExponentialEffect)
            effect = apply_non_linear_incremental_factor(isLeft, h, level);
        
        SbVec3f test = get_spiral_direction(SbVec3f(0,1,0));
        Inspect::Vec3f("1,0,0 in:", test);

        SbVec3f spiral_direction = get_spiral_direction(centerline_coords->point[h]);
        // find the rotation from 
        SbRotation temp_rot(direction_vector, spiral_direction);
        SbVec3f temp_axis;
        float   angle;
        temp_rot.getValue(temp_axis,angle);
        SbRotation rot(temp_axis, angle * .02 * fFractalTreeSpec->getInfluence(isLeft, level) * influence * effect);

        SbMatrix rot_matrix;
        SbVec3f translation(0,0,0);
        SbVec3f scaleFactor(1,1,1);
        SbVec3f center(centerline_coords->point[h-1]);
        SbRotation scaleOrientation(SbVec3f(1,0,0), 0);
        rot_matrix.setTransform (translation, rot, scaleFactor, scaleOrientation, center);
        for(int n = h; n < num_coords; n++) {
            SbVec3f result;
            rot_matrix.multVecMatrix(centerline_coords->point[n], result);
            centerline_coords->point.set1Value(n, result);  
        }
    }
    */
}
