// METMakerAlg.cxx

#include "METMakerAlg.h"
#include "METInterface/IMETMaker.h"

#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingETComposition.h"
#include "xAODMissingET/MissingETAssociationMap.h"

#include "xAODJet/JetContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJetContainer.h"

#include "MuonSelectorTools/IMuonSelectionTool.h"
#include "ElectronPhotonSelectorTools/IAsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/IAsgPhotonIsEMSelector.h"
// #include "TauAnalysisTools/ITauSelectionTool.h"

using std::string;
using namespace xAOD;

namespace met {

  //**********************************************************************

  METMakerAlg::METMakerAlg(const std::string& name,
			   ISvcLocator* pSvcLocator )
    : ::AthAlgorithm( name, pSvcLocator ),
    m_muonSelTool(""),
    m_elecSelLHTool(""),
    m_photonSelIsEMTool("")
    // m_tauSelTool("")
 {
    declareProperty( "Maker",          m_metmaker                        );
    declareProperty( "METMapName",     m_mapname   = "METAssoc"          );
    declareProperty( "METCoreName",    m_corename  = "MET_Core"          );
    declareProperty( "METName",        m_outname   = "MET_Reference"     );

    declareProperty( "METSoftClName",  m_softclname  = "SoftClus"        );
    declareProperty( "METSoftTrkName", m_softtrkname = "PVSoftTrk"       );

    declareProperty( "InputJets",      m_jetColl   = "AntiKt4LCTopoJets" );
    declareProperty( "InputElectrons", m_eleColl   = "Electrons"         );
    declareProperty( "InputPhotons",   m_gammaColl = "Photons"           );
    declareProperty( "InputTaus",      m_tauColl   = "TauJets"           );
    declareProperty( "InputMuons",     m_muonColl  = "Muons"             );

    declareProperty( "MuonSelectionTool",        m_muonSelTool           );
    declareProperty( "ElectronLHSelectionTool",  m_elecSelLHTool         );
    declareProperty( "PhotonIsEMSelectionTool" , m_photonSelIsEMTool     );
    // declareProperty( "TauSelectionTool",         m_tauSelTool            );

    declareProperty( "DoTruthLeptons", m_doTruthLep = false              );

  }

  //**********************************************************************

  METMakerAlg::~METMakerAlg() { }

  //**********************************************************************

  StatusCode METMakerAlg::initialize() {
    ATH_MSG_INFO("Initializing " << name() << "...");
    ATH_MSG_INFO("Retrieving tools...");

    // retrieve tools
    if( m_metmaker.retrieve().isFailure() ) {
      ATH_MSG_ERROR("Failed to retrieve tool: " << m_metmaker->name());
      return StatusCode::FAILURE;
    };

    if( m_muonSelTool.retrieve().isFailure() ) {
      ATH_MSG_ERROR("Failed to retrieve tool: " << m_muonSelTool->name());
      return StatusCode::FAILURE;
    };

    if( m_elecSelLHTool.retrieve().isFailure() ) {
      ATH_MSG_ERROR("Failed to retrieve tool: " << m_elecSelLHTool->name());
      return StatusCode::FAILURE;
    };

    if( m_photonSelIsEMTool.retrieve().isFailure() ) {
      ATH_MSG_ERROR("Failed to retrieve tool: " << m_photonSelIsEMTool->name());
      return StatusCode::FAILURE;
    };

    // if( m_tauSelTool.retrieve().isFailure() ) {
    //   ATH_MSG_ERROR("Failed to retrieve tool: " << m_tauSelTool->name());
    //   return StatusCode::FAILURE;
    // };

    return StatusCode::SUCCESS;
  }

  //**********************************************************************

