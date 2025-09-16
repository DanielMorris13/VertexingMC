#ifndef MYGEN_H
#define MYGEN_H

#include "TRandom3.h"
#include "TH1F.h"
#include "TString.h"

class MyGen : public TRandom3 {
   
public:
    
    MyGen();
    MyGen(TString pathRootFiles, unsigned int seed = 1234, int maxUnifMult = 50);
    
    virtual ~MyGen();
    
    //coordinate vertice
    double genVerX();
    double genVerY();
    double genVerZ();
    double genUnifVerZ();
    
    //molteplicità
    int genFixMult() {return 50;}
    int genUnifMult();
    int genDistrMult();
    
    //direzione
    double genPhi();                //uniforme tra [0,2Pi]
    double genThetaDistr();
    double genThetaScatt();
    
    //punti di noise
    void genNoise();
    double getZnoise() const {return fZnoise;}
    double getPhiNoise() const {return fPhiNoise;}

    void setPathRootFiles(TString path) {fpathRootFiles=path;}
    TString getPathRootFiles() const {return fpathRootFiles;}
    
private:
  
    const double fXYZ;
    const double frmsXY;
    const double frmsZ;

    double fZnoise;
    double fPhiNoise;
    
    int fMaxUnifMult;
   
    const double fPi;
    
    const double fRmsTheta;     //scattering
    
    TH1F *fHMult;
    TH1F *fHEta;
    TString fpathRootFiles;
    
    void InHisto();
    
    ClassDef(MyGen,1)
};

#endif
