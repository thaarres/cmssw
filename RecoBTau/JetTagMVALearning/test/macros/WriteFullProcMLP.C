#define WriteFullProcMLP_cxx
#include "WriteFullProcMLP.h"
//Usage: Can be used with the following little macro for instance
/*
void Run(int varset = 0){
gSystem->CompileMacro("~/Anal_Macros/CP3llbb/WriteFullProcMLP.C");

WriteFullProcMLP xRecoDUSG("Reco", "DUSG");
xRecoDUSG.setVarSet(varset);xRecoDUSG.Loop();
WriteFullProcMLP xRecoC("Reco", "C");
xRecoC.setVarSet(varset); xRecoC.Loop();
WriteFullProcMLP xRecoCombined("Reco", "Combined");
xRecoCombined.setVarSet(varset); xRecoCombined.Loop();

WriteFullProcMLP xPseudoDUSG("Pseudo", "DUSG");
xPseudoDUSG.setVarSet(varset); xPseudoDUSG.Loop();
WriteFullProcMLP xPseudoC("Pseudo", "C");
xPseudoC.setVarSet(varset); xPseudoC.Loop();
WriteFullProcMLP xPseudoCombined("Pseudo", "Combined");
xPseudoCombined.setVarSet(varset); xPseudoCombined.Loop();

WriteFullProcMLP xNoDUSG("No", "DUSG");
xNoDUSG.setVarSet(varset); xNoDUSG.Loop();
WriteFullProcMLP xNoC("No", "C");
xNoC.setVarSet(varset); xNoC.Loop();
WriteFullProcMLP xNoCombined("No", "Combined");
xNoCombined.setVarSet(varset); xNoCombined.Loop();
}
*/


//std::vector<TString> VariablesInput; 

void WriteFullProcMLP::Loop()
 {

  std::cout << "Loop varSet = " << varSet << std::endl;
  addVariables();
  FillMLP(varSet);
  
  WriteHeader();
  WriteInput();
  WriteCategory();
  WriteCount("var1", "input", "trackSip2dSig");
  //********Chunk for Combined BvsDUSG and BvsC starts***********
  WriteNormalize();
  WriteSplitters();
  if (writeSplittersForNonMultiple) WriteSplittersForNonMultiple();
  WriteLRx();
  WriteLRall();
  WriteLRsplit();
  WriteOptional();
  if (writeMatrices) {WriteMatrices(); WriteOutput("input", "jetPt"); return;}
  WriteProcMLP(mlp);
  WriteBiasNorm(procBiasNorm+tagVtx, "var1", procMLP+tagVtx, "var1");
  WriteLRBias(procLike+tagVtx, "var1", procBiasNorm+tagVtx, "var1");
  //********Chunk for Combined BvsDUSG and BvsC starts***********
  
  if (Flavour != "Combined") {
    WriteOutput(procLike+tagVtx, "var1");
    return;
  }
  
  tagVtx = "C";
  
  //Need to reset Vector for ProcOptional
  Oprocess.clear();
  Oid.clear();
  //Need to reset mlp with the corresponding variables with C instead of "DUSG" tag
  FillMLP(varSet);
  
  
  //For combined xml, copy previuos structure from WriteNormalize to WriteLRBias
  //  then add ProcLinear and output
  //**************Insert here Chunk for Combined: begin**************
  WriteNormalize();
  WriteSplitters();
  if (writeSplittersForNonMultiple) WriteSplittersForNonMultiple();
  WriteLRx();
  WriteLRall();
  WriteLRsplit();
  WriteOptional();
  if (writeMatrices) {WriteMatrices(); WriteOutput("input", "jetPt"); return;}
  WriteProcMLP(mlp);
  WriteBiasNorm(procBiasNorm+tagVtx, "var1", procMLP+tagVtx, "var1");
  WriteLRBias(procLike+tagVtx, "var1", procBiasNorm+tagVtx, "var1");
  //**************Insert here Chunk for Combined: end****************
  WriteLinear();
  WriteOutput(procLinear, "var1");

}


void WriteFullProcMLP::WriteLinear() {
  xmlFile << "	<processor id=\"" << procLinear << "\" name=\"ProcLinear\">" << std::endl;
  xmlFile << "		<input>" << std::endl;
  xmlFile << "\t\t\t<var source=\"" << procLike+"DUSG" << "\" name=\"" << "var1" << "\"/>" << std::endl;
  xmlFile << "\t\t\t<var source=\"" << procLike+"C" << "\" name=\"" << "var1" << "\"/>" << std::endl;
  xmlFile << "\t\t</input>" << std::endl;
  xmlFile << "\t\t<config>" << std::endl;
  xmlFile << "\t\t\t<coefficients offset=\"0.0\" coeff1=\"0.75\" coeff2=\"0.25\"/>" << std::endl;
  xmlFile << "\t\t</config>" << std::endl;
  xmlFile << "\t\t<output>" << std::endl;
  xmlFile << "\t\t\t<var name=\"" << "var1" << "\"/>" << std::endl;
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;

}

