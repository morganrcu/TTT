/*
 * VinodthSegmentationCommand.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: morgan
 */

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <itkLineIterator.h>

#include "AdherensJunctionSegmentationCommand.h"


void ttt::AdherensJunctionSegmentationCommand::InitDefGraph() {
	m_Descriptor->m_SkeletonGraph= boost::shared_ptr<ttt::SkeletonGraph>(new ttt::SkeletonGraph);

	for (typename ttt::AdherensJunctionVertices<3>::iterator it = m_Locations->begin();it != m_Locations->end(); it++) {
		itk::Point<float, 3> pos;

		for (int i = 0; i < 3; i++) {
			pos[i] = (*it)->GetPosition()[i];
		}
		ttt::SkeletonPoint pt = ttt::SkeletonPoint(pos);
		boost::add_vertex(pt,*(m_Descriptor->m_SkeletonGraph));
	}
}

bool ttt::AdherensJunctionSegmentationCommand::IntersectLine(itk::Point<float ,3> l1posA, itk::Point<float, 3> l1posB, itk::Point<float, 3> l2posA, itk::Point<float, 3> l2posB ){


    double x1 = l1posA[0];
    double x2 = l1posB[0];
    double x3 = l2posA[0];
    double x4 = l2posB[0];

    double y1 = l1posA[1];
    double y2 = l1posB[1];
    double y3 = l2posA[1];
    double y4 = l2posB[1];

    //std::cout<< "x1: " << x1 << "x2: " << x2 << "x3: " << x3 << "x4: " << x4 <<std::endl;
    //std::cout<< "y1: " << y1 << "y2: " << y2 << "y3: " << y3 << "y4: " << y4 <<std::endl;

    bool cross = false;

    double denom, numerA, numerB, uA, uB;


        denom = ((y4 - y3) * (x2 - x1)) - ((x4 - x3) * (y2 - y1));
        numerA = ((x4 - x3) * (y1 - y3)) - ((y4 - y3) * (x1 - x3));
        numerB = ((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3));

        if(denom == 0.0)
        {
            if(numerA == 0.0 && numerB == 0.0)
            {
                cross = false;
            }//if

        }//if
        else
        {
            float uA = numerA / denom;
            float uB = numerB / denom;

            if (uA > 0.0 && uA < 1.0 && uB > 0.0 && uB < 1.0)
            {
                cross = true;
            }//if

            else
            {
                cross = false;

            }//else
        }//else

        return cross;

    /*
    float c1 = (l1posA[0] - l2posA[0]) * (l2posB[1] - l2posA[0]) - (l1posA[1] - l2posA[1]) * (l2posB[0] - l2posA[0]);
    float c2 = (l1posB[0] - l2posA[0]) * (l2posB[1] - l2posA[1]) - (l1posB[1] - l2posA[1]) * (l2posB[0] - l2posA[0]);
    //float c3 = (l1posB[0] - l1posA[0]) * (l2posA[1] - l1posB[1]) - (l1posB[1] - l1posA[1]) * (l2posA[0] - l1posB[0]);
    //float c4 = (l1posB[0] - l1posA[0]) * (l2posB[1] - l1posB[1]) - (l1posB[1] - l1posA[1]) * (l2posB[0] - l1posB[0]);
    float c3 = (l2posA[0] - l1posA[0]) * (l1posB[1] - l1posA[0]) - (l2posA[1] - l1posA[1]) * (l1posB[0] - l1posA[0]);
    float c4 = (l2posB[0] - l1posA[0]) * (l1posB[1] - l1posA[1]) - (l2posB[1] - l1posA[1]) * (l1posB[0] - l1posA[0]);

    return((c1*c2 < 0) && (c3*c4 <0));*/
}

