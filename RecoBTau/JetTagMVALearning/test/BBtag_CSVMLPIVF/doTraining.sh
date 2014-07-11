# echo "BEFORE RUNNING THIS OUT OF THE BOX, CHECK THE INSTRUCTIONS"
# echo "https://twiki.cern.ch/twiki/bin/view/CMS/BTagSoftwareMVATrainer"
# 
# # read -p "PARALLEL PROCESSING: how many cores can you afford? " answer
# 
# echo "ADAPT BIASES IF NECESSARY!!!"
# echo ">>>> IF YOU DON'T KNOW WHAT IS MEANT: ASK SOMEONE ;-)"
# 
# echo "Calculating the bias: ARE YOU SURE THAT YOU HAVE ENOUGH STATISTICS TO DETERMINE THE BIAS ACCURATELY?"
# g++ bb_biasForXml.cpp `root-config --cflags --glibs` -o bias
# ./bias $path_to_rootfiles $prefix
# echo "ARE YOU SURE THAT YOU HAVE ENOUGH STATISTICS TO DETERMINE THE BIAS ACCURATELY?"

answer=4
#answer=1

path_to_rootfiles=/afs/cern.ch/user/t/thaarres/CSVTraining/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/rootfiles/RadionToHH_4b_M-1000_TuneZ2star_8TeV-Madgraph_pythia6_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM/RecoVsRecoReco/
#Combinations="NoVertex_B_BB NoVertex_B_C PseudoVertex_B_BB PseudoVertex_B_C RecoVertex_B_BB RecoVertex_B_C RecoRecoVertex_B_BB RecoRecoVertex_B_C"
Combinations="NoVertex_B_BB  PseudoVertex_B_BB RecoVertex_B_BB RecoRecoVertex_B_BB"
CAT="Reco Pseudo No RecoReco"
prefix="CombinedSVV2"

# echo "Filling the 2D pt/eta histograms" 
# 
# g++ ./bb_histoJetEtaPt.cpp `root-config --cflags --glibs` -o histos
# ./histos $path_to_rootfiles $prefix

# echo "Reweighting the trees according to the pt/eta weights and saving the relevant variables " 
# 
#files=("MVATrainer_No_B_BB_cfg.py" "MVATrainer_No_B_C_cfg.py" "MVATrainer_Pseudo_B_BB_cfg.py" "MVATrainer_Pseudo_B_C_cfg.py" "MVATrainer_Reco_B_BB_cfg.py" "MVATrainer_Reco_B_C_cfg.py" "MVATrainer_RecoReco_B_BB_cfg.py" "MVATrainer_RecoReco_B_C_cfg.py")
files=("MVATrainer_No_B_BB_cfg.py" "MVATrainer_Pseudo_B_BB_cfg.py" "MVATrainer_Reco_B_BB_cfg.py" "MVATrainer_RecoReco_B_BB_cfg.py")

l=0
while [ $l -lt 4  ]
do
	jobsrunning=0
	while [ $jobsrunning -lt $answer ]
	do
echo ${files[l]}
		nohup cmsRun ${files[l]} &
		let jobsrunning=$jobsrunning+1
		let l=$l+1
	done
	wait
done

# echo ">>>> CHECK THAT THE train*_save.root FILES ARE CORRECTLY PRODUCED! OPEN A FILE AND CHECK THAT THE WEIGHT BRANCH IS NOT EMPTY...."
# echo " "
# echo " "
# echo "If you want to do the actual training: by default the Train*xml files are used, for detailed studies one could use CSVMVA*xml. First chose which ones you want to use and then uncomment the following lines in the doTraining.sh script."
# 
# CombinationsArray=("NoVertex_B_BB" "NoVertex_B_C" "PseudoVertex_B_BB" "PseudoVertex_B_C" "RecoVertex_B_BB" "RecoVertex_B_C" "RecoRecoVertex_B_BB" "RecoRecoVertex_B_C")
# l=0
# while [ $l -lt 8 ]
# do
# 	jobsrunning=0
# 	while [[ $jobsrunning -lt $answer && $jobsrunning -lt 8 ]] 
# 	do
# 		echo Processing ${CombinationsArray[l]}
# 		mkdir tmp${CombinationsArray[l]}
# 		cd tmp${CombinationsArray[l]}
#  		echo mvaTreeTrainer ../Train_${CombinationsArray[l]}.xml ${CombinationsArray[l]}.mva ../train_${CombinationsArray[l]}_save.root
#  		nohup mvaTreeTrainer ../Train_${CombinationsArray[l]}.xml ${CombinationsArray[l]}.mva ../train_${CombinationsArray[l]}_save.root &
# #		echo mvaTreeTrainer ../CSVMVA_${CombinationsArray[l]}_defaultFromDB.xml tmp.mva ../train_${CombinationsArray[l]}_save.root
# #		nohup mvaTreeTrainer ../CSVMVA_${CombinationsArray[l]}_defaultFromDB.xml tmp.mva ../train_${CombinationsArray[l]}_save.root &
# 		cd ..
# 		let jobsrunning=$jobsrunning+1
# 		let l=$l+1
# 	done
# 	wait
# done


# echo "Combine the B versus BB and B versus C training "
# 
# VertexCategory=("NoVertex" "PseudoVertex" "RecoVertex")
# l=0
# while [ $l -lt 3 ]
# do
# 	jobsrunning=0
# 	while [[ $jobsrunning -lt $answer  && $jobsrunning -lt 3 ]] 
# 	do
# 		mkdir tmp${VertexCategory[l]}
# 		cd tmp${VertexCategory[l]}
# 		cp ../tmp${VertexCategory[l]}_B_C/*.xml .
# 		cp ../tmp${VertexCategory[l]}_B_C/*.txt .
# 		cp ../tmp${VertexCategory[l]}_B_BB/*.xml .
# 		cp ../tmp${VertexCategory[l]}_B_BB/*.txt .
#  		echo mvaTreeTrainer -l ../Train_${VertexCategory[l]}.xml ${prefix}MVA_${VertexCategory[l]}.mva ../train_${VertexCategory[l]}_B_BB_save.root ../train_${VertexCategory[l]}_B_C_save.root
#  		nohup mvaTreeTrainer -l ../Train_${VertexCategory[l]}.xml ${prefix}MVA_${VertexCategory[l]}.mva ../train_${VertexCategory[l]}_B_BB_save.root ../train_${VertexCategory[l]}_B_C_save.root &
# #		echo mvaTreeTrainer -l ../CSVMVA_${VertexCategory[l]}_defaultFromDB.xml ${prefix}MVA_${VertexCategory[l]}.mva ../train_${VertexCategory[l]}_B_BB_save.root ../train_${VertexCategory[l]}_B_C_save.root
# #		nohup mvaTreeTrainer -l ../CSVMVA_${VertexCategory[l]}_defaultFromDB.xml ${prefix}MVA_${VertexCategory[l]}.mva ../train_${VertexCategory[l]}_B_BB_save.root ../train_${VertexCategory[l]}_B_C_save.root &
# 		cd ..
# 		let jobsrunning=$jobsrunning+1
# 		let l=$l+1
# 	done
# 	wait
# done

