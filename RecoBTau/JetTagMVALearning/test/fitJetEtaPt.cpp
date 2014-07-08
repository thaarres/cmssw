#include <assert.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <cmath>

#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>

using namespace std;

double compute(double params[7][6], double x, double y)
{
	
	double facs[7];
	for(int i = 0; i < 7; i++) {
		double *v = params[i];
		//polynome of 9th order
		//facs[i] = v[0] + y * (v[1] + y * (v[2] + y * (v[3] + y * (v[4] + y * (v[5]+ y * (v[6] + y * (v[7] + y * (v[8] + y * v[9]))))))));
		facs[i] = v[0] + y * (v[1] + y * (v[2] + y * (v[3] + y * (v[4] + y * v[5]))));
	}

	double xs[5];
	xs[0] = x * x; //x^2
	xs[1] = xs[0] * xs[0]; //x^4
	xs[2] = xs[1] * xs[0]; //x^6
	xs[3] = xs[1] * xs[1]; //x^8
	xs[4] = xs[2] * xs[1]; //x^10
	xs[5] = xs[2] * xs[2]; //x^12

	return facs[0] +
	       facs[1] * (2 * xs[0] - 1) +
	       facs[2] * (8 * xs[1] - 8 * xs[0] + 1) +
	       facs[3] * (32 * xs[2] - 48 * xs[1] + 18 * xs[0] - 1) +
	       facs[4] * (128 * xs[3] - 256 * xs[2] + 160 * xs[1] - 32 * xs[0] + 1) +
	       facs[5] * (512 * xs[4] - 1280 * xs[3] + 1120 * xs[2] - 400 * xs[1] + 50 * xs[0] - 1) +
	       facs[6] * (2048 * xs[5] - 6144 * xs[4] + 6912 * xs[3] - 3584 * xs[2] + 840 * xs[1] - 72 * xs[0] + 1);
}

