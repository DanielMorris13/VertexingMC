#include "reconstruction.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "Hits.h"
#include "TClonesArray.h"
#include <Riostream.h>
#include "vector"
#include "TrackRec.h"
#include <algorithm>
#include "TNtuple.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TString.h"

using namespace std;

void reconstruction(TString nameFile){
    
    delete gRandom;
    TrackRec *trackRec = new TrackRec(567);
    gRandom = trackRec;
    
    vector <double> container;
    container.reserve(100);
    
    //Nuovi TClonesArray per punti ricostruiti con lo smearing
    TClonesArray *smearingL1 = new TClonesArray("Hits",200);
    TClonesArray &ptr1 = *smearingL1;
    
    TClonesArray *smearingL2 = new TClonesArray("Hits",200);
    TClonesArray &ptr2 = *smearingL2;
    
    //Dichiarazione stuct per leggere il tree
    typedef struct {
        double X,Y,Z;
        int mult;} Vertex;
    static Vertex point;
    
    //Dichiarazione TClonesArray per leggere il tree
    TClonesArray *hitsL1 = new TClonesArray("Hits",200);
    TClonesArray *hitsL2 = new TClonesArray("Hits",200);
    
    TFile hfile(nameFile +".root");
    
    //lettura Ttree e branch
    TTree *tree = (TTree*)hfile.Get("tree");
    TBranch *b1= tree->GetBranch("Vertex");
    TBranch *b2= tree->GetBranch("Hits_L1");
    TBranch *b3= tree->GetBranch("Hits_L2");
    
    //Definizione degli indirizzi per la lettura dei dati su Ttree
    b1->SetAddress(&point.X);
    b2->SetAddress(&hitsL1);
    b3->SetAddress(&hitsL2);
    
    TFile *ofile = new TFile("rec_" + nameFile + ".root","RECREATE");
    
    //Ntuple per salvare elementi per analisi
    double ztrue, zrec, multtrue;
    TNtuple *nt = new TNtuple( "tntuple", "Reconstruction", "ztrue:zrec:multtrue" );
    nt->Branch("ztrue", &ztrue);
    nt->Branch("zrec", &zrec);
    nt->Branch("multtrue", &multtrue);
    
    //10^4 bin -> 100 um
    TH1D *h1 = new TH1D("h1", "h1", 10000, -50., 50.);
    h1->SetDirectory(0);
    
    //loop sugli ingressi nel TTree
    for(int ev=0; ev<tree->GetEntries(); ev++){
        //prendo evento del tree (con relativi vertici e hits)
        tree->GetEvent(ev);
       
        //prendo hits di L1 e per ognuno faccio lo smearing
        int num1 = hitsL1->GetEntries();
        for(int j=0; j<num1; j++){
            Hits *tst=(Hits*)hitsL1->At(j);
            new(ptr1[j])Hits(trackRec->genZrec(tst->getZ()),trackRec->genPhirec(tst->getPhi(),trackRec->getRL1()));
        }
        
        //prendo hits di L2 e per ognuno faccio lo smearing
        int num2 = hitsL2->GetEntries();
        for(int j=0; j<num2; j++){
          Hits *tst2=(Hits*)hitsL2->At(j);
          new(ptr2[j]) Hits(trackRec->genZrec(tst2->getZ()),trackRec->genPhirec(tst2->getPhi(),trackRec->getRL2()));
        }
        
        //ricostruzione tracklets e vertici
        for(int i = 0 ; i<num1 ; i++){
          Hits *sm1 = (Hits*)smearingL1->At(i);
          for(int j = 0 ; j < num2 ; j++){
            Hits *sm2 = (Hits*)smearingL2->At(j);
            if( TMath::Abs((sm2->getPhi()-sm1->getPhi()))<trackRec->getDelPhiMax() ){
              double tempZ = trackRec->TrackletsRec(sm1->getZ(),trackRec->getRL1(),sm2->getZ(),trackRec->getRL2());
                    container.push_back(tempZ);
                    h1->Fill(tempZ);
            }
          }
        }
        
        //ordino gli elementi del vettore
        sort(container.begin(),container.end());
        
        int dimensione = container.size();
        //-------------------------------------------------------------------------------------------------------------------------------- CASO VETTORE VUOTO
        if(dimensione == 0){
            zrec = 200;
        }
        else{
            int maxFreqHist = h1->GetBinContent(h1->GetMaximumBin());              // Max. frequenza dell'istogramma
            double maxDistanza = 0.1;                                              // Massima distanza fra due picchi [cm]
            
            //---------------------------------------------------------------------------------------------------------------------------- CASO FREQ MAX = 1
            if(maxFreqHist == 1){

                if( dimensione == 1 ){
                    zrec = container[0];
                }
                else {
                    double somma = 0.;
                    double media = 0.;
                    int count = 1;                                                 // # massimo di elementi in un gruppo
                    int ngruppi = 0;                                               // # numero di gruppi elementi vicini { N = 0 un gruppo , N > 0 piu' gruppi}
                
                    for(int i = 0; i < dimensione ; i++){
                        int j = 1;                                                 // contatore di elementi in un gruppo
                        somma = container[i];
                        while( (TMath::Abs(container [ i ] - container[i+j]) < maxDistanza) && (( i+j ) < dimensione )) {
                            somma += container[i+j];
                            j++;
                        }
                        if ( j == count ) ngruppi++;
                        else if( j > count ) {
                            media = somma/j;
                            count=j;
                            ngruppi=0;
                        }
                        i = i+j-1;
                    }
                
                    if(ngruppi==0) {
                        zrec = media;
                    }
                    else {
                        zrec = 200;
                    }
                }
            }
            //---------------------------------------------------------------------------------------------------------------------------- CASO FREQ MAX >1
            else {
                int maxFreq = 0;
                double zMax = 0.;
                int nmax = 1;                                                           // # picchi con stessa frequenza
                int binmaxf = 0;
                double valMedioZ = 0;
                bool condition = 0;                                                     // 1 = Ricostruisco  0 = non ricostruisco
                
                for(int i = 0; i < dimensione ; i++){
                    int bintemp = h1->FindBin(container[i]);
                    int freqtemp = h1->GetBinContent(bintemp);
                    double ztemp = h1->GetBinCenter(bintemp);
                    
                    if(freqtemp > maxFreq) {
                        maxFreq = freqtemp;
                        zMax = ztemp;
                        nmax = 1;
                        binmaxf = bintemp;
                        condition = 1;
                    }
                    else if(freqtemp == maxFreq && bintemp != binmaxf){
                        if (TMath::Abs(zMax - ztemp) > maxDistanza){
                            zMax = ztemp;
                            binmaxf = bintemp;
                            nmax=1;
                            condition = 0; 
                        }
                        else {
                            valMedioZ = (zMax + ztemp)/2;
                            zMax = ztemp;
                            binmaxf = bintemp;
                            nmax++;
                            condition = 1;
                        }
                     }
                }
                
            double Suma = 0;
            int k = 0;                                                                  // k = # di elementi nell'intorno considerato
            
            if(condition == 1 && nmax == 1){
                for(int i =0; i < dimensione ; i++){
                    if(TMath::Abs(container[i]-zMax)<0.1){
                        Suma+=container[i];
                        k++;
                    }
                }
              zrec = Suma/k;
            }
            else if(condition == 1 && nmax>=2 ){
                    for( int i =0 ; i < dimensione ; i++ ){
                        if(TMath::Abs(container[i]-valMedioZ)<0.1){
                            Suma+=container[i];
                            k++;
                        }
                    }
                zrec = Suma/k;
            }
            else if (condition==0 && nmax==1){
                  zrec = 200;
                }
        }
  }
        ztrue = point.Z;
        multtrue = point.mult;
        std::cout<<"evento: "<<ev<< " zrec: "<< zrec<<" ztrue: "<<ztrue<<std::endl;
        nt->Fill(ztrue, zrec, multtrue);
        container.clear();
        h1->Reset("ICESM");
        smearingL1->Clear();
        smearingL2->Clear();
    }
    
    ofile->Write();
    ofile->Close();
}
