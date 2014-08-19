/*
 * ProjectTableModel.h
 *
 *  Created on: Jul 28, 2014
 *      Author: morgan
 */

#ifndef PROJECTTABLEMODEL_H_
#define PROJECTTABLEMODEL_H_

#include <QAbstractTableModel>
#include "tissuetrackingabstractproject2.h"
namespace ttt {

class ProjectTableModel: public QAbstractTableModel {
private:
	ttt::TissueTrackingAbstractProject2 * m_Project;
	const static int NUM_COLS=6; //PlatenessLow PlatenessHigh Platenes sSteps VertexnessLow VertexnessHigh VertexnessSteps
public:
	ProjectTableModel(ttt::TissueTrackingAbstractProject2 * project,QObject * parent);
	virtual ~ProjectTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //Qt::ItemFlags flags(const QModelIndex & index) const ;

};

}



#endif /* CELLFEATURETABLEMODEL_H_ */