void WriteFullProcMLP::WriteBiasNorm(TString processID, TString VarID, TString parentVarProcess, TString parentVarID) {
  xmlFile << "	<processor id=\"" << processID << "\" name=\"ProcNormalize\">" << std::endl;
  xmlFile << "		<input>" << std::endl;
  xmlFile << "\t\t\t<var source=\"" << parentVarProcess << "\" name=\"" << parentVarID << "\"/>" << std::endl;
  xmlFile << "\t\t</input>" << std::endl;
  xmlFile << "\t\t<config>" << std::endl;
  xmlFile << "\t\t\t<pdf smooth=\"3\" size=\"500\"/>" << std::endl;
  xmlFile << "\t\t</config>" << std::endl;
  xmlFile << "\t\t<output>" << std::endl;
  xmlFile << "\t\t\t<var name=\"" << VarID << "\"/>" << std::endl;
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;

}

void WriteFullProcMLP::WriteProcMLP(std::vector<std::pair<TString, TString> > Vec) {


  xmlFile << "\t<processor id=\"" << procMLP+tagVtx << "\" name=\"ProcMLP\">" << std::endl;

  xmlFile << "\t\t<input>" << std::endl;
  for (unsigned int i = 0; i < Vec.size(); i++) {
    xmlFile << "\t\t\t<var source=\"" << Vec[i].first << "\" name=\"" << Vec[i].second << "\"/>" << std::endl;
  }
  xmlFile << "\t\t</input>" << std::endl;

  xmlFile << "\t\t<config>" << std::endl;
  xmlFile << "\t\t\t<config steps=\"" << NIter << "\" boost=\"-1\" limiter=\"0\">" << Struct << "</config>" << std::endl;
  xmlFile << "\t\t</config>" << std::endl;

  xmlFile << "\t\t<output>" << std::endl;
  xmlFile << "\t\t\t<var name=\"" << "var1" << "\"/>" << std::endl;
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;


}

void WriteFullProcMLP::WriteMatrix(TString ProcessorID, std::vector<TString> Vec) {
  xmlFile << "\t<processor id=\"" << ProcessorID+tagVtx << "\" name=\"ProcMatrix\">" << std::endl;

  xmlFile << "\t\t<input>" << std::endl;
  for (unsigned int i = 0; i < Vec.size(); i++) {
    xmlFile << "\t\t\t<var source=\"" << procOpt+tagVtx << "\" name=\"" << Vec[i] << "\"/>" << std::endl;
  }
  xmlFile << "\t\t</input>" << std::endl;

  xmlFile << "\t\t<config>" << std::endl;
  xmlFile << "\t\t\t<fill signal=\"true\" background=\"true\"/>" << std::endl;
  xmlFile << "\t\t</config>" << std::endl;

  xmlFile << "\t\t<output>" << std::endl;
  for (unsigned int i = 0; i < Vec.size(); i++) {
    xmlFile << "\t\t\t<var name=\"" << Vec[i] << "\"/>" << std::endl;
  }
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;

}

void WriteFullProcMLP::WriteOptional() {
  xmlFile << "\t<processor id=\"" << procOpt+tagVtx << "\" name=\"ProcOptional\">" << std::endl;

  xmlFile << "\t\t<input>" << std::endl;
  for (unsigned int i = 0; i < Oprocess.size(); i++) {
    xmlFile << "\t\t\t<var source=\"" << Oprocess[i] << "\" name=\"" << Oid[i] << "\"/>" << std::endl;
  }
  xmlFile << "\t\t</input>" << std::endl;

  xmlFile << "\t\t<config>" << std::endl;
  for (unsigned int i = 0; i < Oprocess.size(); i++) {
    xmlFile << "\t\t\t<neutral pos=\"0\"/>" << std::endl;
  }
  xmlFile << "\t\t</config>" << std::endl;

  xmlFile << "\t\t<output>" << std::endl;
  for (unsigned int i = 0; i < Oprocess.size(); i++) {
    xmlFile << "\t\t\t<var name=\"" << Oid[i] << "\"/>" << std::endl;
  }
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;

}

void WriteFullProcMLP::WriteLRBias(TString processID, TString VarID, TString parentVarProcess, TString parentVarID) {


       xmlFile << "\t<processor id=\"" << processID << "\" name=\"ProcLikelihood\">" << std::endl;
       xmlFile << "\t\t<input>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procCat << "\" name=\"" << "var1" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << parentVarProcess << "\" name=\"" << parentVarID << "\"/>" << std::endl;
       xmlFile << "\t\t</input>" << std::endl;
       xmlFile << "\t\t<config>" << std::endl;
       xmlFile << "\t\t\t<general strict=\"true\"/>" << std::endl;
       xmlFile << "\t\t\t<category count=\"19\"/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg size=\"500\"/>" << std::endl;
 
       xmlFile << "<bias_table>" << std::endl;
       xmlFile << mapBias[VtxCat][tagVtx] << std::endl;
       xmlFile << "</bias_table>" << std::endl;

       xmlFile << "\t\t</config>" << std::endl;
       xmlFile << "\t\t<output>" << std::endl;
       xmlFile << "\t\t\t<var name=\"" << VarID << "\"/>" << std::endl;
       xmlFile << "\t\t</output>" << std::endl;
       xmlFile << "\t</processor>" << std::endl;
       xmlFile << std::endl;


}

