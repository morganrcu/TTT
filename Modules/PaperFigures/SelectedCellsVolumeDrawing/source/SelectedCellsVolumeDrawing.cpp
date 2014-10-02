#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkPointSet.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTIFFWriter.h>
#include "AdherensJunctionSegmentationDijkstraCommand.h"
#include "readgt.h"
#include "TrackVolumeDrawer.hpp"
#include "CellPolygonDrawer.h"
#include "DefaultColorer.h"

#include "itkIndex.h"

#include "qtsqltissuetrackingproject2.h"
#include "jsontissuetrackingproject2.h"

typedef itk::Image<float, 3> PlatenessImageType;
typedef itk::ImageFileReader<PlatenessImageType> PlatenessReaderType;
typedef itk::Image<float, 3> VertexnessImageType;
typedef itk::ImageFileReader<VertexnessImageType> VertexnessReaderType;

typedef std::vector<itk::Index<3> > IndexSetType;



int main(int argc, char ** argv){

	//ttt::MySQLTissueTrackingProject2 m_Project;
	ttt::JSONTissueTrackingProject2 m_Project;

	m_Project.SetDirectory("/home/morgan/TTTProjects/miniroi/");
	//m_Project.SetProjectID(26);
	m_Project.Open();
	//m_Project.OpenProject(2);//Notum


	int init=0;
	//int end=m_Project.GetNumFrames();
	int end=13;


	int numFrames=end-init;
	std::vector<ttt::TrackedTissueDescriptor::Pointer> tissues(numFrames);

	vtkSmartPointer<vtkRenderer> renderer= vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow= vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor= vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderWindow->AddRenderer(renderer);
	renderWindowInteractor->SetRenderWindow(renderWindow);


	for(int t=0;t<m_Project.GetNumFrames();t++){
		tissues[t]=m_Project.GetTrackedTissueDescriptor(t);
	}


	ttt::TrackedTissueDescriptor::Pointer currentTissue=tissues[0];

	typedef std::vector<double> Color ;

	std::map<int,Color> m_TrackID2Color;


	for(int t=0;t<tissues.size()-1;t++){
		ttt::TrackedTissueDescriptor::Pointer nextTissue =tissues[t+1];

		BGL_FORALL_VERTICES(v,*(currentTissue->m_CellGraph),ttt::TrackedCellGraph){
			int ID=boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetID();

			std::vector<double> color;
			if(m_TrackID2Color.count(ID)){
				color=m_TrackID2Color[ID];
			}else{

				  //color=generator.generateRandomColor(rand());
				  color.push_back((double)rand()/RAND_MAX );
				  color.push_back((double)rand()/RAND_MAX );
				  color.push_back((double)rand()/RAND_MAX );
				  m_TrackID2Color[ID]=color;
			}

			ttt::TrackedCellVertexType nextV=ttt::CellID2VertexDescriptor(ID,nextTissue);
			if(nextV!=-1){

				itk::Point<double,3> a =boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetCentroid();
				itk::Point<double,3> b =boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),nextV).GetCentroid();

				vtkSmartPointer<vtkLineSource> line=vtkSmartPointer<vtkLineSource>::New();

					//line->SetPoint1(a[0],a[1],3*t);
				line->SetPoint1(a[0],3*t,a[1]);
					//line->SetPoint2(b[0],b[1],3*(t+1));
				line->SetPoint2(b[0],3*(t+1),b[1]);

				line->Update();
				vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
				mapper->SetInputConnection(line->GetOutputPort());
				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

				actor->GetProperty()->SetLineWidth(5);
				actor->GetProperty()->SetColor(color[0],color[1],color[2]);
				actor->SetMapper(mapper);


				renderer->AddActor(actor);
			}else{
				std::pair<ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType> children=CellParentID2VertexDescriptor(ID,nextTissue);
				if(children.first==-1 || children.second==-1) continue;

				itk::Point<double,3> a =boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetCentroid();
				itk::Point<double,3> b =boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.first).GetCentroid();

				itk::Point<double,3> c =boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetCentroid();
				itk::Point<double,3> d =boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.second).GetCentroid();
				int IDA=boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.first).GetID();
				int IDB=boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.second).GetID();

					m_TrackID2Color[IDA]=color;
					m_TrackID2Color[IDB]=color;


					vtkSmartPointer<vtkLineSource> lineA=vtkSmartPointer<vtkLineSource>::New();

					//lineA->SetPoint1(a[0],a[1],3*t);
					lineA->SetPoint1(a[0],3*t,a[1]);
					//lineA->SetPoint2(b[0],b[1],3*(t+1));
					lineA->SetPoint2(b[0],3*(t+1),b[1]);
					lineA->Update();
					vtkSmartPointer<vtkOpenGLPolyDataMapper> mapperA = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
					mapperA->SetInputConnection(lineA->GetOutputPort());
					vtkSmartPointer<vtkActor> actorA = vtkSmartPointer<vtkActor>::New();

					actorA->GetProperty()->SetColor(color[0],color[1],color[2]);
					actorA->SetMapper(mapperA);
					actorA->GetProperty()->SetLineWidth(5);

					renderer->AddActor(actorA);

					vtkSmartPointer<vtkLineSource> lineB=vtkSmartPointer<vtkLineSource>::New();

					//lineB->SetPoint1(c[0],c[1],3*t);
					lineB->SetPoint1(c[0],3*t,c[1]);
					lineB->SetPoint2(d[0],3*(t+1),d[1]);
					lineB->Update();
					vtkSmartPointer<vtkOpenGLPolyDataMapper> mapperB = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
					mapperB->SetInputConnection(lineB->GetOutputPort());
					vtkSmartPointer<vtkActor> actorB = vtkSmartPointer<vtkActor>::New();

					actorB->GetProperty()->SetColor(color[0],color[1],color[2]);
					actorB->SetMapper(mapperB);

					actorB->GetProperty()->SetLineWidth(5);

					renderer->AddActor(actorB);



				}
			}
			currentTissue=nextTissue;

		}
		renderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel
		renderWindow->Render();

		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput(renderWindow);
		windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
		windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
		windowToImageFilter->Update();

		vtkSmartPointer<vtkTIFFWriter> writer = vtkSmartPointer<vtkTIFFWriter>::New();
		writer->SetFileName(fileName.c_str());
		writer->SetInputConnection(windowToImageFilter->GetOutputPort());
		writer->Write();

		renderWindowInteractor->Start();

	}


}


