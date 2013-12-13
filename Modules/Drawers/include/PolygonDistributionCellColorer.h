#ifndef POLYGONDISTRIBUTIONCELLCOLORER_H_
#define POLYGONDISTRIBUTIONCELLCOLORER_H_

namespace ttt{
template<class TissueDescriptor> class PolygonDistributionCellColorer: public Colorer<typename TissueDescriptorTraits<TissueDescriptor>::CellVertexType> {
private:
	typename TissueDescriptor::Pointer m_TissueDescriptor;
public:

	int m_MaxDegree=8;

	void SetTissueDescriptor(const typename TissueDescriptor::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}
	virtual void GetObjectColor(const typename TissueDescriptorTraits<TissueDescriptor>::CellVertexType  & vertex, double  color[3]) {

		typedef itk::Point<double, 3> itkpt;

		int degree=boost::degree(vertex,*m_TissueDescriptor->m_CellGraph);

		double deg = degree/m_MaxDegree;

		color[0] = deg;
		color[1] = deg<0.5?deg*2:1-(deg-0.5);
		color[2] = 1 - deg;

	}
	virtual ~PolygonDistributionCellColorer(){

	}
};
}



#endif /* POLYGONDISTRIBUTIONCOLORER_H_ */