int main(int argc, char **argv)
{ 
	// one or two input files
	assert(argc == 2 || argc == 3); // if not one or two input files -> abort

	std::auto_ptr<TFile> inFile(new TFile(argv[1]));
	assert(inFile.get());
	TH2D *th2 = dynamic_cast<TH2D*>(inFile->Get("jets"));
	assert(th2);
	th2->Sumw2();
	th2->GetXaxis()->SetTitle("#eta");
	th2->GetYaxis()->SetTitle("log(p_{T} + 50)");
	//th2->GetZaxis()->SetTitle("#jets");

	if (argc == 3) {
		std::auto_ptr<TFile> inFile2(new TFile(argv[2]));
		assert(inFile2.get());
		TH2D *th2b = dynamic_cast<TH2D*>(inFile2->Get("jets"));
		assert(th2b);
		th2b->Sumw2();
		th2->SetName("jets_ratio");
		th2->Divide(th2b); // if there is a second input file, divide the histogram of the b jet pt/eta through the histogram of the non-b jet pt/eta
	}

	TFile g("out.root", "RECREATE"); //rootfile with control plots
	//create histogram that will contain the eta values of the jets for a certain ptbin (the real value of eta is transformed between -1 and 1 because the Chebychev polynominial is only defined on that range)
	TH1D *th1[th2->GetNbinsY()];
	for(int i = 0; i < th2->GetNbinsY(); i++) {
		th1[i] = new TH1D(Form("ptslice%d", i), "slice",th2->GetNbinsX(), -1.0, +1.0); //number of bins related to number of etabins in histoJetEtaPt.C 
		th1[i]->GetXaxis()->SetTitle("#eta (scale transformed)");
		//th1[i]->GetYaxis()->SetTitle("#jets");
		th1[i]->SetDirectory(0); 
	}

///////////////// define the fitfunction for the eta distribution in a certain pt slice
	TF1 *cheb = new TF1("ChebS8", "[0] +"
	                              "[1] * (2 * (x^2) - 1) + "
	                              "[2] * (8 * (x^4) - 8 * (x^2) + 1) + "
	                              "[3] * (32 * (x^6) - 48 * (x^4) + 18 * (x^2) - 1) + "
	                              "[4] * (128 * (x^8) - 256 * (x^6) + 160 * (x^4) - 32 * (x^2) + 1) +"
	                              "[5] * (512 * (x^10) - 1280 * (x^8) + 1120 * (x^6) - 400 * (x^4) + 50 * (x^2) - 1) +"
	                              "[6] * (2048 * (x^12) - 6144 * (x^10) + 6912 * (x^8) - 3584 * (x^6) + 840 * (x^4) - 72 * (x^2) + 1)",
	                    -1.0, +1.0);
	cheb->SetParLimits(0, 0.0, 100000.0);

///////////////// fit the etadistributions for each ptslice and fill the distributions of the fitcoefficients for the different pt slices
	TH1D *coeffs[7];
	for(int i = 0; i < 7; i++) {
		coeffs[i] = new TH1D(Form("coeff%d", i), "coeffs", th2->GetNbinsY()-2, 0.5, th2->GetNbinsY()-2 + 0.5); //for each coefficient create a histogram with the number of ptbins
		coeffs[i]->GetXaxis()->SetTitle("log(p_{t} + 50)");
		//coeffs[i]->GetYaxis()->SetTitle("#jets");
		coeffs[i]->SetDirectory(0);
	}
	
	std::cout << "number of ptbins: " << th2->GetNbinsY() << std::endl;
	std::cout << "number of etabins: " << th2->GetNbinsX() << std::endl;

	for(int y = 1; y <= th2->GetNbinsY()-2; y++) { //loop over ptbins
		for(int x = 1; x <= th2->GetNbinsX(); x++) //loop over etabins 
		{
			th1[y]->SetBinContent(x, th2->GetBinContent(x, y)); // weight!
		}
		th1[y]->SetDirectory(&g);
		th1[y]->Fit(cheb, "QRNB"); // fit the new histogram with the crazy fitfunction!
		th1[y]->Write();
		cheb->SetName(Form("cheb%d", y));
		cheb->Write();
		
		for(int i = 0; i < 7; i++) {
			coeffs[i]->SetBinContent(y, cheb->GetParameter(i));
			coeffs[i]->SetBinError(y, cheb->GetParError(i));
		}
	}

///////////////// fit the coefficients as function of pt
	double params[7][6];
	TF1 *pol[7];
	for(int i = 0; i < 7; i++) {
		//the following piece of code is not relevant: expo, arg and f1 not used afterwards and coeffs[i] is refitted with pol5
		/*coeffs[i]->Fit("expo", "Q0");
		TF1 *f1 = coeffs[i]->GetFunction("expo");
		double expo, arg;
		if (f1->GetParameter(0) >= -100000.0) { 
			expo = std::exp(f1->GetParameter(0));
			arg = f1->GetParameter(1);
		} else {
			coeffs[i]->Scale(-1.0);
			coeffs[i]->Fit("expo", "Q0");
			f1 = coeffs[i]->GetFunction("expo");
			expo = -std::exp(f1->GetParameter(0));
			arg = f1->GetParameter(1);
			coeffs[i]->Scale(-1.0);
		}*/
		coeffs[i]->Fit("pol5", "0B");
		pol[i] = coeffs[i]->GetFunction("pol5");
		pol[i]->SetName(Form("pol%d", i));
		pol[i]->Write();
		for(int j = 0; j < 6; j++)
			params[i][j] = pol[i]->GetParameter(j);
	}

///////////////// store the fit"function" in the histogram and calculate the chi2
	double chi2 = 0.0;
	int ndf = 0;
	TH2D *th2c = new TH2D(*th2);
	th2c->SetName("fit");
	//th2c->GetZaxis()->SetTitle("Fitted #jets");
	for(int y = 1; y <= th2->GetNbinsY(); y++) { // ptbins
		int ry = y > th2->GetNbinsY()-4 ? th2->GetNbinsY()-4 : y;
		for(int x = 1; x <= th2->GetNbinsX(); x++) { // etabins
			//std::cout << "original histo, now in ptbin y: " << y << " (ry: " << ry <<") and etabin x: " << x << std::endl;
			//using the coefficients of the fitted functions, the value is calculated (this value will be close to the original value in case 1 rootfile is given and will the close to the original value of the divided histograms if 2 rootfiles are given)
			double val = compute(params, (x - 0.5) /(0.5*(float) th2->GetNbinsX()) - 1.0, ry);
			//std::cout << "val " << val << " for " << (x - 0.5) / 25.0 - 1.0 << " and for ry: " << ry  << std::endl;
			if (val < 0)
				val = 0;
			double error = th2->GetBinError(x, y);
			if (error > 0 && val > 0) {
				double chi = th2->GetBinContent(x, y) - val;
				chi2 += chi * chi / (error*error);
				ndf++;
			}
			th2c->SetBinContent(x, y, val); 
		}
	}
	std::cout << "chi2/ndf(" << ndf << "): " << (chi2 / ndf) << std::endl;



///////////////// writing histos
	for(int i = 0; i < 7; i++)
	{
		coeffs[i]->SetDirectory(&g);
		coeffs[i]->Write();
	}
	th2->SetDirectory(&g);
	th2->Write(); //write the original histogram (or the result of the two histograms divided by eachother)
	th2c->SetDirectory(&g);
	th2c->Write(); //write the histogram with the new weights
	
	TH2D* th2_divide = (TH2D*) th2->Clone();
	TH2D* th2c_copy = (TH2D*) th2c->Clone();
	th2_divide->Divide(th2c_copy); //make ratio of th2 and th2c; this should be a 'reweighted (in the ideal case flat) distribution' because you divide the 'data' by a 'fit of the data'
	th2_divide->SetName("th2_divide"); //IMPORTANT!! otherwise by projecting you will end up with unwanted behaviour
	th2_divide->Write("jets_weighted");
	
	//make projections of the 2D plots
	TH1D* th2_ProjX = th2->ProjectionX();
	TH1D* th2_ProjY = th2->ProjectionY();
	th2_ProjX->Write("jets_ProjEta");
	th2_ProjY->Write("jets_ProjPt");	
	
	TH1D* th2c_ProjX = th2c->ProjectionX();
	TH1D* th2c_ProjY = th2c->ProjectionY();
	th2c_ProjX->Write("fit_ProjEta");
	th2c_ProjY->Write("fit_ProjPt");
	
	TH1D* th2_divide_ProjX = th2_divide->ProjectionX();
	TH1D* th2_divide_ProjY = th2_divide->ProjectionY();
	th2_divide_ProjX->Write("jets_weighted_ProjEta");
	th2_divide_ProjY->Write("jets_weighted_ProjPt");
	
	//projections where Pt is retransformed (see the transformation log(jetPt+50) in histoJetEtaPt.cpp)
	TAxis* axis_th2_ProjY = th2_ProjY->GetXaxis();
	const int nbins = axis_th2_ProjY->GetNbins();
	cout<<" nbins = "<<nbins<<endl;
	Double_t xbinsarray_th2_ProjY[nbins+1];
	Double_t xbinsarray_th2_ProjY_retransformed[nbins+1];
	for(int b=0; b<nbins; b++)
	{
	   xbinsarray_th2_ProjY[b] = axis_th2_ProjY->GetBinLowEdge(b);
	   //cout<<"  bin edge "<<b<<": "<<xbinsarray_th2_ProjY[b]<<endl;
		 xbinsarray_th2_ProjY_retransformed[b] = exp(xbinsarray_th2_ProjY[b]) - 50; //inverse transformation as done in histoJetEtaPt.cpp
		 //cout<<"  bin edge retransformed "<<b<<": "<<xbinsarray_th2_ProjY_retransformed[b]<<endl;
	}
	xbinsarray_th2_ProjY[nbins] = axis_th2_ProjY->GetBinUpEdge(nbins);
	xbinsarray_th2_ProjY_retransformed[nbins] = exp(xbinsarray_th2_ProjY[nbins]) - 50; //inverse transformation as done in histoJetEtaPt.cpp
	TH1D* th2_ProjY_retransformed = new TH1D("jets_ProjPt_retransformed","jets_ProjPt_retransformed",nbins,xbinsarray_th2_ProjY_retransformed);
	TH1D* th2_divide_ProjY_retransformed = new TH1D("jets_weighted_ProjPt_retransformed","jets_weighted_ProjPt_retransformed",nbins,xbinsarray_th2_ProjY_retransformed);
	//Double_t max = 0;
	for(int b=0; b<nbins+1; b++)
	{
	   //cout<< "th2_ProjY->GetBinContent("<<b<<") = "<<th2_ProjY->GetBinContent(b)<<endl;
	   th2_ProjY_retransformed->SetBinContent(b,th2_ProjY->GetBinContent(b));
		 th2_divide_ProjY_retransformed->SetBinContent(b,th2_divide_ProjY->GetBinContent(b));
		 //if(th2_divide_ProjY->GetBinContent(b) > max) max = th2_divide_ProjY->GetBinContent(b);
	}
	//th2_divide_ProjY_retransformed->GetYaxis()->SetRangeUser(0,max+5); //it's clearer in this way to judge a distributions as 'flat'
	th2_ProjY_retransformed->Write("jets_ProjPt_retransformed");
	th2_divide_ProjY_retransformed->Write("jets_weighted_ProjPt_retransformed");
	
	g.Close();

///////////////// writing relevant parameters for reweighting
	std::ofstream of("out.txt");
	of << std::setprecision(16);
	for(int i = 0; i < 7; i++)
		for(int j = 0; j < 6; j++)
			of << params[i][j] << (j < 5 ? "\t" : "\n");
	of.close();

	return 0;
}
