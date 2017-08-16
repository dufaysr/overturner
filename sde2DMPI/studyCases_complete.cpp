/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "studyCases.hpp"

void StudyCaseTransitionProbabilitiesMPI(const AbstractAdvDiffProblem& prob, std::string model,
									  int ncelly, int ncellz, int nyloc, int nzloc, 
									  double Times[], int nTimes, bool binary)
{
	std::cout << "\nRunning StudyCaseTransitionProbabilitiesMPI..." << std::endl;
	int myid, iyproc, izproc, nprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Status status;
	std::string filename;
	int fileIsFree, tagIsFree;
	iyproc = myid/ncellz;
	izproc = myid - (iyproc*ncellz);

	if (myid == 0){
		std::cout << "Writing fInfo..." << std::endl;
		std::string fInfo_filename = wd::root + "out/" + model + "/info.out"; 
		std::ofstream fInfo(fInfo_filename.c_str(), std::ios::out | std::ios::trunc);
//		auto tt = std::time(nullptr);
//	    auto tm = *std::localtime(&tt);
//	    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
	    fInfo << "studyCaseTrajectories\n";
	    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
	    fInfo << "The values used are :\n";
	    prob.printInfo(fInfo);
	    fInfo << "nyloc = " << nyloc << "\n";
    	fInfo << "nzloc = " << nzloc << "\n";
    	fInfo << "ncelly = " << ncelly << "\n";
    	fInfo << "ncellz = " << ncellz << "\n";
    	fInfo.close();
	}

	const int Nloc = nyloc*nzloc;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	double *yStart = new double[Nloc];
	double *zStart = new double[Nloc];
	double dy = L/ncelly;
	double dz = H/ncellz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;
	for (int iybox=0; iybox<nyloc; iybox++){
		for (int izbox=0; izbox<nzloc; izbox++){
			yStart[iybox*nzloc+izbox] = prob.getL0() + iyproc*dy + (iybox+.5)*dybox;
			zStart[iybox*nzloc+izbox] = prob.getH0() + izproc*dz + (izbox+.5)*dzbox;
		}
	}
	Particles2D part(Nloc,yStart,zStart);
	delete[] yStart; delete[] zStart;

	BISolver *solver;
	BoxEstimator *estim;

	// if (myid == 0){ 
	 	double tstart,t; // timing
		double tot_seconds, minutes; // timing
	// }

	for(int i=0; i<nTimes; i++)
	{
		if (myid==0){
			std::cout << "--------------------ITERATION " << i << "/" << nTimes-1 <<"--------------------" << std::endl;
			tstart = MPI_Wtime();
			std::cout << "1/3 : Generating trajectories..." << std::endl;
			t = MPI_Wtime();
		}
		solver = new BISolver(part);
		part = solver->Run(prob,Times[i]);
		delete solver;
		if (myid == 0){
			tot_seconds = MPI_Wtime()-t;
			minutes = floor(tot_seconds/60);
			std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
	
			std::cout << "2/3 : Computing estimator..." << std::endl;
			t = MPI_Wtime();
		}
		estim = new BoxEstimator(ncelly,ncellz,H,L);
		estim->Estimate(part);
		if (myid == 0){
			tot_seconds = MPI_Wtime()-t;
			minutes = floor(tot_seconds/60);
			std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

			std::cout << "3/3 : Writing in the files..." << std::endl;
			t = MPI_Wtime();
		}

		/* Write on files : the idea is to serialize this output operation */
		filename = wd::root + "out/" + model + "/M" + std::to_string(i) +  ".bin";
		fileIsFree = 0, tagIsFree = 1;
		if (myid==0)
			fileIsFree = 1;
		else // wait for receiving fileIsFree from previous process (taskid-1)
			MPI_Recv(&fileIsFree,1,MPI_INT,myid-1,tagIsFree,MPI_COMM_WORLD,&status);
		
		if (fileIsFree){ // the process can perform the output operation
			std::ofstream f;
			if (myid == 0)
				f.open(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
			else
				f.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
			estim->Print(f,binary);
    		f.close();
		}
	
		if(myid != nprocs-1) // the process has finished output. If it is not the last one, send fileIsFree to next process
			MPI_Send(&fileIsFree,1,MPI_INT,myid+1,tagIsFree,MPI_COMM_WORLD);
		/* end write on file */
		delete estim;
		
		if (myid == 0){
			tot_seconds = MPI_Wtime()-t;
			minutes = floor(tot_seconds/60);
			std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

			tot_seconds = MPI_Wtime()-tstart;
			minutes = floor(tot_seconds/60);
			std::cout << "Complete iteration took " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
		}
	}
}