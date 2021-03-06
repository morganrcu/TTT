/*
 * VinodthSegmentationCommand.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: morgan
 */

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>

#include <itkMyFastMarchingImageFilter.h>
#include <itkLineIterator.h>
#include <itkImageFileWriter.h>
#include <itkWatershedImageFilter.h>
#include <itkMinimumMaximumImageFilter.h>
#include <itkMaximumImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>
#include <itkCastImageFilter.h>

#include "tttVoronoiTesselationImageFilter.h"
#include <deque>
#include <queue>
#include <limits.h>
#include <values.h>
#define COLOR_WHITE 0
#define COLOR_GRAY 1
#define COLOR_BLACK 2
#include "AdherensJunctionSegmentationDijkstraCommand.h"

void ttt::AdherensJunctionSegmentationDijkstraCommand::ComputSpeedImage() {

	typedef itk::MaximumImageFilter<PlatenessImageType, VertexnessImageType,
			SpeedImageType> MaximumFilterType;

	MaximumFilterType::Pointer maximumFilter = MaximumFilterType::New();

	maximumFilter->SetInput1(m_Plateness);

	maximumFilter->SetInput2(m_Vertexness);

	maximumFilter->Update();

	m_Speed = maximumFilter->GetOutput();

}
void ttt::AdherensJunctionSegmentationDijkstraCommand::InitDefGraph() {
	m_Descriptor = ttt::TissueDescriptor::New();

	m_Descriptor->m_SkeletonGraph = boost::shared_ptr<ttt::SkeletonGraph<2> >(
			new ttt::SkeletonGraph<2> );

	for (typename ttt::AdherensJunctionVertices<3>::iterator it =
			m_Locations->begin(); it != m_Locations->end(); it++) {
		itk::Point<float, 3> pos;

		m_Plateness->TransformIndexToPhysicalPoint((*it)->GetPosition(), pos);

		ttt::SkeletonPoint pt = ttt::SkeletonPoint(pos);
		SkeletonVertexType n = boost::add_vertex(pt,
				*(m_Descriptor->m_SkeletonGraph));
		//std::cout << "Adding " << pos << " " << n << std::endl;
		m_IndexToVertex.insert(IndexAndSkeletonVertexType(*it, n));
		//m_IndexToVertex.left.[*it]=;

	}
#if 0
	for(MapType::iterator it=m_PointToVertex.begin();it!=m_PointToVertex.end();it++) {
		std::cout << it->first << " " << it->second << std::endl;
	}
#endif
}

void ttt::AdherensJunctionSegmentationDijkstraCommand::GetNeighbors(
		const itk::Index<3> & index, std::vector<itk::Index<3> > & neighbors) {
	typename PlatenessImageType::SizeType size =
			m_Speed->GetLargestPossibleRegion().GetSize();
	neighbors.clear();
	for (int i = -1; i < 2; i++) {
		itk::Offset<3> off;
		off[0] = i;
		for (int j = -1; j < 2; j++) {
			off[1] = j;
			for (int k = -1; k < 2; k++) {
				off[2] = k;
				if (i == 0 && j == 0 && k == 0)
					continue;
				itk::Index<3> neigh = index + off;
				if (neigh[0] < 0 || neigh[1] < 0 || neigh[2] < 0
						|| neigh[0] >= size[0] || neigh[1] >= size[1]
						|| neigh[2] >= size[2]) {
					continue;
				}
				neighbors.push_back(neigh);
			}
		}
	}
}

template<class IndexType> class IndexContainer {
public:
	IndexContainer() {
		m_Time = -1;
		m_Index.Fill(-1);
	}
	IndexContainer(const IndexContainer<IndexType> & other) {
		this->SetIndex(other.GetIndex());
		this->SetValue(other.GetValue());
	}
	~IndexContainer() {

	}
	bool operator<(const IndexContainer & other) const {
		return m_Time > other.GetValue();
	}
	bool operator==(const IndexContainer & other) const {
		return other.GetIndex()[0] == m_Index[0]
				&& other.GetIndex()[1] == m_Index[1]
				&& other.GetIndex()[2] == m_Index[2];
	}
	double GetValue() const {
		return m_Time;
	}
	void SetValue(double time) {
		m_Time = time;
	}
	IndexType GetIndex() const {
		return m_Index;
	}
	void SetIndex(const IndexType & newIndex) {
		m_Index = newIndex;
	}
private:
	IndexType m_Index;
	double m_Time;
};

