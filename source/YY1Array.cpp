#include "YY1Array.h"
#include <TMath.h>

using namespace std;
using namespace TMath;

YY1Array::YY1Array()
{
  //The first detector is ok, but the rest we need to rotate.
  for(int i=1; i<8; i++){
    YY1Detector &d = detectors.at(i);
    double angle = -i * 45.;
    d.RotateZ(angle);
  }
  
  //Same convention as the single YY1 detector.
  position.SetXYZ(0,0,0);
  normal.SetXYZ(0,0,1);
  orientation.SetXYZ(0,1,0);
  deadLayer = 150.;
}

int YY1Array::GetNSegments()
{
  return 128;
}

TVector3 YY1Array::GetSegPosition(int i)
{
  int det = i / 16;
  int seg = i % 16;
  return detectors.at(det).GetSegPosition(seg);
}
    
double YY1Array::GetSegSolidAngle(int i, TVector3 &pos)
{
  int det = i / 16;
  int seg = i % 16;
  return detectors.at(det).GetSegSolidAngle(seg,pos); 
}

void YY1Array::SetPosition(TVector3 &pos)
{
  for(YY1Detector &d : detectors){
    d.SetPosition(pos);
  }
}

void YY1Array::SetPosition(double x, double y, double z)
{
  for(YY1Detector &d : detectors){
    d.SetPosition(x,y,z);
  }
}

void YY1Array::RotateX(double angle)
{
  for(YY1Detector &d : detectors){
    d.RotateX(angle);
  }
}

void YY1Array::RotateY(double angle)
{
  for(YY1Detector &d : detectors){
    d.RotateY(angle);
  } 
}

void YY1Array::RotateZ(double angle)
{
  for(YY1Detector &d : detectors){
    d.RotateZ(angle);
  } 
}

void YY1Array::Rotate(double angle, TVector3 &axis)
{
  for(YY1Detector &d : detectors){
    d.Rotate(angle,axis);
  } 
}
