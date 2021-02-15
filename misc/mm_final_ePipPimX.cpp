#include<TChain.h>
#include<TCanvas.h>
#include<Math/Vector4D.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"


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

    TH2F *h1 = new TH2F("e#pi^{+}#pi^{-}X vs P() esec=1",
                        ";electron momentum [GeV];missing mass [GeV]",
                        200, 0.0, 10, 200, 0.6, 1.3);
    TH2F *h2 = new TH2F("e#pi^{+}#pi^{-}X vs P() esec=2",
                        ";electron momentum [GeV];missing mass [GeV]",
                        200, 0.0, 10, 200, 0.6, 1.3);
    TH2F *h3 = new TH2F("e#pi^{+}#pi^{-}X vs P() esec=3",
                        ";electron momentum [GeV];missing mass [GeV]",
                        200, 0.0, 10, 200, 0.6, 1.3);
    TH2F *h4 = new TH2F("e#pi^{+}#pi^{-}X vs P() esec=4",
                        ";electron momentum [GeV];missing mass [GeV]",
                        200, 0.0, 10, 200, 0.6, 1.3);
    TH2F *h5 = new TH2F("e#pi^{+}#pi^{-}X vs P() esec=5",
                        ";electron momentum [GeV];missing mass [GeV]",
                        200, 0.0, 10, 200, 0.6, 1.3);
    TH2F *h6 = new TH2F("e#pi^{+}#pi^{-}X vs P() esec=6",
                        ";electron momentum [GeV];missing mass [GeV]",
                        200, 0.0, 10, 200, 0.6, 1.3);

    for(int i = 1; i < argc; i++){
        TFile f1(argv[i]);
        TTree *t1 = (TTree*)f1.Get("h22");

        t1->SetBranchAddress("ex", &ex);
        t1->SetBranchAddress("ey", &ey);
        t1->SetBranchAddress("ez", &ez);
        t1->SetBranchAddress("pipx", &pipx);
        t1->SetBranchAddress("pipy", &pipy);
        t1->SetBranchAddress("pipz", &pipz);
        t1->SetBranchAddress("pimx", &pimx);
        t1->SetBranchAddress("pimy", &pimy);
        t1->SetBranchAddress("pimz", &pimz);

        int nentries = t1->GetEntries();

        for(int j = 0; j < nentries; j++){
            t1->GetEntry(j);

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

            auto ePipPimX = beam + target - pip - pim - ele;
            
            double mm_ePipPimX = ePipPimX.M();
            double p_ele = ele.P();

            TLeaf *esec = t1->GetLeaf("esec");
            esec->GetBranch()->GetEntry(j);
            int e_sector = esec->GetValue();

            switch(e_sector){
                case 1:
                    h1->Fill(p_ele, mm_ePipPimX);
                    break;
                case 2:
                    h2->Fill(p_ele, mm_ePipPimX);
                    break;
                case 3:
                    h3->Fill(p_ele, mm_ePipPimX);
                    break;
                case 4:
                    h4->Fill(p_ele, mm_ePipPimX);
                    break;
                case 5:
                    h5->Fill(p_ele, mm_ePipPimX);
                    break;
                case 6:
                    h6->Fill(p_ele, mm_ePipPimX);
                    break;
            }
        }
    }
    TCanvas *c1 = new TCanvas("c1", "c1", 3300, 1600);
    c1->Divide(3,2);
    c1->cd(1);
    h1->Draw("COLZ");
    c1->cd(2);
    h2->Draw("COLZ");
    c1->cd(3);
    h3->Draw("COLZ");
    c1->cd(4);
    h4->Draw("COLZ");
    c1->cd(5);
    h5->Draw("COLZ");
    c1->cd(6);
    h6->Draw("COLZ");
    c1->Print("sector_mm_pe.pdf");

    return 0;
}
