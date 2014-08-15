/*
 * AssociationGraph.h
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#ifndef ASSOCIATIONGRAPH_H_
#define ASSOCIATIONGRAPH_H_
#include <vnl/vnl_sparse_matrix.h>
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

namespace ttt {
template <class ObservationType, class TrackType> class Mitosis{
public:
	ObservationType A;
	ObservationType B;
	TrackType track;
	bool operator<(const Mitosis & other ) const{
		if(track<other.track){
			return true;
		}else if(track>other.track){
			return false;
		}else{
			if(A<other.A){
				return true;
			}else if(A>other.A){
				return false;
			}else{
				if(B<other.B){
					return true;
				}else{
					return false;
				}
			}
		}
	}
	Mitosis(){

	}
	Mitosis(const Mitosis & other){
		A=other.A;
		B=other.B;
		track=other.track;
	}
};
template<class ObservationType,class TrackType> class AssociationGraph{
public:
	typedef Mitosis<ObservationType,TrackType> MitosisType;
	typedef boost::adjacency_list_traits < boost::vecS, boost::vecS, boost::directedS > Traits;

	typedef boost::property <
			boost::edge_capacity_t,
			double,
			boost::property <
				boost::edge_weight_t,
				double
				>
		> FlowGraphEdgePropertyType;

	typedef boost::adjacency_list <
					boost::vecS,
					boost::vecS,
					boost::bidirectionalS,
					boost::no_property,
					FlowGraphEdgePropertyType
				> FlowGraphType;


	typedef boost::property_map < FlowGraphType, boost::edge_weight_t >::type Weight;
	typedef boost::graph_traits<FlowGraphType>::vertices_size_type size_type;
	typedef boost::graph_traits<FlowGraphType>::vertex_descriptor FlowGraphVertexType;
	typedef boost::graph_traits<FlowGraphType>::edge_descriptor FlowGraphEdgeType;



public:

	AssociationGraph(){


		m_Tp=boost::add_vertex(m_FlowGraph);
		m_Tn=boost::add_vertex(m_FlowGraph);
		m_A=boost::add_vertex(m_FlowGraph);
		m_D=boost::add_vertex(m_FlowGraph);

		bool result;
		boost::tie(m_TpAEdge,result)=boost::add_edge(m_Tp,m_A,FlowGraphEdgePropertyType(),m_FlowGraph);
		assert(result);

		boost::get(boost::edge_weight,m_FlowGraph,m_TpAEdge)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,m_TpAEdge)=0;


		boost::tie(m_DTnEdge,result)=boost::add_edge(m_D,m_Tn,FlowGraphEdgePropertyType(),m_FlowGraph);
		assert(result);

		boost::get(boost::edge_weight,m_FlowGraph,m_DTnEdge)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,m_DTnEdge)=0;


		boost::tie(m_ADEdge,result)=boost::add_edge(m_A,m_D,FlowGraphEdgePropertyType(),m_FlowGraph);

		boost::get(boost::edge_weight,m_FlowGraph,m_ADEdge)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,m_ADEdge)=0;



		m_L.clear();
		m_R.clear();
		m_S.clear();

		m_NumTracks=0;
		m_NumObs=0;
		m_NumMythosysHypothesis=0;
		m_NumAssociationHypothesis=0;
		m_NumTerminationHypothesis=0;
		m_NumCreationHypothesis=0;
		m_NumApoptosisHypothesis=0;

	}

	void AddTrack(const ObservationType & track){
		FlowGraphVertexType l=boost::add_vertex(m_FlowGraph);
		m_L.insert(TrackAndFlowGraphVertexType(track,l));
		//m_L.left[track]=l;


		FlowGraphEdgeType  edge = boost::add_edge(m_Tp,l,FlowGraphEdgePropertyType(),m_FlowGraph).first;
		boost::get(boost::edge_weight,m_FlowGraph,edge)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,edge)=1;
		m_TpL.push_back(edge);

		boost::get(boost::edge_capacity,m_FlowGraph,m_ADEdge)++;
		boost::get(boost::edge_capacity,m_FlowGraph,m_DTnEdge)++;

		m_NumTracks++;
	}

	void AddObservation(const ObservationType & observation){
		FlowGraphVertexType r=boost::add_vertex(m_FlowGraph);
		m_R.insert(ObservationAndFlowGraphVertexType(observation,r));
		//m_R.left[observation]=r;

		FlowGraphEdgeType  edge = boost::add_edge(r,m_Tn,FlowGraphEdgePropertyType(),m_FlowGraph).first;
		boost::get(boost::edge_weight,m_FlowGraph,edge)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,edge)=1;
		m_RTn.push_back(edge);

		boost::get(boost::edge_capacity,m_FlowGraph,m_TpAEdge)++;
		m_NumObs++;
	}

	void AddAssociationHypothesis(const TrackType & track,const ObservationType & observation, double cost){

		//TrackAndFlowGraphVertexBimapType::left_map & m_Lleft = m_L.left;
		FlowGraphEdgeType edge=		boost::add_edge(m_L.left.at(track),m_R.left.at(observation),FlowGraphEdgePropertyType(),m_FlowGraph).first; //TODO WTF!
		boost::get(boost::edge_weight,m_FlowGraph,edge)=cost;
		boost::get(boost::edge_capacity,m_FlowGraph,edge)=1;
		m_LR.push_back(edge);

		m_NumAssociationHypothesis++;
	}

	void AddMitosisHypothesis(const TrackType & track, const ObservationType & cellA,const ObservationType & cellB, double cost){

		MitosisType hypothesis;
		hypothesis.track=track;
		if(cellA < cellB){
			hypothesis.A=cellA;
			hypothesis.B=cellB;
		}else{
			hypothesis.B=cellA;
			hypothesis.A=cellB;
		}

		if(m_S.left.find(hypothesis)!=m_S.left.end()) return;
		FlowGraphVertexType s=boost::add_vertex(m_FlowGraph);

		m_S.insert(MitosisAndFlowGraphVertexType(hypothesis,s));
		//m_S.left.[hypothesis]=s;

		FlowGraphEdgeType edgeLS=boost::add_edge(m_L.left.at(track),s,FlowGraphEdgePropertyType(),m_FlowGraph).first;

		boost::get(boost::edge_weight,m_FlowGraph,edgeLS)=cost;

		boost::get(boost::edge_capacity,m_FlowGraph,edgeLS)=1;

		FlowGraphEdgeType edgeAS=boost::add_edge(m_A,s,FlowGraphEdgePropertyType(),m_FlowGraph).first;

		boost::get(boost::edge_weight,m_FlowGraph,edgeAS)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,edgeAS)=1;

		FlowGraphEdgeType edgeSRA =boost::add_edge(s,m_R.left.at(hypothesis.A),FlowGraphEdgePropertyType(),m_FlowGraph).first;

		boost::get(boost::edge_weight,m_FlowGraph,edgeSRA)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,edgeSRA)=1;

		FlowGraphEdgeType edgeSRB =boost::add_edge(s,m_R.left.at(hypothesis.B),FlowGraphEdgePropertyType(),m_FlowGraph).first;
		boost::get(boost::edge_weight,m_FlowGraph,edgeSRB)=0;
		boost::get(boost::edge_capacity,m_FlowGraph,edgeSRB)=1;

		m_NumMythosysHypothesis++;

	}

	void AddCreationHypothesis(const ObservationType & obs,double cost){
		FlowGraphEdgeType edge = boost::add_edge(m_A,m_R.left.at(obs),FlowGraphEdgePropertyType(),m_FlowGraph).first;
		boost::get(boost::edge_weight,m_FlowGraph,edge)=cost;
		boost::get(boost::edge_capacity,m_FlowGraph,edge)=1;
		m_AR.push_back(edge);

		m_NumCreationHypothesis++;
	}

	void AddTerminationHypothesis(const TrackType & track,double cost){
		FlowGraphEdgeType edge=boost::add_edge(m_L.left.at(track),m_D,FlowGraphEdgePropertyType(),m_FlowGraph).first;
		boost::get(boost::edge_weight,m_FlowGraph,edge)=cost;
		boost::get(boost::edge_capacity,m_FlowGraph,edge)=1;
		m_LD.push_back(edge);

		m_NumTerminationHypothesis++;
	}

	void AddApoptosisHypothesis(const TrackType & track,double cost){
		//TODO


	}

	void GetProblem(vnl_vector<double> & costVector, vnl_vector<double> & flowVector, vnl_vector<double> & capacityVector, vnl_sparse_matrix<double> & flowMatrix){
		int nRows=1+ m_NumTracks + 1 + m_NumObs +1 +1; // T- + numTrakcs + A + numObs + D + T+
		int nCols=(m_NumTracks+1) + m_NumAssociationHypothesis + m_NumMythosysHypothesis + m_NumTerminationHypothesis +  m_NumCreationHypothesis + m_NumApoptosisHypothesis + (m_NumObs +1) +1; //AD-HYPOTHESIS

		flowMatrix.clear();
		flowMatrix.resize(nRows,nCols);
		capacityVector.clear();
		costVector.clear();
		flowVector.clear();
		assert(flowVector.set_size(nRows));
		assert(costVector.set_size(nCols));
		assert(capacityVector.set_size(nCols));
		int edge=0;

#define PRINT_ROW(r) for(int c=0;c<flowMatrix.cols();c++){ std::cout << flowMatrix.get(r,c) << "\t";  } std::cout << std::endl;
#define PRINT_MATRIX 		{std::cout << "===============================" << std::endl;std::cout << "Tp\t";PRINT_ROW(m_Tp);std::cout << "A\t";PRINT_ROW(m_A);int k=0;for(typename std::map<TrackType,FlowGraphVertexType>::iterator it= m_L.begin(); it!= m_L.end();++it){ std::cout << "L"<<k++<<"\t";PRINT_ROW(it->second);}k=0; for(typename std::map<TrackType,FlowGraphVertexType>::iterator it= m_R.begin(); it!= m_R.end();++it){ std::cout << "R"<<k++<<"\t";	PRINT_ROW(it->second);}	std::cout << "D\t";PRINT_ROW(m_D);std::cout << "Tn\t"; PRINT_ROW(m_Tn); std::cout << "==============================="<< std::endl;}

		//flowMatrix.put(m_Tp,edge,-boost::get(boost::edge_capacity,m_FlowGraph,m_TpAEdge));
		flowMatrix.put(m_Tp,edge,-1);
		//PRINT_ROW(m_Tp);
		//PRINT_MATRIX
		//flowMatrix.put(m_A,edge,boost::get(boost::edge_capacity,m_FlowGraph,m_TpAEdge));
		flowMatrix.put(m_A,edge,1);

		//PRINT_MATRIX
		costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,m_TpAEdge));
		capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,m_TpAEdge));
		//costVector[edge]=;
		edge++;

		for(FlowGraphEdgeContainerType::iterator it= m_TpL.begin();it!=m_TpL.end();++it){
			//flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-1);
//			flowMatrix.put(boost::target(*it,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::target(*it,m_FlowGraph),edge,+1);
			//costVector[edge]=;
			costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,*it));
			capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			//PRINT_MATRIX

			edge++;
		}
		for(FlowGraphEdgeContainerType::iterator it= m_AR.begin();it!=m_AR.end();++it){

			//flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-1);

			//flowMatrix.put(boost::target(*it,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::target(*it,m_FlowGraph),edge,+1);
			//costVector[edge]=;
			costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,*it));
			capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));

			//PRINT_MATRIX
			m_Creations[edge]=*it;
			edge++;
		}

		for(FlowGraphEdgeContainerType::iterator it= m_LR.begin();it!=m_LR.end();++it){
			//std::cout << boost::source(*it,m_FlowGraph) << " " << m_Tp << std::endl;
			//flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-1);
			//PRINT_MATRIX
			//flowMatrix.put(boost::target(*it,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::target(*it,m_FlowGraph),edge,1);
			//PRINT_MATRIX
			//costVector[edge]=boost::get(boost::edge_weight,m_FlowGraph,*it);
			costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,*it));
			capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			m_Associations[edge]=*it;

			edge++;
		}


		for(FlowGraphEdgeContainerType::iterator it= m_LD.begin();it!=m_LD.end();++it){
			//flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-1);
			//flowMatrix.put(boost::target(*it,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::target(*it,m_FlowGraph),edge,1);

			costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,*it));
			capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			//costVector[edge]=;
			//PRINT_MATRIX
			m_Deletions[edge]=*it;

			edge++;
		}

		//flowMatrix.put(m_A,edge,-boost::get(boost::edge_capacity,m_FlowGraph,m_ADEdge));
		flowMatrix.put(m_A,edge,-1);
		flowMatrix.put(m_D,edge,+1);
		costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,m_ADEdge));
		capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,m_ADEdge));

		edge++;
		for(FlowGraphEdgeContainerType::iterator it= m_RTn.begin();it!=m_RTn.end();++it){
			//flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::source(*it,m_FlowGraph),edge,-1);
			//flowMatrix.put(boost::target(*it,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			flowMatrix.put(boost::target(*it,m_FlowGraph),edge,1);
			//PRINT_MATRIX
			//costVector[edge]=boost::get(boost::edge_weight,m_FlowGraph,*it);
			costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,*it));
			capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,*it));
			edge++;
		}

		//flowMatrix.put(m_D,edge,-boost::get(boost::edge_capacity,m_FlowGraph,m_DTnEdge));
		flowMatrix.put(m_D,edge,-1);
		//flowMatrix.put(m_Tn,edge,boost::get(boost::edge_capacity,m_FlowGraph,m_DTnEdge));
		flowMatrix.put(m_Tn,edge,1);
		costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,m_DTnEdge));
		capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,m_DTnEdge));

		edge++;

		for(typename MitosisAndFlowGraphVertexBimapType::const_iterator it = m_S.begin();it!=m_S.end();++it){
			{
			boost::graph_traits<FlowGraphType>::in_edge_iterator itIn,itIn_end;
			assert(boost::in_degree(it->right,m_FlowGraph)==2);
			boost::tie(itIn,itIn_end)=boost::in_edges(it->right,m_FlowGraph);

			while(itIn!=itIn_end){
				//flowMatrix.put(boost::source(*itIn,m_FlowGraph),edge,-boost::get(boost::edge_capacity,m_FlowGraph,*itIn));
				flowMatrix.put(boost::source(*itIn,m_FlowGraph),edge,-1);
				if(boost::source(*itIn,m_FlowGraph)!=m_A){
					costVector.put(edge,boost::get(boost::edge_weight,m_FlowGraph,*itIn));
					capacityVector.put(edge,boost::get(boost::edge_capacity,m_FlowGraph,*itIn));

					m_Mythosis[edge]=*itIn;
				}
				++itIn;
			}
			}

			{
			boost::graph_traits<FlowGraphType>::out_edge_iterator itOut,itOut_end;
			assert(boost::out_degree(it->right,m_FlowGraph)==2);
			boost::tie(itOut,itOut_end)=boost::out_edges(it->right,m_FlowGraph);

			while(itOut!=itOut_end){
				//flowMatrix.put(boost::target(*itOut,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*itOut));
				//flowMatrix.put(boost::target(*itOut,m_FlowGraph),edge,boost::get(boost::edge_capacity,m_FlowGraph,*itOut));
				flowMatrix.put(boost::target(*itOut,m_FlowGraph),edge,1);
				++itOut;
			}
			}
			capacityVector.put(edge,1);
			edge++;
		}

		flowVector.fill(0);


		flowVector[m_Tp]=-(m_NumObs+m_NumTracks);
		flowVector[m_Tn]=(m_NumObs+m_NumTracks);

		//PRINT_MATRIX

	}

	void DecodeSolution(
			const vnl_vector<double> & solution,
			std::vector<std::pair<ObservationType,TrackType> > & associations,
			std::vector<ObservationType> & toCreate,
			std::vector<TrackType>  & toDelete,
			std::vector<MitosisType>  & mitosisHypothesis
			){
		associations.clear();
		toCreate.clear();
		toDelete.clear();
		int flowSent=0;
		for(int c=0;c<solution.size();c++){
			if(solution.get(c)>0){
				flowSent++;
				if(m_Associations.find(c)!=m_Associations.end()){
					FlowGraphEdgeType edge=m_Associations[c];

					FlowGraphVertexType track=boost::source(edge,m_FlowGraph);
					FlowGraphVertexType obs=boost::target(edge,m_FlowGraph);

					associations.push_back(std::pair<ObservationType,TrackType>(m_R.right.at(obs),m_L.right.at(track)));

				}else if(m_Deletions.find(c)!=m_Deletions.end()){
					FlowGraphEdgeType edge=m_Deletions[c];

					FlowGraphVertexType track=boost::source(edge,m_FlowGraph);
					toDelete.push_back(m_L.right.at(track));

				}else if(m_Creations.find(c)!=m_Creations.end()){
					FlowGraphEdgeType edge=m_Creations[c];

					FlowGraphVertexType obs =boost::target(edge,m_FlowGraph);
					toCreate.push_back(m_R.right.at(obs));
				}else if(m_Mythosis.find(c)!=m_Mythosis.end()){
					FlowGraphEdgeType edge=m_Mythosis[c];
					FlowGraphVertexType s =boost::target(edge,m_FlowGraph);

					MitosisType mitosis;
					FlowGraphVertexType l= boost::source(edge,m_FlowGraph);
					assert(l!=m_A);
					mitosis.track=m_L.right.at(l);

					assert(boost::out_degree(s,m_FlowGraph)==2);
					boost::graph_traits<FlowGraphType>::out_edge_iterator itOut,itOut_end;

					boost::tie(itOut,itOut_end)=boost::out_edges(s,m_FlowGraph);
					FlowGraphVertexType rA=boost::target(*itOut,m_FlowGraph);
					++itOut;
					FlowGraphVertexType rB=boost::target(*itOut,m_FlowGraph);
					mitosis.A=m_R.right.at(rA);
					mitosis.B=m_R.right.at(rB);
					mitosisHypothesis.push_back(mitosis);
				}else{

				}


			}
		}
		std::cout << "Sent " << flowSent << std::endl;
	}
	void Print(){
		std::cout<< "Num Tracks: " << m_NumTracks << std::endl;
		std::cout<< "Num Obs: " << m_NumObs << std::endl;
		std::cout<< "Num Mythosys: " << m_NumMythosysHypothesis << std::endl;
		std::cout<< "Num Association: " << m_NumAssociationHypothesis << std::endl;
		std::cout<< "Num Termination: " << m_NumTerminationHypothesis << std::endl;
		std::cout<< "Num Creation: " << m_NumCreationHypothesis << std::endl;
		std::cout<< "Num Apoptosis: " << m_NumApoptosisHypothesis << std::endl;

		std::cout << "AD capacity " << boost::get(boost::edge_capacity,m_FlowGraph,m_ADEdge) << std::endl;
	}
private:
	FlowGraphType m_FlowGraph;

	FlowGraphVertexType m_Tn;
	FlowGraphVertexType m_Tp;

	FlowGraphVertexType m_A;
	FlowGraphVertexType m_D;


	FlowGraphEdgeType m_TpAEdge;
	FlowGraphEdgeType m_DTnEdge;
	FlowGraphEdgeType m_ADEdge;

	typedef std::vector<FlowGraphEdgeType> FlowGraphEdgeContainerType;
	FlowGraphEdgeContainerType m_TpL;
	FlowGraphEdgeContainerType m_RTn;
	FlowGraphEdgeContainerType m_LR;
	FlowGraphEdgeContainerType m_LD;
	FlowGraphEdgeContainerType m_AR;

	int m_NumTracks;
	int m_NumObs;
	int m_NumMythosysHypothesis;
	int m_NumAssociationHypothesis;
	int m_NumTerminationHypothesis;
	int m_NumCreationHypothesis;
	int m_NumApoptosisHypothesis;


	std::map<int,FlowGraphEdgeType> m_Creations;
	std::map<int,FlowGraphEdgeType> m_Deletions;
	std::map<int,FlowGraphEdgeType> m_Associations;
	std::map<int,FlowGraphEdgeType> m_Mythosis;

	//std::map<TrackType,FlowGraphVertexType> m_L;
	typedef boost::bimaps::bimap< boost::bimaps::set_of<TrackType> , boost::bimaps::set_of<FlowGraphVertexType> > TrackAndFlowGraphVertexBimapType;
	typedef boost::bimaps::bimap< boost::bimaps::set_of<ObservationType>, boost::bimaps::set_of<FlowGraphVertexType> > ObservationAndFlowGraphVertexBimapType;

	typedef typename TrackAndFlowGraphVertexBimapType::value_type TrackAndFlowGraphVertexType;
	typedef typename ObservationAndFlowGraphVertexBimapType::value_type ObservationAndFlowGraphVertexType;

	TrackAndFlowGraphVertexBimapType m_L;
	ObservationAndFlowGraphVertexBimapType m_R;

	typedef boost::bimaps::bimap< boost::bimaps::set_of< MitosisType >, boost::bimaps::set_of<FlowGraphVertexType> > MitosisAndFlowGraphVertexBimapType;
	typedef typename MitosisAndFlowGraphVertexBimapType::value_type MitosisAndFlowGraphVertexType;
	//typedef std::map<Mitosis,FlowGraphVertexType> MitosisHypothesisToFlowGraphVertexMapType;
	MitosisAndFlowGraphVertexBimapType m_S;

};
}
#endif /* ASSOCIATIONGRAPH_H_ */
