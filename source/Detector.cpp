#include "Detector.h"
#include "YY1Detector.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Detector * Detector::Create(string type)
{
  if(type == "YY1"){
    return new YY1Detector();
  }
  else{
    cout << "Detector::Create(): Unknown detector type: " << type << endl;
    exit(EXIT_FAILURE);
  }
}

void Detector::SetPosition(TVector3 &pos)
{
  position = pos;
}

void Detector::SetPosition(double x, double y, double z)
{
  position.SetXYZ(x,y,z);
}

TVector3 Detector::GetPosition()
{
  return position;
}

void Detector::SetNormal(TVector3 &n)
{
  normal = n.Unit();
}

void Detector::SetNormal(double nx, double ny, double nz)
{
  normal.SetXYZ(nx,ny,nz);
  normal.SetMag(1);
}

TVector3 Detector::GetNormal()
{
  return normal;
}

void Detector::SetOrientation(TVector3 &ori)
{
  orientation = ori.Unit();
}

void Detector::SetOrientation(double ox, double oy, double oz)
{
  orientation.SetXYZ(ox,oy,oz);
  orientation.SetMag(1);
}

TVector3 Detector::GetOrientation()
{
  return orientation;
}

void Detector::SetDeadLayer(double dead_layer)
{
  deadLayer = dead_layer;
}

double Detector::GetDeadLayer()
{
  return deadLayer;
}

double Detector::GetSolidAngle(TVector3 &pos)
{
  double result = 0.;
  int nSeg = GetNSegments();
  for(int i=0; i<nSeg; i++){
    result += GetSegSolidAngle(i,pos);
  }
  return result;
}
