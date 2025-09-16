#include "ztrue.h"

#include "TFile.h"
#include "TMath.h"
#include <Riostream.h>
#include "TNtuple.h"
#include "TH1F.h"
#include "TF1.h"
#include "TAxis.h"
#include "TString.h"
#include <TCanvas.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <cstdio>  

using namespace std;

void ztrue(TString nameFile){
    
    gStyle->SetOptFit(1);
    
    TFile infile(nameFile+".root");
    TFile *ofile = new TFile("ztrue_" + nameFile + ".root","RECREATE");
    TCanvas *c3 = new TCanvas("c3","c3");
    
    float ztrue, zrec, multtrue;
    TNtuple *nt = (TNtuple*)infile.Get("tntuple");
    nt->SetBranchAddress("ztrue", &ztrue);
    nt->SetBranchAddress("zrec", &zrec);
    nt->SetBranchAddress("multtrue", &multtrue);
    
    const int n = 14;                       //10 per z->gaus,   14 per z-> unif
   
    double eff[n];
    double seff[n];
    
    double risoluzione[n];
    double srisoluzione[n];
    
    //per z -> gaus
    //double zvert[n]= {-17.5, -12.5, -7.5, -3.75, -1.25, 1.25, 3.75, 7.5, 12.5, 17.5};
    //double szvert[n]= {2.5, 2.5, 2.5, 1.25, 1.25, 1.25, 1.25, 2.5, 2.5, 2.5};
   
    //per z -> unif
    double zvert[n]= {-27.5, -22.5, -17.5, -12.5, -7.5, -3.75, -1.25, 1.25, 3.75, 7.5, 12.5, 17.5, 22.5, 27.5};
    double szvert[n]= {2.5, 2.5, 2.5, 2.5, 2.5, 1.25, 1.25, 1.25, 1.25, 2.5, 2.5, 2.5, 2.5, 2.5,};
    
    TH1F *residuo[n];
    char nome[30];
    char titolo[50];
    TF1*f0=new TF1("f0", "gaus", -300.,300);
    TF1*f1=new TF1("f1", "gaus", -500.,500);        //Miglioramento fit per istogramma i = 0, i= n-1
    
    for(int i =0; i<n; i++){
        double evgen=0.;
        double evrec=0.;
        
        //Deprected method 
        //sprintf(nome,"h%i",i);
        snprintf(nome, sizeof(nome), "h%i", i);
        
        //Deprected method 
        //sprintf(titolo,"Residui per zTrue %f ",zvert[i]);
        snprintf(titolo, sizeof(titolo), "Residui per zTrue %f ",zvert[i]);
        residuo[i]=new TH1F(nome,titolo,100,-1000,1000);
        residuo[i]->SetDirectory(0);
        
        for(int ev=0; ev<nt->GetEntries(); ev++){
            nt->GetEvent(ev);
            if(ztrue>(zvert[i]-szvert[i])&&ztrue<(zvert[i]+szvert[i])){
                evgen++;
                if(zrec<200.){
                    evrec++;
                    residuo[i]->Fill((ztrue-zrec)*10000);
                }
            }
        }
        
        eff[i]= evrec/evgen;
        
        //da paper
        seff[i] =  sqrt(((evrec+1.)*(evrec+2.))/((evgen+2.)*(evgen+3.)) - ((evrec+1.)/(evgen+2.)) * ((evrec+1.)/(evgen+2.)));
         
        cout<<"evrec: "<<evrec<<"    "<<"evgen: "<<evgen<<endl;
        cout<<"efficienza "<<i<<" "<<eff[i]<<endl;
        
        residuo[i]->GetXaxis()->SetTitle("ztrue - zrec [um]");
        
        if(i==0 || i== n-1){
            residuo[i]->Fit("f1", "RME");
            risoluzione[i] = f1->GetParameter(2);
            srisoluzione[i] = f1->GetParError(2);
        }else if(i>0 && i<n-1){
            residuo[i]->Fit("f0", "RME");
            risoluzione[i] = f0->GetParameter(2);
            srisoluzione[i] = f0->GetParError(2);
        }
        
        residuo[i]->Write();
    }

    TGraphErrors *geff = new TGraphErrors(n,zvert,eff,szvert,seff);
    geff->SetMarkerStyle(8);
    geff->SetMarkerSize(1);
    geff->SetMarkerColor(2);
    geff->SetTitle("Efficienza vs zTrue");
    geff->GetXaxis()->SetTitle("zTrue [cm]");
    geff->GetYaxis()->SetTitle("Efficienza");
    geff->Draw("AP");
    
    TCanvas *c4 = new TCanvas("c4","c4");
    TGraphErrors *gres= new TGraphErrors(n,zvert,risoluzione,szvert,srisoluzione);
    gres->SetMarkerStyle(21);
    gres->SetTitle("Risoluzione vs zTrue");
    gres->GetXaxis()->SetTitle("zTrue [cm]");
    gres->GetYaxis()->SetTitle("Risoluzione [um]");
    gres->Draw("AP");
    gres->SetMarkerColor(2);
    
    geff->Write();
    gres->Write();
    
    infile.Close();

    ofile->Write();
    ofile->Close();
     
    
}
