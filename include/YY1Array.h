#ifndef YY1_ARRAY_H
#define YY1_ARRAY_H
#include "Detector.h"
#include "YY1Detector.h"
#include <array>
#include <TVector3.h>

/**
 * This class is used for the up- and downstream arrays of YY1 detectors in the
 * IRIS setup. In this implementation the YY1 detectors are arranged in a circle
 * with the first detector in the top position. The segments are counted from
 * the center towards the outer edge for each detector and the detectors are
 * counted clockwise when looking at the front of the array.
 */
class YY1Array : public Detector {
  private:
    std::array<YY1Detector,8> detectors;
    
  public:
    YY1Array();
    ~YY1Array() = default;

    virtual void SetPosition(TVector3 &pos);
    virtual void SetPosition(double x, double y, double z);
    virtual void RotateX(double angle);
    virtual void RotateY(double angle);
    virtual void RotateZ(double angle);
    virtual void Rotate(double angle, TVector3 &axis);

    virtual int GetNSegments();
    virtual TVector3 GetSegPosition(int i);
    virtual double GetSegSolidAngle(int i, TVector3 &pos);
}; 
#endif
