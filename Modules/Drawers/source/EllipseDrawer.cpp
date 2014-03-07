//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

#include <vtkParametricEllipsoid.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPolyDataMapper.h>
#include "EllipseDrawer.h"
void ttt::EllipseDrawer::Reset(){
	for(std::vector<vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		m_Renderer->RemoveActor(*it);
	}
	m_Actors.clear();
}
void ttt::EllipseDrawer::Draw(){
	this->Reset();
	for(std::map<unsigned long int,Ellipse<double> >::iterator it= (*m_Ellipses).begin();it!=(*m_Ellipses).end();it++){

		Ellipse<double> ellipse = it->second;
        vtkSmartPointer<vtkParametricEllipsoid> parametricEllipsoid = vtkSmartPointer<vtkParametricEllipsoid>::New();
        parametricEllipsoid->SetXRadius(ellipse.m_Xrad);
        parametricEllipsoid->SetYRadius(ellipse.m_Yrad);


        vtkSmartPointer<vtkParametricFunctionSource> funcsrc = vtkSmartPointer<vtkParametricFunctionSource>::New();
        funcsrc->SetParametricFunction(parametricEllipsoid);
        funcsrc->Update();
        vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputConnection(funcsrc->GetOutputPort());
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(map);
        actor->SetPosition(ellipse.m_Ycent,ellipse.m_Xcent,0);
        double degrot = ellipse.m_Theta*180/M_PI;
        actor->RotateZ(degrot);
        actor->VisibilityOff();
        m_Renderer->AddActor(actor);
        m_Actors.push_back(actor);

	}
#if 0
	int cont;
	BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph) {
            ProjectType::SpacingType s = m_Project.GetSpacing();
            /*std::vector<SkeletonVertexType> svt = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v).m_SkeletonNodes;
            std::vector<itk::Point<double,3> > points;
            for(std::vector<SkeletonVertexType>::iterator itr = svt.begin(); itr != svt.end(); ++itr){
                itk::Point<double,3> pos = boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,*itr).position;
                points.push_back(pos);
            }*/

            EllipseG a = boost::get(giaa::EllipsePropertyTag(),descriptor->m_CellGraph,v);
            //a.CalcEllipseValues(points);
            //std::vector<double> ell_color = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v).m_Color;
            std::vector<double> ell_color;
            for(int i = 0 ; i < 3; i++){
                ell_color.push_back(1);
            }
            itk::Point<double,3> center = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v).m_Centroid;
            std::cout<< v << " " << s[0] << " " << s[1] << " " << a.xrad << std::endl;
            if(!isnan(a.xrad)&& a.xrad < 100){
                //giaaTovktVertexToDual[v]->GetProperty()->SetColor(1,0,0);
                std::vector<SkeletonVertexType> svt = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v).m_SkeletonNodes;
                //std::cout << v << " " << svt.size() << std::endl;
                /*for(std::vector<SkeletonVertexType>::iterator itr = svt.begin(); itr != svt.end(); ++itr){
                    giaaTovtkVertexToPrimal[*itr]->GetProperty()->SetColor(0,0,0);
                }*/
                vtkSmartPointer<vtkParametricEllipsoid> ellipse = vtkSmartPointer<vtkParametricEllipsoid>::New();
                ellipse->SetXRadius(a.xrad*s[0]);
                ellipse->SetYRadius(a.yrad*s[1]);

                vtkSmartPointer<vtkParametricFunctionSource> funcsrc = vtkSmartPointer<vtkParametricFunctionSource>::New();
                funcsrc->SetParametricFunction(ellipse);
                funcsrc->Update();
                vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
                map->SetInputConnection(funcsrc->GetOutputPort());
                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(map);
                actor->SetPosition(a.xcent*s[0],a.ycent*s[1],0);
                double degrot = a.theta*180/M_PI;
                actor->RotateZ(degrot);
                actor->GetProperty()->SetColor(ell_color[0],ell_color[1],ell_color[2]);
                //m_CurrentRenderer->AddActor(actor);
                //m_RenderWindow->Render();

                vtkSmartPointer<vtkLineSource> L1 = vtkSmartPointer<vtkLineSource>::New();
                double hyp_half = a.xrad;
                double pta1_x = a.xcent + hyp_half*cos(a.theta);
                double pta1_y = a.ycent + hyp_half*sin(a.theta);
                double pta2_x = a.xcent + hyp_half*cos(M_PI + a.theta);
                double pta2_y = a.ycent + hyp_half*sin(M_PI + a.theta);
                L1->SetPoint1(s[0]*pta1_x,s[1]*pta1_y,center[2]*s[2]);
                L1->SetPoint2(s[0]*pta2_x,s[1]*pta2_y,center[2]*s[2]);

                vtkSmartPointer<vtkPolyDataMapper> mapper_a = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper_a->SetInputConnection(L1->GetOutputPort());

                double dista = sqrt(pow(pta1_x - pta2_x,2) + pow(pta1_y - pta2_y,2));
                //std::cout<< "Look here " << a.xrad << " " << dista << std::endl;

                vtkSmartPointer<vtkLineSource> L2 = vtkSmartPointer<vtkLineSource>::New();
                hyp_half = a.yrad;
                double ptb1_x = a.xcent + hyp_half*cos(M_PI/2+a.theta);
                double ptb1_y = a.ycent + hyp_half*sin(M_PI/2+a.theta);
                double ptb2_x = a.xcent + hyp_half*cos(M_PI *1.5 + a.theta);
                double ptb2_y = a.ycent + hyp_half*sin(M_PI *1.5 + a.theta);
                L2->SetPoint1(s[0]*ptb1_x,s[1]*ptb1_y,center[2]*s[2]);
                L2->SetPoint2(s[0]*ptb2_x,s[1]*ptb2_y,center[2]*s[2]);


                vtkSmartPointer<vtkPolyDataMapper> mapper_b = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper_b->SetInputConnection(L2->GetOutputPort());

                vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
                actor2->SetMapper(mapper_a);
                actor2->GetProperty()->SetLineWidth(3);
                actor2->GetProperty()->SetColor(0,0,0);
                m_CurrentRenderer->AddActor(actor2);

                vtkSmartPointer<vtkActor> actor3 = vtkSmartPointer<vtkActor>::New();
                actor3->SetMapper(mapper_b);
                actor3->GetProperty()->SetLineWidth(3);
                actor3->GetProperty()->SetColor(0,0,0);
                m_CurrentRenderer->AddActor(actor3);
                //renWin->Render();
            }

            else{
                std::cout<<"entering nan"<<std::endl;
               /* std::vector<SkeletonVertexType> svt = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v).m_SkeletonNodes;
                std::vector<double > distances;
                itk::Point<double,3> center = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v).m_Centroid;
                for(std::vector<SkeletonVertexType>::iterator itr = svt.begin(); itr != svt.end(); ++itr){
                    itk::Point<double,3> pos = boost::get(giaa::SkeletonPointPropertyTag(),descriptor->m_SkeletonGraph,*itr).position;
                    double dist = sqrt(pow(center[0]-pos[0],2)+pow(center[1]-pos[1],2)+pow(center[2]-pos[2],2));
                    distances.push_back(dist);
                }

                double sum = 0.0;
                for(int i = 0; i < distances.size(); i++){
                    sum+=distances[i];
                }

                double mean = sum/distances.size();

                vtkSmartPointer<vtkRegularPolygonSource> polygonSource = vtkSmartPointer<vtkRegularPolygonSource>::New();

              //polygonSource->GeneratePolygonOff();
              polygonSource->SetNumberOfSides(50);
              polygonSource->SetRadius(mean*s[0]);
              polygonSource->SetCenter(center[0]*s[0],center[1]*s[1],center[2]*s[2]);
              polygonSource->Update();

              // Visualize
              vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
              mapper->SetInputConnection(polygonSource->GetOutputPort());
              vtkSmartPointer<vtkActor> actor =
                vtkSmartPointer<vtkActor>::New();
              actor->SetMapper(mapper);
              m_CurrentRenderer->AddActor(actor);
              //renWin->Render();*/

            }

        }
#endif
}
void ttt::EllipseDrawer::Show(){
	for(std::vector<vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		vtkSmartPointer<vtkActor> actor = *it;
		actor->VisibilityOn();
	}

}
void ttt::EllipseDrawer::Hide(){
	for(std::vector<vtkSmartPointer<vtkActor> >::iterator it= m_Actors.begin();it!=m_Actors.end();it++){
		vtkSmartPointer<vtkActor> actor = *it;
		actor->VisibilityOff();
	}
}
