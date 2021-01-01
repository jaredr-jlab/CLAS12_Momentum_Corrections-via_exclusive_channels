#include<TChain.h>
#include<TCanvas.h>
#include<Math/Vector4D.h>
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"


int main(int argc, char **argv){
    
    float pipx, pipy, pipz, pimx, pimy, pimz;
    float ex, ey, ez, prox, proy, proz;

    double mpip = 0.13957, mpim = 0.13957;
    double mele = 0.00051099891, mpro = 0.938272;

    auto beam = ROOT::Math::PxPyPzMVector(0, 0, 10.604, mele);
    auto target = ROOT::Math::PxPyPzMVector(0, 0, 0, mpro);
    auto pip = ROOT::Math::PxPyPzMVector();
    auto pim = ROOT::Math::PxPyPzMVector();
    auto ele = ROOT::Math::PxPyPzMVector();
    auto pro = ROOT::Math::PxPyPzMVector();

    TH1F *h1 = new TH1F("e#pi^{+}#pi^{-}p", "missing mass", 200, 0.0, 0.35);

    TChain chain("h22");
    for(int i = 1; i < argc; i++){
        chain.Add(argv[i]);
    }

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

    for(int j = 0; j < nentries; j++){
        chain.GetEntry(j);

        pip.SetPx(pipx);
        pip.SetPy(pipy);
        pip.SetPz(pipz);
        pip.SetM(mpip);

        pim.SetPx(pimx);
        pim.SetPy(pimy);
        pim.SetPz(pimz);
        pim.SetM(mpim);

        ele.SetPx(ex);
        ele.SetPy(ey);
        ele.SetPz(ez);
        ele.SetM(mele);

        pro.SetPx(prox);
        pro.SetPy(proy);
        pro.SetPz(proz);
        pro.SetM(mpro);

        auto ePipPimPX = beam + target - pip - pim - ele -pro;
        
        double mass_ePipPimPX = ePipPimPX.M();

        h1->Fill(mass_ePipPimPX);
    }
    TCanvas *c1 = new TCanvas("c1", "c1", 1100, 800);
    h1->Draw();
    c1->Print("mmass.pdf");

    return 0;
}
