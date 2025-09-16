#include "TrackRec.h"
#include "Riostream.h"

ClassImp(TrackRec)

TrackRec::TrackRec (unsigned int seed):  TRandom3(seed),
  fRaggioL1(4.),        //cm
  fRaggioL2(7.),
  fSigmaZ(0.012),
  fSigmaA(0.003),
  fDeltaPhiMax(0.004)     
{
}

TrackRec::~TrackRec() {
    //cout<<"DESTRUCTOR\n";
}

double TrackRec::genZrec (double zHit){
  double zrec = zHit + Gaus(0., fSigmaZ);
  return zrec;
}

double TrackRec::genPhirec(double phiHit , double r){
  double phirec = phiHit + (Gaus(0, fSigmaA))/r;
  return phirec;
}

double TrackRec::TrackletsRec(double x1, double y1, double x2, double y2){
    double zInt;
    
    if(x1 == x2){
        zInt = x1;
    }
    else{
        double m = ( y2 -y1 )/( x2 - x1 );
        double q = y1 - m*x1;
        zInt = - q/m;
    }
    return zInt;
}
