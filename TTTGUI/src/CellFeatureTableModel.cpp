/*
 * CellFeatureTableModel.cpp
 *
 *  Created on: May 22, 2014
 *      Author: morgan
 */

#include <CellFeatureTableModel.h>
#include <QVector3D>
CellFeatureTableModel::CellFeatureTableModel(ttt::TissueTrackingAbstractProject2<3> * project, unsigned int frame,QObject *parent) :QAbstractTableModel(parent) {
	m_Project=project;
	m_Frame=frame;

	m_TrackedCentroids= m_Project->GetTrackedCentroids(m_Frame);
	m_TrackedAreas= m_Project->GetTrackedAreas(m_Frame);
	m_TrackedPerimeters= m_Project->GetTrackedPerimeter(m_Frame);
	m_TrackedXX=m_Project->GetTrackedXX(m_Frame);
	m_TrackedXY=m_Project->GetTrackedXY(m_Frame);
	m_TrackedYY=m_Project->GetTrackedYY(m_Frame);

	m_Descriptor = m_Project->GetTrackedTissueDescriptor(m_Frame);

}

int CellFeatureTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_Descriptor->GetNumCells();
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

	switch(role){
	case Qt::DisplayRole:

		switch(index.column()){
		case 0:
			return QVariant(boost::get(ttt::TrackedCellPropertyTag<3>(),m_Descriptor->GetCellGraph(),index.row()).GetID());
		case 1:
		{
			itk::Point<double,3> point=this->m_TrackedCentroids[index.row()].GetValue();
			return QString("[%1,%2,%3]").arg(point[0]).arg(point[1]).arg(point[2]);
		}

		case 2:
		{
			//itk::Vector<double,3> velocity=boost::get(ttt::TrackedCellPropertyTag(),*descriptor->m_CellGraph,index.row()).GetVelocity();
			//return QString("[%1,%2,%3]").arg(velocity[0]).arg(velocity[1]).arg(velocity[2]);
			return QVariant();
		}
		case 3:
			return QVariant((double)this->m_TrackedAreas[index.row()].GetValue());
		case 4:

			return QVariant((double)this->m_TrackedPerimeters[index.row()].GetValue());
		case 5:
			return QVariant((double)this->m_TrackedXX[index.row()].GetValue());
		case 6:
			return QVariant((double)this->m_TrackedYY[index.row()].GetValue());
		case 7:
			return QVariant((double)this->m_TrackedXY[index.row()].GetValue());
	}
	break;
	case Qt::CheckStateRole:

            return QVariant();
    }
	return QVariant("");
}
