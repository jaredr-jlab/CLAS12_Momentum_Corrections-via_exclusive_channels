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

    //------------------Instantiate 2D Histograms-------------------------------
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
    TCanvas *c = new TCanvas("c", "c", 2200, 1600);
    c->Divide(2, 2);
    c->cd(1);
    H_me_pele->Draw("COLZ");
    c->cd(2);
    H_pip_pele->Draw("COLZ");
    c->cd(3);
    H_pim_pele->Draw("COLZ");
    c->cd(4);
    H_pro_pele->Draw("COLZ");
    c->Print("quantites_pele.pdf");

    SliceFit(H_me_pele, 10, ylow_me, yup_me);
    SliceFit(H_pip_pele, 10, ylow_pip, yup_pip);
    SliceFit(H_pim_pele, 10, ylow_pim, yup_pim);
    SliceFit(H_pro_pele, 10, ylow_pro, yup_pro);

    delete H_me_pele;
    delete H_pip_pele;
    delete H_pim_pele;
    delete H_pro_pele;
    delete c;

    return 0;
}