void ttt::AdherensJunctionSegmentationCommand::DoPrimalComputation() {

	typedef itk::Point<double, 3> itkpt;
	typedef double PixelType;
	bool cross;

	std::map<ttt::SkeletonVertexType, std::vector<ttt::SkeletonVertexType> > svtedgemap;
	std::vector<boost::tuple<ttt::SkeletonVertexType, ttt::SkeletonVertexType> > vecdeledges;



	std::cout << "num edges before primal " << boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;

	BGL_FORALL_VERTICES(v,*m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
			std::vector<ttt::SkeletonVertexType> vecvt;
			svtedgemap[v] = vecvt;
	}


	BGL_FORALL_VERTICES(source,*m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
		//ttt::SkeletonVertexType c1 = source;
		itkpt sourcePos = boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, source).position;

		std::vector<double> distances;
		std::map<double, ttt::SkeletonVertexType> distovtmap;

		BGL_FORALL_VERTICES(target, *m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
			if (target != source) {
				itkpt targetPos = boost::get(ttt::SkeletonPointPropertyTag(), *m_Descriptor->m_SkeletonGraph, target).position;
				double dist = sqrt(pow(sourcePos[0] - targetPos[0], 2) + pow(sourcePos[1] - targetPos[1], 2));//2D distances
				distances.push_back(dist);
				distovtmap[dist] = target;
			}
		}

		std::sort(distances.begin(), distances.end());

		for (int k = 0; k < m_K; k++) {

			ttt::SkeletonVertexType target = distovtmap[distances[k]];

			if(boost::edge(source,target,*m_Descriptor->m_SkeletonGraph).second) continue;

			itkpt targetPos = boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, target).position;

			typename PlatenessImageType::IndexType sourcePt;
			typename PlatenessImageType::IndexType targetPt;

			for (int j = 0; j < 3; j++) {
				sourcePt[j] = sourcePos[j];
				targetPt[j] = targetPos[j];
			}

			double intensepix = 0;
			double totalpix = 0;
			itk::LineIterator<PlatenessImageType> it1(m_Plateness, sourcePt, targetPt);
			it1.GoToBegin();

			while (!it1.IsAtEnd()) {
				typename PlatenessImageType::IndexType pixelIndex;
				PixelType pixel = it1.Get();
				intensepix+=pixel;

#if 0
				//std::cout<< "pix val: "<< pixel<<std::endl;
				if (pixel > .02) {
					intensepix++;
				}
#endif
				++it1;
				totalpix++;
			}

			double percent = (intensepix / totalpix);

			cross = false;

			if (percent > m_Threshold) {

				BGL_FORALL_EDGES(e, *m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
					ttt::SkeletonVertexType edgeSource = boost::source(e,*m_Descriptor->m_SkeletonGraph);

					ttt::SkeletonVertexType edgeTarget = boost::target(e,*m_Descriptor->m_SkeletonGraph);


					itkpt edgeSourcePos = boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, edgeSource).position;
					itkpt edgeTargetPos= boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, edgeTarget).position;

					cross = IntersectLine(sourcePos, targetPos, edgeSourcePos, edgeTargetPos);
					//std::cout << "cross: " << cross << std::endl;
					if (cross) {
						break;
					}

#if 0
					if (l2posA[0] == pos1[0] && l2posA[1] == pos1[1]&& l2posA[2] == pos1[2]) {
						std::vector<itkpt> linepts;
						linepts.push_back(l2posB);
						linepts.push_back(pos3);
						double ang = angleBetween2Lines(pos1, linepts) * 180.0/ 3.14159;
						std::cout << "angle is pos1 if " << ang << std::endl;
						if (ang < 45) {
							cross = true;
							break;
						}
					}else if (l2posB[0] == pos1[0] && l2posB[1] == pos1[1] && l2posB[2] == pos1[2]) {
						std::vector<itkpt> linepts;
						linepts.push_back(l2posA);
						linepts.push_back(pos3);
						double ang = angleBetween2Lines(pos1, linepts) * 180.0/ 3.1459;
						std::cout << "angle is pos1 else" << ang << std::endl;
						if (ang < 45) {
							cross = true;
							break;
						}
					}
#endif

				}
				if(!cross){
					boost::add_edge(source, target,*m_Descriptor->m_SkeletonGraph);

					SkeletonGraph::adjacency_iterator neigh,neigh_end;
					boost::tie(neigh,neigh_end)=boost::adjacent_vertices(source,*m_Descriptor->m_SkeletonGraph);

					for(;neigh!=neigh_end;++neigh){
						itkpt neighPos =  boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, *neigh).position;

						double ang = angleBetween2Lines(sourcePos, targetPos,neighPos)* 180.0 / 3.14159;

						if (ang < 45) {

							double distpos3 = distitkpt(sourcePos, targetPos);

							double distl2posB = distitkpt(targetPos, neighPos);
							if (distpos3 < distl2posB) {
								boost::tuple < ttt::SkeletonVertexType, ttt::SkeletonVertexType> delpair(source, *neigh);

								vecdeledges.push_back(delpair);

							} else {
								cross = true;
								break;
							}
						}
					}
					for (int i = 0; i < vecdeledges.size(); i++) {

						//boost::tuple < ttt::SkeletonVertexType_lS, ttt::SkeletonVertexType_lS > pts = vecdeledges[i];
						boost::tuple < ttt::SkeletonVertexType, ttt::SkeletonVertexType > pts = vecdeledges[i];
						//ttt::SkeletonVertexType_lS src = boost::get<0>(pts);
						ttt::SkeletonVertexType src = boost::get<0>(pts);
						//ttt::SkeletonVertexType_lS tgt = boost::get<1>(pts);
						ttt::SkeletonVertexType tgt = boost::get<1>(pts);
						std::cout << "removing " << src << " " << tgt << std::endl;
						//boost::remove_edge(src, tgt, m_Descriptor->m_SkeletonGraph_listS);
						boost::remove_edge(src, tgt, *m_Descriptor->m_SkeletonGraph);
					}
					boost::add_edge(source, target,*m_Descriptor->m_SkeletonGraph);
				}