#if 0
int main(int argc, char ** argv) {
#if 0
	//2. Leer gt

	ttt::QTSQLTissueTrackingProject2 m_Project;
	m_Project.SetHost("localhost");
	m_Project.SetDBName("TuftsTissueTracker");
	m_Project.SetUser("root");
	m_Project.SetPassword("ttt1Tracker");
	m_Project.SetProjectID(16);
	m_Project.Open();
#endif

	ttt::JSONTissueTrackingProject2 m_Project;

	m_Project.SetDirectory("/home/morgan/TTTProjects/miniroi/");

	m_Project.Open();

	ttt::CellPolygonDrawer<ttt::TrackedTissueDescriptor> drawer;
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();

	renWin->AddRenderer(renderer);
	drawer.SetRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<	vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	renWin->SetAlphaBitPlanes(1); //enable usage of alpha channel

	std::vector<ttt::TrackedTissueDescriptor::Pointer> tissues(numFrames);
	int k=0;
	for(int t=init;t<end;t++){
		tissues[k++]=m_Project.GetTrackedTissueDescriptor(t);
	}
	DrawTrackVolume("test-gt.tiff",tissues);


	//iren->Start();
	for (unsigned int t = 0; t < m_Project.GetNumFrames(); t++) {
		ttt::TrackedTissueDescriptor::Pointer gt = m_Project.GetTrackedTissueDescriptor(t);
		std::stringstream fileNameStream;
		fileNameStream << "leg-" << t << ".tiff";
		std::string fileName;
		fileNameStream >> fileName;
		//drawGraph(gt, fileName);

		drawer.SetTissueDescriptor(gt);
		drawer.Draw();
		drawer.Show();
		renWin->Render();
		renderer->Render();

		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput(renWin);
		windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
		windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
		windowToImageFilter->Update();

		vtkSmartPointer<vtkTIFFWriter> writer =	vtkSmartPointer<vtkTIFFWriter>::New();
		writer->SetFileName(fileName.c_str());

		writer->SetInputConnection(windowToImageFilter->GetOutputPort());
		writer->Write();

	}

}
#endif
