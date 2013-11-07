/*
 * VinodthSegmentationCommand.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: morgan
 */

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <itkLineIterator.h>

#include "VinodthSegmentationCommand.h"


void VinodthSegmentationCommand::InitDefGraph() {
	typedef double PixelType;
	bool cross;
#if 0
	boost::graph_traits<giaa::SkeletonGraph>::edge_iterator ei, ei_end,enext;

	boost::tie(ei, ei_end) = boost::edges(m_Descriptor->m_SkeletonGraph);
	for (enext = ei; ei != ei_end; ei = enext) {
		++enext;
		remove_edge(*ei, m_Descriptor->m_SkeletonGraph);
	}

	BGL_FORALL_VERTICES(v, *m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph)
	{
		boost::clear_vertex(v, *m_Descriptor->m_SkeletonGraph);
	}

	boost::graph_traits<giaa::SkeletonGraph>::vertex_iterator vi, vi_end, next;
	boost::tie(vi, vi_end) = vertices(*m_Descriptor->m_SkeletonGraph);
	for (next = vi; vi != vi_end; vi = next) {
		++next;
		remove_vertex(*vi, *m_Descriptor->m_SkeletonGraph);
	}


	std::cout << "num vertices before initdef "
			<< boost::num_vertices(*m_Descriptor->m_SkeletonGraph)
			<< std::endl;
	std::cout << "num edges before initdef "
			<< boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;
#endif
	m_Descriptor->m_SkeletonGraph= boost::shared_ptr<giaa::SkeletonGraph>(new giaa::SkeletonGraph);

	typedef typename PointSetType::PointsContainer PointsContainer;
	PointsContainer::Pointer localMaxPoints = m_Locations->GetPoints();
	typedef PointsContainer::Iterator PointIterator;
	for (PointIterator it = localMaxPoints->Begin();
			it != localMaxPoints->End(); it++) {
		itk::Point<double, 3> pos;

		for (int i = 0; i < 3; i++) {
			pos[i] = it.Value()[i];
		}
		giaa::SkeletonPoint pt = giaa::SkeletonPoint(pos);
		boost::add_vertex(pt,*(m_Descriptor->m_SkeletonGraph));
	}
	std::cout << "num vertices after initdef " << boost::num_vertices(*m_Descriptor->m_SkeletonGraph) << std::endl;
	std::cout << "num edges after initdef " << boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;
}

