{ 
int cut_num = 0;
float lcut  = -0.03;
float rcut  = 0.07;
if(cut_num == 0){
    lcut = 0;
    rcut = 0;
}

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

TH1F *H1 = new TH1F("e#pi^{+}#pi^{-}pX",
                    "Missing Mass Squared; mm^{2}[GeV]; bin count",
                    500, -0.11, 0.09);
TH1F *H_pim = new TH1F("e#pi^{+}pX",
                       "Missing Mass Squared: expecting #pi^{-}; mm^{2}[GeV]; bin count",
                       400, -0.4, 0.6);
TH1F *H_pip = new TH1F("e#pi^{-}pX",
                       "Missing Mass Squared: expecting #pi^{+}; mm^{2}[GeV]; bin count",
                       400, -0.4, 0.6);
TH1F *H_pro = new TH1F("e#pi^{+}#pi^{-}X",
                       "Missing Mass: expecting proton; mm[GeV]; bin count",
                       400, 0.6, 1.3);
TH1F *h1 = new TH1F("e#pi^{+}#pi^{-}pX",
                    "Missing Mass Squared; mm^{2}[GeV]; bin count",
                    500, -0.11, 0.09);
TH1F *h_pim = new TH1F("e#pi^{+}pX",
                       "Missing Mass Squared: expecting #pi^{-}; mm^{2}[GeV]; bin count",
                       400, -0.4, 0.6);
TH1F *h_pip = new TH1F("e#pi^{-}pX",
                       "Missing Mass Squared: expecting #pi^{+}; mm^{2}[GeV]; bin count",
                       400, -0.4, 0.6);
TH1F *h_pro = new TH1F("e#pi^{+}#pi^{-}X",
                       "Missing Mass: expecting proton; mm[GeV]; bin count",
                       400, 0.6, 1.3);

TChain chain("h22");

chain.Add("./rootfiles/out_skim8_005032.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005038.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005197.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005222.hipo_3.hipo.root");

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

    auto ePipPimPX = beam + target - pip - pim - ele - pro;
    double mm2_ePipPimPX = ePipPimPX.M2();

    auto ePipPX = beam + target - pip - ele - pro;
    double mm2_ePipPX = ePipPX.M2();

    auto ePimPX = beam + target - pim - ele - pro;
    double mm2_ePimPX = ePimPX.M2();

    auto ePipPimX = beam + target - pip - pim - ele;
    double mm_ePipPimX = ePipPimX.M();

    H1->Fill(mm2_ePipPimPX);
    H_pim->Fill(mm2_ePipPX);
    H_pip->Fill(mm2_ePimPX);
    H_pro->Fill(mm_ePipPimX);

    switch(cut_num){
        case 0:
            h1->Fill(mm2_ePipPimPX);
            h_pim->Fill(mm2_ePipPX);
            h_pip->Fill(mm2_ePimPX);
            h_pro->Fill(mm_ePipPimX);
            break;
        case 1:
            h1->Fill(mm2_ePipPimPX);
            if(mm2_ePipPimPX > lcut && mm2_ePipPimPX < rcut){
                h_pim->Fill(mm2_ePipPX);
                h_pip->Fill(mm2_ePimPX);
                h_pro->Fill(mm_ePipPimX);
                h_pim->SetLineColor(kRed);
                h_pip->SetLineColor(kRed);
                h_pro->SetLineColor(kRed);
            }
            break;
        case 2:
            h_pim->Fill(mm2_ePipPX);
            if(mm2_ePipPX > lcut && mm2_ePipPX < rcut){
                h1->Fill(mm2_ePipPimPX);
                h_pip->Fill(mm2_ePimPX);
                h_pro->Fill(mm_ePipPimX);
                h1->SetLineColor(kRed);
                h_pip->SetLineColor(kRed);
                h_pro->SetLineColor(kRed);
            }
            break;
        case 3:
            h_pip->Fill(mm2_ePimPX);
            if(mm2_ePimPX > lcut && mm2_ePimPX < rcut){
                h1->Fill(mm2_ePipPimPX);
                h_pim->Fill(mm2_ePipPX);
                h_pro->Fill(mm_ePipPimX);
                h1->SetLineColor(kRed);
                h_pim->SetLineColor(kRed);
                h_pro->SetLineColor(kRed);
            }
            break;
        case 4:
            h_pro->Fill(mm_ePipPimX);
            if(mm_ePipPimX > lcut && mm_ePipPimX < rcut){
                h1->Fill(mm2_ePipPimPX);
                h_pim->Fill(mm2_ePipPX);
                h_pip->Fill(mm2_ePimPX);
                h1->SetLineColor(kRed);
                h_pim->SetLineColor(kRed);
                h_pip->SetLineColor(kRed);
            }
            break;
    }
}
TCanvas *c1 = new TCanvas("c1", "c1", 1100, 800);
c1->Divide(2,2);

c1->cd(1);
H1->Draw();
h1->Draw("SAMES");
if(cut_num == 1){
    TLine lline, rline;
    lline.SetLineColor(2);
    double ymin = h1->GetMinimum();
    double ymax = h1->GetMaximum();
    lline.DrawLine(lcut, ymin, lcut, ymax);
    rline.SetLineColor(2);
    rline.DrawLine(rcut, ymin, rcut, ymax);
}

c1->cd(2);
H_pim->Draw();
h_pim->Draw("SAMES");
if(cut_num == 2){
    TLine lline, rline;
    lline.SetLineColor(2);
    double ymin = h_pim->GetMinimum();
    double ymax = h_pim->GetMaximum();
    lline.DrawLine(lcut, ymin, lcut, ymax);
    rline.SetLineColor(2);
    rline.DrawLine(rcut, ymin, rcut, ymax);
}

c1->cd(3);
H_pip->Draw();
h_pip->Draw("SAMES");
if(cut_num == 3){
    TLine lline, rline;
    lline.SetLineColor(2);
    double ymin = h_pip->GetMinimum();
    double ymax = h_pip->GetMaximum();
    lline.DrawLine(lcut, ymin, lcut, ymax);
    rline.SetLineColor(2);
    rline.DrawLine(rcut, ymin, rcut, ymax);
}

c1->cd(4);
H_pro->Draw();
h_pro->Draw("SAMES");
if(cut_num == 4){
    TLine lline, rline;
    lline.SetLineColor(2);
    double ymin = h_pro->GetMinimum();
    double ymax = h_pro->GetMaximum();
    lline.DrawLine(lcut, ymin, lcut, ymax);
    rline.SetLineColor(2);
    rline.DrawLine(rcut, ymin, rcut, ymax);
}
c1->Print(Form("mm_compare_cut%i_%.4g_%.4g.pdf", cut_num, lcut, rcut));
}
