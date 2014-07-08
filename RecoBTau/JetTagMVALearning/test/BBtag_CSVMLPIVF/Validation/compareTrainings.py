######################################################################
###                                                                ###
### Script for plotting comparisons of different performances      ###
###                                                                ###
### Main thing to be changed by YOU: list of paths.                ###
###                                                                ###
### Easily adjustable                                              ###
### - legend                                                       ###
### - color                                                        ### 
###                                                                ###
### Assumes you have your CSV performance files                    ###
### in different directories for different training settings       ###
###                                                                ###
### TODO: - make it usable for pT/eta comparisons too              ###
###       - let setting be specified 'outside' the script when running
###                                                                ###
######################################################################
###                                                                ###
### What the user needs to do:                                     ###
###                                                                ###
### 1) Adjust the list of training settings that you would like to compare
### 2) Update the maps of paths/colors/legends such that the maps are not empty for you new variable
### 3) Run as python -i compareTrainings.py                    ###
###                                                                ###
######################################################################

from ROOT import *
gROOT.SetStyle("Plain")

############################################################################################ 
### This is the list of training settings you would like to compare                      ###
### => Change it if you would like to compare more/different settings.                   ###
### => If you add new variables, update the maps of path/color/legend accordingly (once) ###
############################################################################################ 

settingList = [
								"New (CMSSW_534, CSV LR)", #the CSVV1
								"New (CMSSW_534, CSV git)"#, #the CSVV1 but in CMSSW_5_3_13_patch3 with git recipe 
               ]

#################################################
### here are just some maps of paths          ###
###  and legend names and colors for plotting ###
### - paths: adjust only once                 ### 
### - legend/color: adjust when you want      ###
#################################################

path = "/user/gvonsem/BTagServiceWork/MVA/testBtagVal_NewJuly2012/CMSSW_5_3_4_patch1/src/Validation/RecoB/"

pathList = {
						"New (CMSSW_534, CSV LR)": "/user/gvonsem/BTagServiceWork/MVA/testBtagVal_NewJuly2012/CMSSW_5_3_4_patch1/src/Validation/RecoB/test_withPFnoPU/INTERACTIVE_validationwithPFnoPU_Fix_Adding2HighPtBins_nonFit-Reweighting/DQM_V0001_R000000001__POG__BTAG__categories.root",
						"New (CMSSW_534, CSV git)": "/user/kderoove/bTagging/CMSSW_5_3_13_patch3/src/Validation/RecoB/test/CSV_LR_gitRecipeTest/DQM_V0001_R000000001__POG__BTAG__BJET.root"#,
						} 

leg = {
				"New (CMSSW_534, CSV LR)": "CSVV1",
				"New (CMSSW_534, CSV git)": "CSVV1 git"#,
       }

color = {
			"New (CMSSW_534, CSV LR)": 1,
			"New (CMSSW_534, CSV git)": 8#,
         }

#####################################
### Now we're gonna loop and plot ###
#####################################
   
plotList1bin   = {}
plotList1bin_C = {}
plotList   = {}
plotList_C = {}

for setting in settingList:
    ### Get file and plot
    fileName = pathList[setting]
    file     = TFile.Open(fileName)
    print setting
    if setting == "New (CMSSW_534, CSV LR)"   or setting == "New (CMSSW_534, CSV git)":
       plot1bin     = file.Get("DQMData/Run 1/Btag/Run summary/CSV_ETA_0-1v2_PT_90-150/FlavEffVsBEff_DUSG_discr_CSV_ETA_0-1v2_PT_90-150")
       plot1bin_C   = file.Get("DQMData/Run 1/Btag/Run summary/CSV_ETA_0-1v2_PT_90-150/FlavEffVsBEff_C_discr_CSV_ETA_0-1v2_PT_90-150")
       plot     = file.Get("DQMData/Run 1/Btag/Run summary/CSV_GLOBAL/FlavEffVsBEff_DUSG_discr_CSV_GLOBAL")
       plot_C   = file.Get("DQMData/Run 1/Btag/Run summary/CSV_GLOBAL/FlavEffVsBEff_C_discr_CSV_GLOBAL")
		elif
			print "check path to DQM file or histogram directory and name inside the DQM file"
		### Set name of plot
    plot1bin.SetName(pathList[setting])
    plot1bin_C.SetName(pathList[setting])
    plot.SetName(pathList[setting])
    plot_C.SetName(pathList[setting])
    ### Fill plot list
    plotList1bin[setting]   = plot1bin  
    plotList1bin_C[setting] = plot1bin_C
    plotList[setting]   = plot  
    plotList_C[setting] = plot_C


### Make canvas ###

Plots2 = TCanvas("Plots2","",1200,600)
Plots2.Divide(2)

Plots2.cd(1).SetLogy()

leg3 = TLegend(0.2,0.6,0.45,0.9)
leg3.SetFillColor(0);

### and draw ###

first=true
for setting in settingList:
    plotList[setting].SetMarkerColor(color[setting])
    if first:
        plotList[setting].GetXaxis().SetTitle("B efficiency")
        plotList[setting].GetYaxis().SetTitle("DUSG efficiency")
        plotList[setting].SetTitle("")
        plotList[setting].Draw()
        first=false
    else         :
        plotList[setting].Draw("same")  
    leg3.AddEntry(plotList[setting],leg[setting],"p")
  
leg3.Draw()

###################
### Second plot ###
###################

Plots2.cd(2).SetLogy()

## and draw again

first=true
for setting in settingList:
    plotList_C[setting].SetMarkerColor(color[setting])
    if first:
        plotList_C[setting].Draw()
        plotList_C[setting].GetXaxis().SetTitle("B efficiency")
        plotList_C[setting].GetYaxis().SetTitle("C efficiency")
        plotList_C[setting].SetTitle("")
        first=false
    else         :
        plotList_C[setting].Draw("same")  

###########
### FIN ###
###########
