#include "YY1Detector.h"

YY1Detector::YY1Detector()
{
  position.SetXYZ(0,0,0);
  normal.SetXYZ(0,0,1);
  orientation.SetXYZ(0,1,0);
  deadLayer = 150.;
}

int YY1Detector::GetNSegments()
{
  return 16;
}

TVector3 YY1Detector::GetSegPosition(int i)
{
  TVector3 segPos = position + orientation * (50 + 5.*(i+0.5));
  return segPos;
}

double YY1Detector::GetSegSolidAngle(int i, TVector3 &pos)
{
  double thetaBinEdge_low = 0;//deg
  double thetaBinEdge_high = 0;//deg
  double InnerRadius = 50.;//mm
  double OuterRadius = 130.;//mm
  double pitch = 5.; //mm
  double YY1SolidAngle = 0;//msr
  double pi = TMath::Pi();
  double Zoffset = position.Z();//Detector position along z axis in mm

  thetaBinEdge_high = std::atan2(OuterRadius-(15-i)*pitch,Zoffset) * (180./pi);
  thetaBinEdge_low = std::atan2(OuterRadius-(15-i+1)*pitch,Zoffset) * (180./pi);

  if(i==15) YY1SolidAngle = 0.5*(std::cos(thetaBinEdge_high*(pi/180.))-std::cos(thetaBinEdge_low*(pi/180.)))*4*pi*1000.*0.291;
  else if(i==14) YY1SolidAngle = 0.5*(std::cos(thetaBinEdge_high*(pi/180.))-std::cos(thetaBinEdge_low*(pi/180.)))*4*pi*1000.*0.543;
  else if(i==13) YY1SolidAngle = 0.5*(std::cos(thetaBinEdge_high*(pi/180.))-std::cos(thetaBinEdge_low*(pi/180.)))*4*pi*1000.*0.735;
  else YY1SolidAngle = 0.5*(std::cos(thetaBinEdge_high*(pi/180.))-std::cos(thetaBinEdge_low*(pi/180.)))*4*pi*1000.*0.883;

  return YY1SolidAngle/8.0;
  
}
