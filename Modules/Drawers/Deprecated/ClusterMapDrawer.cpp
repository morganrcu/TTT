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


BGL_FORALL_VERTICES(v,descriptor->m_CellGraph,giaa::CellGraph){
                Cell a = boost::get(giaa::CellPropertyTag(),descriptor->m_CellGraph,v);
                    if(a.trackID != -1 && a.trackID != 380){
                       ColorCell(a,clusterid[a.trackID],1);
                    }
                }
