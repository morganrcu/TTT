
#ifndef CELLCENTROID_H_
#define CELLCENTROID_H_


#include <map>
#include <itkPoint.h>
#include "tttDescriptionDataTypes.h"
#include "TissueDescriptorFeature.h"
#include <vnl/algo/vnl_symmetric_eigensystem.h>

template<class T> class CellCentroid : public TissueDescriptorFeature<T,typename ttt::TissueDescriptorTraits<T>::CellVertexType, itk::Point<double,3> > {

public:
	typedef CellCentroid<T> Self;
	typedef boost::shared_ptr<Self> Pointer;

	typedef itk::Point<double,3> PointType;

	typedef typename ttt::TissueDescriptorTraits<T>::CellPropertyTagType CellPropertyTagType;
	typedef typename ttt::TissueDescriptorTraits<T>::SkeletonPointPropertyTag SkeletonPointPropertyTag;
	typedef typename ttt::TissueDescriptorTraits<T>::CellType CellType;

	static Pointer New(){
		return Pointer(new Self);
	}

	void Compute(){
		typename boost::graph_traits<typename T::DualGraphType>::vertex_iterator it,it_end;

		std::vector<PointType> points,centeredPoints;

		typedef typename std::vector<ttt::SkeletonVertexType>::const_iterator SkeletonPointIterator;

		boost::tie(it,it_end)=boost::vertices(*(this->GetTissueDescriptor()->m_CellGraph));
		while(it!=it_end){
			typename ttt::TissueDescriptorTraits<T>::CellVertexType v = *it;
			points.clear();

			PointType meanPoint;
			meanPoint.Fill(0);
			CellType cell =boost::get(CellPropertyTagType(),*(this->GetTissueDescriptor()->m_CellGraph),v);

			int total=0;

			for(SkeletonPointIterator it= cell.Begin();
					it!=cell.End();
					++it){
				itk::Point<double,3> position = boost::get(SkeletonPointPropertyTag(),*(this->GetTissueDescriptor()->m_SkeletonGraph),*it).position;
				points.push_back(position);
				meanPoint[0]+=position[0];
				meanPoint[1]+=position[1];
				meanPoint[2]+=position[2];
				total++;

			}
			meanPoint[0]/=total;
			meanPoint[1]/=total;
			meanPoint[2]/=total;


			vnl_matrix<double> A(total,3);
			int row=0;

			for(std::vector<PointType>::iterator it = points.begin();it!=points.end();++it){
				PointType diff = *it-meanPoint;
				centeredPoints.push_back(diff);
				A(row,0)=diff[0];
				A(row,1)=diff[1];
				A(row,2)=diff[2];
				row++;

			}
			vnl_matrix<double> S= A.transpose()*A;

			vnl_symmetric_eigensystem<double > eigensystem(S);


			vnl_vector<double> supportingPlane = eigensystem.get_eigenvector(0);

			std::vector<PointType> projectedPoints;
			for(std::vector<PointType>::iterator it = centeredPoints.begin();it!=centeredPoints.end();++it){

				PointType a = *it;
				PointType proj;

				double dot = a[0]*supportingPlane[0] + a[1]*supportingPlane[1] + a[2]*supportingPlane[2];
				proj[0]=a[0]-dot*supportingPlane[0];
				proj[1]=a[1]-dot*supportingPlane[1];
				proj[2]=a[2]-dot*supportingPlane[2];
				projectedPoints.push_back(proj);

			}




			std::vector<vnl_vector<double> > transformedPoints;

			for(std::vector<PointType>::iterator it = projectedPoints.begin();it!=projectedPoints.end();++it){

				PointType a = *it;
				vnl_vector<double> vec(3);
				vec(0)=a[0]; vec(1)=a[1];vec(2)=a[2];

				vnl_vector<double> ap= eigensystem.V.transpose() * vec;
				vnl_vector<double> transformed(2);
				transformed(0)=ap[1];
				transformed(1)=ap[2];
				transformedPoints.push_back(transformed);

			}


			transformedPoints.push_back(transformedPoints[0]);
			vnl_vector<double> centroid(2);

			centroid.fill(0);

			double area=0;
			for(int i=0;i<transformedPoints.size()-1;i++){
				centroid(0)+=(transformedPoints[i](0)+ transformedPoints[i+1](0))*(transformedPoints[i](0)*transformedPoints[i+1](1) - transformedPoints[i+1](0)*transformedPoints[i](1));
				centroid(1)+=(transformedPoints[i](1)+ transformedPoints[i+1](1))*(transformedPoints[i](0)*transformedPoints[i+1](1) - transformedPoints[i+1](0)*transformedPoints[i](1));
				area+=(transformedPoints[i](0)*transformedPoints[i+1](1)-transformedPoints[i+1](0)*transformedPoints[i](1));
			}
			area=area/2;
			centroid(0)=centroid(0)/(6*area);
			centroid(1)=centroid(1)/(6*area);

			vnl_vector<double> cp(3);
			cp(0)=0;
			cp(1)=centroid(0);
			cp(2)=centroid(1);

			vnl_vector<double> cu = eigensystem.V*cp;


			PointType cg;
			cg[0]=cu[0]+meanPoint[0];
			cg[1]=cu[1]+meanPoint[1];
			cg[2]=cu[2]+meanPoint[2];


			this->Insert(*it,cg);
			++it;


		}

	}

};



#endif /* CELLAREA_H_ */
