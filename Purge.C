using namespace std;

void Purge(int run = 0){
    const char *REPLAY_OUTPUT_DIR = gSystem->Getenv("REPLAY_OUT_PATH");

    const char* filename = Form("%s/g2p_%d.root", REPLAY_OUTPUT_DIR, run);

    int inc = 0;
    while (!gSystem->AccessPathName(filename)) {
        TFile *f = new TFile(filename, "UPDATE");
        TIter prev(f->GetListOfKeys(), kIterBackward);
        while ((key = (TKey*)prev())) {
            if ((strcmp(key->GetClassName(),"TH1F") == 0)) {
                key->Delete();
                delete key;
            }
        }
        f->Purge();
        f->Save();
        f->Close();

        inc++;
        filename = Form("%s/g2p_%d_%d.root", REPLAY_OUTPUT_DIR, run, inc);
    }
}