#if 0
				if (!cross) {
					vecdeledges.clear();

					BGL_FORALL_EDGES(e, *m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){

						ttt::SkeletonVertexType src = boost::source(e,*m_Descriptor->m_SkeletonGraph);
						ttt::SkeletonVertexType tgt = boost::target(e,*m_Descriptor->m_SkeletonGraph);

						itkpt l2posA = boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, src).position;
						itkpt l2posB = boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, tgt).position;

						if (l2posA[0] == pos3[0] && l2posA[1] == pos3[1] && l2posA[2] == pos3[2]) {

							std::vector<itkpt> linepts;
							linepts.push_back(l2posB);
							linepts.push_back(pos1);

							double ang = angleBetween2Lines(pos3, linepts)* 180.0 / 3.14159;

							std::cout << "angle is pos3 if " << ang << std::endl;

							if (ang < 45) {
								double distpos3 = distitkpt(pos1, pos3);
								double distl2posB = distitkpt(pos3, l2posB);
								if (distpos3 < distl2posB) {
									//boost::remove_edge(src,tgt,descriptor->m_SkeletonGraph_listS);
									std::cout << "added 1 " << src << " " << tgt
											<< std::endl;
									//boost::tuple < ttt::SkeletonVertexType_lS, ttt::SkeletonVertexType_lS> delpair(src, tgt);
									boost::tuple < ttt::SkeletonVertexType, ttt::SkeletonVertexType> delpair(src, tgt);
									std::cout << boost::get<0>(delpair) << " " << boost::get<1>(delpair) << std::endl;

									vecdeledges.push_back(delpair);

								} else {
									cross = true;
									break;
								}
							}
						}else if (l2posB[0] == pos3[0] && l2posB[1] == pos3[1] 	&& l2posB[2] == pos3[2]) {
							std::vector<itkpt> linepts;
							linepts.push_back(l2posA);
							linepts.push_back(pos1);

							double ang = angleBetween2Lines(pos3, linepts) * 180.0 / 3.1459;
							std::cout << "angle is pos3 else " << ang
									<< std::endl;
							if (ang < 45) {
								double distpos3 = distitkpt(pos1, pos3);
								double distl2posA = distitkpt(pos3, l2posA);
								if (distpos3 < distl2posA) {
									//std::cout<<"calculated distances"<<std::endl;
									//boost::remove_edge(src,tgt,descriptor->m_SkeletonGraph_listS);
									//std::cout<<"removed edge"<<std::endl;
									std::cout << "added 2 " << src << " " << tgt
											<< std::endl;

									//boost::tuple < ttt::SkeletonVertexType_lS, ttt::SkeletonVertexType_lS > delpair(src, tgt);
									boost::tuple < ttt::SkeletonVertexType, ttt::SkeletonVertexType > delpair(src, tgt);
									std::cout << boost::get<0>(delpair) << " "
											<< boost::get<1>(delpair)
											<< std::endl;
									vecdeledges.push_back(delpair);
								} else {
									cross = true;
									break;
								}
							}

							//scanf("%d",&cont);
						}
						//this->UpdateDisplay();
						//scanf("%d",&cont);
					}

					for (int i = 0; i < vecdeledges.size(); i++) {

						//boost::tuple < ttt::SkeletonVertexType_lS, ttt::SkeletonVertexType_lS > pts = vecdeledges[i];
						boost::tuple < ttt::SkeletonVertexType, ttt::SkeletonVertexType > pts = vecdeledges[i];
						//ttt::SkeletonVertexType_lS src = boost::get<0>(pts);
						ttt::SkeletonVertexType src = boost::get<0>(pts);
						//ttt::SkeletonVertexType_lS tgt = boost::get<1>(pts);
						ttt::SkeletonVertexType tgt = boost::get<1>(pts);
						std::cout << "removing " << src << " " << tgt << std::endl;
						//boost::remove_edge(src, tgt, m_Descriptor->m_SkeletonGraph_listS);
						boost::remove_edge(src, tgt, *m_Descriptor->m_SkeletonGraph);
					}

					std::cout << "*** Cross is " << cross << std::endl;
					if (!cross) {
						bool notfound = true;
						//std::vector<ttt::SkeletonVertexType_lS> vt_edges =	svtedgemap[vtof10];
						std::vector<ttt::SkeletonVertexType> vt_edges =	svtedgemap[vtof10];
//						for (std::vector<ttt::SkeletonVertexType_lS>::iterator itr = vt_edges.begin(); itr != vt_edges.end(); ++itr) {
						for (std::vector<ttt::SkeletonVertexType>::iterator itr = vt_edges.begin(); itr != vt_edges.end(); ++itr) {
							if (*itr == v) {
								notfound = false;
								std::cout << "************" << std::endl;
								std::cout << "EDGE already exists" << std::endl;
								break;
							}
						}
						if (notfound) {
							//boost::add_edge(v, vtof10,m_Descriptor->m_SkeletonGraph_listS);

							svtedgemap[v].push_back(vtof10);
							std::cout << "**** Adding edge **** " << vtof10 << std::endl;
						}
					}
				}
