#ifndef RecoAlgos_RobertoSelector_h
#define RecoAlgos_RobertoSelector_h
/** \class RobertoSelector
 *
 * selects a subset of a collection. 
 * 
 * \author Luca Lista, INFN
 *
 * \version $Revision: 1.1 $
 *
 * $Id: RobertoSelector.h,v 1.1 2007/11/28 14:16:27 govoni Exp $
 *
 */

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "PhysicsTools/UtilAlgos/interface/ParameterAdapter.h"
#include "PhysicsTools/UtilAlgos/interface/NonNullNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/StoreManagerTrait.h"
#include "PhysicsTools/UtilAlgos/interface/SelectedOutputCollectionTrait.h"
#include "PhysicsTools/UtilAlgos/interface/NullPostProcessor.h"
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>

template<typename Selector, 
         typename OutputCollection = typename helper::SelectedOutputCollectionTrait<typename Selector::collection>::type,
	 typename SizeSelector = NonNullNumberSelector,
	 typename PostProcessor = helper::NullPostProcessor<OutputCollection>,
	 typename StoreManager = typename helper::StoreManagerTrait<OutputCollection>::type,
	 typename Base = typename helper::StoreManagerTrait<OutputCollection>::base
	 >
class RobertoSelector : public Base {
public:
  /// constructor 
  explicit RobertoSelector( const edm::ParameterSet & cfg ) :
    Base( cfg ),
    src_( cfg.template getParameter<edm::InputTag>( "src" ) ),
    filter_( false ),
    selector_( cfg ),
    sizeSelector_( reco::modules::make<SizeSelector>( cfg ) ),
    postProcessor_( cfg ) {
    const std::string filter( "filter" );
    std::vector<std::string> bools = cfg.template getParameterNamesForType<bool>();
    bool found = std::find( bools.begin(), bools.end(), filter ) != bools.end();
    if ( found ) filter_ = cfg.template getParameter<bool>( filter );
    postProcessor_.init( * this );
   }
  /// destructor
  virtual ~RobertoSelector() { }
  
private:
  /// process one event
  bool filter( edm::Event& evt, const edm::EventSetup& evtSetup ) {
    using namespace std;
    edm::Handle<typename Selector::collection> source;
    evt.getByLabel( src_, source );
    StoreManager manager (source) ;
    selector_.select( source, evt, evtSetup );
    manager.cloneAndStore( selector_.begin(), selector_.end(), evt );
    bool result = ( ! filter_ || sizeSelector_( manager.size() ) );
    edm::OrphanHandle<OutputCollection> filtered = manager.put( evt );
    postProcessor_.process( filtered, evt );
    return result;
  }
  /// source collection label
  edm::InputTag src_;
  /// filter event
  bool filter_;
  /// Object collection selector
  Selector selector_;
  /// selected object collection size selector
  SizeSelector sizeSelector_;
  /// post processor
  PostProcessor postProcessor_;
};

#endif
