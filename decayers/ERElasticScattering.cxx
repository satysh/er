/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERElasticScattering.h"

// STD
#include <iostream>

//ROOT
#include <TMath.h>
#include <TLorentzVector.h>
#include <TF1.h>
#include <TLorentzRotation.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TVirtualMC.h>

// FairRoot
#include <FairRunSim.h>
#include <FairLogger.h>

using TMath::DegToRad;
using TMath::RadToDeg;

TGraph* thetaCDFGr = NULL;
TGraph* thetaInvCDFGr = NULL;

//-------------------------Globals----------------------------------
Double_t globalTheta;
Double_t ThetaCDF(Double_t *x, Double_t *par)
{
    return thetaCDFGr->Eval(x[0]);
}

Double_t ThetaInvCDF(Double_t *x, Double_t *par)
{
    return thetaInvCDFGr->Eval(x[0]);
}
//------------------------------------------------------------------

ERElasticScattering::ERElasticScattering(TString name):
    ERDecay(name),
    fThetaFileName(""),
    fTheta1(0.),
    fTheta2(0.),
    fPhi1(0),
    fPhi2(360.),
    fTargetIonName(""),
    fTargetIonPDG(NULL),
    fThetaInvCDF(NULL),
    fCDFmin(0.),
    fCDFmax(1.),
    fThetaTargetIon1(0.),
    fThetaTargetIon2(0.),
    fCDFminTargetIon(0.),
    fCDFmaxTargetIon(0.),
    fDetPos(0.),
    fDetThetaWidth(0.),
    fIonMass(0.),
    fInteractNumInTarget(0),
    fCDFRangesSum(0.),
    fThetaCMSumPri(0.),
    fThetaCMSumTar(0.),
    fNumOfPriIons(0),
    fNumOfTarIons(0),
    ionMassTrueOrFalseTester(kFALSE)
{
}
ERElasticScattering::ERElasticScattering(TString name, Int_t index):
    ERElasticScattering(name)
{
    fRanIndex = index;
}
ERElasticScattering::~ERElasticScattering()
{
}

void ERElasticScattering::SetTargetIon(Int_t A, Int_t Z, Int_t Q)
{
    FairRunSim* run = FairRunSim::Instance();
    fTargetIonName = fName + TString("_TargetIon");
    FairIon* ion = new FairIon(fTargetIonName,A,Z,Q);
    run->AddNewIon(ion);
}

Bool_t ERElasticScattering::Init()
{
    if (!ERDecay::Init())
    {
        return kFALSE;
    }

    fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
    if ( ! fTargetIonPDG )
    {
        LOG(FATAL) << "Target ion not found in pdg database!" << FairLogger::endl;
        return kFALSE;
    }

    SetIonMass(fInputIonPDG->Mass());
    SetTargetIonMass(fTargetIonPDG->Mass());

    if (fTheta1 != 0. && fTheta2 != 0.)
        ionMassTrueOrFalseTester = kTRUE;
    else
        RangesCalculate (fInputIonPDG->Mass(), fTargetIonPDG->Mass());

    if (fThetaFileName != "")
    {
        LOG(INFO) << "ElasticScattering " << fName << " initialize from theta distribution file" << FairLogger::endl;

        TString path = TString(gSystem->Getenv("VMCWORKDIR")) + "/input/" + fThetaFileName;
        std::ifstream f;
        f.open(path.Data());
        if (!f.is_open())
        {
            LOG(FATAL) << "Can't open file " << path << FairLogger::endl;
            return kFALSE;
        }

        Int_t nPoints = std::count(std::istreambuf_iterator<char>(f),
                               std::istreambuf_iterator<char>(), '\n');
        f.seekg(0, std::ios::beg);
        TVectorD tet(nPoints);
        TVectorD sigma(nPoints);

        LOG(DEBUG2) << "nPoints = " << nPoints << FairLogger::endl;

        Int_t i = 0;
        while (!f.eof())
        {
            if (i == nPoints) break;
            f >> tet(i) >> sigma(i);
            LOG(DEBUG2) << i << ": " << tet(i) << "\t" << sigma(i) << FairLogger::endl;
            i++;
        }

        thetaCDFGr = new TGraph(tet, sigma);
        thetaInvCDFGr = new TGraph(sigma, tet);

        fThetaCDF = new TF1("thetaCDF", ThetaCDF, 0., 180., 0);
        fThetaInvCDF = new TF1("thetaInvCDF", ThetaInvCDF, 0., 1., 0);

        fCDFmin = fThetaCDF->Eval(fTheta1);
        fCDFmax = fThetaCDF->Eval(fTheta2);
        fCDFminTargetIon = fThetaCDF->Eval(fThetaTargetIon1);
        fCDFmaxTargetIon = fThetaCDF->Eval(fThetaTargetIon2);
    }

    return kTRUE;
}

