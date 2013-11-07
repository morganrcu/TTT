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



        std::vector<double> area;
        std::cout<< "in area map"<<std::endl;
        BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph){
            Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v);
            //double areaval = a.GetArea(descriptor->m_SkeletonGraph);
            //a.m_area = areaval;
            std::cout<< v << " " << a.m_area << std::endl;
            area.push_back(a.m_area);   
        }

        
        std::sort(area.begin(),area.end());

        double areamax = area[area.size()-1];
        double areamin = area[0];

        double binsize = areamax/6.0;

        std::cout<< "values" << areamin << " " << areamax << " " << binsize <<std::endl;

        int cont;  

        std::vector<int> vals;
        for(int i = 0; i < 6;i++){
            vals.push_back(0);
        }

        BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph){
            Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v);
             itk::Point<double,3> pos = a.m_Centroid;
             //double areaval = a.GetArea(descriptor->m_SkeletonGraph);
            //std::cout<<"getting bin"<<std::endl;               
            int bin = this->GetBin(areamin, areamax, 6, a.m_area);
            ColorCell(a,bin,1);
            vals[bin]++;
            //scanf("%d",&cont);
        }
        this->SetUpAreaPlot(vals);
