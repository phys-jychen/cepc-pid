#include "PID.h"

Int_t PID::OriginalHits(const string& file, const string& tree)
{
    DisableImplicitMT();
    RDataFrame* dm = new RDataFrame(tree, file);
    string outname = file;
    outname = outname.substr(outname.find_last_of('/') + 1);
    outname = "hit_" + outname;
    auto fout = dm->Define("nhits", "(Int_t) vecEcalCellID.size()")
    .Define("CellID", [] (const vector<Int_t>& vecEcalCellID, const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Int_t>
    {
        vector<Int_t> CellID;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            else
                CellID.emplace_back(vecEcalCellID.at(i));
        }
        return CellID;
    }, {"vecEcalCellID", "vecEcalEdepCell", "nhits"})
    .Define("Hit_X", [] (const vector<Int_t>& vecEcalCellID, const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Double_t>
    {
        vector<Double_t> Hit_X;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            Double_t x = (vecEcalCellID.at(i) % 100 - 1 - nCellsXBias) * CellWidthX;
            Hit_X.emplace_back(x);
        }
        return Hit_X;
    }, {"vecEcalCellID", "vecEcalEdepCell", "nhits"})
    .Define("Hit_Y", [] (const vector<Int_t>& vecEcalCellID, const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Double_t>
    {
        vector<Double_t> Hit_Y;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            Double_t y = ((vecEcalCellID.at(i) % 10000) / 100 - 1 - nCellsYBias) * CellWidthY;
            Hit_Y.emplace_back(y);
        }
        return Hit_Y;
    }, {"vecEcalCellID", "vecEcalEdepCell", "nhits"})
    .Define("Hit_Z", [] (const vector<Int_t>& vecEcalCellID, const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Double_t>
    {
        vector<Double_t> Hit_Z;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            Double_t z = (vecEcalCellID.at(i) / 10000 - 1) * Thick;
            Hit_Z.emplace_back(z);
        }
        return Hit_Z;
    }, {"vecEcalCellID", "vecEcalEdepCell", "nhits"})
    .Define("Hit_Energy", [] (const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Double_t>
    {
        vector<Double_t> Hit_Energy;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            else
                Hit_Energy.emplace_back(vecEcalEdepCell.at(i));
        }
        return Hit_Energy;
    }, {"vecEcalEdepCell", "nhits"})
    .Define("Edep", "EcalEdepSum")
    // Time stamp corresponding to the fist hit (fastest one)
    .Define("Hit_Time_first", [] (const vector<Double_t>& vecEcalToaCell, const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Double_t>
    {
        vector<Double_t> Hit_Time_first;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            else
                Hit_Time_first.emplace_back(vecEcalToaCell.at(i));
        }
        return Hit_Time_first;
    }, {"vecEcalToaCell", "vecEcalEdepCell", "nhits"})
    // Time stamp corresponding to the maximum energy deposition per hit
    .Define("Hit_Time_maxE", [] (const vector<Double_t>& vecEcalHitTimeCell, const vector<Double_t>& vecEcalEdepCell, const Int_t& nhits)->vector<Double_t>
    {
        vector<Double_t> Hit_Time_maxE;
        for (Int_t i = 0; i < nhits; ++i)
        {
            if (vecEcalEdepCell.at(i) <= threshold)
                continue;
            else
                Hit_Time_maxE.emplace_back(vecEcalHitTimeCell.at(i));
        }
        return Hit_Time_maxE;
    }, {"vecEcalHitTimeCell", "vecEcalEdepCell", "nhits"})
    .Snapshot(tree, outname);
    delete dm;

    TFile* f = new TFile((TString) outname, "READ");
    TTree* t = f->Get<TTree>((TString) tree);
    t->SetBranchStatus("*", false);
    const vector<TString> remain = { "CellID", "Edep", "EvtID", "Hit_Energy", "Hit_Time_first", "Hit_Time_maxE", "Hit_X", "Hit_Y", "Hit_Z", "ParticleEnergy" };
    for (const TString& re : remain)
        t->SetBranchStatus(re, true);
    TFile* fnew = new TFile((TString) outname, "RECREATE");
    TTree* tnew = t->CloneTree();
    tnew->Write(nullptr, TObject::kWriteDelete, 0);
    f->Close();
    fnew->Close();

    return 0;
}