Bool_t ERElasticScattering::Stepping()
{
/*
    LOG(INFO) << "ERElasticScattering::Stepping " << gMC->CurrentVolName() << FairLogger::endl;
    LOG(INFO) << "fDecayFinish: " << fDecayFinish << FairLogger::endl;
    LOG(INFO) << "TrackPid: " << gMC->TrackPid() << ", PdgCode: " << fInputIonPDG->PdgCode() << FairLogger::endl;
*/
    if (!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() && TString(gMC->CurrentVolName()).Contains(fVolumeName))
    {
        TLorentzVector curPos;
        gMC->TrackPosition(curPos);
        //fDecayPosZ = 0.;
        if (curPos.Z() >= fDecayPosZ)
        {
            gMC->SetMaxStep(fStep);
            TLorentzVector fInputIonV;
            gMC->TrackMomentum(fInputIonV);
            Double_t iM = GetIonMass();
            Double_t tM = GetTargetIonMass();
            Double_t iM2 = pow(iM,2);
            Double_t tM2 = pow(tM,2);

            // Input MC ion mass identify
            if (!ionMassTrueOrFalseTester)
            {
                Double_t mcIonMass = fInputIonV.E() / fInputIonV.Gamma();
                //std::cout.precision(12);
                //std::cout << "PDG Input ion mass: " << fInputIonPDG->Mass() << " ";
                //std::cout << "Input Ion mcMass: " << mcIonMass << std::endl;
                SetIonMass(mcIonMass);
                iM = mcIonMass;
                ionMassTrueOrFalseTester=kTRUE;
                RangesCalculate(iM, tM); // For the angles are drawing the ranges
                //std::cout << "primary ion mass: " << iM << ", target ion mass: " << tM << std::endl;
                //std::cout.precision(3);
            }

            Double_t inputIonT = sqrt(pow(fInputIonV.P(),2)+iM2) - iM;

            LOG(DEBUG) << "ElasticScattering: " << fName << FairLogger::endl;
            LOG(DEBUG) << "  Input ion with Ekin = " << inputIonT
                        << ", mass = " << iM
                        << " mom = (" << fInputIonV.Px() << "," << fInputIonV.Py() << "," << fInputIonV.Pz() << ")" << FairLogger::endl;

            Double_t invariant = pow((iM+tM),2)+2*tM*inputIonT;
            Double_t shorty = pow(invariant-iM2-tM2,2);
            Double_t Pcm = sqrt((shorty-4*iM2*tM2)/(4*invariant));

            LOG(DEBUG) << "  CM momentum: " << Pcm << FairLogger::endl;
            LOG(DEBUG) << "  CM Ekin: " << sqrt(pow(Pcm,2)+iM2) - iM << FairLogger::endl;



            Double_t theta = ThetaGen();
            Double_t phi = fRnd->Uniform(fPhi1*DegToRad(), fPhi2*DegToRad());
            Write_Cur_Theta(theta);
            // In case of target ion registration
            if (fIonTester)
            {
                phi = phi + 180.*DegToRad();
                fThetaCMSumTar += theta*RadToDeg();
                fNumOfTarIons++;
            }
            else
            {
                fThetaCMSumPri += theta*RadToDeg();
                fNumOfPriIons++;
            }

            if (fThetaFileName != "")
            {
                LOG(DEBUG) << "  CM [CDFmin,CDFmax] = [" << fCDFmin << "," << fCDFmax << "]" << FairLogger::endl;
            }

            TLorentzVector out1V (Pcm*sin(theta)*cos(phi), Pcm*sin(theta)*sin(phi), Pcm*cos(theta), sqrt(pow(Pcm,2) + iM2));
            TLorentzVector out2V (-out1V.Px(), -out1V.Py(), -out1V.Pz(), sqrt(pow(Pcm,2) + tM2));
            LOG(DEBUG) << "BEFORE BOOST=======================================================" << FairLogger::endl;
            LOG(DEBUG) << "  CM Theta = " << theta*RadToDeg() << ", phi = " << phi*RadToDeg() << FairLogger::endl;
            LOG(DEBUG) << "  CM out1 state(px,py,pz,E) = "<<out1V.Px()<<", "<<out1V.Py()<<", "<<out1V.Pz()
                        << ", " << out1V.E() << FairLogger::endl;
            LOG(DEBUG) << "  CM out2 state(px,py,pz,E) = "<<out2V.Px()<<", "<<out2V.Py()<<", "<<out2V.Pz()
                        << ", " << out2V.E() << FairLogger::endl;
            LOG(DEBUG) << "  CM out1 Ekin = "<< sqrt(pow(out1V.P(),2)+iM2) - iM << FairLogger::endl;
            LOG(DEBUG) << "  CM out2 Ekin = "<< sqrt(pow(out2V.P(),2)+tM2) - tM << FairLogger::endl;
/*
            LOG(DEBUG) << "  Boosting with beta (fInputIon) = " << fInputIonV.Beta()
                        << ", gamma = " << fInputIonV.Gamma() << FairLogger::endl;
*/
            TLorentzVector targetV(0,0,0,tM);
            TLorentzVector cmV = targetV + fInputIonV;
            TVector3 cmVBoost = cmV.BoostVector();
            LOG(DEBUG) << "  tM in targetV(0, 0, 0, tM): " << tM << FairLogger::endl;
            LOG(DEBUG) << "  cmV components: (" << cmV.Px() << ", " << cmV.Py() << ", " << cmV.Pz() << ", " << cmV.E() << ")" << FairLogger::endl;
            LOG(DEBUG) << "  Boosting with beta = " << cmV.Beta()
                        << ", gamma = " << cmV.Gamma() << FairLogger::endl;
            LOG(DEBUG) << "  Module of cmV.BoostVector: " << sqrt(cmVBoost.Px()*cmVBoost.Px() + cmVBoost.Py()*cmVBoost.Py() + cmVBoost.Pz()*cmVBoost.Pz()) << FairLogger::endl;
            LOG(DEBUG) << "  cmV.BoostVector components: (" << cmVBoost.Px() << ", " << cmVBoost.Py() << ", " << cmVBoost.Pz() << ")" << FairLogger::endl;
            theta = cmV.Theta();
            phi = cmV.Phi();
            LOG(DEBUG) << "  Rotation angles: theta = " << theta*RadToDeg() << ", Phi = " << phi*RadToDeg() << FairLogger::endl;

            //out1V.Boost(cmV.BoostVector());
            //out2V.Boost(cmV.BoostVector());
/*
            // we use second case
            out1V.RotateY(theta);
            out1V.RotateZ(phi);
            out1V.Boost(cmV.BoostVector());

            out2V.RotateY(theta);
            out2V.RotateZ(phi);
            out2V.Boost(cmV.BoostVector());
*/
            // third case
            out1V.RotateZ(-phi);
            out1V.RotateY(theta);
            out1V.RotateZ(phi);
            out1V.Boost(cmV.BoostVector());

            out2V.RotateZ(-phi);
            out2V.RotateY(theta);
            out2V.RotateZ(phi);
            out2V.Boost(cmV.BoostVector());

            LOG(DEBUG) << "AFTER BOOST=======================================================" << FairLogger::endl;
            LOG(DEBUG) << "  Lab theta primary ion = " << out1V.Theta()*RadToDeg() << " phi = " << out1V.Phi()*RadToDeg() << FairLogger::endl;
            LOG(DEBUG) << "  Lab out1 T = "<< sqrt(pow(out1V.P(),2)+iM2) - iM <<  FairLogger::endl;
            LOG(DEBUG) << "  Lab out2 T = "<< sqrt(pow(out2V.P(),2)+tM2) - tM <<  FairLogger::endl;
            LOG(DEBUG) << "  Lab theta target ion = " << out2V.Theta()*RadToDeg() << " phi = " << out2V.Phi()*RadToDeg() << FairLogger::endl;
            LOG(DEBUG) << "  Lab out1 state(px,py,pz,E) = " << out1V.Px() << "," << out1V.Py() << "," << out1V.Pz()
                        << "," << out1V.E() << FairLogger::endl;
            LOG(DEBUG) << "  Lab out2 state(px,py,pz,E) = "<<out2V.Px()<<","<<out2V.Py()<<","<<out2V.Pz()
                        << "," << out2V.E() << FairLogger::endl;

            //curPos[2] += 0.0007; //TODO

            AddParticleToStack(fInputIonPDG->PdgCode(),curPos,out1V);
            AddParticleToStack(fTargetIonPDG->PdgCode(),curPos,out2V);

            fDecayFinish = kTRUE;
            gMC->StopTrack();
            gMC->SetMaxStep(10000.);

            // Interactions numbers counter
            fInteractNumInTarget++;
        }
    }

    return kTRUE;
}