void WriteFullProcMLP::WriteLRx(){

 for (unsigned int i = 0; i < Vname.size(); i++) {
 
   if (Vtreatment[i] == "Input" || Vmultiple[i] == "false" || Vname[i] == "jetPt" || Vname[i] == "jetEta") continue;
    
   if ((Vtreatment[i].Contains("LRx") || Vtreatment[i] == "All") &&  Vmultiple[i] == "true") {
     TString idx = "";
     for (unsigned int j = 0; j < 4; j++) {
       if (j == 0) idx = "_1";
       if (j == 1) idx = "_2";
       if (j == 2) idx = "_3";
       if (j == 3) idx = "_4";
       
       xmlFile << "\t<processor id=\"" << procLike+Vname[i]+idx+tagVtx << "\" name=\"ProcLikelihood\">" << std::endl;
       xmlFile << "\t\t<input>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procCat << "\" name=\"" << "var1" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"" << Vname[i]+idx << "\"/>" << std::endl;
       xmlFile << "\t\t</input>" << std::endl;
       xmlFile << "\t\t<config>" << std::endl;
       xmlFile << "\t\t\t<general strict=\"true\"/>" << std::endl;
       xmlFile << "\t\t\t<category count=\"19\"/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg/>" << std::endl;
       xmlFile << "\t\t</config>" << std::endl;
       xmlFile << "\t\t<output>" << std::endl;
       xmlFile << "\t\t\t<var name=\"" << "LR_"+Vname[i]+idx << "\"/>" << std::endl;
       xmlFile << "\t\t</output>" << std::endl;
       xmlFile << "\t</processor>" << std::endl;
       xmlFile << std::endl;
      
      //For the moment I only consider first 2 tracks
      if (j == 0 || j == 1) {
        Oprocess.push_back(procLike+Vname[i]+idx+tagVtx);
        Oid.push_back("LR_"+Vname[i]+idx);
      }
      
      //IPD3Sig is an exception
      if ((j==2 || j==3) && Vname[i]=="trackSip3dSig") {
        Oprocess.push_back(procLike+Vname[i]+idx+tagVtx);
        Oid.push_back("LR_"+Vname[i]+idx);
      }


     }

   }
 }


}

void WriteFullProcMLP::WriteLRall(){

 for (unsigned int i = 0; i < Vname.size(); i++) {
 
   if (Vtreatment[i] == "Input" || Vmultiple[i] == "false" || Vname[i] == "jetPt" || Vname[i] == "jetEta") continue;
    
   if ((Vtreatment[i].Contains("LRall") || Vtreatment[i] == "All") &&  Vmultiple[i] == "true") {
       xmlFile << "\t<processor id=\"" << procLike+Vname[i]+"_all"+tagVtx << "\" name=\"ProcLikelihood\">" << std::endl;
       xmlFile << "\t\t<input>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procCat << "\" name=\"" << "var1" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procNorm+tagVtx << "\" name=\"" << Vname[i] << "\"/>" << std::endl;
       xmlFile << "\t\t</input>" << std::endl;
       xmlFile << "\t\t<config>" << std::endl;
       xmlFile << "\t\t\t<general strict=\"true\"/>" << std::endl;
       xmlFile << "\t\t\t<category count=\"19\"/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg/>" << std::endl;
       xmlFile << "\t\t</config>" << std::endl;
       xmlFile << "\t\t<output>" << std::endl;
       xmlFile << "\t\t\t<var name=\"" << "LR_"+Vname[i]+"_all" << "\"/>" << std::endl;
       xmlFile << "\t\t</output>" << std::endl;
       xmlFile << "\t</processor>" << std::endl;
       xmlFile << std::endl;

       Oprocess.push_back(procLike+Vname[i]+"_all"+tagVtx);
       Oid.push_back("LR_"+Vname[i]+"_all");   

   }
   
 }


}

void WriteFullProcMLP::WriteLRsplit(){
 for (unsigned int i = 0; i < Vname.size(); i++) {
 
   if (Vtreatment[i] == "Input" || Vmultiple[i] == "false" || Vname[i] == "jetPt" || Vname[i] == "jetEta") continue;
    
   if ((Vtreatment[i].Contains("LRsplit") || Vtreatment[i] == "All") &&  Vmultiple[i] == "true") {
        
       xmlFile << "\t<processor id=\"" << procLike+Vname[i]+"_split"+tagVtx << "\" name=\"ProcLikelihood\">" << std::endl;
       xmlFile << "\t\t<input>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procCat << "\" name=\"" << "var1" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"" << Vname[i]+"_1" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"" << Vname[i]+"_2" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"" << Vname[i]+"_3" << "\"/>" << std::endl;
       xmlFile << "\t\t\t<var source=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"" << Vname[i]+"_4" << "\"/>" << std::endl;
       xmlFile << "\t\t</input>" << std::endl;
       xmlFile << "\t\t<config>" << std::endl;
       xmlFile << "\t\t\t<general strict=\"true\"/>" << std::endl;
       xmlFile << "\t\t\t<category count=\"19\"/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg/>" << std::endl;
       xmlFile << "\t\t\t<sigbkg/>" << std::endl;
       xmlFile << "\t\t</config>" << std::endl;
       xmlFile << "\t\t<output>" << std::endl;
       xmlFile << "\t\t\t<var name=\"" << "LR_"+Vname[i]+"_split" << "\"/>" << std::endl;
       xmlFile << "\t\t</output>" << std::endl;
       xmlFile << "\t</processor>" << std::endl;
       xmlFile << std::endl;

       Oprocess.push_back(procLike+Vname[i]+"_split"+tagVtx);
       Oid.push_back("LR_"+Vname[i]+"_split");   
     

   }
 }

}

