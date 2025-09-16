#include "PCollision.h"
#include "TMath.h"
#include <Riostream.h>

ClassImp(PCollision)

  PCollision::PCollision(): TObject(),
    fcheck(1),
    fDelta(0.),
    ft(0.),
    fPx0(0.),
    fPy0(0.),
    fPz0(0.),
    fPtheta0(0.),
    fPphi0(0.),
    fX(0.),
    fY(0.),
    fZ(0.),
    fC1(0.),
    fC2(0.),
    fC3(0.),
    fH(13.5),
    fRbp(3.),
    fRl1(4.),
    fRl2(7.){
  }


  PCollision::~PCollision() {
      //cout<<"DESTRUCTOR\n";
  }


  void PCollision::SetPar(double x0, double y0, double z0 , double theta0 , double phi0){
    fcheck = 1;
    fDelta = 0.;
    ft = 0.;
      
    fX = 0.;
    fY = 0.;
    fZ = 0.;
    
    fPx0 = x0;
    fPy0 = y0;
    fPz0 = z0;
      
    fPtheta0 = theta0;
    fPphi0 = phi0;
    
    fC1 = TMath::Sin(fPtheta0)*TMath::Cos(fPphi0);
    fC2 = TMath::Sin(fPtheta0)*TMath::Sin(fPphi0);
    fC3 = TMath::Cos(fPtheta0);
      
  }


  void PCollision::CalcPIntersection(double r){
      double temp1 = fPx0*fC1 + fPy0*fC2 ;
      double temp2 = fC1*fC1 + fC2*fC2 ;
      fDelta = temp1*temp1 - temp2*( fPx0*fPx0 + fPy0*fPy0 - r*r);
      
      ft = (TMath::Sqrt( fDelta ) - temp1)/temp2;
      
      fX = fPx0 + ft*fC1;
      fY = fPy0 + ft*fC2;
      fZ = fPz0 + ft*fC3;
    
      //controllo che il punto sia sul rivelatore
      if( (int)r == int(fRl1) || (int)r == (int)fRl2 ){
          IntersectionCheck(r);
      }

  }


  void PCollision::IntersectionCheck(double r){
    double temp3 = fX*fX + fY*fY - r*r;
    
    if( TMath::Abs(temp3) > 1.e-6 || TMath::Abs(fZ) > fH ) {
        //qui non uso temp3 == 0 poiche' sono variabile double -> ogni double differisce da se stesso per un epsilon minore di 10^-6 -> la condizione potrebbe sembrare falsa
        
      fcheck = 0;
    }
  }
