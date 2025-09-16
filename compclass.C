void compclass(TString myopt="fast"){
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }
  gSystem->CompileMacro("Hits.cxx",opt.Data());
  gSystem->CompileMacro("MyGen.cxx",opt.Data());
  gSystem->CompileMacro("PCollision.cxx",opt.Data());
  gSystem->CompileMacro("Scattering.cxx",opt.Data());
  gSystem->CompileMacro("simulation.C",opt.Data());
  gSystem->CompileMacro("TrackRec.cxx",opt.Data());
  gSystem->CompileMacro("reconstruction.C",opt.Data());

}
