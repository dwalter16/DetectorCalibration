#include "Source.h"
#include <algorithm>

using namespace std;

void Source::SetParticle(std::string p)
{
  particle = p;
}
    
string Source::GetParticle()
{
  return particle;
}
    
void Source::AddPeak(double energy)
{
  peakEnergies.push_back(energy);
  //Alway list energies in ascending order.
  sort(peakEnergies.begin(),peakEnergies.end());
}
    
void Source::RemovePeak(int i)
{
  peakEnergies.erase(peakEnergies.begin()+i);
}
    
double Source::GetPeak(int i)
{
  return peakEnergies.at(i);
}
    
vector<double> & Source::GetPeaks()
{
  return peakEnergies;
}
    
int Source::GetNPeaks()
{
  return peakEnergies.size();
}
    
void Source::SetPosition(TVector3 &pos)
{
  position = pos;
}

void Source::SetPosition(double x, double y, double z)
{
  position.SetXYZ(x,y,z);
}

void Source::SetX(double x)
{
  TVector3 pos = GetPosition();
  pos.SetX(x);
  SetPosition(pos);
}

void Source::SetY(double y)
{
  TVector3 pos = GetPosition();
  pos.SetY(y);
  SetPosition(pos);
}


void Source::SetZ(double z)
{
  TVector3 pos = GetPosition();
  pos.SetZ(z);
  SetPosition(pos);
}

TVector3 Source::GetPosition()
{
  return position;
}