void WriteFullProcMLP::WriteSplittersForNonMultiple() {

 for (unsigned int i = 0; i < Vname.size(); i++) {
 
   if (Vtreatment[i] != "Input" && Vmultiple[i] == "false") {
    
      xmlFile << "\t<processor id=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"ProcSplitter\">" << std::endl;
      xmlFile << "\t\t<input>" << std::endl;
      xmlFile << "\t\t\t<var source=\"" << procNorm+tagVtx << "\" name=\"" << Vname[i] << "\"/>" << std::endl;
      xmlFile << "\t\t</input>" << std::endl;
      xmlFile << "\t\t<config>" << std::endl;
      xmlFile << "\t\t\t<select first=\"1\"/>" << std::endl;
      xmlFile << "\t\t</config>" << std::endl;
      xmlFile << "\t\t<output>" << std::endl;
      xmlFile << "\t\t\t<var name=\"" << Vname[i]+"_1\"" << "/>" << std::endl;
      xmlFile << "\t\t\t<var name=\"" << Vname[i]+"_rest\"" << "/>" << std::endl;
      xmlFile << "\t\t</output>" << std::endl;
      xmlFile << "\t</processor>" << std::endl;
      xmlFile << std::endl;
      
      Oprocess.push_back(procSplit+Vname[i]+tagVtx);
      Oid.push_back(Vname[i]+"_1");   

   }
 }


}

void WriteFullProcMLP::WriteSplitters() {

 for (unsigned int i = 0; i < Vname.size(); i++) {
 
   if (Vtreatment[i] == "Input" || Vmultiple[i] == "false" || Vname[i] == "jetPt" || Vname[i] == "jetEta") continue;
    
   if ((Vtreatment[i].Contains("LRsplit") || Vtreatment[i].Contains("LRx") || Vtreatment[i] == "All") &&  Vmultiple[i] == "true") {
      xmlFile << "\t<processor id=\"" << procSplit+Vname[i]+tagVtx << "\" name=\"ProcSplitter\">" << std::endl;
      xmlFile << "\t\t<input>" << std::endl;
      xmlFile << "\t\t\t<var source=\"" << procNorm+tagVtx << "\" name=\"" << Vname[i] << "\"/>" << std::endl;
      xmlFile << "\t\t</input>" << std::endl;
      xmlFile << "\t\t<config>" << std::endl;
      xmlFile << "\t\t\t<select first=\"3\"/>" << std::endl;
      xmlFile << "\t\t</config>" << std::endl;
      xmlFile << "\t\t<output>" << std::endl;
      xmlFile << "\t\t\t<var name=\"" << Vname[i]+"_1\"" << "/>" << std::endl;
      xmlFile << "\t\t\t<var name=\"" << Vname[i]+"_2\"" << "/>" << std::endl;
      xmlFile << "\t\t\t<var name=\"" << Vname[i]+"_3\"" << "/>" << std::endl;
      xmlFile << "\t\t\t<var name=\"" << Vname[i]+"_4\"" << "/>" << std::endl;
      xmlFile << "\t\t</output>" << std::endl;
      xmlFile << "\t</processor>" << std::endl;
      xmlFile << std::endl;
      
      //For the moment I only consider first 2 tracks
      Oprocess.push_back(procSplit+Vname[i]+tagVtx);
      Oid.push_back(Vname[i]+"_1");   
      Oprocess.push_back(procSplit+Vname[i]+tagVtx);
      Oid.push_back(Vname[i]+"_2");   



   }
 }

}

void WriteFullProcMLP::WriteNormalize() {

  xmlFile << "	<processor id=\"" << procNorm+tagVtx << "\" name=\"ProcNormalize\">" << std::endl;
  xmlFile << "		<input>" << std::endl;

  for (unsigned int i = 0; i < Vname.size(); i++) {
    if (Vtreatment[i] == "Input") continue;
    if (Vname[i].Contains("count_")) {
      xmlFile << "\t\t\t<var source=\"" << procCount << "\" name=\"" << Vname[i] << "\"/>" << std::endl;
    }
    else {
      xmlFile << "\t\t\t<var source=\"input\" name=\"" << Vname[i] << "\"/>" << std::endl;
    }
  }

  xmlFile << "\t\t</input>" << std::endl;
  xmlFile << "\t\t<config>" << std::endl;

  for (unsigned int i = 0; i < Vname.size(); i++) {
    if (Vtreatment[i] == "Input") continue;
    if (Vname[i] == "jetPt" || Vname[i] == "jetEta") {
      xmlFile << "\t\t\t<pdf/> <!-- " << Vname[i] << " -->" << std::endl;
    }
    else {
      if (FixRange) {
        xmlFile << "\t\t\t<pdf smooth=\"" << Vsmooth[i] << "\" size=\"" << Vsize[i] << "\" lower=\"" << Vlower[i] << "\" upper=\"" << Vupper[i] << "\"/> <!-- " << Vname[i] << " -->" << std::endl;
      }
      else {
        xmlFile << "\t\t\t<pdf smooth=\"" << Vsmooth[i] << "\" size=\"" << Vsize[i] << "\"/> <!-- " << Vname[i] << " -->" << std::endl;
      
      }
      
      
      
    }
  }


  xmlFile << "\t\t</config>" << std::endl;
  xmlFile << "\t\t<output>" << std::endl;
  
  for (unsigned int i = 0; i < Vname.size(); i++) {
    if (Vtreatment[i] == "Input") continue;
      xmlFile << "\t\t\t<var name=\"" << Vname[i] << "\"/>" << std::endl;
      //Oprocess.push_back(procNorm+tagVtx);
      //Oid.push_back(Vname[i]);      

  }
  
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;



  //std::cout  << "Vname.size()=" << Vname.size() << std::endl;
  //for (unsigned int i = 0; i < Vname.size(); i++) {
  //  std::cout << "i = \t" << i << "\t" << Vname[i] << std::endl;
  //}


}

