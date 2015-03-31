#pragma once


// defines for TESTING...

//#define INTENSITY 3

class FractalTreeSpec;

class Influence
{
  public:
    //SbVec3f             fDirection;
    //float               fIntensity;  // 0 to 1
    //int                 fNumLayers;
    //int                 fNumCoordsPerLayer;
    //bool                fUseLinearEffect;
    //bool                fUseExponentialEffect;
    FractalTreeSpec *   fFractalTreeSpec;

    Influence(void); // 
    Influence(FractalTreeSpec * fractalTreeSpec);
    Influence(SbVec3f direction, float intensity);
    ~Influence();
    
    // inlines ...
    //void     setNumCoordsPerLayer(int num_prototype_coords) { fNumCoordsPerLayer = num_prototype_coords; }
    //void     setNumLayers(int num_layers){ fNumLayers = num_layers; }

    // methods...
    void        influence_coords(bool isLeft, SoCoordinate3 *in_coords, int layer);   // returns an 'influenced' set of coordinates
    float       apply_incident_angle_factor(bool isLeft, SbLine segment);             // angle of incidence with respect to the fDirection
    float       apply_fractal_level_factor(bool isLeft, SbLine segment, int level);
    float       apply_linear_incremental_factor(bool isLeft, int whichSegment, int layer);
    float       apply_non_linear_incremental_factor(bool isLeft, int which_segment, int which_level);
    float       apply_blended_matrices(SbPList *matrix_list);
};

class SpiralInfluence : public Influence
{
public:
//    SbVec3f fDirection;
//    float   fIntensity;  // 0 to 1
//    int     fNumLayers;
//    int     fNumCoordsPerLayer;
    SpiralInfluence(FractalTreeSpec * fts);
    //SpiralInfluence(SbVec3f direction, float intensity);
    ~SpiralInfluence();

    void       influence_coords(bool isLeft, SoCoordinate3 *in_coords, int layer);   // returns an 'influenced' set of coordinates
//    float      apply_incident_angle_factor(SbLine segment);             // angle of incidence with respect to the fDirection
//    float      apply_fractal_level_factor(SbLine segment, int level);
//    float      apply_linear_incremental_factor(int whichSegment, int layer);
//    float      apply_non_linear_incremental_factor(int which_segment, int which_level);
    SbVec3f    get_spiral_direction(SbVec3f location);
};

