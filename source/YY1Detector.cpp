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
  double InnerRadius = 50.;//mm
  double OuterRadius = 130.;//mm
  double pitch = 5.; //mm
  double pi = TMath::Pi();
  double ThetaStripSubtend[16];
  for(int m=0; m<16; m++){
    if(i==15) ThetaStripSubtend[m] = 13.102*(pi/180.);
    else if(i==14) ThetaStripSubtend[m] = 24.5*(pi/180.);
    else if(i==13) ThetaStripSubtend[m] = 33.07*(pi/180.);
    else ThetaStripSubtend[m] = 39.756*(pi/180.);   
  }
  
  const int ner = 6;//Number of radial elements
  const int net = 6;//Number of angular elements

  double cosangleBetween[ner][net]={0};
  double dist[ner][net]={0};
  double AreaElement[ner][net]={0};
  double SAElement[ner][net]={0};
  double SAstripTotal = 0;

  TVector3 element;
  TVector3 view;

  for(int j=0; j<ner; j++){
    for(int k=0; k<net; k++){
       
      double rseg= InnerRadius + i*pitch + (0.5+j)*(pitch/ner);
      double tseg = -ThetaStripSubtend[i]/2. + (0.5+k)*(ThetaStripSubtend[i]/net);

      element = rseg*orientation;
      element.Rotate(tseg,normal);
      view = (position + element) - pos; //view of strip element from source position
      cosangleBetween[j][k] = abs(normal.Dot(view.Unit()));
      dist[j][k] = view.Mag(); 
      AreaElement[j][k] = (pitch/ner)*(element.Mag()*ThetaStripSubtend[i]/net);//Approximate area of element
      SAElement[j][k] = AreaElement[j][k] * cosangleBetween[j][k] / (dist[j][k]*dist[j][k]);

      SAstripTotal += SAElement[j][k];
	
    }
  }
  
  return SAstripTotal;
}