void ttt::AdherensJunctionSegmentationDijkstraCommand::DoFastMarching() {

	typedef itk::Image<int, 3> VoronoiImageType;

	typedef itk::ImageFileWriter<VoronoiImageType> VoronoiWriterType;

	VoronoiWriterType::Pointer voronoiWriter = VoronoiWriterType::New();
	voronoiWriter->SetFileName("VertexVoronoi.mha");



	typedef ttt::VoronoiTesselationImageFilter<VoronoiImageType> VoronoiFilterType;

	VoronoiFilterType::Pointer voronoiFilter = VoronoiFilterType::New();



	typedef ttt::FastMarchingImageFilter<LevelSetImageType, PlatenessImageType> FastMarchingImageFilterType;

	FastMarchingImageFilterType::Pointer fastMarching = FastMarchingImageFilterType::New();
	fastMarching->SetInput(m_Speed);

	typedef VoronoiFilterType::NodeContainer VoronoiNodeContainer;
	typedef VoronoiFilterType::LevelSetNodeType VoronoiNodeType;

	typedef FastMarchingImageFilterType::NodeContainer NodeContainer;
	typedef FastMarchingImageFilterType::NodeType NodeType;
	NodeContainer::Pointer seeds = NodeContainer::New();
	seeds->Initialize();

	VoronoiNodeContainer::Pointer seedsVoronoi= VoronoiNodeContainer::New();;
	seedsVoronoi->Initialize();

	int k = 0;

	for (ttt::AdherensJunctionVertices<3>::iterator it = m_Locations->begin();
			it != m_Locations->end(); it++) {
		PlatenessImageType::IndexType seedPosition = (*it)->GetPosition();

		NodeType node;
		const double seedValue = 0.0;
		node.SetValue(seedValue);
		node.SetIndex(seedPosition);

		seeds->InsertElement(k, node);

		VoronoiNodeType nodeVoronoi;

		nodeVoronoi.SetValue(seedValue);
		node.SetIndex(seedPosition);
		seedsVoronoi->InsertElement(k,nodeVoronoi);

		k++;
	}

	voronoiFilter->SetOutputSize(m_Speed->GetLargestPossibleRegion().GetSize());
	voronoiFilter->SetOutputDirection(m_Speed->GetDirection());
	voronoiFilter->SetOutputRegion(m_Speed->GetLargestPossibleRegion());
	voronoiFilter->SetOutputSpacing(m_Speed->GetSpacing());
	voronoiFilter->SetOutputOrigin(m_Speed->GetOrigin());

	voronoiFilter->SetTrialPoints(seeds);
	voronoiFilter->Update();

	voronoiWriter->SetInput(voronoiFilter->GetOutput());
	voronoiWriter->Update();

	fastMarching->SetStoppingValue(m_StoppingValue);
	fastMarching->SetTrialPoints(seeds);
	fastMarching->SetOutputSize(m_Speed->GetBufferedRegion().GetSize());



	fastMarching->SetVoronoiImage(voronoiFilter->GetOutput());
	fastMarching->Update();

	m_LevelSet = fastMarching->GetOutput();
	m_Labels = fastMarching->GetClusterImage();

	itk::ImageRegionIterator<LevelSetImageType> levelsetIterator(m_LevelSet,
			m_LevelSet->GetLargestPossibleRegion());

	levelsetIterator.GoToBegin();

	while (!levelsetIterator.IsAtEnd()) {

		if (levelsetIterator.Value()
				== static_cast<double>(itk::NumericTraits<double>::max() / 2.0)) {
			levelsetIterator.Set(-1);
		}

		++levelsetIterator;
	}

	typedef itk::ImageFileWriter<LevelSetImageType> LevelSetWriterType;

	LevelSetWriterType::Pointer levelSetWriter = LevelSetWriterType::New();

	levelSetWriter->SetFileName("LevelSet.mha");
	levelSetWriter->SetInput(m_LevelSet);
	levelSetWriter->Update();

}

