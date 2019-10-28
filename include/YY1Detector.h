#ifndef YY1_DETECTOR_H
#define YY1_DETECTOR_H
#include "Detector.h"

/**
 * Detector class for the YY1 silicon detector from Micron Semiconductor Ltd.
 * The specific geometry of the YY1 design is used to implement various
 * functions relating to positions, distances and solid angles.
 *
 * The YY1Detector is initialised with a position (0,0,0), a normal vector along
 * the +z-direction and the orientation vector along the +y-direction.
 */
class YY1Detector : public Detector {
  private:
  
  public:
    YY1Detector();
    virtual ~YY1Detector() = default;
    
    /**
     * Get the number of segments of the detector (16 for YY1).
     */
    virtual int GetNSegments();

    virtual TVector3 GetSegPosition(int i);
    
    /**
     * Get the solid angle of a given strip of a YY1 detector.
     * @param i is the strip number with i=0 being the inner most strip
     * @param pos is the TVector pointing to the calibration source location (currently not implemented! i.e. pos set to origin)
     */
    virtual double GetSegSolidAngle(int i, TVector3 &pos);
};
#endif