//Choose processID, parent variable id, and variable id
//At the end the variable is added to the list of variables
//  the settings for this variable: size, lower upper are hardcoded
void WriteFullProcMLP::WriteCount(TString VarID, TString parentVarProcess, TString parentVarID) {

  xmlFile << "\t<processor id=\"" << procCount << "\" name=\"ProcCount\">" << std::endl;
  xmlFile << "\t\t<input>" << std::endl;
  xmlFile << "\t\t\t<var source=\"" << parentVarProcess << "\" name=\"" << parentVarID << "\"/>" << std::endl;
  xmlFile << "\t\t</input>" << std::endl;
  xmlFile << "\t\t<config>" << std::endl;
  xmlFile << "\t\t</config>" << std::endl;
  xmlFile << "\t\t<output>" << std::endl;
  xmlFile << "\t\t\t<var name=\"" << "count_"+VarID << "\"/>" << std::endl;
  xmlFile << "\t\t</output>" << std::endl;
  xmlFile << "\t</processor>" << std::endl;
  xmlFile << std::endl;

  addVariable("count_"+VarID, "false", "false", "All", "0", "100", "0", "30");
}


void WriteFullProcMLP::WriteCategory() {//This one is basically hardcoded (we don't plan to change the bins anytime soon)

  xmlFile << "\t<processor id=\"" << procCat << "\" name=\"ProcCategory\">" << std::endl;
  xmlFile << "		<input>" << std::endl;
  xmlFile << "			<var source=\"input\" name=\"jetPt\"/>" << std::endl;
  xmlFile << "			<var source=\"input\" name=\"jetEta\"/>" << std::endl;
  xmlFile << "		</input>" << std::endl;
  xmlFile << "		<config>" << std::endl;
  xmlFile << "                        <group><box><range max=\"40\"/><range min=\"-1.2\" max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range max=\"40\"/><range min=\"-2.1\" max=\"2.1\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range max=\"40\"/><range/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"40\" max=\"60\"/><range min=\"-1.2\" max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"40\" max=\"60\"/><range min=\"-2.1\" max=\"2.1\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"40\" max=\"60\"/><range/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"60\" max=\"90\"/><range min=\"-1.2\" max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"60\" max=\"90\"/><range min=\"-2.1\" max=\"2.1\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"60\" max=\"90\"/><range/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"90\" max=\"150\"/><range min=\"-1.2\" max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"90\" max=\"150\"/><range min=\"-2.1\" max=\"2.1\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"90\" max=\"150\"/><range/></box></group>" << std::endl;
  xmlFile << "												<group><box><range min=\"150\" max=\"400\"/><range min=\"-1.2\"  max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"150\" max=\"400\"/><range min=\"-2.1\" max=\"2.1\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"150\" max=\"400\"/><range/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"400\" max=\"600\"/><range min=\"-1.2\" max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range min=\"400\" max=\"600\"/><range/></box></group>" << std::endl;
  xmlFile << "												<group><box><range/><range min=\"-1.2\" max=\"1.2\"/></box></group>" << std::endl;
  xmlFile << "                        <group><box><range/><range/></box></group>       " << std::endl;
  xmlFile << "		</config>" << std::endl;
  xmlFile << "		<output>" << std::endl;
  xmlFile << "			<var name=\"var1\"/>" << std::endl;
  xmlFile << "		</output>" << std::endl;
  xmlFile << "	</processor>" << std::endl;
  xmlFile << std::endl;
}

void WriteFullProcMLP::WriteInput() {

  xmlFile << "\t<input id=\"input\">" << std::endl;
  std::cout << "Vname.size()=" << Vname.size() << std::endl;
  for (unsigned int i = 0; i < Vname.size(); i++) {
    //if (Vtreatment[i] == "Input") continue;
    xmlFile << "\t\t<var name=\"" << Vname[i] << "\" multiple=\"" << Vmultiple[i] << "\" optional=\"" << Voptional[i] << "\"/>" << std::endl;
    //std::cout << "i = \t" << i << "\t" << Vname[i] << std::endl;
  }
  xmlFile << "\t</input>" << std::endl;
  xmlFile << std::endl;
  

}

void WriteFullProcMLP::WriteHeader() {

  xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>" << std::endl;
  xmlFile << "<MVATrainer>" << std::endl;
  xmlFile << "\t<!-- Warning: Auto-generated file from MVA calibration extractor. -->" << std::endl;
  xmlFile << "\t<!--          This trainer configuration is incomplete! -->" << std::endl;
  xmlFile << "\t<general>" << std::endl;
  xmlFile << "\t\t<option name=\"trainfiles\">train_%1$s%2$s.%3$s</option>" << std::endl;
  xmlFile << "\t</general>" << std::endl;


}

