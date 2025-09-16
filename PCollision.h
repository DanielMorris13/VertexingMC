#ifndef PCOLLISION_H
#define PCOLLISION_H

#include "TObject.h"

class PCollision : public TObject{
    
  public:
    
    PCollision();
    virtual ~PCollision();

    
    void SetPar(double x0, double y0, double z0 , double theta0 , double phi0);
    void CalcPIntersection(double r);
    
    
    bool getCheck() const   {return fcheck;}
    
    double getTheta0()const     {return fPtheta0;}
    double getPhi0()const       {return fPphi0;}
    
    double getXc()const     {return fX;}
    double getYc()const     {return fY;}
    double getZc()const     {return fZ;}

    double getRbp() const   {return fRbp;}
    double getRl1() const   {return fRl1;}
    double getRl2() const   {return fRl2;}

    
  private:
    
    //- Funzione che verifica che il punto di intersezione calcolato stia sul rivelatore
    void IntersectionCheck(double r);
    
    
    bool fcheck;        // == 1 il punto intersezione e' sul rivelatore , ==  0 altrimenti

    double fDelta;
    double ft;

    // -  Punto iniziale dal quale calcoliamo l'intersezione
    double fPx0;
    double fPy0;
    double fPz0;
    
    //- Direzione iniziale particella
    double fPtheta0;
    double fPphi0;
    
    // - Punto di intersezione con beam pipe / layer
    double fX;
    double fY;
    double fZ;
    
    // - Componenti del versore parallelo alla retta che interseca bp / layer (Versore parallelo alla direzione della particella)
    double fC1;
    double fC2;
    double fC3;
    
    const double fH;      // mezza lunghezza del rivelatore
    const double fRbp;    // raggio beam pipe
    const double fRl1;    // raggio layer1
    const double fRl2;    // raggio layer2
    
    
    ClassDef(PCollision,1)
};
#endif
