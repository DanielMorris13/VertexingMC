#include "simulation.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include <Riostream.h>
#include "MyGen.h"
#include "PCollision.h"
#include "Scattering.h"
#include "Hits.h"
#include "TString.h"


void simulation(TString pathRootFiles, TString nameFile, int kNevents, bool scatt , int Nnoise){
    
    delete gRandom;
    MyGen *Gen = new MyGen(pathRootFiles, 13013,60);       
    gRandom = Gen;
    
    PCollision *Col = new PCollision();
    Scattering *Sca = new Scattering(scatt);
   
    TFile *ofile = new TFile( nameFile + ".root", "RECREATE" );
    TTree *tree = new TTree( "tree", "Vertici e Hits" );

    typedef struct {
        double X,Y,Z;
        int mult;} Vertex;
    static Vertex point;
    
    TClonesArray *ptrhits2 = new TClonesArray("Hits",200);
    TClonesArray &hits2 = *ptrhits2;
    
    TClonesArray *ptrhits3 = new TClonesArray("Hits",200);
    TClonesArray &hits3 = *ptrhits3;
    
    tree->Branch("Vertex", &point.X,"X/D:Y:Z:mult/I");
    tree->Branch("Hits_L1", &ptrhits2);
    tree->Branch("Hits_L2", &ptrhits3);

    
    //loop su eventi
    for( int i = 0 ; i<kNevents ; i++ ){
        std::cout<<"VERTEX n° "<<i<<std::endl;
    //----------------------------------------------------- Modificare SOLO QUI------------------------------------------------------------------------------------------------------------------------
        point.mult = Gen->genDistrMult();        //Gen->genUnifMult(); -> per distr. uniforme;    Gen->genDistrMult(); -> per distr. assegnata;    Gen->genFixMult(); -> per molt. fissata;
        point.X = Gen->genVerX();
        point.Y = Gen->genVerY();
        point.Z = Gen->genUnifVerZ();                //Gen->genVerZ(); per distr. normale;            Gen->genUnifVerZ() -> per distr. uniforme tra (-30,30);
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        int tothitsL1=0;
        int tothitsL2=0;

     //loop sulla molteplicità
        for(int j = 0; j<point.mult; j++){
            
            //intersezione con beam pipe
            Col->SetPar(point.X, point.Y, point.Z, Gen->genThetaDistr() ,Gen->genPhi());
            Col->CalcPIntersection(Col->getRbp());
            
            //scattering beam pipe
              Sca->SetInitialPars(Col->getTheta0(), Col->getPhi0());
              Sca->Rotate(Gen->genThetaScatt(), Gen->genPhi(), Col->getCheck());
            
            //intersezione con Layer1
            Col->SetPar(Col->getXc(), Col->getYc(), Col->getZc(), Sca->getNewTheta(), Sca->getNewPhi());
            Col->CalcPIntersection(Col->getRl1());
            
            if(Col->getCheck()==1){
                new(hits2[tothitsL1])Hits(Col->getZc(), Col->getPhi0(), j);
                tothitsL1++;
            }
            
            //scattering Layer1
            Sca->SetInitialPars(Col->getTheta0(), Col->getPhi0());
            Sca->Rotate(Gen->genThetaScatt(), Gen->genPhi(), Col->getCheck());
                
            //intersezione con Layer2
            Col->SetPar(Col->getXc(), Col->getYc(), Col->getZc(), Sca->getNewTheta(), Sca->getNewPhi());
            Col->CalcPIntersection(Col->getRl2());
            
            if(Col->getCheck()==1){
                new(hits3[tothitsL2])Hits(Col->getZc(), Col->getPhi0(), j);
                tothitsL2++;
            }
            
        }
    
        //Noise L1
        for(int i = 0; i < Nnoise ; i++){
          Gen->genNoise();
          new( hits2[tothitsL1] ) Hits(Gen->getZnoise(), Gen->getPhiNoise(), -20);
          tothitsL1++;
        }
        
        //Noise L2
        for(int i = 0; i < Nnoise ; i++){
          Gen->genNoise();
          new( hits3[tothitsL2] ) Hits(Gen->getZnoise(), Gen->getPhiNoise(), -20);
          tothitsL2++;
        }
        
        tree->Fill();
        ptrhits2->Clear();
        ptrhits3->Clear();
    }
    
    ofile->Write();
    ofile->Close();
  }

