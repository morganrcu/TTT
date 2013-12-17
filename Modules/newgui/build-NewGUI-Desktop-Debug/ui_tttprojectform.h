/********************************************************************************
** Form generated from reading UI file 'tttprojectform.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TTTPROJECTFORM_H
#define UI_TTTPROJECTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TTTProjectForm
{
public:
    QAction *actionAddFrame;
    QAction *actionRemoveFrame;
    QAction *actionFrameUp;
    QAction *actionFrameDown;
    QGridLayout *gridLayout;
    QTableView *tableView;
    QGroupBox *butonGroupBox;
    QVBoxLayout *verticalLayout;
    QPushButton *addFrameButton;
    QPushButton *pushButton_2;
    QGroupBox *projectGroupBox;
    QGroupBox *previewGroupBox;

    void setupUi(QWidget *TTTProjectForm)
    {
        if (TTTProjectForm->objectName().isEmpty())
            TTTProjectForm->setObjectName(QStringLiteral("TTTProjectForm"));
        TTTProjectForm->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TTTProjectForm->sizePolicy().hasHeightForWidth());
        TTTProjectForm->setSizePolicy(sizePolicy);
        actionAddFrame = new QAction(TTTProjectForm);
        actionAddFrame->setObjectName(QStringLiteral("actionAddFrame"));
        actionRemoveFrame = new QAction(TTTProjectForm);
        actionRemoveFrame->setObjectName(QStringLiteral("actionRemoveFrame"));
        actionFrameUp = new QAction(TTTProjectForm);
        actionFrameUp->setObjectName(QStringLiteral("actionFrameUp"));
        actionFrameDown = new QAction(TTTProjectForm);
        actionFrameDown->setObjectName(QStringLiteral("actionFrameDown"));
        gridLayout = new QGridLayout(TTTProjectForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableView = new QTableView(TTTProjectForm);
        tableView->setObjectName(QStringLiteral("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);

        butonGroupBox = new QGroupBox(TTTProjectForm);
        butonGroupBox->setObjectName(QStringLiteral("butonGroupBox"));
        verticalLayout = new QVBoxLayout(butonGroupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        addFrameButton = new QPushButton(butonGroupBox);
        addFrameButton->setObjectName(QStringLiteral("addFrameButton"));

        verticalLayout->addWidget(addFrameButton);

        pushButton_2 = new QPushButton(butonGroupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);


        gridLayout->addWidget(butonGroupBox, 0, 1, 1, 1);

        projectGroupBox = new QGroupBox(TTTProjectForm);
        projectGroupBox->setObjectName(QStringLiteral("projectGroupBox"));

        gridLayout->addWidget(projectGroupBox, 3, 0, 1, 1);

        previewGroupBox = new QGroupBox(TTTProjectForm);
        previewGroupBox->setObjectName(QStringLiteral("previewGroupBox"));

        gridLayout->addWidget(previewGroupBox, 2, 1, 1, 1);


        retranslateUi(TTTProjectForm);

        QMetaObject::connectSlotsByName(TTTProjectForm);
    } // setupUi

    void retranslateUi(QWidget *TTTProjectForm)
    {
        TTTProjectForm->setWindowTitle(QApplication::translate("TTTProjectForm", "Form", 0));
        actionAddFrame->setText(QApplication::translate("TTTProjectForm", "Add Frame", 0));
#ifndef QT_NO_TOOLTIP
        actionAddFrame->setToolTip(QApplication::translate("TTTProjectForm", "Add New Frame", 0));
#endif // QT_NO_TOOLTIP
        actionAddFrame->setShortcut(QApplication::translate("TTTProjectForm", "Ctrl+I", 0));
        actionRemoveFrame->setText(QApplication::translate("TTTProjectForm", "Remove Frame", 0));
#ifndef QT_NO_TOOLTIP
        actionRemoveFrame->setToolTip(QApplication::translate("TTTProjectForm", "Remove Frame", 0));
#endif // QT_NO_TOOLTIP
        actionRemoveFrame->setShortcut(QApplication::translate("TTTProjectForm", "Ctrl+D", 0));
        actionFrameUp->setText(QApplication::translate("TTTProjectForm", "Up", 0));
#ifndef QT_NO_TOOLTIP
        actionFrameUp->setToolTip(QApplication::translate("TTTProjectForm", "Move Frame Up", 0));
#endif // QT_NO_TOOLTIP
        actionFrameUp->setShortcut(QApplication::translate("TTTProjectForm", "Ctrl+Up", 0));
        actionFrameDown->setText(QApplication::translate("TTTProjectForm", "Down", 0));
        actionFrameDown->setShortcut(QApplication::translate("TTTProjectForm", "Ctrl+Down", 0));
        butonGroupBox->setTitle(QApplication::translate("TTTProjectForm", "Actions", 0));
        addFrameButton->setText(QApplication::translate("TTTProjectForm", "Add Frame...", 0));
        pushButton_2->setText(QApplication::translate("TTTProjectForm", "Remove Frame...", 0));
        projectGroupBox->setTitle(QApplication::translate("TTTProjectForm", "Project", 0));
        previewGroupBox->setTitle(QApplication::translate("TTTProjectForm", "Preview", 0));
    } // retranslateUi

};

namespace Ui {
    class TTTProjectForm: public Ui_TTTProjectForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TTTPROJECTFORM_H