Double_t ERElasticScattering::ThetaGen()
{
    Double_t theta = 0.;
    if (fThetaFileName == "")
    {
        theta = acos(fRnd->Uniform(cos(fTheta1*DegToRad()), cos(fTheta2*DegToRad())));
    }
    else
    {
        Double_t dF1 = fabs(fCDFmax-fCDFmin); // N15
        Double_t dF2 = 0.*fabs(fCDFmaxTargetIon-fCDFminTargetIon); // B11
        Double_t dLength = dF1 + dF2;
/*
        std::cout.precision(12);
        std::cout << "summ: "<< dLength << std::endl;
        std::cout.precision(3);
*/

        if (fCDFRangesSum == 0.)
            fCDFRangesSum = dLength;

        Double_t Rnd = fRnd->Uniform(0., 1.)*dLength;
        Double_t curCDF;
        if (Rnd <= dF1)
        {
            curCDF = fCDFmin + Rnd;
            fIonTester = kFALSE;
        }
        else
        {
            curCDF = fCDFminTargetIon + Rnd - dF1;
            fIonTester = kTRUE;
        }

        theta = fThetaInvCDF->Eval(curCDF)*DegToRad();
    }
    return theta;
}

void ERElasticScattering::RangesCalculate(Double_t iM, Double_t tM)
{
    LOG(DEBUG) << "ERElasticScattering::RangesCalculate(" << iM << ", " << tM << ")" << FairLogger::endl;
    Double_t rAng = fDetPos*DegToRad();
    Double_t ratio = iM/tM;
    Double_t ratio2 = ratio*ratio;
    Double_t fDetThetaWRad = fDetThetaWidth*TMath::DegToRad(); // Detectors fDetThetaWRad
    Double_t Radius = 218.;
    // Primary Ion
    if (iM != tM)
    {
        fTheta1 = TMath::RadToDeg()*acos( -ratio*sin(rAng-fDetThetaWRad)*sin(rAng-fDetThetaWRad)
                    + cos(rAng-fDetThetaWRad)*sqrt(1.-ratio2*sin(rAng-fDetThetaWRad)*sin(rAng-fDetThetaWRad)) );
        fTheta2 = TMath::RadToDeg()*acos( -ratio*sin(rAng+fDetThetaWRad)*sin(rAng+fDetThetaWRad)
                    + cos(rAng+fDetThetaWRad)*sqrt(1.-ratio2*sin(rAng+fDetThetaWRad)*sin(rAng+fDetThetaWRad)) );
    }
    else
    {
        fTheta1 = TMath::RadToDeg()*2.*(rAng - fDetThetaWRad);
        fTheta2 = TMath::RadToDeg()*2.*(rAng + fDetThetaWRad);
    }
    LOG(INFO) << "  N15: CMTheta1: " << fTheta1 << ", CMTheta2: " << fTheta2
                << ", average value: " << 0.5*(fTheta2-fTheta1) + fTheta1 << FairLogger::endl;

    // Target Ion
    fThetaTargetIon1 = 180. - 2.*(fDetPos + fDetThetaWidth);
    fThetaTargetIon2 = 180. - 2.*(fDetPos - fDetThetaWidth);
}

Double_t ERElasticScattering::GetProbability(Double_t dTheta, Int_t primOrTarIon)
{
    Double_t detPosCM;
    if (primOrTarIon == 1) // for N15 == 1
        detPosCM = 0.5*(fTheta1+fTheta2);
    else                   // for B11
        detPosCM = 0.5*(fThetaTargetIon1+fThetaTargetIon2);
    Double_t CDF1 = fThetaCDF->Eval(detPosCM - dTheta);
    Double_t CDF2 = fThetaCDF->Eval(detPosCM + dTheta);
    return (CDF2 - CDF1);
}

ClassImp(ERElasticScattering)

void ERElasticScattering::Write_Cur_Theta(Double_t th)
{
    TString outFileName;
    outFileName.Form("mc_learning/input/cur_theta_%d.txt", fRanIndex);
    std::ofstream fout(outFileName);
    fout << th*TMath::RadToDeg() << std::endl;
    fout.close();
}
