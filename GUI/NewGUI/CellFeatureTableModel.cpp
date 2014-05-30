/*
 * CellFeatureTableModel.cpp
 *
 *  Created on: May 22, 2014
 *      Author: morgan
 */

#include <CellFeatureTableModel.h>
#include <QVector3D>
CellFeatureTableModel::CellFeatureTableModel(ttt::TissueTrackingProject * project, QObject *parent) :QAbstractTableModel(parent) {
	m_Project=project;
}

int CellFeatureTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_Project->GetTrackedTissueDescriptor()->GetNumCells();
}

CellFeatureTableModel::~CellFeatureTableModel() {
	// TODO Auto-generated destructor stub
}
#if 0
Qt::ItemFlags CellFeatureTableModel::flags(const QModelIndex & index) const {
	//return  Qt::NoItemFlags;
}
#endif
int CellFeatureTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 8; //TrackID Centroid Velocity Area Perimeter Width Height Rotation
}

QVariant CellFeatureTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("ID");
            case 1:
            	return QString("centroid");
            case 2:
                return QString("Velocity");
            case 3:
        		return QString("Area");
        	case 4:
        		return QString("Perimeter");
        	case 5:
        		return QString("Width");
        	case 6:
        		return QString("Height");
        	case 7:
        		return QString("Rotation");
        	}
        }
    }
    return QVariant();
}

QVariant CellFeatureTableModel::data(const QModelIndex &index, int role) const
{
	ttt::TrackedTissueDescriptor::Pointer descriptor = m_Project->GetTrackedTissueDescriptor();
	switch(role){
	case Qt::DisplayRole:

		switch(index.column()){
		case 0:
			return QVariant(boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,index.row()).GetID());
		case 1:
		{
			itk::Point<double,3> point=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,index.row()).GetCentroid();
			return QString("[%1,%2,%3]").arg(point[0]).arg(point[1]).arg(point[2]);
		}

		case 2:
		{
			itk::Vector<double,3> velocity=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,index.row()).GetVelocity();
			return QString("[%1,%2,%3]").arg(velocity[0]).arg(velocity[1]).arg(velocity[2]);
		}
		case 3:
			return QString("area");
		case 4:
			return QString("perimeter");
		case 5:
			return QString("width");
		case 6:
			return QString("height");
		case 7:
			return QString("rotation");
	}
	break;
	case Qt::CheckStateRole:

            return QVariant();
    }
	return QVariant("");
}