bool VinodthSegmentationCommand::IntersectLine(itk::Point<float ,3> l1posA, itk::Point<float, 3> l1posB, itk::Point<float, 3> l2posA, itk::Point<float, 3> l2posB ){


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

void VinodthSegmentationCommand::DoPrimalComputation() {

	typedef itk::Point<double, 3> itkpt;
	typedef double PixelType;
	bool cross;

	std::map<giaa::SkeletonVertexType, std::vector<giaa::SkeletonVertexType> > svtedgemap;
	std::vector<boost::tuple<giaa::SkeletonVertexType, giaa::SkeletonVertexType> > vecdeledges;





#if 0
	std::cout << "num vertices before primal " << boost::num_vertices(*m_Descriptor->m_SkeletonGraph) << std::endl;
	boost::graph_traits<giaa::SkeletonGraph>::edge_iterator ei, ei_end, enext;
	boost::tie(ei, ei_end) = edges(*m_Descriptor->m_SkeletonGraph);

	for (enext = ei; ei != ei_end; ei = enext) {
		++enext;
		remove_edge(*ei, *m_Descriptor->m_SkeletonGraph);
	}
#endif
	int cont;
	std::cout << "num edges before primal " << boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;

	BGL_FORALL_VERTICES(v,*m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
			std::vector<giaa::SkeletonVertexType> vecvt;
			svtedgemap[v] = vecvt;
	}


	BGL_FORALL_VERTICES(v,*m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
		giaa::SkeletonVertexType c1 = v;
		itkpt pos1 = boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, v).position;

		std::vector<double> distances;
		std::map<double, giaa::SkeletonVertexType> distovtmap;

		BGL_FORALL_VERTICES(vt, *m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
			if (vt != v) {
				itkpt pos2 = boost::get(giaa::SkeletonPointPropertyTag(), *m_Descriptor->m_SkeletonGraph, vt).position;
				double dist = sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2));//2D distances
				distances.push_back(dist);
				distovtmap[dist] = vt;
			}
		}
		std::sort(distances.begin(), distances.end());

		for (int i = 0; i < 10; i++) {

			giaa::SkeletonVertexType vtof10 = distovtmap[distances[i]];

			giaa::SkeletonVertexType c2 = vtof10;
			itkpt pos3 = boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, vtof10).position;

			typename PlatenessImageType::IndexType pt1;
			typename PlatenessImageType::IndexType pt2;

			for (int j = 0; j < 3; j++) {
				pt1[j] = pos1[j];
				pt2[j] = pos3[j];
			}

			double intensepix = 0;
			double totalpix = 0;
			itk::LineIterator<PlatenessImageType> it1(m_Plateness, pt1, pt2);
			it1.GoToBegin();
			while (!it1.IsAtEnd()) {
				typename PlatenessImageType::IndexType pixelIndex;
				PixelType pixel = it1.Get();
				//std::cout<< "pix val: "<< pixel<<std::endl;
				if (pixel > .02) {
					intensepix++;
				}
				++it1;
				totalpix++;
			}
			//std::cout<<"\n"<<std::endl;
			double percent = (intensepix / totalpix);
			std::cout << "percent " << percent << " " << i << "\n" << std::endl;
			//scanf("%d",&cont);
			cross = false;
			if (percent > .6) {

				BGL_FORALL_EDGES(e, *m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
					giaa::SkeletonVertexType src = boost::source(e,*m_Descriptor->m_SkeletonGraph);

					giaa::SkeletonVertexType tgt = boost::target(e,*m_Descriptor->m_SkeletonGraph);


					itkpt l2posA = boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, src).position;
					itkpt l2posB = boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, tgt).position;

					cross = IntersectLine(pos1, pos3, l2posA, l2posB);
					//std::cout << "cross: " << cross << std::endl;
					if (cross) {
						break;
					}

					if (l2posA[0] == pos1[0] && l2posA[1] == pos1[1]
							&& l2posA[2] == pos1[2]) {
						std::vector<itkpt> linepts;
						linepts.push_back(l2posB);
						linepts.push_back(pos3);
						double ang = angleBetween2Lines(pos1, linepts) * 180.0
								/ 3.14159;
						std::cout << "angle is pos1 if " << ang << std::endl;
						if (ang < 45) {
							cross = true;
							break;
						}
					}

					else if (l2posB[0] == pos1[0] && l2posB[1] == pos1[1]
							&& l2posB[2] == pos1[2]) {
						std::vector<itkpt> linepts;
						linepts.push_back(l2posA);
						linepts.push_back(pos3);
						double ang = angleBetween2Lines(pos1, linepts) * 180.0
								/ 3.1459;
						std::cout << "angle is pos1 else" << ang << std::endl;
						if (ang < 45) {
							cross = true;
							break;
						}
					}

				}

				if (!cross) {
					vecdeledges.clear();
					BGL_FORALL_EDGES(e, *m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
						giaa::SkeletonVertexType src = boost::source(e,*m_Descriptor->m_SkeletonGraph);
						giaa::SkeletonVertexType tgt = boost::target(e,*m_Descriptor->m_SkeletonGraph);

						itkpt l2posA = boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, src).position;
						itkpt l2posB = boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, tgt).position;

						if (l2posA[0] == pos3[0] && l2posA[1] == pos3[1]
								&& l2posA[2] == pos3[2]) {
							std::vector<itkpt> linepts;
							linepts.push_back(l2posB);
							linepts.push_back(pos1);
							double ang = angleBetween2Lines(pos3, linepts)
									* 180.0 / 3.14159;
							std::cout << "angle is pos3 if " << ang
									<< std::endl;
							if (ang < 45) {
								double distpos3 = distitkpt(pos1, pos3);
								double distl2posB = distitkpt(pos3, l2posB);
								if (distpos3 < distl2posB) {
									//boost::remove_edge(src,tgt,descriptor->m_SkeletonGraph_listS);
									std::cout << "added 1 " << src << " " << tgt
											<< std::endl;
									//boost::tuple < giaa::SkeletonVertexType_lS, giaa::SkeletonVertexType_lS> delpair(src, tgt);
									boost::tuple < giaa::SkeletonVertexType, giaa::SkeletonVertexType> delpair(src, tgt);
									std::cout << boost::get<0>(delpair) << " " << boost::get<1>(delpair) << std::endl;

									vecdeledges.push_back(delpair);

								} else {
									cross = true;
									break;
								}
							}
						}

						else if (l2posB[0] == pos3[0] && l2posB[1] == pos3[1]
								&& l2posB[2] == pos3[2]) {
							std::vector<itkpt> linepts;
							linepts.push_back(l2posA);
							linepts.push_back(pos1);
							double ang = angleBetween2Lines(pos3, linepts)
									* 180.0 / 3.1459;
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

									//boost::tuple < giaa::SkeletonVertexType_lS, giaa::SkeletonVertexType_lS > delpair(src, tgt);
									boost::tuple < giaa::SkeletonVertexType, giaa::SkeletonVertexType > delpair(src, tgt);
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

						//boost::tuple < giaa::SkeletonVertexType_lS, giaa::SkeletonVertexType_lS > pts = vecdeledges[i];
						boost::tuple < giaa::SkeletonVertexType, giaa::SkeletonVertexType > pts = vecdeledges[i];
						//giaa::SkeletonVertexType_lS src = boost::get<0>(pts);
						giaa::SkeletonVertexType src = boost::get<0>(pts);
						//giaa::SkeletonVertexType_lS tgt = boost::get<1>(pts);
						giaa::SkeletonVertexType tgt = boost::get<1>(pts);
						std::cout << "removing " << src << " " << tgt << std::endl;
						//boost::remove_edge(src, tgt, m_Descriptor->m_SkeletonGraph_listS);
						boost::remove_edge(src, tgt, *m_Descriptor->m_SkeletonGraph);
					}

					std::cout << "*** Cross is " << cross << std::endl;
					if (!cross) {
						bool notfound = true;
						//std::vector<giaa::SkeletonVertexType_lS> vt_edges =	svtedgemap[vtof10];
						std::vector<giaa::SkeletonVertexType> vt_edges =	svtedgemap[vtof10];
//						for (std::vector<giaa::SkeletonVertexType_lS>::iterator itr = vt_edges.begin(); itr != vt_edges.end(); ++itr) {
						for (std::vector<giaa::SkeletonVertexType>::iterator itr = vt_edges.begin(); itr != vt_edges.end(); ++itr) {
							if (*itr == v) {
								notfound = false;
								std::cout << "************" << std::endl;
								std::cout << "EDGE already exists" << std::endl;
								break;
							}
						}
						if (notfound) {
							//boost::add_edge(v, vtof10,m_Descriptor->m_SkeletonGraph_listS);
							boost::add_edge(v, vtof10,*m_Descriptor->m_SkeletonGraph);
							svtedgemap[v].push_back(vtof10);
							std::cout << "**** Adding edge **** " << vtof10 << std::endl;
						}
					}
				}

			}
		}

	}
