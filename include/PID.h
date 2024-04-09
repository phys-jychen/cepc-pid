#ifndef PID_HH
#define PID_HH
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include "TH2D.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TObject.h"
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include "TMVA/TMVAGui.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/TMVAMultiClassGui.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"

using namespace std;
using namespace ROOT;
using namespace TMath;

// Energy threshold
const Double_t threshold = 0.0;

// Number of cells
const Int_t nCellsX = 50;
const Int_t nCellsY = 50;
const Int_t nLayer = 30;

// Cell size
const Double_t CellWidthX = 1.0;
const Double_t CellWidthY = 1.0;
const Double_t Thick = 1.0;

const Double_t nCellsXBias = 0.5 * (nCellsX - 1);
const Double_t nCellsYBias = 0.5 * (nCellsY - 1);

class PID
{
public:
    PID() = default;
    ~PID() = default;

    static Int_t OriginalHits(const string& file, const string& tree);

    static Int_t GenNtuple(const string& file, const string& tree);

    void AddTrainSig(const string& file, const string& tree)
    {
        train_sig.insert(pair<TString, TString>(TString(file), TString(tree)));
    }

    void AddTrainBkg(const string& file, const string& tree)
    {
        train_bkg.insert(pair<TString, TString>(TString(file), TString(tree)));
    }

    void AddTestSig(const string& file, const string& tree)
    {
        test_sig.insert(pair<TString, TString>(TString(file), TString(tree)));
    }

    void AddTestBkg(const string& file, const string& tree)
    {
        test_bkg.insert(pair<TString, TString>(TString(file), TString(tree)));
    }

    void AddVar(const string& v, const Char_t& type)
    {
        var.insert(pair<TString, Char_t>(TString(v), type));
    }

    Int_t TrainBDT();
    static Int_t BDTNtuple(const string& fname, const string& tname);

    void Clear()
    {
        var.clear();
        train_sig.clear();
        train_bkg.clear();
        test_sig.clear();
        test_bkg.clear();
    }


private:
    static Int_t NewScale(const vector<Double_t>& pos_x, const vector<Double_t>& pos_y, const vector<Double_t>& pos_z, const Int_t& RatioX, const Int_t& RatioY, const Int_t& RatioZ);

    map<TString, Char_t> var;
    map<TString, TString> train_sig;
    map<TString, TString> train_bkg;
    map<TString, TString> test_sig;
    map<TString, TString> test_bkg;
};

#endif