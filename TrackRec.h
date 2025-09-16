#ifndef TRACKREC_H
#define TRACKREC_H

#include "TRandom3.h"

class TrackRec : public TRandom3 {
    
  public:  
    TrackRec(unsigned int seed);
    virtual ~TrackRec();

    double genZrec (double zHit);
    double genPhirec (double phiHit ,double r);
    
    double getRL1() const {return fRaggioL1;}
    double getRL2() const {return fRaggioL2;}
    double getDelPhiMax () {return fDeltaPhiMax;}
    
    double TrackletsRec(double x1, double y1, double x2, double y2);
    
private:
  const double fRaggioL1;
  const double fRaggioL2;

  const double fSigmaZ;
  const double fSigmaA;

  const double fDeltaPhiMax;

    ClassDef(TrackRec,1)

};

#endif