void WriteFullProcMLP::WriteOutput(TString processID, TString VarID) {


  xmlFile << "	<output>" << std::endl;
  xmlFile << "		<var source=\"" << processID << "\" name=\"" << VarID << "\"/>" << std::endl;
  xmlFile << "	</output>" << std::endl;
  xmlFile << "</MVATrainer>" << std::endl;

}

void WriteFullProcMLP::WriteMatrices() {
  
  std::vector<TString> rotAll; rotAll.clear();
  std::vector<TString> rotT; rotT.clear();
  std::vector<TString> rotT1; rotT1.clear();
  std::vector<TString> rotT2; rotT2.clear();
  std::vector<TString> rotLR1; rotLR1.clear();
  std::vector<TString> rotLR2; rotLR2.clear();
  std::vector<TString> rotLRall; rotLRall.clear();
  std::vector<TString> rotLRsplit; rotLRsplit.clear();
  
  
  rotAll.push_back("trackSip2dSig_1");
  rotAll.push_back("trackSip2dSig_2");
  rotAll.push_back("trackSip3dSig_1");
  rotAll.push_back("trackSip3dSig_2");
  rotAll.push_back("trackSip2dVal_1");
  rotAll.push_back("trackSip2dVal_2");
  rotAll.push_back("trackSip3dVal_1");
  rotAll.push_back("trackSip3dVal_2");
  rotAll.push_back("trackPtRel_1");
  rotAll.push_back("trackPtRel_2");
  rotAll.push_back("trackPPar_1");
  rotAll.push_back("trackPPar_2");
  rotAll.push_back("trackEtaRel_1");
  rotAll.push_back("trackEtaRel_2");
  rotAll.push_back("trackDeltaR_1");
  rotAll.push_back("trackDeltaR_2");
  rotAll.push_back("trackPtRatio_1");
  rotAll.push_back("trackPtRatio_2");
  rotAll.push_back("trackPParRatio_1");
  rotAll.push_back("trackPParRatio_2");
  rotAll.push_back("trackJetDist_1");
  rotAll.push_back("trackJetDist_2");
  rotAll.push_back("trackDecayLenVal_1");
  rotAll.push_back("trackDecayLenVal_2");
  rotAll.push_back("jetPt_1");
  rotAll.push_back("jetEta_1");
  rotAll.push_back("vertexMass_1");
  rotAll.push_back("vertexNTracks_1");
  rotAll.push_back("vertexEnergyRatio_1");
  rotAll.push_back("trackSip2dSigAboveCharm_1");
  rotAll.push_back("trackSip3dSigAboveCharm_1");
  rotAll.push_back("flightDistance2dSig_1");
  rotAll.push_back("flightDistance3dSig_1");
  rotAll.push_back("flightDistance2dVal_1");
  rotAll.push_back("flightDistance3dVal_1");
  rotAll.push_back("trackSumJetEtRatio_1");
  rotAll.push_back("jetNSecondaryVertices_1");
  rotAll.push_back("vertexJetDeltaR_1");
  rotAll.push_back("trackSumJetDeltaR_1");
  rotAll.push_back("jetNTracks_1");
  rotAll.push_back("trackSip2dValAboveCharm_1");
  rotAll.push_back("trackSip3dValAboveCharm_1");
  rotAll.push_back("vertexFitProb_1");
  rotAll.push_back("chargedHadronEnergyFraction_1");
  rotAll.push_back("neutralHadronEnergyFraction_1");
  rotAll.push_back("photonEnergyFraction_1");
  rotAll.push_back("electronEnergyFraction_1");
  rotAll.push_back("muonEnergyFraction_1");
  rotAll.push_back("chargedHadronMultiplicity_1");
  rotAll.push_back("neutralHadronMultiplicity_1");
  rotAll.push_back("photonMultiplicity_1");
  rotAll.push_back("electronMultiplicity_1");
  rotAll.push_back("muonMultiplicity_1");
  rotAll.push_back("hadronMultiplicity_1");
  rotAll.push_back("hadronPhotonMultiplicity_1");
  rotAll.push_back("totalMultiplicity_1");
  rotAll.push_back("massVertexEnergyFraction_1");
  rotAll.push_back("vertexBoostOverSqrtJetPt_1");
  rotAll.push_back("nTracks_1");
  rotAll.push_back("category_1");
  rotAll.push_back("count_var1_1");
  rotAll.push_back("LR_trackSip2dSig_1");
  rotAll.push_back("LR_trackSip2dSig_2");
  rotAll.push_back("LR_trackSip3dSig_1");
  rotAll.push_back("LR_trackSip3dSig_2");
  rotAll.push_back("LR_trackSip2dVal_1");
  rotAll.push_back("LR_trackSip2dVal_2");
  rotAll.push_back("LR_trackSip3dVal_1");
  rotAll.push_back("LR_trackSip3dVal_2");
  rotAll.push_back("LR_trackPtRel_1");
  rotAll.push_back("LR_trackPtRel_2");
  rotAll.push_back("LR_trackPPar_1");
  rotAll.push_back("LR_trackPPar_2");
  rotAll.push_back("LR_trackEtaRel_1");
  rotAll.push_back("LR_trackEtaRel_2");
  rotAll.push_back("LR_trackDeltaR_1");
  rotAll.push_back("LR_trackDeltaR_2");
  rotAll.push_back("LR_trackPtRatio_1");
  rotAll.push_back("LR_trackPtRatio_2");
  rotAll.push_back("LR_trackPParRatio_1");
  rotAll.push_back("LR_trackPParRatio_2");
  rotAll.push_back("LR_trackJetDist_1");
  rotAll.push_back("LR_trackJetDist_2");
  rotAll.push_back("LR_trackDecayLenVal_1");
  rotAll.push_back("LR_trackDecayLenVal_2");
  rotAll.push_back("LR_trackSip2dSig_all");
  rotAll.push_back("LR_trackSip3dSig_all");
  rotAll.push_back("LR_trackSip2dVal_all");
  rotAll.push_back("LR_trackSip3dVal_all");
  rotAll.push_back("LR_trackPtRel_all");
  rotAll.push_back("LR_trackPPar_all");
  rotAll.push_back("LR_trackEtaRel_all");
  rotAll.push_back("LR_trackDeltaR_all");
  rotAll.push_back("LR_trackPtRatio_all");
  rotAll.push_back("LR_trackPParRatio_all");
  rotAll.push_back("LR_trackJetDist_all");
  rotAll.push_back("LR_trackDecayLenVal_all");
  rotAll.push_back("LR_trackSip2dSig_split");
  rotAll.push_back("LR_trackSip3dSig_split");
  rotAll.push_back("LR_trackSip2dVal_split");
  rotAll.push_back("LR_trackSip3dVal_split");
  rotAll.push_back("LR_trackPtRel_split");
  rotAll.push_back("LR_trackPPar_split");
  rotAll.push_back("LR_trackEtaRel_split");
  rotAll.push_back("LR_trackDeltaR_split");
  rotAll.push_back("LR_trackPtRatio_split");
  rotAll.push_back("LR_trackPParRatio_split");
  rotAll.push_back("LR_trackJetDist_split");
  rotAll.push_back("LR_trackDecayLenVal_split");


  rotT.push_back("trackSip2dSig_1");
  rotT.push_back("trackSip2dSig_2");
  rotT.push_back("trackSip3dSig_1");
  rotT.push_back("trackSip3dSig_2");
  rotT.push_back("trackSip2dVal_1");
  rotT.push_back("trackSip2dVal_2");
  rotT.push_back("trackSip3dVal_1");
  rotT.push_back("trackSip3dVal_2");
  rotT.push_back("trackPtRel_1");
  rotT.push_back("trackPtRel_2");
  rotT.push_back("trackPPar_1");
  rotT.push_back("trackPPar_2");
  rotT.push_back("trackEtaRel_1");
  rotT.push_back("trackEtaRel_2");
  rotT.push_back("trackDeltaR_1");
  rotT.push_back("trackDeltaR_2");
  rotT.push_back("trackPtRatio_1");
  rotT.push_back("trackPtRatio_2");
  rotT.push_back("trackPParRatio_1");
  rotT.push_back("trackPParRatio_2");
  rotT.push_back("trackJetDist_1");
  rotT.push_back("trackJetDist_2");
  rotT.push_back("trackDecayLenVal_1");
  rotT.push_back("trackDecayLenVal_2");
  rotT.push_back("LR_trackSip2dSig_1");
  rotT.push_back("LR_trackSip2dSig_2");
  rotT.push_back("LR_trackSip3dSig_1");
  rotT.push_back("LR_trackSip3dSig_2");
  rotT.push_back("LR_trackSip2dVal_1");
  rotT.push_back("LR_trackSip2dVal_2");
  rotT.push_back("LR_trackSip3dVal_1");
  rotT.push_back("LR_trackSip3dVal_2");
  rotT.push_back("LR_trackPtRel_1");
  rotT.push_back("LR_trackPtRel_2");
  rotT.push_back("LR_trackPPar_1");
  rotT.push_back("LR_trackPPar_2");
  rotT.push_back("LR_trackEtaRel_1");
  rotT.push_back("LR_trackEtaRel_2");
  rotT.push_back("LR_trackDeltaR_1");
  rotT.push_back("LR_trackDeltaR_2");
  rotT.push_back("LR_trackPtRatio_1");
  rotT.push_back("LR_trackPtRatio_2");
  rotT.push_back("LR_trackPParRatio_1");
  rotT.push_back("LR_trackPParRatio_2");
  rotT.push_back("LR_trackJetDist_1");
  rotT.push_back("LR_trackJetDist_2");
  rotT.push_back("LR_trackDecayLenVal_1");
  rotT.push_back("LR_trackDecayLenVal_2");
  rotT.push_back("LR_trackSip2dSig_all");
  rotT.push_back("LR_trackSip3dSig_all");
  rotT.push_back("LR_trackSip2dVal_all");
  rotT.push_back("LR_trackSip3dVal_all");
  rotT.push_back("LR_trackPtRel_all");
  rotT.push_back("LR_trackPPar_all");
  rotT.push_back("LR_trackEtaRel_all");
  rotT.push_back("LR_trackDeltaR_all");
  rotT.push_back("LR_trackPtRatio_all");
  rotT.push_back("LR_trackPParRatio_all");
  rotT.push_back("LR_trackJetDist_all");
  rotT.push_back("LR_trackDecayLenVal_all");
  rotT.push_back("LR_trackSip2dSig_split");
  rotT.push_back("LR_trackSip3dSig_split");
  rotT.push_back("LR_trackSip2dVal_split");
  rotT.push_back("LR_trackSip3dVal_split");
  rotT.push_back("LR_trackPtRel_split");
  rotT.push_back("LR_trackPPar_split");
  rotT.push_back("LR_trackEtaRel_split");
  rotT.push_back("LR_trackDeltaR_split");
  rotT.push_back("LR_trackPtRatio_split");
  rotT.push_back("LR_trackPParRatio_split");
  rotT.push_back("LR_trackJetDist_split");
  rotT.push_back("LR_trackDecayLenVal_split");
  
  rotT1.push_back("trackSip2dSig_1");
  rotT1.push_back("trackSip3dSig_1");
  rotT1.push_back("trackSip2dVal_1");
  rotT1.push_back("trackSip3dVal_1");
  rotT1.push_back("trackPtRel_1");
  rotT1.push_back("trackPPar_1");
  rotT1.push_back("trackEtaRel_1");
  rotT1.push_back("trackDeltaR_1");
  rotT1.push_back("trackPtRatio_1");
  rotT1.push_back("trackPParRatio_1");
  rotT1.push_back("trackJetDist_1");
  rotT1.push_back("trackDecayLenVal_1");  
  
  rotT2.push_back("trackSip2dSig_2");
  rotT2.push_back("trackSip3dSig_2");
  rotT2.push_back("trackSip2dVal_2");
  rotT2.push_back("trackSip3dVal_2");
  rotT2.push_back("trackPtRel_2");
  rotT2.push_back("trackPPar_2");
  rotT2.push_back("trackEtaRel_2");
  rotT2.push_back("trackDeltaR_2");
  rotT2.push_back("trackPtRatio_2");
  rotT2.push_back("trackPParRatio_2");
  rotT2.push_back("trackJetDist_2");
  rotT2.push_back("trackDecayLenVal_2");  
  
  rotLR1.push_back("LR_trackSip2dSig_1");
  rotLR1.push_back("LR_trackSip3dSig_1");
  rotLR1.push_back("LR_trackSip2dVal_1");
  rotLR1.push_back("LR_trackSip3dVal_1");
  rotLR1.push_back("LR_trackPtRel_1");
  rotLR1.push_back("LR_trackPPar_1");
  rotLR1.push_back("LR_trackEtaRel_1");
  rotLR1.push_back("LR_trackDeltaR_1");
  rotLR1.push_back("LR_trackPtRatio_1");
  rotLR1.push_back("LR_trackPParRatio_1");
  rotLR1.push_back("LR_trackJetDist_1");
  rotLR1.push_back("LR_trackDecayLenVal_1");  
  
  rotLR2.push_back("LR_trackSip2dSig_2");
  rotLR2.push_back("LR_trackSip3dSig_2");
  rotLR2.push_back("LR_trackSip2dVal_2");
  rotLR2.push_back("LR_trackSip3dVal_2");
  rotLR2.push_back("LR_trackPtRel_2");
  rotLR2.push_back("LR_trackPPar_2");
  rotLR2.push_back("LR_trackEtaRel_2");
  rotLR2.push_back("LR_trackDeltaR_2");
  rotLR2.push_back("LR_trackPtRatio_2");
  rotLR2.push_back("LR_trackPParRatio_2");
  rotLR2.push_back("LR_trackJetDist_2");
  rotLR2.push_back("LR_trackDecayLenVal_2");
  

  rotLRall.push_back("LR_trackSip2dSig_all");
  rotLRall.push_back("LR_trackSip3dSig_all");
  rotLRall.push_back("LR_trackSip2dVal_all");
  rotLRall.push_back("LR_trackSip3dVal_all");
  rotLRall.push_back("LR_trackPtRel_all");
  rotLRall.push_back("LR_trackPPar_all");
  rotLRall.push_back("LR_trackEtaRel_all");
  rotLRall.push_back("LR_trackDeltaR_all");
  rotLRall.push_back("LR_trackPtRatio_all");
  rotLRall.push_back("LR_trackPParRatio_all");
  rotLRall.push_back("LR_trackJetDist_all");
  rotLRall.push_back("LR_trackDecayLenVal_all");
  
  rotLRsplit.push_back("LR_trackSip2dSig_split");
  rotLRsplit.push_back("LR_trackSip3dSig_split");
  rotLRsplit.push_back("LR_trackSip2dVal_split");
  rotLRsplit.push_back("LR_trackSip3dVal_split");
  rotLRsplit.push_back("LR_trackPtRel_split");
  rotLRsplit.push_back("LR_trackPPar_split");
  rotLRsplit.push_back("LR_trackEtaRel_split");
  rotLRsplit.push_back("LR_trackDeltaR_split");
  rotLRsplit.push_back("LR_trackPtRatio_split");
  rotLRsplit.push_back("LR_trackPParRatio_split");
  rotLRsplit.push_back("LR_trackJetDist_split");
  rotLRsplit.push_back("LR_trackDecayLenVal_split");


  WriteMatrix("rotAll", rotAll);
  WriteMatrix("rotT", rotT);
  WriteMatrix("rotT1", rotT1);
  WriteMatrix("rotT2", rotT2);
  WriteMatrix("rotLR1", rotLR1);
  WriteMatrix("rotLR2", rotLR2);
  WriteMatrix("rotLRall", rotLRall);
  WriteMatrix("rotLRsplit", rotLRsplit);

}
