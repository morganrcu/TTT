/*
 * CellMomentCalculator.h
 *
 *  Created on: May 1, 2014
 *      Author: morgan
 */

#ifndef CELLMOMENTCALCULATOR_H_
#define CELLMOMENTCALCULATOR_H_
#include "tttDescriptionDataTypes.h"
#include "Feature.h"
#include "FeatureMap.h"
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
template<class TissueDescriptorType> class CellMomentCalculator{
public:
	typedef typename ttt::TissueDescriptorTraits<TissueDescriptorType>::CellType CellType;
	typedef typename ttt::TissueDescriptorTraits<TissueDescriptorType>::CellVertexType CellVertexType;
	typedef itk::Point<double,3> PointType;
	typedef itk::Vector<double,3> VectorType;

	typedef FeatureMap<CellVertexType,double> XXFeatureMapType;
	typedef FeatureMap<CellVertexType,double> YYFeatureMapType;
	typedef FeatureMap<CellVertexType,double> XYFeatureMapType;
	typedef FeatureMap<CellVertexType,double> AreaFeatureMapType;
	typedef FeatureMap<CellVertexType,double> PerimeterFeatureMapType;
	typedef FeatureMap<CellVertexType,double> NormalFeatureMapType;

	typedef FeatureMap<CellVertexType,itk::Point<double,3> > CentroidFeatureMapType;


private:
	typename TissueDescriptorType::Pointer m_TissueDescriptor;

	FeatureMap<CellVertexType,itk::Vector<double,3> > m_Normal;

	FeatureMap<CellVertexType,double> m_Area;
	FeatureMap<CellVertexType,double> m_Perimeter;
	FeatureMap<CellVertexType,itk::Point<double,3> > m_Centroid;
	FeatureMap<CellVertexType,double> m_XX;
	FeatureMap<CellVertexType,double> m_YY;
	FeatureMap<CellVertexType,double> m_XY;

public:

	FeatureMap<CellVertexType,itk::Point<double,3> > & GetCentroid(){
		return m_Centroid;
	}

	FeatureMap<CellVertexType,double>  & GetPerimeter(){
		return m_Perimeter;
	}
	FeatureMap<CellVertexType,double>  & GetXX(){
		return m_XX;
	}
	FeatureMap<CellVertexType,double > & GetYY(){
		return m_YY;
	}
	FeatureMap<CellVertexType,double > & GetXY(){
		return m_XY;
	}

	FeatureMap<CellVertexType,double > & GetAreas(){
		return m_Area;
	}


	void SetTissueDescriptor(const typename TissueDescriptorType::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}
	void GetCellMoments(const CellType & cell, Feature<double> & cellPerimeter, Feature<double> & cellArea, Feature<PointType> & cellCentroid, Feature<VectorType> & cellNormal, Feature<double> & cellXX, Feature<double> & cellYY, Feature<double> & cellXY) const {
		int total=0;
		std::vector<PointType> points,centeredPoints;
		PointType meanPoint;
		meanPoint.Fill(0);


		for(typename CellType::PerimeterIterator perimeterIt= cell.PerimeterBegin();perimeterIt!=cell.PerimeterEnd();++perimeterIt){
					itk::Point<double,3> position = boost::get(ttt::SkeletonPointPropertyTag(),*(m_TissueDescriptor->m_SkeletonGraph),*perimeterIt).position;
					//std::cout << position << std::endl;
		}

		for(typename CellType::PerimeterIterator perimeterIt= cell.PerimeterBegin();perimeterIt!=cell.PerimeterEnd();++perimeterIt){
			itk::Point<double,3> position = boost::get(ttt::SkeletonPointPropertyTag(),*(m_TissueDescriptor->m_SkeletonGraph),*perimeterIt).position;
			points.push_back(position);
			meanPoint[0]+=position[0];
			meanPoint[1]+=position[1];
			meanPoint[2]+=position[2];
			total++;
		}

		meanPoint[0]/=total;
		meanPoint[1]/=total;
		meanPoint[2]/=total;
		//std::cout << "MeanPoint " << meanPoint << std::endl;
		vnl_matrix<double> A(total,3);

		int row=0;

		for(std::vector<PointType>::iterator pointIt = points.begin();pointIt!=points.end();++pointIt){
			PointType diff = *pointIt-meanPoint;
			centeredPoints.push_back(diff);
			A(row,0)=diff[0];
			A(row,1)=diff[1];
			A(row,2)=diff[2];
			row++;

		}
		//std::cout << "A:" << std::endl;
		//std::cout << A << std::endl;

		vnl_matrix_fixed<double,3,3> S= A.transpose()*A; //FEATURE: Scatter Matrix

		vnl_symmetric_eigensystem<double > eigensystem(S);

		vnl_vector_fixed<double,3> normalPlane = eigensystem.get_eigenvector(0); //FEATURE: Supporting plane

		VectorType normalVector;
		normalVector.SetVnlVector(normalPlane);
		cellNormal.SetValue(normalVector);

		std::vector<PointType> projectedPoints;
		//std::cout << "Projections" << std::endl;
		for(std::vector<PointType>::iterator pointIt = centeredPoints.begin();pointIt!=centeredPoints.end();++pointIt){

			PointType a = *pointIt;
			PointType proj;

			double dot = a[0]*normalPlane[0] + a[1]*normalPlane[1] + a[2]*normalPlane[2];
			proj[0]=a[0]-dot*normalPlane[0];
			proj[1]=a[1]-dot*normalPlane[1];
			proj[2]=a[2]-dot*normalPlane[2];
			projectedPoints.push_back(proj);
			//std::cout << proj << std::endl;
		}

		std::vector<vnl_vector<double> > transformedPoints;

		//std::cout << "Transformed" << std::endl;

		for(std::vector<PointType>::iterator pointIt = projectedPoints.begin();pointIt!=projectedPoints.end();++pointIt){

			PointType a = *pointIt;
			vnl_vector<double> vec(3);
			vec(0)=a[0]; vec(1)=a[1];vec(2)=a[2];

			vnl_vector<double> ap= eigensystem.V.transpose() * vec;
			vnl_vector<double> transformed(2);
			transformed(0)=ap[1];
			transformed(1)=ap[2];
			transformedPoints.push_back(transformed);
			//std::cout << transformed << std::endl;

		}

		transformedPoints.push_back(transformedPoints[0]);


		vnl_vector<double> centroid2D(2);

		centroid2D.fill(0);
		double perimeter=0;
		double area=0;

		double xx=0;
		double xy=0;
		double yy=0;

		for(int i=0;i<transformedPoints.size()-1;i++){
			perimeter+=(transformedPoints[i+1]-transformedPoints[i]).magnitude();

			double common= (transformedPoints[i](0)*transformedPoints[i+1](1) - transformedPoints[i+1](0)*transformedPoints[i](1));

			area+=common;
			centroid2D(0)+=(transformedPoints[i](0)+ transformedPoints[i+1](0))*common;
			centroid2D(1)+=(transformedPoints[i](1)+ transformedPoints[i+1](1))*common;
			xx+= (pow(transformedPoints[i](0),2)+transformedPoints[i](0)* transformedPoints[i+1](0) + pow(transformedPoints[i+1](0),2) )*common;
			yy+= (pow(transformedPoints[i](1),2)+transformedPoints[i](1)* transformedPoints[i+1](1) + pow(transformedPoints[i+1](1),2) )*common;
			xy+= (2*transformedPoints[i](0)*transformedPoints[i](1) + transformedPoints[i](0)*transformedPoints[i+1](1) +  transformedPoints[i+1](0)*transformedPoints[i](1) )*common;

		}
		area=area/2;

		cellPerimeter.SetValue(perimeter);
		cellArea.SetValue(area);

		centroid2D(0)=centroid2D(0)/(6*area);
		centroid2D(1)=centroid2D(1)/(6*area);
		//std::cout << "Centroid2D" << std::endl;
		//std::cout<< centroid2D << std::endl;

		xx=xx/(12*area);
		yy=yy/(12*area);
		xy=xy/(24*area);

		cellXX.SetValue(xx);
		cellYY.SetValue(yy);
		cellXY.SetValue(xy);

		vnl_vector<double> cp(3);
		cp(0)=0;
		cp(1)=centroid2D(0);
		cp(2)=centroid2D(1);

		vnl_vector<double> cu = eigensystem.V*cp;

		//std::cout << "CU" << std::endl;
		//std::cout << cu << std::endl;

		PointType cg;
		PointType centroid3D;

		centroid3D[0]=cu[0]+meanPoint[0];
		centroid3D[1]=cu[1]+meanPoint[1];
		centroid3D[2]=cu[2]+meanPoint[2];

		//std::cout << "Centroid3D" << centroid3D << std::endl;

		cellCentroid.SetValue(centroid3D);




	}
	void Compute(){
		typename boost::graph_traits<typename TissueDescriptorType::DualGraphType>::vertex_iterator it,it_end;

		std::vector<PointType> points,centeredPoints;

		boost::tie(it,it_end)=boost::vertices(*(m_TissueDescriptor->m_CellGraph));

		typedef typename ttt::TissueDescriptorTraits<TissueDescriptorType>::CellPropertyTagType CellPropertyTagType;
		while(it!=it_end){
			typename ttt::TissueDescriptorTraits<TissueDescriptorType>::CellVertexType v = *it;
			points.clear();

			PointType meanPoint;
			meanPoint.Fill(0);
			CellType cell =boost::get(CellPropertyTagType(),*(m_TissueDescriptor->m_CellGraph),v);

			Feature<double> cellPerimeter;
			Feature<double> cellArea;
			Feature<PointType> cellCentroid;
			Feature<VectorType> cellNormal;

			Feature<double> cellXX;
			Feature<double> cellYY;
			Feature<double> cellXY;

			this->GetCellMoments(cell,cellPerimeter,cellArea,cellCentroid,cellNormal,cellXX,cellYY,cellXY);

			m_Area[*it]=cellArea;
			m_Perimeter[*it]=cellPerimeter;
			m_Centroid[*it]=cellCentroid;
			m_Normal[*it]=cellNormal;
			m_XX[*it]=cellXX;
			m_XY[*it]=cellXY;
			m_YY[*it]=cellYY;

			++it;


		}
	}


};



#endif /* CELLMOMENTCALCULATOR_H_ */
