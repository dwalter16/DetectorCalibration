#include "Detector.h"
#include "YY1Detector.h"
#include "YY1Array.h"
#include <iostream>
#include <cstdlib>
#include <TMath.h>

using namespace std;
using namespace TMath;

Detector * Detector::Create(string type)
{
  if(type == "YY1"){
    return new YY1Detector();
  }
  else if(type == "YY1Array"){
    return new YY1Array();
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

TVector3 Detector::GetNormal()
{
  return normal;
}

TVector3 Detector::GetOrientation()
{
  return orientation;
}

void Detector::RotateX(double angle)
{
  double radians = angle * DegToRad();
  normal.RotateX(radians);
  orientation.RotateX(radians);
}
 
void Detector::RotateY(double angle)
{
  double radians = angle * DegToRad();
  normal.RotateY(radians);
  orientation.RotateY(radians);
}
    
void Detector::RotateZ(double angle)
{
  double radians = angle * DegToRad();
  normal.RotateZ(radians);
  orientation.RotateZ(radians);
}
    
void Detector::Rotate(double angle, TVector3 &axis)
{
  double radians = angle * DegToRad();
  normal.Rotate(radians,axis);
  orientation.Rotate(radians,axis);
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

double Detector::GetEffectiveThickness(int i, TVector3 &pos)
{
  TVector3 segPos = GetSegPosition(i);
  TVector3 sourceView = segPos - pos;

  double invCos = 1./Abs(normal.Dot(sourceView.Unit()));
  //double invCos = 1./cos(TMath::Pi() - sourceView.Angle(normal));
  double teff = deadLayer * invCos;

  return teff;
}