#if 0
	//boost::graph_traits<giaa::SkeletonGraph_listS>::vertex_iterator vi, vi_end, next;
	boost::graph_traits<giaa::SkeletonGraph>::vertex_iterator vi, vi_end, next;
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
	BGL_FORALL_VERTICES(v, *m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
		for(int i=0;i<3;i++){
			boost::get(giaa::SkeletonPointPropertyTag(),*m_Descriptor->m_SkeletonGraph, v).position[i]*=this->m_Plateness->GetSpacing()[i];
		}
	}
	//std::cout << "num vertices after primal " << boost::num_vertices(m_Descriptor->m_SkeletonGraph_listS) << std::endl;
	std::cout << "num vertices after primal " << boost::num_vertices(*m_Descriptor->m_SkeletonGraph) << std::endl;
	//std::cout << "num edges after primal " << boost::num_edges(m_Descriptor->m_SkeletonGraph_listS) << std::endl;
	std::cout << "num edges after primal " << boost::num_edges(*m_Descriptor->m_SkeletonGraph) << std::endl;
	BGL_FORALL_EDGES(e,*m_Descriptor->m_SkeletonGraph,giaa::SkeletonGraph){
		std::cout << boost::source(e,*m_Descriptor->m_SkeletonGraph) << "-" << boost::target(e,*m_Descriptor->m_SkeletonGraph) << std::endl;
	}
}

double VinodthSegmentationCommand::angleBetween2Lines(itk::Point<double,3> srcpt, std::vector<itk::Point<double,3> > linepts){


        typedef itk::Point<double, 3> itkpt;
        itkpt tgt1 = linepts[0];
        itkpt tgt2 = linepts[1];

        double angle1 = atan2(srcpt[1] - tgt1[1],srcpt[0] - tgt1[0]);
        double angle2 = atan2(srcpt[1] - tgt2[1],srcpt[0] - tgt2[0]);


        /*
        double angle1 = Math.atan2(line1.getY1() - line1.getY2(),
                                   line1.getX1() - line1.getX2());
        double angle2 = Math.atan2(line2.getY1() - line2.getY2(),
                                   line2.getX1() - line2.getX2());*/

        float x1 = srcpt[0];
        float y1 = srcpt[1];
        float x2 = tgt1[0];
        float y2 = tgt1[1];
        float x3 = tgt2[0];
        float y3 = tgt2[1];

        float dx21 = x2-x1;
        float dx31 = x3-x1;
        float dy21 = y2-y1;
        float dy31 = y3-y1;
        float m12 = sqrt( dx21*dx21 + dy21*dy21 );
        float m13 = sqrt( dx31*dx31 + dy31*dy31 );
        float theta = acos( (dx21*dx31 + dy21*dy31) / (m12 * m13) );
        return theta;
}

double VinodthSegmentationCommand::distitkpt(itk::Point<float,3> pta, itk::Point<float,3> ptb){

    double dist = sqrt(pow(pta[0] - ptb[0],2) + pow(pta[1] - ptb[1],2) + pow(pta[2] - ptb[2],2));
    return dist;
}
