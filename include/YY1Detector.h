#ifndef YY1_DETECTOR_H
#define YY1_DETECTOR_H
#include "Detector.h"

/**
 * Detector class for the YY1 silicon detector from Mcron Semiconductor Ltd.
 * The specific geometry of the YY1 design is used to implement various
 * functions relating to positions, distances and solid angles.
 */
class YY1Detector : public Detector {
  private:
  
  public:
    YY1Detector();
    ~YY1Detector() = default;
    
    /**
     * Get the number of segments of the detector (16 for YY1).
     */
    virtual int GetNSegments();

    virtual TVector3 GetSegPosition(int i);
    
    virtual double GetSegSolidAngle(int i, TVector3 &pos);
    
    virtual double GetSolidAngle(TVector3 &pos);
};
#endif
