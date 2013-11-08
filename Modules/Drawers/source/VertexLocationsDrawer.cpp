#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include "VertexLocationsDrawer.h"
void ttt::VertexLocationsDrawer::Draw(){

	       typedef PointSetType::PointsContainer PointsContainer;

	        PointsContainer::Pointer localMaxPoints = m_VertexLocations->GetPoints();

	        typedef  PointsContainer::Iterator PointIterator;

	        for (PointIterator it = localMaxPoints->Begin();
	             it != localMaxPoints->End(); it++) {

	            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	            sphereSource->SetCenter(it.Value()[0]*m_Spacing[0],
	                                    it.Value()[1]*m_Spacing[1],
	                                    it.Value()[2]*m_Spacing[2]);
	            sphereSource->SetRadius(5*m_Spacing[0]);
	            vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	            sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	            vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	            sphereActor->SetMapper(sphereMapper);
	            m_Renderer->AddActor(sphereActor);
	        }

#if 0
	//descriptor = this->m_Project.GetTissueDescriptor(m_CurrentFrame);

	        typedef itk::Point<double,3> itkpt;
	        ProjectType::SpacingType s = m_Project.GetSpacing();
	        std::cout<<"descriptor is " << descriptor<<std::endl;
	        std::cout<<"num vertices  of listS "<<boost::num_vertices(descriptor->m_SkeletonGraph_listS)<<std::endl;
	        std::cout<<"num edges of listS "<<boost::num_edges(descriptor->m_SkeletonGraph_listS)<<std::endl;
	        BGL_FORALL_VERTICES(v,descriptor->m_SkeletonGraph_listS,giaa::SkeletonGraph_listS){
	            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	            itkpt pos= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,v).position;
	            sphereSource->SetCenter(pos[0]*s[0] /*m_Project.GetSpacing()[0]*/,
	                                    pos[1]*s[1] /*m_Project.GetSpacing()[1]*/,
	                                    0 /*m_Project.GetSpacing()[2]*/);
	            sphereSource->SetRadius(5*s[0]/*m_Project.GetSpacing()[0]*/);
	            vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	            sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	            vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	            sphereActor->SetMapper(sphereMapper);
	            if(v == c1){
	                sphereActor->GetProperty()->SetColor(0,1,0);
	            }
	            else if (v == c2){
	                sphereActor->GetProperty()->SetColor(0,0,1);
	            }
	            giaaTovtkVertexToPrimal_def[v] = sphereActor;
	            vtkTogiaaVertex[sphereActor] = v;
	            m_CurrentRenderer->AddActor(sphereActor);
	        }


	            BGL_FORALL_EDGES(e,descriptor->m_SkeletonGraph_listS,giaa::SkeletonGraph_listS) {

	                typedef itk::Point<double,3> itkpt;
	                SkeletonVertexType_lS src = boost::source(e,descriptor->m_SkeletonGraph_listS);
	                SkeletonVertexType_lS tgt = boost::target(e,descriptor->m_SkeletonGraph_listS);
	                //std::cout<< "In edges " << src << " " << tgt << std::endl;
	                vtkSmartPointer<vtkLineSource> newLine = vtkSmartPointer<vtkLineSource>::New();
	                itkpt a= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,boost::source(e,descriptor->m_SkeletonGraph_listS)).position;
	                //std::cout<< a[0] << " " << a[1] << " " << a[2] << std::endl;
	                //IndexType a=m_SkeletonGraph[].position;
	                newLine->SetPoint1(a[0]*s[0],a[1]*s[1],0);
	                itkpt b= boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph_listS,boost::target(e,descriptor->m_SkeletonGraph_listS)).position;
	                //IndexType b=m_SkeletonGraph[boost::target(e,)].position;
	                newLine->SetPoint2(b[0]*s[0],b[1]*s[1],0);
	                vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	                mapper->SetInputConnection(newLine->GetOutputPort());

	                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	                tuple<SkeletonVertexType_lS,SkeletonVertexType_lS> pts (src,tgt);
	                giaaTovtkEdge[pts] = actor;
	                tuple<SkeletonVertexType_lS,SkeletonVertexType_lS> ptt (tgt,src);
	                giaaTovtkEdge[ptt] = actor;


	                actor->SetMapper(mapper);
	                actor->GetProperty()->SetLineWidth(5);
	                m_CurrentRenderer->AddActor(actor);
	            }
#endif
}


