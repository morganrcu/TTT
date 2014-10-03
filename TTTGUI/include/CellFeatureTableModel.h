/*
 * CellFeatureTableModel.h
 *
 *  Created on: May 22, 2014
 *      Author: morgan
 */

#ifndef CELLFEATURETABLEMODEL_H_
#define CELLFEATURETABLEMODEL_H_
#include <QAbstractTableModel>
#include "tissuetrackingabstractproject2.h"
#include "FeatureMap.h"
class CellFeatureTableModel : public QAbstractTableModel{
	Q_OBJECT
	ttt::TissueTrackingAbstractProject2 * m_Project;
	unsigned int m_Frame;

	FeatureMap<ttt::TrackedCellVertexType,itk::Point<double,3> > m_TrackedCentroids;
	FeatureMap<ttt::TrackedCellVertexType,double> m_TrackedAreas;
	FeatureMap<ttt::TrackedCellVertexType,double > m_TrackedPerimeters;
	FeatureMap<ttt::TrackedCellVertexType,double > m_TrackedXX;
	FeatureMap<ttt::TrackedCellVertexType,double > m_TrackedXY;
	FeatureMap<ttt::TrackedCellVertexType,double > m_TrackedYY;
	ttt::TrackedTissueDescriptor::Pointer m_Descriptor;


public:
	CellFeatureTableModel(ttt::TissueTrackingAbstractProject2 * project,unsigned int frame,QObject * parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //Qt::ItemFlags flags(const QModelIndex & index) const ;
	virtual ~CellFeatureTableModel();
};

#endif /* CELLFEATURETABLEMODEL_H_ */

