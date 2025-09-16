#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "Hits.h"
#include "TClonesArray.h"
#include <Riostream.h>
#include "TH1D.h"


void deltaphi(){
    
    //Dichiarazione stuct per leggere il tree
    typedef struct {
        double X,Y,Z;
        int mult;} Vertex;
    static Vertex point;
    
    //Dichiarazione TClonesArray per leggere il tree
    TClonesArray *hitsL1 = new TClonesArray("Hits",200);
    TClonesArray *hitsL2 = new TClonesArray("Hits",200);
    
    TFile hfile("sim.root");
    
    //lettura ttree e branch
    TTree *tree = (TTree*)hfile.Get("tree");
    TBranch *b1= tree->GetBranch("Vertex");
    TBranch *b2= tree->GetBranch("Hits_L1");
    TBranch *b3= tree->GetBranch("Hits_L2");
    
    //Definizione degli indirizzi per la lettura dei dati su ttree
    b1->SetAddress(&point.X);
    b2->SetAddress(&hitsL1);
    b3->SetAddress(&hitsL2);
    
    TFile *ofile = new TFile("outputDeltaPhi.root","RECREATE");
    
    TH1D *h1 = new TH1D("h1","h1", 50, -0.01, 0.01);
    h1->SetDirectory(0);
    
    for(int ev=0; ev<tree->GetEntries(); ev++){
        cout<<"EVENTO: "<<ev<<endl;
        tree->GetEvent(ev);
        
        double delta = 0.;
        
        int num1 = hitsL1->GetEntries();
        int num2 = hitsL2->GetEntries();
        
        for(int i=0; i<num1; i++){
            Hits *tst=(Hits*)hitsL1->At(i);
            int index1 = tst->getIndex();
            if(index1 >= 0){
                for( int j=0; j<num2; j++){
                    Hits *tst2=(Hits*)hitsL2->At(j);
                    int index2 = tst2->getIndex();
                    if(index2 == index1){
                        delta = tst2->getPhi() - tst->getPhi();
                        h1->Fill(delta);
                    }
                }
            }
        }
    }
    
    h1->Draw();
    h1->Write();
    
    hfile.Close();
    
    ofile->Write();
    ofile->Close();
}
