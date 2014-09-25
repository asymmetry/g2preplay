//////////////////////////////////////////////////////////////////////////
//                                                                          
// rootlogon.C                                                            
//                                                                          
// Load Lib, paths and possible scripts to the analyzer upon start
//                                                                          
//////////////////////////////////////////////////////////////////////////
//	
// Author : Jin Huang <mailto:jinhuang@jlab.org>    Mar 2008
//
//////////////////////////////////////////////////////////////////////////
{
    gSystem->Load("ReplayCore_C.so");
    gSystem->Load("libg2p.so");

    gSystem->AddIncludePath("-I$ANALYZER");
    gInterpreter->AddIncludePath("$ANALYZER/");

    gSystem->AddIncludePath("-I$ANALYZER/src");
    gInterpreter->AddIncludePath("$ANALYZER/src/");

    gSystem->AddIncludePath("-I$ANALYZER/hana_decode");
    gInterpreter->AddIncludePath("$ANALYZER/hana_decode/");

    gSystem->AddIncludePath("-I$ANALYZER/hana_scaler");
    gInterpreter->AddIncludePath("$ANALYZER/hana_scaler/");

    char* macros[] = {  //list of scripts to load, end with '\0'
        "replay_L.C",
        "replay_R.C",
        "\0"
    };

    if (*macros[0]!=0)
        cout << "\nrootlogon.C: Loading macro's:" << endl;
    for(UInt_t i=0; *macros[i]!=0; i++) {
        cout << "\t " << macros[i] << endl;
        gROOT->LoadMacro(macros[i]);
    }

    printf("\nrootlogon.C: Done!\n\n");
}