void ttt::AdherensJunctionSegmentationDijkstraCommand::PruneLevelSet(
		double value) {

	itk::ImageRegionIterator<LevelSetImageType> levelsetIterator(m_LevelSet,
			m_LevelSet->GetLargestPossibleRegion());
	itk::ImageRegionIterator<LabelImageType> labelIterator(m_Labels,
			m_Labels->GetLargestPossibleRegion());
	while (!levelsetIterator.IsAtEnd()) {
		if (levelsetIterator.Value() >= value) {
			levelsetIterator.Set(-1);
			labelIterator.Set(-1);
		}
		++levelsetIterator;
		++labelIterator;
	}
}
#if 0
void ttt::AdherensJunctionSegmentationDijkstraCommand::ComputeLevelsetThreshold(double percentage) {

	itk::ImageRegionConstIterator<LevelSetImageType> levelsetConstIterator(m_LevelSet,m_LevelSet->GetLargestPossibleRegion());
	std::vector<double> frontValues;

	while(!levelsetConstIterator.IsAtEnd()) {
		if(levelsetConstIterator.Value()!=-1) {
			frontValues.push_back(levelsetConstIterator.Value());
		}
		++levelsetConstIterator;
	}

	std::sort(frontValues.begin(),frontValues.end());

	unsigned long index = round(frontValues.size()*percentage);

	m_LevelSetThreshold=frontValues[index];

}
#endif
#if 0
void ttt::AdherensJunctionSegmentationDijkstraCommand::AllocateLabelsImage() {
	m_Labels = LabelImageType::New();
	m_Labels->SetRegions(m_Speed->GetLargestPossibleRegion());
	m_Labels->SetSpacing(m_Speed->GetSpacing());
	m_Labels->SetOrigin(m_Speed->GetOrigin());
	m_Labels->Allocate();
	m_Labels->FillBuffer(itk::NumericTraits< unsigned long >::max());
}
#endif
#if 0
void ttt::AdherensJunctionSegmentationDijkstraCommand::DoVertexSegmentation() {

	ColorImageType::Pointer colors = ColorImageType::New();
	colors->SetRegions(m_Plateness->GetLargestPossibleRegion());
	colors->SetSpacing(m_Plateness->GetSpacing());
	colors->SetOrigin(m_Plateness->GetOrigin());
	colors->Allocate();
	colors->FillBuffer(COLOR_WHITE);

	itk::ImageRegionConstIteratorWithIndex<LevelSetImageType> levelsetConstIterator(m_LevelSet,m_LevelSet->GetLargestPossibleRegion());

	for(typename IndexAndSkeletonVertexBimapType::const_iterator it = m_IndexToVertex.begin();it!=m_IndexToVertex.end();++it) {
		m_Labels->SetPixel(it->left->GetPosition(),it->right);
		colors->SetPixel(it->left->GetPosition(),COLOR_BLACK);
	}

	levelsetConstIterator.GoToBegin();

	while(!levelsetConstIterator.IsAtEnd()) {

		if(levelsetConstIterator.Value()!=-1 && colors->GetPixel(levelsetConstIterator.GetIndex())==COLOR_WHITE) {
			Index current = levelsetConstIterator.GetIndex();

			bool backtrackingFinished=false;
			std::list<Index> path;
			path.clear();
			while (!backtrackingFinished) {
				//PATH = PATH + current

				if(colors->GetPixel(current)==COLOR_BLACK) {
					backtrackingFinished=true;
				} else {
					double currentValue=m_LevelSet->GetPixel(current);
					path.push_back(current);
					Index minNeigh;
					double minValue=currentValue;

					std::vector<Index> neighbors;
					this->GetNeighbors(current,neighbors);
					for(std::vector<Index>::iterator itNeigh=neighbors.begin();itNeigh!=neighbors.end();++itNeigh) {
						Index neigh = *itNeigh;
						double neighValue = m_LevelSet->GetPixel(neigh);
						if(neighValue!=-1 && neighValue<minValue) {
							minValue=neighValue;
							minNeigh=neigh;
						}
					}
					assert(currentValue>minValue);
					current = minNeigh;
				}
			}
			unsigned long label = m_Labels->GetPixel(current);
			for(std::list<Index>::iterator it= path.begin();it!=path.end();it++) {
				m_Labels->SetPixel(*it,label);
				colors->SetPixel(*it,COLOR_BLACK);
			}
		}
		++levelsetConstIterator;
	}
}
#endif
double ttt::AdherensJunctionSegmentationDijkstraCommand::ComputePath(
		const SkeletonVertexType & a, const SkeletonVertexType & b) {
	double weight = 0;

	typedef std::deque<IndexContainer<Index> > HeapType;
	DistanceImageType::Pointer distances = DistanceImageType::New();
	distances->SetRegions(m_Plateness->GetLargestPossibleRegion());
	distances->SetSpacing(m_Plateness->GetSpacing());
	distances->SetOrigin(m_Plateness->GetOrigin());
	distances->Allocate();
	distances->FillBuffer(INFINITY);

	ColorImageType::Pointer colors = ColorImageType::New();
	colors->SetRegions(m_Plateness->GetLargestPossibleRegion());
	colors->SetSpacing(m_Plateness->GetSpacing());
	colors->SetOrigin(m_Plateness->GetOrigin());
	colors->Allocate();
	colors->FillBuffer(COLOR_WHITE);

	Index sourceIndex = m_IndexToVertex.right.find(a)->second->GetPosition();
	Index targetIndex = m_IndexToVertex.right.find(b)->second->GetPosition();

	IndexContainer<Index> sourceIndexContainer;
	sourceIndexContainer.SetValue(0);
	sourceIndexContainer.SetIndex(sourceIndex);

	HeapType trialHeap;
	trialHeap.push_back(sourceIndexContainer);
	std::make_heap(trialHeap.begin(), trialHeap.end());

	distances->SetPixel(sourceIndex, 0);

	bool found = false;
	int exploredNodes = 0;

	while (!trialHeap.empty() && !found) {

		IndexContainer<Index> current = trialHeap.front();

		Index currentIndex = current.GetIndex();
		double currentDistance = current.GetValue();

		std::pop_heap(trialHeap.begin(), trialHeap.end());
		trialHeap.pop_back();
		colors->SetPixel(current.GetIndex(), COLOR_BLACK);

		exploredNodes++;

		if (exploredNodes % 10000 == 0) {
			//std::cout << exploredNodes << "/"  << " Pendientes: " << trialHeap.size() << " " << currentDistance << std::endl;
		}

		if (currentIndex == targetIndex) {
			found = true;
		} else {
			std::vector<Index> neighs;
			this->GetNeighbors(currentIndex, neighs);
			for (std::vector<Index>::iterator it = neighs.begin();
					it != neighs.end(); it++) {
				Index neigh = *it;
				char neighcolor = colors->GetPixel(neigh);
				if (neighcolor == COLOR_BLACK)
					continue;
				if (m_Labels->GetPixel(neigh) == a
						|| m_Labels->GetPixel(neigh) == b) {
					double speedValue = m_Speed->GetPixel(neigh);
					double nextDistance = 1.0 / speedValue;
					double candDist = currentDistance + nextDistance;
					double neighDist = distances->GetPixel(neigh);

					if (candDist < neighDist) {
						IndexContainer<Index> current;
						current.SetValue(candDist);
						current.SetIndex(neigh);

						if (neighcolor == COLOR_GRAY) {
							typename HeapType::iterator findres = std::find(
									trialHeap.begin(), trialHeap.end(),
									current);
							findres->SetValue(candDist);
							std::update_heap_pos(trialHeap.begin(),
									trialHeap.end(), findres);
						} else {
							trialHeap.push_back(current);
							std::push_heap(trialHeap.begin(), trialHeap.end());
							colors->SetPixel(neigh, COLOR_GRAY);
						}
						distances->SetPixel(neigh, candDist);
					}
				}
			}
		}
	}

	if (found) {

		bool backtrackingFinished = false;
		std::list<Index> path;
		path.clear();

		Index currentIndex = targetIndex;
		int length = 0;
		while (!backtrackingFinished) {
			//PATH = PATH + current
			if (currentIndex == sourceIndex) {
				backtrackingFinished = true;
			} else {

				double currentValue = distances->GetPixel(currentIndex);
				weight += m_Speed->GetPixel(currentIndex);
				length++;
				path.push_back(currentIndex);

				Index minNeigh;
				double minValue = currentValue;

				std::vector<Index> neighbors;
				this->GetNeighbors(currentIndex, neighbors);
				for (std::vector<Index>::iterator itNeigh = neighbors.begin();
						itNeigh != neighbors.end(); ++itNeigh) {
					Index neigh = *itNeigh;
					double neighValue = distances->GetPixel(neigh);

					if (neighValue < minValue) {
						minValue = neighValue;
						minNeigh = neigh;
					}
				}
				assert(currentValue > minValue);
				currentIndex = minNeigh;
			}
		}

		return weight / length;
	} else {
		return -1;
	}
}
void ttt::AdherensJunctionSegmentationDijkstraCommand::BuildGraph() {
#if 0
	BGL_FORALL_VERTICES_T(a,*m_Descriptor->m_SkeletonGraph,SkeletonGraph) {
		BGL_FORALL_VERTICES_T(b,*m_Descriptor->m_SkeletonGraph,SkeletonGraph) {
			if(!boost::edge(a,b,*m_Descriptor->m_SkeletonGraph).second) {
				double weight=this->ComputePath(a,b);
				if(weight>0.1) {
					std::cout << "(" << a << "," << b << ") = " << weight << std::endl;
					boost::add_edge(a,b,*m_Descriptor->m_SkeletonGraph);
				} else if(weight > 0) {
					std::cout <<"Rejected";
				} else {
					std::cout << "Not found" << std::endl;
				}
			}
		}
	}
#endif

	vnl_matrix<unsigned char> tested(m_Locations->size(), m_Locations->size());
	tested.fill(0);
	itk::ImageRegionConstIteratorWithIndex<LabelImageType> labelConstIterator(
			m_Labels, m_Labels->GetLargestPossibleRegion());

	labelConstIterator.GoToBegin();
	int k = 0;
	while (!labelConstIterator.IsAtEnd()) {

		if (labelConstIterator.Value()
				!= itk::NumericTraits<unsigned long>::max()) {

			Index current = labelConstIterator.GetIndex();
			unsigned long currentValue = labelConstIterator.Value();

			std::vector<Index> neighbors;
			this->GetNeighbors(current, neighbors);

			for (std::vector<Index>::iterator itNeigh = neighbors.begin();
					itNeigh != neighbors.end(); ++itNeigh) {

				Index neigh = *itNeigh;
				unsigned long neighValue = m_Labels->GetPixel(neigh);

				if (neighValue != itk::NumericTraits<unsigned long>::max()
						&& neighValue != currentValue) {

					if (!boost::edge(currentValue, neighValue,
							*m_Descriptor->m_SkeletonGraph).second
							&& tested(neighValue, currentValue) == 0
							&& tested(currentValue, neighValue) == 0) {

						//double weight=this->ComputePath(currentValue,neighValue);
						double weight = 1;
						if (weight > 0.01) {
							//std::cout << "(" << currentValue << "," <<  neighValue << ") = " << weight <<  std::endl;
							boost::add_edge(currentValue, neighValue,
									*m_Descriptor->m_SkeletonGraph);
						} else if (weight > 0) {
							//std::cout <<"Rejected";
						} else {
							//std::cout << "Not found" << std::endl;
						}
						tested(neighValue, currentValue) = 1;
						tested(currentValue, neighValue) = 1;
					}
				}
			}
		}
		++labelConstIterator;
	}
}
void ttt::AdherensJunctionSegmentationDijkstraCommand::StoreLabels() {
#if 0
	itk::ImageRegionIterator<LabelImageType> labelIterator(m_Labels,m_Labels->GetLargestPossibleRegion());

	labelIterator.GoToBegin();
	while(!labelIterator.IsAtEnd()) {

		if(labelIterator.Value()==itk::NumericTraits< int>::max()) {
			labelIterator.Set(m_Locations->size()+1);
		}
		++labelIterator;
	}
#endif
	typedef itk::Image<float, 3> FloatLabelImageType;
	typedef itk::CastImageFilter<LabelImageType, FloatLabelImageType> CastType;

	CastType::Pointer caster = CastType::New();
	caster->SetInput(m_Labels);
	caster->Update();

	typedef itk::ImageFileWriter<FloatLabelImageType> LabelWriterType;

	LabelWriterType::Pointer labelWriter = LabelWriterType::New();

	labelWriter->SetFileName("VertexLabels.mha");
	labelWriter->SetInput(caster->GetOutput());
	labelWriter->Update();
}
void ttt::AdherensJunctionSegmentationDijkstraCommand::Do() {
	this->ComputSpeedImage();
	this->InitDefGraph();
	this->DoFastMarching();

	//this->ComputeLevelsetThreshold(0.3);
	//this->PruneLevelSet(m_LevelSetThreshold);
	this->StoreLabels();
	//this->AllocateLabelsImage();

	//this->DoVertexSegmentation();

	this->BuildGraph();

	//m_Labels->ReleaseData();
}

