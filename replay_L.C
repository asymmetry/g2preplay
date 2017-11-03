//HRS-L replay script
const char** PATHS;
const char* RAW_DATA_FORMAT;
const char* STD_REPLAY_OUTPUT_DIR;
const char* CUSTOM_REPLAY_OUTPUT_DIR;
const char* SUMMARY_PHYSICS_PATH;
const char* SUMMARY_PHYSICS_FORMAT;

using namespace std;

void replay_L(Int_t runnumber=0,Int_t defAll=0,Int_t defEvt=-1,Int_t fstEvt=0,Bool_t QuietRun = kFALSE){

  //==================================
  //  Input & Output Definition
  //==================================
  char* RNAME="%s/g2p_%d.root";
  char* ODEF="LHRS.odef";
  char* CUTS="LHRS.cuts";

  const char *pPATHS[]={
    ".",
    "raw",
    "/cache/mss/halla/g2p/raw",
    0      
  };
  
  const char *pRAW_DATA_FORMAT="%s/g2p_%d.dat.%d";
  const char *pSTD_REPLAY_OUTPUT_DIR=gSystem->Getenv("REPLAY_OUT_PATH");
  const char *pCUSTOM_REPLAY_OUTPUT_DIR=gSystem->Getenv("REPLAY_OUT_PATH");
  TString pSUMMARY_PHYSICS_PATH=gSystem->Getenv("REPLAY_OUT_PATH");
  pSUMMARY_PHYSICS_PATH.Append("/summaryphy_%d.log");
  const char *pSUMMARY_PHYSICS_FORMAT=pSUMMARY_PHYSICS_PATH.Data();

  PATHS=pPATHS;
  RAW_DATA_FORMAT=pRAW_DATA_FORMAT;
  STD_REPLAY_OUTPUT_DIR=pSTD_REPLAY_OUTPUT_DIR;
  CUSTOM_REPLAY_OUTPUT_DIR=pSTD_REPLAY_OUTPUT_DIR;
  SUMMARY_PHYSICS_FORMAT=pSUMMARY_PHYSICS_FORMAT;
      
  //==================================
  //  Module switches
  //==================================
  Bool_t bHelicity=kFALSE; // This part goes to Chao's code
  Bool_t bBeam=kTRUE;
  Bool_t bScaler=kTRUE;
  Bool_t bPhysics=kTRUE; // Always calculate gold tracks
  
  //==================================
  //  L-HRS Detectors
  //==================================
  THaApparatus* HRSL = new THaHRS("L","Left arm HRS");
  
  // add detectors that are not in the default config
  HRSL->AddDetector(new THaCherenkov("cer", "Gas Cherenkov counter"));
  HRSL->AddDetector(new THaShower("prl1", "Pre-shower pion rej."));
  HRSL->AddDetector(new THaShower("prl2", "Show pion rej."));
  gHaApps->Add(HRSL);
  
  //==================================
  //  Scaler
  //==================================
  if(bScaler){
    THaScalerGroup* LeftScalers = new THaScalerGroup("evleft");
    gHaScalers->Add(LeftScalers);
  }

  //==================================
  //  Decoder Data
  //==================================
  THaApparatus* dec = new THaDecData("DL","Misc. Decoder Data");
  gHaApps->Add(dec);

  //==================================
  //  Beam
  //==================================
  if(bBeam){
    THaIdealBeam *ib = new THaIdealBeam("ib", "Ideal beam");
    gHaApps->Add(ib);
    THaG2PUnRasteredBeam *Lurb = new THaG2PUnRasteredBeam("Lurb","Unrastered beam from L-HRS");
    gHaApps->Add(Lurb);
    THaG2PRasteredBeam *Lrb = new THaG2PRasteredBeam("Lrb","Unrastered beam from L-HRS");
    gHaApps->Add(Lrb);
    THaG2PHappex *happex=new THaG2PHappex("happex.L","happex");
    Lurb->AddDetector(happex);
    THaG2PClockDecode *clock=new THaG2PClockDecode("Lclk","clock info");
    Lurb->AddDetector(clock);
  }
  TString beamchoice = "ib";
      
  //==================================
  //  Physics
  //==================================
  if(bPhysics){
    gHaPhysics->Add(new THaGoldenTrack("L.gold", "HRS-L Golden Track", "L"));
  }
  
  //=====================================
  //  Set up Analyzer and replay data
  //=====================================
  ReplayCore(
	     runnumber,     //run #
	     defAll,        //-1=replay all;0=ask for a number
	     defEvt,        //defaut replay event num
	     RNAME,         //output file format
	     ODEF,          //output define
	     CUTS,          //empty cut define
	     bScaler,       //replay scalar
	     bHelicity,     //repaly helicity
	     fstEvt,        //First Event To Replay
	     QuietRun       //whether ask user for inputs
	     );
}
