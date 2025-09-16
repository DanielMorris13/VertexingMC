#ifndef SCATTERING_H
#define SCATTERING_H

#include "TObject.h"

class Scattering : public TObject {
    
  public:
  
  Scattering();
  Scattering(bool on);
  virtual ~Scattering();
    
    
  //- Direzione PRIMA dello scattering, nel SR lab
  void SetInitialPars( double theta0 , double phi0 );
    
  //- Calcola direzione DOPO lo scattering, già convertita nel SR lab
  void Rotate(double thetascatt, double phiscatt, bool checkCollision );

  //- Get angoli DOPO lo scattering, nel SR lab
  double getNewTheta() const   {return fThetaNew;}
  double getNewPhi() const     {return fPhiNew;}
    
    
  private:
    
  //- Converte angoli DOPO scattering da SR' --> SR lab
  void Converter();
    
  
  bool fSwitch;     //On == 1  Off == 0

  //- Angoli PRIMA dello scattering, nel SR lab
  double fTheta0;
  double fPhi0;


  //- Angoli DOPO dello scattering, nel SR lab
  double fThetaNew;
  double fPhiNew;

  double fRotationMatrix[3][3];
  double fRotationVector[3];
  double fCi[3];
    
  const double fPi;


  ClassDef(Scattering,1)
};

#endif
