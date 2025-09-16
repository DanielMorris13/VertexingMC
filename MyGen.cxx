#include <Riostream.h>
#include "MyGen.h"
#include "TMath.h"
#include "TAxis.h"
#include "TFile.h"


ClassImp(MyGen)

MyGen::MyGen(): TRandom3(),
    fXYZ(0.),
    frmsXY(0.),
    frmsZ(0.),
    fMaxUnifMult(0),
    fPi(2*TMath::Pi()),
    fRmsTheta(0.001),
    fHMult(NULL),
    fHEta(NULL),
    fZnoise(0.),
    fPhiNoise(0.),
    fpathRootFiles("")
        {
        
        }


MyGen::MyGen( TString pathRootFiles, unsigned int seed, int maxUnifMult): TRandom3(seed),
    fXYZ(0.),
    frmsXY(0.01),            //cm
    frmsZ(5.3),
    fMaxUnifMult(maxUnifMult+0.25),
    fPi(2*TMath::Pi()),
    fRmsTheta(0.001),
    fHMult(NULL),
    fHEta(NULL),
    fZnoise(0.),
    fPhiNoise(0.),
    fpathRootFiles(pathRootFiles)
    {
        InHisto();
    }



MyGen::~MyGen() {
    //cout<<"DESTRUCTOR\n";
}


void MyGen::InHisto(){
    
    TFile fkinem(fpathRootFiles+"kinem.root");
    
    //-------------------------------------------------------------------------------
    fHMult = (TH1F*)fkinem.Get("hmul");
    fHMult->SetDirectory(0);
    
    //-------------------------------------------------------------------------------
    TH1F *temp1 = (TH1F*)fkinem.Get("heta");
    temp1->SetDirectory(0);
    
    fkinem.Close();
    
    temp1->SetMinimum(0);
    TAxis *xa=temp1->GetXaxis();
    double step = xa->GetBinWidth(1);
    int b1=xa->FindBin(-2.);
    int b2=xa->FindBin(2.);
    double xlow=xa->GetBinLowEdge(b1);
    double xhig=xa->GetBinUpEdge(b2);
    double nobins=b2-b1+1;
    double step2 = (xhig-xlow)/nobins;
    //cout << "Check: "<<step<<"; "<<step2<<endl;
      
    TH1F* temp2 = new TH1F("fHEtaNew","#eta distribution 2",nobins,xlow,xhig);
    double j=1;
    for(double i=b1;i<=b2;i++){
        temp2->SetBinContent(j++,temp1->GetBinContent(i));
      }
    
    fHEta = (TH1F*)temp2->Clone();
    fHEta->SetDirectory(0);

}

double MyGen::genVerX(){
    double x = Gaus(fXYZ, frmsXY);
    return x;
}


double MyGen::genVerY(){
  double y = Gaus(fXYZ, frmsXY);
    return y;
}

double MyGen::genVerZ(){
  double z = Gaus(fXYZ, frmsZ);
    return z;
}

double MyGen::genUnifVerZ(){
  double z = -30. + Rndm()*60.;
  return z;
}


int MyGen::genUnifMult(){
    int a = (int)( Rndm()*fMaxUnifMult );
    return a;
}


int MyGen::genDistrMult(){
    int moltepl = (int)(fHMult->GetRandom());
    return moltepl;
}


double MyGen::genPhi(){
    double phi = fPi*Rndm();
    return phi;
}

double MyGen::genThetaDistr(){
    double eta = fHEta->GetRandom();
    return 2*TMath::ATan(TMath::Exp(-eta));
}

double MyGen::genThetaScatt(){
    double thetascatt = Gaus(0, fRmsTheta);
    return thetascatt;
}


void MyGen::genNoise(){
  fPhiNoise = Rndm() *fPi;
  fZnoise = -13.5 + Rndm()*27.0;
}
