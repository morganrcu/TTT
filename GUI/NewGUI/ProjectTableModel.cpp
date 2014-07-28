/*
 * ProjectTableModel.cpp
 *
 *  Created on: Jul 28, 2014
 *      Author: morgan
 */

#include <ProjectTableModel.h>

namespace ttt {

ProjectTableModel::ProjectTableModel(ttt::TissueTrackingAbstractProject2 * project, QObject *parent) :QAbstractTableModel(parent) {
	m_Project=project;
}

ProjectTableModel::~ProjectTableModel() {

}


int ProjectTableModel::rowCount(const QModelIndex &parent)const{
	return m_Project->GetNumFrame();
}
int ProjectTableModel::columnCount(const QModelIndex &parent)const{
	return NUM_COLS;
}
QVariant ProjectTableModel::data(const QModelIndex &index, int role )const{
	switch(role){
		case Qt::DisplayRole:
			switch(index.column()){
			case 0:
				return QVariant(m_Project->GetLowestPlatenessScale(index.column()));
			case 1:
				return QVariant(m_Project->GetHighestPlatenessScale(index.column()));
			case 2:
				return QVariant(m_Project->GetPlatenessSteps(index.column()));
			case 3:
				return QVariant(m_Project->GetLowestVertexnessScale(index.column()));
			case 4:
				return QVariant(m_Project->GetHighestVertexnessScale(index.column()));
			case 5:
				return QVariant(m_Project->GetVertexnessSteps(index.column()));
		}
		break;
		case Qt::CheckStateRole:
	            return QVariant();
	    }
		return QVariant("");
}
QVariant ProjectTableModel::headerData(int section, Qt::Orientation orientation, int role)const{
	  if (role == Qt::DisplayRole)
	    {
	        if (orientation == Qt::Horizontal) {
	            switch (section)
	            {
	            case 0:
	            	return QString("Lower Plateness");
	            case 1:
	                return QString("Higher Plateness");
	            case 2:
	        		return QString("Plateness Steps");
	        	case 3:
	        		return QString("Lower Vertexness");
	        	case 4:
	        		return QString("Higher Vertexness");
	        	case 5:
	        		return QString("Vertexness Steps");
	        	}
	        }
	    }
	    return QVariant();
}


} /* namespace ttt */
