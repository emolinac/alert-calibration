#include "../include/constants.h"

const int nwires = 50;

TCut basic_cut  = Form("W<%f",w_max);
        
double fitfunction (double* x, double* par)
{
        double background = par[0] + par[1] * x[0];
        double signal     = par[2] * TMath::Exp(-0.5 * std::pow((x[0] - par[3])/(par[4]),2));

        return background + signal;
}

void macro_create_wire_tot_histos()
{
        gStyle->SetOptFit(1);

        TFile* f = new TFile("../output-files/ntuple_elastics_D2_022994.root");
        TFile* fout = new TFile("../output-files/histos_wire_tot.root","RECREATE");
        gROOT->cd();
        
        TNtuple* ntuple_elastics = (TNtuple*) f->Get("ntuple_elastics");

        TH1F* h_tot = new TH1F("h_tot","",30,350,650);
        TGraphErrors* g_wire_tot_offset   = new TGraphErrors();
        TGraphErrors* g_wire_chisquarendf = new TGraphErrors();

        TF1* fitf = new TF1("fitf", fitfunction, 400, 600, 5);
        fitf->SetParameter(0, 0.2);  
        fitf->SetParameter(1, -0.000001);  
        fitf->SetParameter(2, 1);  
        fitf->SetParameter(3, 470);  
        fitf->SetParameter(4, 50);  
        
        fitf->SetParLimits(2, 0, 2);  
        fitf->SetParLimits(3, 380, 500);
        fitf->SetParLimits(4, 40, 60);  

        for (int i = 1 ; i <= nwires ; i++) {
                TCut wire_cut = Form("ahdc_component==%i",i);

                ntuple_elastics->Project("h_tot", "ahdc_timeOverThreshold", basic_cut + wire_cut);

                h_tot->Scale(1./h_tot->Integral());

                h_tot->Fit("fitf");

                fout->cd();
                h_tot->Write(Form("h_tot%i",i));
                gROOT->cd();

                g_wire_tot_offset->SetPoint(i - 1, i, fitf->GetParameter(3));
                g_wire_tot_offset->SetPointError(i - 1, 0, fitf->GetParError(3));

                g_wire_chisquarendf->SetPoint(i - 1, i, fitf->GetChisquare()/fitf->GetNDF());
                g_wire_chisquarendf->SetPointError(i - 1, 0, 0);

                h_tot->Reset();
        }

        fout->cd();
        g_wire_tot_offset->Write("wire_tot_offset");
        g_wire_chisquarendf->Write("wire_chisquarendf");
        gROOT->cd();

        f->Close();
        fout->Close();
}