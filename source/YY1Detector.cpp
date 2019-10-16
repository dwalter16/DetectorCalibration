#include "YY1Detector.h"

YY1Detector::YY1Detector()
{
  position.SetXYZ(0,0,0);
  orientation.SetXYZ(1,0,0);
  normal.SetXYZ(0,0,1);
  deadLayer = 150.;
}

int YY1Detector::GetNSegments()
{
  return 16;
}

TVector3 YY1Detector::GetSegPosition(int i)
{
  TVector3 segPos = position + orientation * 5.*(i+0.5);
  return segPos;
}

double YY1Detector::GetSegSolidAngle(int i, TVector3 &pos)
{
  return 1.;
}
