/********************************************************************************
** Form generated from reading UI file 'tttpreferences.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TTTPREFERENCES_H
#define UI_TTTPREFERENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tttPreferences
{
public:
    QAction *actionTest;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *spinBox;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QPushButton *testButton;
    QFrame *frame;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *tttPreferences)
    {
        if (tttPreferences->objectName().isEmpty())
            tttPreferences->setObjectName(QStringLiteral("tttPreferences"));
        tttPreferences->resize(400, 300);
        actionTest = new QAction(tttPreferences);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        verticalLayout = new QVBoxLayout(tttPreferences);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(tttPreferences);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        formLayout = new QFormLayout(tab_2);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        spinBox = new QSpinBox(tab_2);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, spinBox);

        lineEdit_2 = new QLineEdit(tab_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_2);

        lineEdit_3 = new QLineEdit(tab_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_3);

        testButton = new QPushButton(tab_2);
        testButton->setObjectName(QStringLiteral("testButton"));

        formLayout->setWidget(4, QFormLayout::FieldRole, testButton);

        frame = new QFrame(tab_2);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        formLayout->setWidget(4, QFormLayout::LabelRole, frame);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(tttPreferences);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(tttPreferences);
        QObject::connect(buttonBox, SIGNAL(accepted()), tttPreferences, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), tttPreferences, SLOT(reject()));
        QObject::connect(testButton, SIGNAL(clicked()), actionTest, SLOT(trigger()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(tttPreferences);
    } // setupUi

    void retranslateUi(QDialog *tttPreferences)
    {
        tttPreferences->setWindowTitle(QApplication::translate("tttPreferences", "Dialog", 0));
        actionTest->setText(QApplication::translate("tttPreferences", "test", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("tttPreferences", "General", 0));
        label->setText(QApplication::translate("tttPreferences", "Host:", 0));
        label_2->setText(QApplication::translate("tttPreferences", "Port:", 0));
        label_3->setText(QApplication::translate("tttPreferences", "User:", 0));
        label_4->setText(QApplication::translate("tttPreferences", "Password:", 0));
        testButton->setText(QApplication::translate("tttPreferences", "Test", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("tttPreferences", "Database", 0));
    } // retranslateUi

};

namespace Ui {
    class tttPreferences: public Ui_tttPreferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TTTPREFERENCES_H
