#include "PID.h"

Int_t main(Int_t argc, Char_t* argv[])
{
    string file, tree = "treeEvt";
    Int_t hit = 0;
    Int_t rec = 0;
    Int_t bdt = 0, classify = 0;
    Int_t help = 0;

    for (Int_t i = 1; i < argc; i++)
    {
        if (string(argv[i]) == string("--help"))
        {
            help = 1;
            cout << endl;
            cout << "****************************************************************" << endl;
            cout << "                        Help information" << endl;
            cout << "****************************************************************" << endl;
            cout << " Collect original hit information:" << endl;
            cout << "     With default tree \'treeEvt\':  iPID -h -f [file]" << endl;
            cout << "     With other specified tree:    iPID -h -f [file] -t [tree]" << endl;
            cout << " Reconstruct variables:" << endl;
            cout << "     With default tree \'treeEvt\':  iPID -r -f [file]" << endl;
            cout << "     With other specified tree:    iPID -r -f [file] -t [tree]" << endl;
            cout << " BDT training and testing:" << endl;
            cout << "     With default tree \'treeEvt\':  iPID -b" << endl;
            cout << "     With other specified tree:    iPID -b -t [tree]" << endl;
            cout << " Classification:" << endl;
            cout << "     With default tree \'treeEvt\':  iPID -c -f [file]" << endl;
            cout << "     With other specified tree:    iPID -c -f [file] -t [tree]" << endl;
            cout << "****************************************************************" << endl << endl;
            break;
        }

        else if (string(argv[i]) == string("-f"))
            file = string(argv[i + 1]);

        else if (string(argv[i]) == string("-t"))
            tree = string(argv[i + 1]);

        else if (string(argv[i]) == string("-h"))
            hit = 1;

        else if (string(argv[i]) == string("-r"))
            rec = 1;

        else if (string(argv[i]) == string("-b"))
            bdt = 1;

        else if (string(argv[i]) == string("-c"))
            classify = 1;
    }

    PID* p = new PID();

    if (hit == 1 && !file.empty())
    {
        cout << "--> Collecting original hits..." << endl;
        cout << "--> File: " << file << endl;
        cout << "--> Tree: " << tree << endl;

        PID::OriginalHits(file, tree);

        cout << "--> Hit collection finished!" << endl;
    }
    else if (rec == 1 && !file.empty())
    {
        cout << "---> Reconstructing variables..." << endl;
        cout << "---> File: " << file << endl;
        cout << "---> Tree: " << tree << endl;

        PID::GenNtuple(file, tree);

        cout << "---> Variable reconstruction finished!" << endl;
    }
    else if (bdt == 1)
    {
        cout << "----> Training and testing..." << endl;
        cout << "----> Tree: " << tree << endl;

        p->AddVar("COG_X_mean",         'D');
        p->AddVar("COG_Y_mean",         'D');
        p->AddVar("COG_Z_mean",         'D');

        p->AddVar("E1E9",               'D');
        p->AddVar("E1Edep",             'D');
        p->AddVar("E25Edep",            'D');
        p->AddVar("E49Edep",            'D');
        p->AddVar("E9E25",              'D');
        p->AddVar("E9E49",              'D');
        p->AddVar("E9Edep",             'D');
        p->AddVar("Ecell_max",          'D');
        p->AddVar("Ecell_max_25",       'D');
        p->AddVar("Ecell_max_49",       'D');
        p->AddVar("Ecell_max_9",        'D');
        p->AddVar("Ecell_second",       'D');
        p->AddVar("Edep",               'D');
        p->AddVar("Emax_sec_diff",      'D');
        p->AddVar("Emax_sec_dist",      'D');
        p->AddVar("Emean",              'D');

        p->AddVar("FD_2D_mean",         'D');
        p->AddVar("FD_2D_rms",          'D');
        p->AddVar("FD_3D_mean",         'D');
        p->AddVar("FD_3D_rms",          'D');

        p->AddVar("hit_layer",          'I');
        p->AddVar("nhits",              'I');
        p->AddVar("shower_density",     'D');
        p->AddVar("shower_end",         'I');
        p->AddVar("shower_layer",       'I');
        p->AddVar("shower_layer_ratio", 'D');
        p->AddVar("shower_length",      'I');
        p->AddVar("shower_radius",      'D');
        p->AddVar("shower_start",       'I');
        p->AddVar("xwidth",             'D');
        p->AddVar("ywidth",             'D');
        p->AddVar("zdepth",             'D');

        string path = "/cefs/higgs/chenjiyuan/cepc-pid/run/cubic/root/thick1_layer30/";

        // Signal
        p->AddTrainSig(path + "job1_e-_60GeV/e-_60GeV.root", tree);
        p->AddTestSig(path + "job3_e-_60GeV/e-_60GeV.root", tree);

        // Background
        p->AddTrainBkg(path + "job2_pi-_60GeV/pi-_60GeV.root", tree);
        p->AddTestBkg(path + "job4_pi-_60GeV/pi-_60GeV.root", tree);

        p->TrainBDT();

        cout << "----> Training and testing finished!" << endl;
	}
    else if (classify == 1 && !file.empty())
    {
        cout << "----> Classifying..." << endl;
        cout << "----> File: " << file << endl;
        cout << "----> Tree: " << tree << endl << endl;

        PID::BDTNtuple(file, tree);

        cout << "----> Classification finished!" << endl;
    }
    else if (help == 0)
    {
        cout << "Invalid input." << endl;
        cout << "Run \'iPID --help\' to display help information." << endl << endl;
    }

    delete p;
    return 0;
}