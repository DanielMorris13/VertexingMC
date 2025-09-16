#include "moltepl.h"

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

void moltepl(TString nameFile){
    
    gStyle->SetOptFit(1);
    
    TFile infile(nameFile+ ".root");
    TFile *ofile = new TFile("moltepl_" + nameFile + ".root","RECREATE");
    TCanvas *c1 = new TCanvas("c1","c1");
    
    float ztrue, zrec, multtrue;
    TNtuple *nt = (TNtuple*)infile.Get("tntuple");
    nt->SetBranchAddress("ztrue", &ztrue);
    nt->SetBranchAddress("zrec", &zrec);
    nt->SetBranchAddress("multtrue", &multtrue);
    
    const int n = 13;
    const double sigma = 5.3;
    
    double eff[n];
    double seff[n];
    
    double risoluzione[n];
    double srisoluzione[n];
    
    double moltepl[n]={2., 3., 4., 5., 7., 10., 13., 17., 22., 27., 33., 40., 47.};
    double smolt[n]={0.5, 0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 2.5, 2.5, 2.5, 3.5, 3.5, 3.5};
    
    TH1F *residuo[n];
    char nome[30];
    char titolo[50];
    TF1*f0=new TF1("f0", "gaus", -350.,350);
    TF1*f1=new TF1("f1", "gaus", -550.,550);        //Miglioramento fit per istogramma i = 0
    
    for(int i =0; i<n; i++){
        double evgen=0.;
        double evrec=0.;
        
        //deprecated
        //sprintf(nome,"h%i",i);
        snprintf(nome,sizeof(nome),"h%i",i);
        //sprintf(titolo,"Residui con molteplicita' %f ",moltepl[i]);
        snprintf(titolo,sizeof(titolo),"Residui con molteplicita' %f ",moltepl[i]);
        residuo[i]=new TH1F(nome,titolo,100,-1000,1000);
        residuo[i]->SetDirectory(0);
        
        for(int ev=0; ev<nt->GetEntries(); ev++){
            nt->GetEvent(ev);
            cout<<"ev: "<<" "<<ev<<" "<<endl;
            if(multtrue>(moltepl[i]-smolt[i])&&multtrue<(moltepl[i]+smolt[i])){
                if(TMath::Abs(ztrue)<3*sigma){
                    evgen++;
                    if(zrec<200.){
                        evrec++;
                        residuo[i]->Fill((ztrue-zrec)*10000);
                    }
                }
            }
        }
        
        eff[i]= evrec/evgen;
        if(int(evgen)==0) eff[i]=0.;

        //da paper
        seff[i] =  sqrt(((evrec+1.)*(evrec+2.))/((evgen+2.)*(evgen+3.)) - ((evrec+1.)/(evgen+2.)) * ((evrec+1.)/(evgen+2.)));        
        residuo[i]->GetXaxis()->SetTitle("ztrue - zrec [um]");
        
        if(i==0){
            residuo[i]->Fit("f1", "RME");
            risoluzione[i] = f1->GetParameter(2);
            srisoluzione[i] = f1->GetParError(2);
        }else if(i>0 && i<n){
            residuo[i]->Fit("f0", "RME");
            risoluzione[i] = f0->GetParameter(2);
            srisoluzione[i] = f0->GetParError(2);
        }
        
        residuo[i]->Write();
    }

    //TCanvas *c1 = new TCanvas();
    TGraphErrors *geff = new TGraphErrors(n,moltepl,eff,smolt,seff);
    geff->SetMarkerStyle(8);
    geff->SetMarkerSize(1);
    geff->SetMarkerColor(4);
    geff->SetTitle("Efficienza vs Molteplicita'");
    geff->GetXaxis()->SetTitle("Molteplicita'");
    geff->GetYaxis()->SetTitle("Efficienza");
    geff->Draw("AP");
    
    TCanvas *c2 = new TCanvas("c2","c2");
    TGraphErrors *gres= new TGraphErrors(n,moltepl,risoluzione,smolt,srisoluzione);
    gres->SetMarkerStyle(21);
    gres->SetTitle("Risoluzione vs Molteplicita'");
    gres->GetXaxis()->SetTitle("Molteplicita'");
    gres->GetYaxis()->SetTitle("Risoluzione [um]");
    gres->Draw("AP");
    gres->SetMarkerColor(4);
    
    geff->Write();
    gres->Write();

    infile.Close();
    
    ofile->Write();
    ofile->Close();
     
    
}
