#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TChain.h>
#include <Math/Vector4D.h>
#include <TFile.h>
#include <TTree.h>

#include "sliceFit.h"


int main(int argc, char **argv){
    
    TH1::AddDirectory(kFALSE);

    float pipx, pipy, pipz, pimx, pimy, pimz;
    float ex, ey, ez, prox, proy, proz;

    double xlow = 0.0;
    double xup = 10.0;
    double ylow_me = -0.6;
    double yup_me = 0.6;
    double ylow_pip = -0.4;
    double yup_pip = 0.6;
    double ylow_pim = -0.4;
    double yup_pim = 0.6;
    double ylow_pro = 0.6;
    double yup_pro = 1.3;

    double mpip = 0.13957, mpim = 0.13957;
    double mele = 0.00051099891, mpro = 0.938272;
    double pipE, pimE, eleE, proE;
    double me_ePipPimPX, mm2_ePipPX, mm2_ePimPX, mm_ePipPimX;
    double p_ele;

    auto beam = ROOT::Math::PxPyPzEVector(0, 0, 10.604, sqrt(10.604*10.604 + mele*mele));
    auto target = ROOT::Math::PxPyPzEVector(0, 0, 0, mpro);
    auto pip = ROOT::Math::PxPyPzEVector();
    auto pim = ROOT::Math::PxPyPzEVector();
    auto ele = ROOT::Math::PxPyPzEVector();
    auto pro = ROOT::Math::PxPyPzEVector();
    auto ePipPimX = ROOT::Math::PxPyPzEVector();
    auto ePipPX = ROOT::Math::PxPyPzEVector();
    auto ePimPX = ROOT::Math::PxPyPzEVector();
    auto ePipPimPX = ROOT::Math::PxPyPzEVector();

    //------------------Initialize 2D Histograms-------------------------------
    //Missing Energy vs Electron Momentum
    TH2F *H_me_pele = new TH2F("me",
                   "Missing Energy;electron momentum [GeV];missing energy [GeV]",
                   200, xlow, xup, 200, ylow_me, yup_me);
    //Pip mass vs Electron Momentum
    TH2F *H_pip_pele = new TH2F("mm2_pip",
                    "Expecting #pi^{+};electron momentum [GeV];missing mass^{2} [GeV]",
                    200, xlow, xup, 200, ylow_pip, yup_pip);
    //Pim mass vs Electron Momentum
    TH2F *H_pim_pele = new TH2F("mm2_pim",
                    "Expecting #pi^{-};electron momentum [GeV];missing mass^{2} [GeV]",
                    200, xlow, xup, 200, ylow_pim, yup_pim);
    //Proton mass vs Electron Momentum
    TH2F *H_pro_pele = new TH2F("mm_proton",
                    "Expecting proton;electron momentum [GeV];missing mass [GeV]",
                    200, xlow, xup, 200, ylow_pro, yup_pro);

    //------------------Chain Input Root Files----------------------------------
    TChain chain("h22");
    for(int i = 1; i < argc; i++){
        chain.Add(argv[i]);
    }

    //------------------Set Chain Branch Addresses------------------------------
    chain.SetBranchAddress("ex", &ex);
    chain.SetBranchAddress("ey", &ey);
    chain.SetBranchAddress("ez", &ez);
    chain.SetBranchAddress("pipx", &pipx);
    chain.SetBranchAddress("pipy", &pipy);
    chain.SetBranchAddress("pipz", &pipz);
    chain.SetBranchAddress("pimx", &pimx);
    chain.SetBranchAddress("pimy", &pimy);
    chain.SetBranchAddress("pimz", &pimz);
    chain.SetBranchAddress("prox", &prox);
    chain.SetBranchAddress("proy", &proy);
    chain.SetBranchAddress("proz", &proz);

    int nentries = chain.GetEntries();

    //------------------Loop Through Entries and Fill 2D Histograms-------------
    for(int j = 0; j < nentries; j++){
        chain.GetEntry(j);

        //Energy components for 4-vectors
        pipE = sqrt(pipx*pipx + pipy*pipy + pipz*pipz + mpip*mpip);
        pimE = sqrt(pimx*pimx + pimy*pimy + pimz*pimz + mpim*mpim);
        eleE = sqrt(ex*ex + ey*ey + ez*ez + mele*mele);
        proE = sqrt(prox*prox + proy*proy + proz*proz + mpro*mpro);

        //Set values for 4-vectors
        pip.SetPxPyPzE(pipx, pipy, pipz, pipE);
        pim.SetPxPyPzE(pimx, pimy, pimz, pimE);
        ele.SetPxPyPzE(ex, ey, ez, eleE);
        pro.SetPxPyPzE(prox, proy, proz, proE);

        //Calculate "missing" quantities
        ePipPimX = beam + target - pip - pim - ele;
        ePipPX = beam + target - pip - ele - pro;
        ePimPX = beam + target - pim - ele - pro;
        ePipPimPX = beam + target - pip - pim - ele - pro;
        
        //Get values from "missing" quantities
        mm_ePipPimX = ePipPimX.M();
        mm2_ePipPX = ePipPX.M2();
        mm2_ePimPX = ePimPX.M2();
        me_ePipPimPX = ePipPimPX.E();
        p_ele = ele.P();

        //Fill 2D Histograms
        H_pro_pele->Fill(p_ele, mm_ePipPimX);
        H_pip_pele->Fill(p_ele, mm2_ePipPX);
        H_pim_pele->Fill(p_ele, mm2_ePimPX);
        H_me_pele->Fill(p_ele, me_ePipPimPX);

    }
    
    //------------------Draw 2D Histograms--------------------------------------
    TCanvas c("c", "c", 2200, 1600);
    c.Divide(2, 2);
    c.cd(1);
    H_me_pele->Draw("COLZ");
    c.cd(2);
    H_pip_pele->Draw("COLZ");
    c.cd(3);
    H_pim_pele->Draw("COLZ");
    c.cd(4);
    H_pro_pele->Draw("COLZ");
    c.Print("quantites_pele.pdf");

    
    //------------------H_me_pele Slice and Fit---------------------------------
    // Instantiate array of 1D histograms for the missing energy slices
    int nSlices = 10;
    int first_slice = 2;
    int last_slice = 9;
    TH1D *hfit1 = new TH1D[nSlices];

    // Slice the 2D Histogram
    SliceHisto2D(H_me_pele, hfit1, nSlices);

    // Instantiate a TGraphErrors for mean values
    TGraphErrors *gr_mean1 = new TGraphErrors(nSlices);

    // Fit the Histogram Slices and get Mean values
    FitSlices(hfit1, gr_mean1, nSlices, ylow_me, yup_me);
    
    // Remove the mean values of unwanted slices
    int lastPoint = 0;
    for (int i=last_slice; i<nSlices; i++){
        lastPoint = gr_mean1->GetN() - 1;
        gr_mean1->RemovePoint(lastPoint);
    }
    for (int i=0; i<first_slice; i++){
        gr_mean1->RemovePoint(0);
    }

    // Draw the Fitted Slices
    TCanvas cf1("cf1", "cf1", 1200, 900);
    cf1.Divide(3, 3);
    gStyle->SetOptFit(111);
    int pad = 1;
    for (int i=first_slice; i<last_slice; i++, pad++){
        cf1.cd(pad);
        (hfit1 + i)->Draw();
    }
    TString me_name = H_me_pele->GetName();
    cf1.Print("sliceFit_" + me_name + ".pdf");

    // Draw Mean values
    TCanvas cm1("cm1", "cm1", 1200, 900);
    gr_mean1->SetMarkerColor(2);
    gr_mean1->SetMarkerStyle(21);
    gr_mean1->SetMarkerSize(1.5);
    gr_mean1->GetXaxis()->SetTitle("electron momentum [GeV]");
    gr_mean1->GetYaxis()->SetTitle("missing energy [GeV]");
    gr_mean1->SetTitle("Missing Engergy");
    gr_mean1->Draw("AP");
    TLegend leg1(0.1, 0.8, 0.3, 0.9);
    leg1.AddEntry(gr_mean1, "Before Corrections", "p");
    leg1.Draw();
    cm1.Print("mean_" + me_name + ".pdf");

    delete[] hfit1;  // cleanup
    delete gr_mean1;  // cleanup

    //------------------H_pip_pele Slice and Fit--------------------------------
    // Initialize array of 1D histograms for the missing energy slices
    nSlices = 10;
    first_slice = 2;
    last_slice = 9;
    TH1D *hfit2 = new TH1D[nSlices];

    // Slice the 2D Histogram
    SliceHisto2D(H_pip_pele, hfit2, nSlices);

    // Instantiate a TGraphErrors for mean values
    TGraphErrors *gr_mean2 = new TGraphErrors(nSlices);

    // Fit the Histogram Slices and get mean values
    FitSlices(hfit2, gr_mean2, nSlices, ylow_pip, yup_pip);

    // Remove the mean values of unwanted slices
    lastPoint = 0;
    for (int i=last_slice; i<nSlices; i++){
        lastPoint = gr_mean2->GetN() - 1;
        gr_mean2->RemovePoint(lastPoint);
    }
    for (int i=0; i<first_slice; i++){
        gr_mean2->RemovePoint(0);
    }

    // Draw the Fitted Slices
    TCanvas cf2("cf2", "cf2", 1200, 900);  // draw SliceFit()
    cf2.Divide(3, 3);
    gStyle->SetOptFit(111);
    pad = 1;
    for (int i=first_slice; i<last_slice; i++, pad++){
        cf2.cd(pad);
        (hfit2 + i)->Draw();
    }
    TString pip_name = H_pip_pele->GetName();
    cf2.Print("sliceFit_" + pip_name + ".pdf");

    // Draw Mean values
    TCanvas cm2("cm2", "cm2", 1200, 900);
    gr_mean2->SetMarkerColor(2);
    gr_mean2->SetMarkerStyle(21);
    gr_mean2->SetMarkerSize(1.5);
    gr_mean2->GetXaxis()->SetTitle("electron momentum [GeV]");
    gr_mean2->GetYaxis()->SetTitle("missing mass^{2} [GeV]");
    gr_mean2->SetTitle("Expecting Pip");
    gr_mean2->Draw("AP");
    TLegend leg2(0.1, 0.8, 0.3, 0.9);
    leg2.AddEntry(gr_mean2, "Before Corrections", "p");
    leg2.Draw();
    cm2.Print("mean_" + pip_name + ".pdf");

    delete[] hfit2;  // cleanup
    delete gr_mean2;  // cleanup

    //------------------H_pim_pele Slice and Fit--------------------------------
    // Initialize array of 1D histograms for the missing energy slices
    nSlices = 10;
    first_slice = 2;
    last_slice = 9;
    TH1D *hfit3 = new TH1D[nSlices];

    // Slice the 2D Histogram
    SliceHisto2D(H_pim_pele, hfit3, nSlices);

    // Instantiate a TGraphErrors for mean values
    TGraphErrors *gr_mean3 = new TGraphErrors(nSlices);

    // Fit the Histogram Slices and get mean values
    FitSlices(hfit3, gr_mean3, nSlices, ylow_pim, yup_pim);

    // Remove the mean values of unwanted slices
    lastPoint = 0;
    for (int i=last_slice; i<nSlices; i++){
        lastPoint = gr_mean3->GetN() - 1;
        gr_mean3->RemovePoint(lastPoint);
    }
    for (int i=0; i<first_slice; i++){
        gr_mean3->RemovePoint(0);
    }

    // Draw the Fitted Slices
    TCanvas cf3("cf3", "cf3", 1200, 900);
    cf3.Divide(3, 3);
    gStyle->SetOptFit(111);
    pad = 1;
    for (int i=first_slice; i<last_slice; i++, pad++){
        cf3.cd(pad);
        (hfit3 + i)->Draw();
    }
    TString pim_name = H_pim_pele->GetName();
    cf3.Print("sliceFit_" + pim_name + ".pdf");

    // Draw Mean values
    TCanvas cm3("cm3", "cm3", 1200, 900);
    gr_mean3->SetMarkerColor(2);
    gr_mean3->SetMarkerStyle(21);
    gr_mean3->SetMarkerSize(1.5);
    gr_mean3->GetXaxis()->SetTitle("electron momentum [GeV]");
    gr_mean3->GetYaxis()->SetTitle("missing mass^{2} [GeV]");
    gr_mean3->SetTitle("Expecting Pim");
    gr_mean3->Draw("AP");
    TLegend leg3(0.1, 0.8, 0.3, 0.9);
    leg3.AddEntry(gr_mean3, "Before Corrections", "p");
    leg3.Draw();
    cm3.Print("mean_" + pim_name + ".pdf");

    delete[] hfit3;  // cleanup
    delete gr_mean3;  // cleanup
    
    //------------------H_pro_pele Slice and Fit--------------------------------
    // Initialize array of 1D histograms for the missing energy slices
    nSlices = 10;
    first_slice = 2;
    last_slice = 9;
    TH1D *hfit4 = new TH1D[nSlices];

    // Slice the 2D Histogram
    SliceHisto2D(H_pro_pele, hfit4, nSlices);

    // Instantiate a TGraphErrors for mean values
    TGraphErrors *gr_mean4 = new TGraphErrors(nSlices);

    // Fit the Histogram Slices and get mean values
    FitSlices(hfit4, gr_mean4, nSlices, ylow_pim, yup_pim);

    // Remove the mean values of unwanted slices
    lastPoint = 0;
    for (int i=last_slice; i<nSlices; i++){
        lastPoint = gr_mean4->GetN() - 1;
        gr_mean4->RemovePoint(lastPoint);
    }
    for (int i=0; i<first_slice; i++){
        gr_mean4->RemovePoint(0);
    }

    // Draw the Fitted Slices
    TCanvas cf4("cf4", "cf4", 1200, 900);  // draw SliceFit()
    cf4.Divide(3, 3);
    gStyle->SetOptFit(111);
    pad = 1;
    for (int i=first_slice; i<last_slice; i++, pad++){
        cf4.cd(pad);
        (hfit4 + i)->Draw();
    }
    TString pro_name = H_pro_pele->GetName();
    cf4.Print("sliceFit_" + pro_name + ".pdf");

    // Draw Mean values
    TCanvas cm4("cm4", "cm4", 1200, 900);
    gr_mean4->SetMarkerColor(2);
    gr_mean4->SetMarkerStyle(21);
    gr_mean4->SetMarkerSize(1.5);
    gr_mean4->GetXaxis()->SetTitle("electron momentum [GeV]");
    gr_mean4->GetYaxis()->SetTitle("missing mass [GeV]");
    gr_mean4->SetTitle("Expecting Proton");
    gr_mean4->Draw("AP");
    TLegend leg4(0.1, 0.8, 0.3, 0.9);
    leg4.AddEntry(gr_mean4, "Before Corrections", "p");
    leg4.Draw();
    cm4.Print("mean_" + pro_name + ".pdf");

    delete[] hfit4;  // cleanup
    delete gr_mean4;  // cleanup

    delete H_me_pele;  // cleanup
    delete H_pip_pele;  // cleanup
    delete H_pim_pele;  // cleanup
    delete H_pro_pele;  // cleanup

    return 0;
}