#endif


			}
		}

	}
#if 0
	//boost::graph_traits<ttt::SkeletonGraph_listS>::vertex_iterator vi, vi_end, next;
	boost::graph_traits<ttt::SkeletonGraph>::vertex_iterator vi, vi_end, next;
	//boost::tie(vi, vi_end) = boost::vertices(m_Descriptor->m_SkeletonGraph_listS);
	boost::tie(vi, vi_end) = boost::vertices(*m_Descriptor->m_SkeletonGraph);

	for (next = vi; vi != vi_end; vi = next) {
		++next;
		//if (boost::degree(*vi, m_Descriptor->m_SkeletonGraph_listS) == 0) {
		if (boost::degree(*vi,*m_Descriptor->m_SkeletonGraph) == 0) {
			//boost::clear_vertex(*vi, m_Descriptor->m_SkeletonGraph_listS);
			boost::clear_vertex(*vi, *m_Descriptor->m_SkeletonGraph);
			//boost::remove_vertex(*vi, m_Descriptor->m_SkeletonGraph_listS);
			boost::remove_vertex(*vi, *m_Descriptor->m_SkeletonGraph);
		}
	}
#endif
	BGL_FORALL_VERTICES(v, *m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
		for(int i=0;i<3;i++){
			boost::get(ttt::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, v).position[i]*=this->m_Plateness->GetSpacing()[i];
		}
	}
	//std::cout << "num vertices after primal " << boost::num_vertices(m_Descriptor->m_SkeletonGraph_listS) << std::endl;
	std::cout << "num vertices after primal " << boost::num_vertices(*m_Descriptor->m_SkeletonGraph) << std::endl;
	//std::cout << "num edges after primal " << boost::num_edges(m_Descriptor->m_SkeletonGraph_listS) << std::endl;
	std::cout << "num edges after primal " << boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;
	BGL_FORALL_EDGES(e,*m_Descriptor->m_SkeletonGraph,ttt::SkeletonGraph){
		std::cout << boost::source(e,*m_Descriptor->m_SkeletonGraph) << "-" << boost::target(e,*m_Descriptor->m_SkeletonGraph) << std::endl;
	}
}

double ttt::AdherensJunctionSegmentationCommand::angleBetween2Lines(const itk::Point<double,3> & A,const itk::Point<double,3> & B,const itk::Point<double,3> & C){


        typedef itk::Point<double, 3> itkpt;
        itkpt tgt1 = B;
        itkpt tgt2 = C;

        //double angle1 = atan2(A[1] - B[1],A[0] - B[0]);
        //double angle2 = atan2(A[1] - C[1],A[0] - C[0]);


        /*
        double angle1 = Math.atan2(line1.getY1() - line1.getY2(),
                                   line1.getX1() - line1.getX2());
        double angle2 = Math.atan2(line2.getY1() - line2.getY2(),
                                   line2.getX1() - line2.getX2());*/

        float x1 = A[0];
        float y1 = A[1];
        float x2 = B[0];
        float y2 = B[1];
        float x3 = C[0];
        float y3 = C[1];

        float dx21 = x2-x1;
        float dx31 = x3-x1;
        float dy21 = y2-y1;
        float dy31 = y3-y1;
        float m12 = sqrt( dx21*dx21 + dy21*dy21 );
        float m13 = sqrt( dx31*dx31 + dy31*dy31 );
        float theta = acos( (dx21*dx31 + dy21*dy31) / (m12 * m13) );
        return theta;
}

double ttt::AdherensJunctionSegmentationCommand::distitkpt(itk::Point<float,3> pta, itk::Point<float,3> ptb){

    double dist = sqrt(pow(pta[0] - ptb[0],2) + pow(pta[1] - ptb[1],2) + pow(pta[2] - ptb[2],2));
    return dist;
}
