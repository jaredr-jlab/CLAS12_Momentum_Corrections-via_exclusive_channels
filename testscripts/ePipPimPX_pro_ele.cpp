{
float pipx, pipy, pipz, pimx, pimy, pimz;
float ex, ey, ez, prox, proy, proz;

double pipE, pimE, eleE, proE;
double e_ele;
double mm_ePipPimX;
double mpip = 0.13957, mpim = 0.13957;
double mele = 0.00051099891, mpro = 0.938272;

//-----------------Instantiate 4-Vectors-------------------
auto beam = ROOT::Math::PxPyPzEVector(0, 0, 10.604, sqrt(10.604*10.604 + mele*mele));
auto target = ROOT::Math::PxPyPzEVector(0, 0, 0, mpro);
auto pip = ROOT::Math::PxPyPzEVector();
auto pim = ROOT::Math::PxPyPzEVector();
auto ele = ROOT::Math::PxPyPzEVector();
auto pro = ROOT::Math::PxPyPzEVector();
auto ePipPimX = ROOT::Math::PxPyPzEVector();

//-----------------Instantiate Histograms------------------

TH2F *h_pm_ee = new TH2F("mm_{pro} vs ele.E()",
                         "; electron energy [GeV]; missing mass proton [GeV]",
                         200, 0.0, 10, 200, 0.6, 1.3);
TH1F *h_ee = new TH1F("ele.E()",
                      "; electron energy [GeV]; bin count",
                      200, 0.0, 10);

//-----------------Chain Root Files------------------------
/*
The string "h22" is the tree name from the root file.
*/
TChain chain("h22");
chain.Add("./rootfiles/out_skim8_005032.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005038.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005197.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005222.hipo_3.hipo.root");

//--------Point Branch Names to Variable Addresses---------
/*
The string "ex" is the branch name from the root file.
The variable address "&ex" points to the float created above.
*/
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

//-----------------Loop Through Branches------------------
for(int j = 0; j < nentries; j++){
    chain.GetEntry(j);

    pipE = sqrt(pipx*pipx + pipy*pipy + pipz*pipz + mpip*mpip);
    pimE = sqrt(pimx*pimx + pimy*pimy + pimz*pimz + mpim*mpim);
    eleE = sqrt(ex*ex + ey*ey + ez*ez + mele*mele);
    proE = sqrt(prox*prox + proy*proy + proz*proz + mpro*mpro);

    pip.SetPxPyPzE(pipx, pipy, pipz, pipE);
    pim.SetPxPyPzE(pimx, pimy, pimz, pimE);
    ele.SetPxPyPzE(ex, ey, ez, eleE);
    pro.SetPxPyPzE(prox, proy, proz, proE);

    ePipPimX = beam + target - pip - pim - ele;
    mm_ePipPimX = ePipPimX.M();

    e_ele = ele.E();

    //--------------Fill Histogram-------------------------
    h_pm_ee->Fill(e_ele, mm_ePipPimX);
    h_ee->Fill(e_ele);
}

TCanvas *c1 = new TCanvas("c1", "c1", 1100, 800);
h_pm_ee->Draw("COLZ");
c1->Print("mmpro_eleE.pdf");

TCanvas *c2 = new TCanvas("c2", "c2", 1100, 800);
h_ee->Draw("COLZ");
c2->Print("eleE.pdf");
}
