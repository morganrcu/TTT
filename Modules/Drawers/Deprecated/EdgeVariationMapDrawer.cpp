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


        std::map<giaa::CellVertexType,double> varval = this->GetEdgeVariance();
        std::map<giaa::CellVertexType,int > result = this->GetStdevBin(varval);
        int cont;
        BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph){
            Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v);
            double var = varval[v];
            if (var > -1){
              int bin = result[v]+3;
              ColorCell(a,bin,1);
              //renWin->Render();
              //scanf("%d",&cont);
            }
        }
