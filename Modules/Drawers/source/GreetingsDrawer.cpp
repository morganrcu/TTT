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



#include <vtkSmartPointer.h>
#include <vtkTextSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include "GreetingsDrawer.h"

ttt::GreetingsDrawer::GreetingsDrawer(){

}

void ttt::GreetingsDrawer::Draw() {

    vtkSmartPointer<vtkTextSource> text = vtkSmartPointer<vtkTextSource>::New();
    text->SetText("Tufts Tissue tracker 0.9");
    text->BackingOff();
    text->Update();

    vtkSmartPointer<vtkPolyDataMapper> textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    textMapper->SetInputConnection(text->GetOutputPort());

    vtkSmartPointer<vtkActor> textActor = vtkSmartPointer<vtkActor>::New();
    textActor->SetMapper(textMapper);

    m_Renderer->AddActor(textActor);

}
