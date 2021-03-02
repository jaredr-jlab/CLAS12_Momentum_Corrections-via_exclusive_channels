{
//TVirtualFitter::SetDefaultFitter("Fumili"); //Minuit, Minuit2, Fumili, GSLMultiMin,
//GSLMultiFit, GSLSimAn, Genetic

float pipx, pipy, pipz, pimx, pimy, pimz;
float ex, ey, ez, prox, proy, proz;

double mpip = 0.13957, mpim = 0.13957;
double mele = 0.00051099891, mpro = 0.938272;

auto beam = ROOT::Math::PxPyPzEVector(0, 0, 10.604, sqrt(10.604*10.604 + mele*mele));
auto target = ROOT::Math::PxPyPzEVector(0, 0, 0, mpro);
auto pip = ROOT::Math::PxPyPzEVector();
auto pim = ROOT::Math::PxPyPzEVector();
auto ele = ROOT::Math::PxPyPzEVector();

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

TChain chain("h22");

chain.Add("./rootfiles/final_ePipPimP.root");
chain.SetBranchAddress("ex", &ex);
chain.SetBranchAddress("ey", &ey);
chain.SetBranchAddress("ez", &ez);
chain.SetBranchAddress("pipx", &pipx);
chain.SetBranchAddress("pipy", &pipy);
chain.SetBranchAddress("pipz", &pipz);
chain.SetBranchAddress("pimx", &pimx);
chain.SetBranchAddress("pimy", &pimy);
chain.SetBranchAddress("pimz", &pimz);

TLeaf *esec = chain.GetLeaf("esec");

int nentries = chain.GetEntries();

for(int j = 0; j < nentries; j++){
    chain.GetEntry(j);

    double pipE = sqrt(pipx*pipx + pipy*pipy + pipz*pipz + mpip*mpip);
    double pimE = sqrt(pimx*pimx + pimy*pimy + pimz*pimz + mpim*mpim);
    double eleE = sqrt(ex*ex + ey*ey + ez*ez + mele*mele);

    pip.SetPxPyPzE(pipx, pipy, pipz, pipE);
    pim.SetPxPyPzE(pimx, pimy, pimz, pimE);
    ele.SetPxPyPzE(ex, ey, ez, eleE);

    auto ePipPimX = beam + target - pip - pim - ele;
    
    double mm_ePipPimX = ePipPimX.M();
    double p_ele = ele.P();

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

TCanvas *c = new TCanvas("c", "c", 3300, 800);
c->Divide(3,2);
c->cd(1);
h1->Draw("COLZ");
c->cd(2);
h2->Draw("COLZ");
c->cd(3);
h3->Draw("COLZ");
c->cd(4);
h4->Draw("COLZ");
c->cd(5);
h5->Draw("COLZ");
c->cd(6);
h6->Draw("COLZ");
//c->Print("sector_mm_pe.pdf");

int minBin, maxBin;
const int nProj = 9;
double mu[nProj], sigma[nProj], f_min[nProj], f_max[nProj];
for (int i=1; i<nProj; i++){
    mu[i] = 0.95;
    sigma[i] = 0.05;
    f_min[i] = 0.8;
    f_max[i] = 1.12;
}

TF1 *f1 = new TF1("f1", "gaus(0)+pol1(3)", 0.8, 1.12);
f1->SetParNames("Constant", "Mean", "Sigma", "p0", "p1");
f1->SetParameters(50, 0.95, 0.04, 1, 1);
TH1D *hfy[nProj];

for (int j=0; j<3; j++){
    minBin = 20;
    maxBin = minBin + 19;
    for (int i=2; i<nProj; i++){
        minBin += 20;
        maxBin += 20;
        hfy[i] = h1->ProjectionY(Form("p_electron [%d, %d) esec=1", i, i+1), minBin, maxBin);
        hfy[i]->Fit(f1, "", "", f_min[i], f_max[i]);
        mu[i] = f1->GetParameter("Mean");
        sigma[i] = f1->GetParameter("Sigma");
        f_min[i] = mu[i] - 3.0 * sigma[i];
        f_max[i] = mu[i] + 3.0 * sigma[i];
    }
}

TCanvas *c1 = new TCanvas("c1", "c1", 1200, 900);
c1->Divide(3, 3);
gStyle->SetOptFit(111);

for (int i=1; i<=7; i++){
    c1->cd(i);
    hfy[i+1]->Draw();
}

c1->Print("sector1_slice_fit_pele.pdf");
}