  StatusCode METMakerAlg::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
  }

  //**********************************************************************

  StatusCode METMakerAlg::execute() {
    ATH_MSG_VERBOSE("Executing " << name() << "...");

    // Create a MissingETContainer with its aux store
    MissingETContainer* newMet = new MissingETContainer();
    if( evtStore()->record(newMet, m_outname).isFailure() ) {
      ATH_MSG_WARNING("Unable to record MissingETContainer: " << m_outname);
      return StatusCode::SUCCESS;
    }
    MissingETAuxContainer* metAuxCont = new MissingETAuxContainer();
    if( evtStore()->record(metAuxCont, m_outname+"Aux.").isFailure() ) {
      ATH_MSG_WARNING("Unable to record MissingETAuxContainer: " << m_outname+"Aux.");
      return StatusCode::SUCCESS;
    }
    newMet->setStore(metAuxCont);

    const MissingETAssociationMap* metMap = 0;
    if( evtStore()->retrieve(metMap, m_mapname).isFailure() ) {
      ATH_MSG_WARNING("Unable to retrieve MissingETAssociationMap: " << m_mapname);
      return StatusCode::SUCCESS;
    }
    metMap->resetObjSelectionFlags();

    // Retrieve containers ***********************************************

    /// MET
    const MissingETContainer* coreMet(0);
    if( evtStore()->retrieve(coreMet, m_corename).isFailure() ) {
      ATH_MSG_WARNING("Unable to retrieve MissingETContainer: " << m_corename);
      return StatusCode::SUCCESS;
    }

    /// Jets
    const JetContainer* jetCont(0);
    if( evtStore()->retrieve(jetCont, m_jetColl).isFailure() ) {
      ATH_MSG_WARNING("Unable to retrieve input jet container: " << m_jetColl);
      return StatusCode::SUCCESS;
    }
    ATH_MSG_DEBUG("Successfully retrieved jet collection");

    /// Electrons
    const ElectronContainer* elCont(0);
    if(!m_eleColl.empty()) {
      if( evtStore()->retrieve(elCont, m_eleColl).isFailure() ) {
	ATH_MSG_WARNING("Unable to retrieve input electron container: " << m_eleColl);
	return StatusCode::SUCCESS;
      }
      ATH_MSG_DEBUG("Successfully retrieved electron collection");
    }

    /// Photons
    const PhotonContainer* phCont(0);
    if(!m_gammaColl.empty()) {
      if( evtStore()->retrieve(phCont, m_gammaColl).isFailure() ) {
	ATH_MSG_WARNING("Unable to retrieve input photon container: " << m_gammaColl);
	return StatusCode::SUCCESS;
      }
      ATH_MSG_DEBUG("Successfully retrieved photon collection");
    }

    /// Taus
    const TauJetContainer* tauCont(0);
    if(!m_tauColl.empty()) {
      if( evtStore()->retrieve(tauCont, m_tauColl).isFailure() ) {
	ATH_MSG_WARNING("Unable to retrieve input tau container: " << m_tauColl);
	return StatusCode::SUCCESS;
      }
      ATH_MSG_DEBUG("Successfully retrieved tau collection");
    }

    /// Muons
    const MuonContainer* muonCont(0);
    m_muonColl="";
    if(!m_muonColl.empty()) {
      if( evtStore()->retrieve(muonCont, m_muonColl).isFailure() ) {
	ATH_MSG_WARNING("Unable to retrieve input muon container: " << m_muonColl);
	return StatusCode::SUCCESS;
      }
      ATH_MSG_DEBUG("Successfully retrieved muon collection");
    }

    std::vector<const xAOD::IParticle*> uniques;
    // Select and flag objects for final MET building ***************************

    MissingETBase::UsageHandler::Policy objScale = MissingETBase::UsageHandler::PhysicsObject;
    if(m_doTruthLep) objScale = MissingETBase::UsageHandler::TruthParticle;
    // Electrons
    if(!m_eleColl.empty()) {
      uniques.clear();
      ConstDataVector<ElectronContainer> metElectrons(SG::VIEW_ELEMENTS);
      for(const auto& el : *elCont) {
    	if(accept(el)) {
    	  metElectrons.push_back(el);
    	}
      }
      if( m_metmaker->rebuildMET("RefEle", xAOD::Type::Electron, newMet,
    				 metElectrons.asDataVector(),
    				 metMap, uniques, objScale).isFailure() ) {
    	ATH_MSG_WARNING("Failed to build electron term.");
      }
      ATH_MSG_DEBUG("Selected " << metElectrons.size() << " MET electrons. "
    		    << uniques.size() << " are non-overlapping.");
    }

    // Photons
    if(!m_gammaColl.empty()) {
      uniques.clear();
      ConstDataVector<PhotonContainer> metPhotons(SG::VIEW_ELEMENTS);
      for(const auto& ph : *phCont) {
    	if(accept(ph)) {
    	  metPhotons.push_back(ph);
    	}
      }
      if( m_metmaker->rebuildMET("RefGamma", xAOD::Type::Photon, newMet,
    				 metPhotons.asDataVector(),
    				 metMap, uniques, objScale).isFailure() ) {
    	ATH_MSG_WARNING("Failed to build photon term.");
      }
      ATH_MSG_DEBUG("Selected " << metPhotons.size() << " MET photons. "
    		    << uniques.size() << " are non-overlapping.");
    }

    // Taus
    if(!m_tauColl.empty()) {
      uniques.clear();
      ConstDataVector<TauJetContainer> metTaus(SG::VIEW_ELEMENTS);
      for(const auto& tau : *tauCont) {
    	if(accept(tau)) {
    	  metTaus.push_back(tau);
    	}
      }
      if( m_metmaker->rebuildMET("RefTau", xAOD::Type::Tau, newMet,
    				 metTaus.asDataVector(),
    				 metMap, uniques, objScale).isFailure() ){
    	ATH_MSG_WARNING("Failed to build tau term.");
      }
      ATH_MSG_DEBUG("Selected " << metTaus.size() << " MET taus. "
    		    << uniques.size() << " are non-overlapping.");
    }

    // Muons
    if(!m_muonColl.empty()) {
      uniques.clear();
      ConstDataVector<MuonContainer> metMuons(SG::VIEW_ELEMENTS);
      for(const auto& mu : *muonCont) {
    	if(accept(mu)) {
    	  metMuons.push_back(mu);
    	}
      }
      
      if(m_doTruthLep) objScale = MissingETBase::UsageHandler::OnlyTrack;
      if( m_metmaker->rebuildMET("Muons", xAOD::Type::Muon, newMet,
    				 metMuons.asDataVector(),
    				 metMap, uniques, objScale).isFailure() ) {
    	ATH_MSG_WARNING("Failed to build muon term.");
      }
      ATH_MSG_DEBUG("Selected " << metMuons.size() << " MET muons. "
    		    << uniques.size() << " are non-overlapping.");
    }

    if( m_metmaker->rebuildJetMET("RefJet", m_softclname, m_softtrkname, newMet,
				  jetCont, coreMet, metMap, false, uniques ).isFailure() ) {
      ATH_MSG_WARNING("Failed to build jet and soft terms.");
    }
    ATH_MSG_DEBUG("Of " << jetCont->size() << " jets, "
		  << uniques.size() << " are non-overlapping.");

    if( m_metmaker->buildMETSum("FinalTrk", newMet, (*newMet)[m_softtrkname] ? (*newMet)[m_softtrkname]->source() : MissingETBase::Source::Track).isFailure() ){
      ATH_MSG_WARNING("Building MET FinalTrk sum failed.");
    }
    if( m_metmaker->buildMETSum("FinalClus", newMet, (*newMet)[m_softtrkname] ? (*newMet)[m_softclname]->source() : MissingETBase::Source::LCTopo).isFailure() ) {
      ATH_MSG_WARNING("Building MET FinalClus sum failed.");
    }

    return StatusCode::SUCCESS;
  }

  //**********************************************************************

  bool METMakerAlg::accept(const xAOD::Muon* mu)
  {
    if( mu->pt()<2.5e3 || mu->pt()/cosh(mu->eta())<4e3 ) return false;
    return m_muonSelTool->accept(*mu);
  }

  bool METMakerAlg::accept(const xAOD::Electron* el)
  {
    if( fabs(el->eta())>2.47 || el->pt()<10e3 ) return false;
    return m_elecSelLHTool->accept(*el);
  }

  bool METMakerAlg::accept(const xAOD::Photon* ph)
  {
    if( !(ph->author()&20) || fabs(ph->eta())>2.47 || ph->pt()<10e3 ) return false;
    return m_photonSelIsEMTool->accept(ph);
  }

  bool METMakerAlg::accept(const xAOD::TauJet* tau)
  {
    if(tau->pt()<20e3 || fabs(tau->eta())>2.5) return false;
    // need to accommodate more than one of these?
    if(!tau->isTau( xAOD::TauJetParameters::IsTauFlag(xAOD::TauJetParameters::JetBDTSigMedium) )) return false;
    if(tau->isTau( xAOD::TauJetParameters::IsTauFlag(xAOD::TauJetParameters::EleBDTMedium) )) return false;
    if(tau->isTau( xAOD::TauJetParameters::IsTauFlag(xAOD::TauJetParameters::MuonVeto) )) return false;

    return true;
  }
  //  { return m_tauSelTool->accept( *tau ); }

}
