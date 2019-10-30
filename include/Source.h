#ifndef SOURCE_H
#define SOURCE_H
#include <vector>
#include <string>
#include <TVector3.h>

class Source {
  private:
    TVector3 position;
    std::string particle;
    std::vector<double> peakEnergies;

  public:
    Source() = default;
    ~Source() = default;
    
    void SetParticle(std::string p);
    
    std::string GetParticle();
    
    void AddPeak(double energy);
    
    void RemovePeak(int i);
    
    double GetPeak(int i);
    
    std::vector<double> & GetPeaks();
    
    int GetNPeaks();
    
    void SetPosition(TVector3 &pos);
    void SetPosition(double x, double y, double z);
    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);
    TVector3 GetPosition();
};
#endif
