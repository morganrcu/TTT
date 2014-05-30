/*
 * CellFeatureTableModel.h
 *
 *  Created on: May 22, 2014
 *      Author: morgan
 */

#ifndef CELLFEATURETABLEMODEL_H_
#define CELLFEATURETABLEMODEL_H_
#include <QAbstractTableModel>
#include "mysqltissuetrackingproject.h"
class CellFeatureTableModel : public QAbstractTableModel{
	Q_OBJECT
	ttt::TissueTrackingProject * m_Project;
public:
	CellFeatureTableModel(ttt::TissueTrackingProject * project,QObject * parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //Qt::ItemFlags flags(const QModelIndex & index) const ;
	virtual ~CellFeatureTableModel();
};

#endif /* CELLFEATURETABLEMODEL_H_ */

