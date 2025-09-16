#include "Scattering.h"
#include "TMath.h"
#include "Riostream.h"

ClassImp(Scattering)

  Scattering::Scattering(): TObject(),
    fSwitch(0),
    fTheta0(0.),
    fPhi0(0.),
    fThetaNew(0.),
    fPhiNew(0.),
    fPi(TMath::Pi())
  {
    for(int i = 0 ; i < 3 ; i++ ){
      for(int j = 0 ; j < 3 ; j++ ){
        fRotationMatrix[i][j] = 0.;
      }
      fRotationVector[i] = 0.;
      fCi[i] = 0.;
    }
  }


  Scattering::Scattering(bool on): TObject(),
    fSwitch(on),
    fTheta0(0.),
    fPhi0(0.),
    fThetaNew(0.),
    fPhiNew(0.),
    fPi(TMath::Pi())
  {
    for(int i = 0 ; i < 3 ; i++ ){
      for(int j = 0 ; j < 3 ; j++ ){
        fRotationMatrix[i][j] = 0.;
      }
      fRotationVector[i] = 0.;
      fCi[i] = 0.;
    }
  }


Scattering::~Scattering() {
    //cout<<"DESTRUCTOR\n";
}


  void Scattering::SetInitialPars(double theta0, double phi0){
      
        fThetaNew = 0.;
        fPhiNew = 0.;
      
        for(int i = 0 ; i < 3 ; i++ ){
            for(int j = 0 ; j < 3 ; j++ ){
                fRotationMatrix[i][j] = 0.;
            }
            fRotationVector[i] = 0.;
            fCi[i] = 0.;
        }
      
        fTheta0 = theta0;
        fPhi0 = phi0;
      
  
  }


  void Scattering::Rotate(double RndmTheta, double RndmPhi, bool checkCollision){
    if(fSwitch == 1 && checkCollision == 1){
    
      fRotationMatrix[0][0] = -TMath::Sin( fPhi0 );
      fRotationMatrix[1][0] = TMath::Cos( fPhi0 );
      fRotationMatrix[2][0] = 0.;
      fRotationMatrix[0][1] = -TMath::Cos( fPhi0 )*TMath::Cos( fTheta0 );
      fRotationMatrix[1][1] = -TMath::Cos( fTheta0 )*TMath::Sin( fPhi0 );
      fRotationMatrix[2][1] = TMath::Sin( fTheta0 );
      fRotationMatrix[0][2] = TMath::Sin( fTheta0 )*TMath::Cos( fPhi0 );
      fRotationMatrix[1][2] = TMath::Sin( fTheta0 )*TMath::Sin( fPhi0 );
      fRotationMatrix[2][2] = TMath::Cos( fTheta0 );
        
      fRotationVector[0] = TMath::Sin( RndmTheta )*TMath::Cos( RndmPhi );
      fRotationVector[1] = TMath::Sin( RndmTheta )*TMath::Sin( RndmPhi );
      fRotationVector[2] = TMath::Cos(RndmTheta);
      
      for ( int i = 0 ; i < 3 ; i++ ){
        fCi[i] = 0.;
        for( int j = 0 ; j < 3 ; j++ ){
          fCi[i] += fRotationMatrix[i][j] * fRotationVector[j];
        }
      }
        
        Converter();
    }
    else{
        fThetaNew = fTheta0;
        fPhiNew = fPhi0;
    }
  }

    void Scattering::Converter(){
 
        double temp1 = fCi[1];
        double tangentePhi = fCi[1]/fCi[0];
        double temp = TMath::ATan( tangentePhi );
        if(temp1 > 0.){                                                 //sono nel 1 o 2 quadrante
          if(tangentePhi > 0) fPhiNew = temp;                           //sono nel primo quadrante
          else fPhiNew = ( temp + fPi);                                 //sono nel secondo quadrante
        }
        else {                                                          //sono nel 3 o 4 quadrante
          if( tangentePhi > 0. ) fPhiNew = ( temp + fPi );              //sono nel terzo quadrante
          else fPhiNew = temp + fPi*2.;                                 //sono nel quarto quadrante
        }
        fThetaNew = TMath::ACos(fCi[2]);
      
    }
