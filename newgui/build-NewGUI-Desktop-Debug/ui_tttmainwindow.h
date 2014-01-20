/********************************************************************************
** Form generated from reading UI file 'tttmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TTTMAINWINDOW_H
#define UI_TTTMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TTTMainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionFrameUp;
    QAction *actionFrameDown;
    QAction *actionAddFrame;
    QAction *actionRemoveFrame;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *projectTab;
    QHBoxLayout *horizontalLayout_13;
    QWidget *widget_4;
    QFormLayout *formLayout;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_4;
    QLineEdit *nameLineEdit;
    QGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QLabel *xLabel;
    QDoubleSpinBox *xDoubleSpinBox;
    QLabel *yLabel;
    QDoubleSpinBox *yDoubleSpinBox;
    QLabel *zLabel;
    QDoubleSpinBox *zDoubleSpinBox;
    QLabel *label;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_3;
    QLabel *incrementLabel;
    QDoubleSpinBox *incrementDoubleSpinBox;
    QLabel *label_2;
    QPushButton *pushButton_5;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QTableView *framesTable;
    QWidget *preprocessingTab;
    QWidget *scaleSelectionTab;
    QVBoxLayout *verticalLayout;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSlider *horizontalSlider;
    QPushButton *pushButton_6;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget_2;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_6;
    QFormLayout *formLayout_5;
    QLabel *scaleLabel;
    QDoubleSpinBox *scaleDoubleSpinBox;
    QPushButton *pushButton_7;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QWidget *tab_2;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_8;
    QFormLayout *formLayout_6;
    QLabel *label_3;
    QSpinBox *spinBox;
    QPushButton *pushButton_8;
    QWidget *widget_9;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_3;
    QWidget *enhancementTab;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_10;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QSlider *horizontalSlider_2;
    QPushButton *pushButton_9;
    QWidget *widget_11;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_6;
    QFrame *frame_5;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_7;
    QFrame *frame_4;
    QWidget *widget_12;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QSpinBox *spinBox_2;
    QPushButton *pushButton_10;
    QWidget *vertexLocationTab;
    QVBoxLayout *verticalLayout_8;
    QWidget *widget_13;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QSlider *horizontalSlider_3;
    QPushButton *pushButton_11;
    QWidget *widget_14;
    QHBoxLayout *horizontalLayout_10;
    QWidget *widget_15;
    QFormLayout *formLayout_7;
    QLabel *thresholdLabel;
    QDoubleSpinBox *thresholdDoubleSpinBox;
    QPushButton *pushButton_12;
    QGroupBox *groupBox_6;
    QFormLayout *formLayout_8;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QWidget *widget_16;
    QVBoxLayout *verticalLayout_9;
    QFrame *frame_6;
    QWidget *cellSegmentationTab;
    QVBoxLayout *verticalLayout_10;
    QWidget *widget_17;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_8;
    QSlider *horizontalSlider_4;
    QPushButton *pushButton_16;
    QWidget *widget_18;
    QHBoxLayout *horizontalLayout_12;
    QWidget *widget_19;
    QFormLayout *formLayout_9;
    QLabel *levelSetThresholdLabel;
    QDoubleSpinBox *levelSetThresholdDoubleSpinBox;
    QPushButton *pushButton_17;
    QWidget *widget_20;
    QVBoxLayout *verticalLayout_11;
    QFrame *frame_7;
    QWidget *cellTrackingTab;
    QWidget *analyticsTab;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TTTMainWindow)
    {
        if (TTTMainWindow->objectName().isEmpty())
            TTTMainWindow->setObjectName(QStringLiteral("TTTMainWindow"));
        TTTMainWindow->resize(1024, 768);
        actionNew = new QAction(TTTMainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(TTTMainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionFrameUp = new QAction(TTTMainWindow);
        actionFrameUp->setObjectName(QStringLiteral("actionFrameUp"));
        actionFrameDown = new QAction(TTTMainWindow);
        actionFrameDown->setObjectName(QStringLiteral("actionFrameDown"));
        actionAddFrame = new QAction(TTTMainWindow);
        actionAddFrame->setObjectName(QStringLiteral("actionAddFrame"));
        actionRemoveFrame = new QAction(TTTMainWindow);
        actionRemoveFrame->setObjectName(QStringLiteral("actionRemoveFrame"));
        centralWidget = new QWidget(TTTMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        projectTab = new QWidget();
        projectTab->setObjectName(QStringLiteral("projectTab"));
        horizontalLayout_13 = new QHBoxLayout(projectTab);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        widget_4 = new QWidget(projectTab);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        formLayout = new QFormLayout(widget_4);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        groupBox_3 = new QGroupBox(widget_4);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        formLayout_4 = new QFormLayout(groupBox_3);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        nameLineEdit = new QLineEdit(groupBox_3);
        nameLineEdit->setObjectName(QStringLiteral("nameLineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(nameLineEdit->sizePolicy().hasHeightForWidth());
        nameLineEdit->setSizePolicy(sizePolicy);

        formLayout_4->setWidget(0, QFormLayout::SpanningRole, nameLineEdit);


        formLayout->setWidget(0, QFormLayout::SpanningRole, groupBox_3);

        groupBox = new QGroupBox(widget_4);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        xLabel = new QLabel(groupBox);
        xLabel->setObjectName(QStringLiteral("xLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, xLabel);

        xDoubleSpinBox = new QDoubleSpinBox(groupBox);
        xDoubleSpinBox->setObjectName(QStringLiteral("xDoubleSpinBox"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, xDoubleSpinBox);

        yLabel = new QLabel(groupBox);
        yLabel->setObjectName(QStringLiteral("yLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, yLabel);

        yDoubleSpinBox = new QDoubleSpinBox(groupBox);
        yDoubleSpinBox->setObjectName(QStringLiteral("yDoubleSpinBox"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, yDoubleSpinBox);

        zLabel = new QLabel(groupBox);
        zLabel->setObjectName(QStringLiteral("zLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, zLabel);

        zDoubleSpinBox = new QDoubleSpinBox(groupBox);
        zDoubleSpinBox->setObjectName(QStringLiteral("zDoubleSpinBox"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, zDoubleSpinBox);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout_2->setWidget(3, QFormLayout::SpanningRole, label);


        formLayout->setWidget(1, QFormLayout::SpanningRole, groupBox);

        groupBox_2 = new QGroupBox(widget_4);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout_3 = new QFormLayout(groupBox_2);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        incrementLabel = new QLabel(groupBox_2);
        incrementLabel->setObjectName(QStringLiteral("incrementLabel"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, incrementLabel);

        incrementDoubleSpinBox = new QDoubleSpinBox(groupBox_2);
        incrementDoubleSpinBox->setObjectName(QStringLiteral("incrementDoubleSpinBox"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, incrementDoubleSpinBox);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout_3->setWidget(1, QFormLayout::SpanningRole, label_2);


        formLayout->setWidget(2, QFormLayout::SpanningRole, groupBox_2);

        pushButton_5 = new QPushButton(widget_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        formLayout->setWidget(3, QFormLayout::SpanningRole, pushButton_5);


        horizontalLayout_13->addWidget(widget_4);

        widget = new QWidget(projectTab);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        frame = new QFrame(widget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout_3->addWidget(frame);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 1, 1, 1, 1);

        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_2->addWidget(pushButton, 1, 0, 1, 1);

        pushButton_4 = new QPushButton(widget_2);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout_2->addWidget(pushButton_4, 1, 3, 1, 1);

        pushButton_3 = new QPushButton(widget_2);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout_2->addWidget(pushButton_3, 1, 2, 1, 1);

        framesTable = new QTableView(widget_2);
        framesTable->setObjectName(QStringLiteral("framesTable"));

        gridLayout_2->addWidget(framesTable, 0, 0, 1, 4);


        verticalLayout_3->addWidget(widget_2);


        horizontalLayout_13->addWidget(widget);

        tabWidget->addTab(projectTab, QString());
        preprocessingTab = new QWidget();
        preprocessingTab->setObjectName(QStringLiteral("preprocessingTab"));
        tabWidget->addTab(preprocessingTab, QString());
        scaleSelectionTab = new QWidget();
        scaleSelectionTab->setObjectName(QStringLiteral("scaleSelectionTab"));
        verticalLayout = new QVBoxLayout(scaleSelectionTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_5 = new QWidget(scaleSelectionTab);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        horizontalLayout_3 = new QHBoxLayout(widget_5);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(widget_5);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_3->addWidget(label_4);

        horizontalSlider = new QSlider(widget_5);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(horizontalSlider);

        pushButton_6 = new QPushButton(widget_5);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        horizontalLayout_3->addWidget(pushButton_6);


        verticalLayout->addWidget(widget_5);

        widget_3 = new QWidget(scaleSelectionTab);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tabWidget_2 = new QTabWidget(widget_3);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setTabPosition(QTabWidget::West);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        widget_6 = new QWidget(tab);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
        widget_6->setSizePolicy(sizePolicy2);
        formLayout_5 = new QFormLayout(widget_6);
        formLayout_5->setSpacing(6);
        formLayout_5->setContentsMargins(11, 11, 11, 11);
        formLayout_5->setObjectName(QStringLiteral("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        scaleLabel = new QLabel(widget_6);
        scaleLabel->setObjectName(QStringLiteral("scaleLabel"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, scaleLabel);

        scaleDoubleSpinBox = new QDoubleSpinBox(widget_6);
        scaleDoubleSpinBox->setObjectName(QStringLiteral("scaleDoubleSpinBox"));

        formLayout_5->setWidget(0, QFormLayout::FieldRole, scaleDoubleSpinBox);

        pushButton_7 = new QPushButton(widget_6);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        formLayout_5->setWidget(1, QFormLayout::SpanningRole, pushButton_7);


        horizontalLayout_4->addWidget(widget_6);

        widget_7 = new QWidget(tab);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        sizePolicy1.setHeightForWidth(widget_7->sizePolicy().hasHeightForWidth());
        widget_7->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(widget_7);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame_2 = new QFrame(widget_7);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame_2);


        horizontalLayout_4->addWidget(widget_7);

        tabWidget_2->addTab(tab, QString());
        widget_7->raise();
        widget_6->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget_2->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalLayout_5 = new QHBoxLayout(tab_3);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        widget_8 = new QWidget(tab_3);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_8->sizePolicy().hasHeightForWidth());
        widget_8->setSizePolicy(sizePolicy3);
        formLayout_6 = new QFormLayout(widget_8);
        formLayout_6->setSpacing(6);
        formLayout_6->setContentsMargins(11, 11, 11, 11);
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        label_3 = new QLabel(widget_8);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_3);

        spinBox = new QSpinBox(widget_8);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        formLayout_6->setWidget(0, QFormLayout::FieldRole, spinBox);

        pushButton_8 = new QPushButton(widget_8);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        formLayout_6->setWidget(1, QFormLayout::SpanningRole, pushButton_8);


        horizontalLayout_5->addWidget(widget_8);

        widget_9 = new QWidget(tab_3);
        widget_9->setObjectName(QStringLiteral("widget_9"));
        verticalLayout_4 = new QVBoxLayout(widget_9);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        frame_3 = new QFrame(widget_9);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        sizePolicy1.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy1);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);

        verticalLayout_4->addWidget(frame_3);


        horizontalLayout_5->addWidget(widget_9);

        tabWidget_2->addTab(tab_3, QString());

        horizontalLayout_2->addWidget(tabWidget_2);


        verticalLayout->addWidget(widget_3);

        tabWidget->addTab(scaleSelectionTab, QString());
        enhancementTab = new QWidget();
        enhancementTab->setObjectName(QStringLiteral("enhancementTab"));
        verticalLayout_5 = new QVBoxLayout(enhancementTab);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        widget_10 = new QWidget(enhancementTab);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        sizePolicy3.setHeightForWidth(widget_10->sizePolicy().hasHeightForWidth());
        widget_10->setSizePolicy(sizePolicy3);
        horizontalLayout_6 = new QHBoxLayout(widget_10);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(widget_10);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        horizontalSlider_2 = new QSlider(widget_10);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(horizontalSlider_2);

        pushButton_9 = new QPushButton(widget_10);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        horizontalLayout_6->addWidget(pushButton_9);


        verticalLayout_5->addWidget(widget_10);

        widget_11 = new QWidget(enhancementTab);
        widget_11->setObjectName(QStringLiteral("widget_11"));
        horizontalLayout_7 = new QHBoxLayout(widget_11);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        groupBox_4 = new QGroupBox(widget_11);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_6 = new QVBoxLayout(groupBox_4);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        frame_5 = new QFrame(groupBox_4);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        sizePolicy1.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy1);
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);

        verticalLayout_6->addWidget(frame_5);


        horizontalLayout_7->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(widget_11);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        verticalLayout_7 = new QVBoxLayout(groupBox_5);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        frame_4 = new QFrame(groupBox_5);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        sizePolicy1.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy1);
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);

        verticalLayout_7->addWidget(frame_4);


        horizontalLayout_7->addWidget(groupBox_5);


        verticalLayout_5->addWidget(widget_11);

        widget_12 = new QWidget(enhancementTab);
        widget_12->setObjectName(QStringLiteral("widget_12"));
        horizontalLayout_8 = new QHBoxLayout(widget_12);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(widget_12);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(label_6);

        spinBox_2 = new QSpinBox(widget_12);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));

        horizontalLayout_8->addWidget(spinBox_2);

        pushButton_10 = new QPushButton(widget_12);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy4);

        horizontalLayout_8->addWidget(pushButton_10);


        verticalLayout_5->addWidget(widget_12);

        tabWidget->addTab(enhancementTab, QString());
        vertexLocationTab = new QWidget();
        vertexLocationTab->setObjectName(QStringLiteral("vertexLocationTab"));
        verticalLayout_8 = new QVBoxLayout(vertexLocationTab);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        widget_13 = new QWidget(vertexLocationTab);
        widget_13->setObjectName(QStringLiteral("widget_13"));
        horizontalLayout_9 = new QHBoxLayout(widget_13);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(widget_13);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        horizontalSlider_3 = new QSlider(widget_13);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setOrientation(Qt::Horizontal);

        horizontalLayout_9->addWidget(horizontalSlider_3);

        pushButton_11 = new QPushButton(widget_13);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        horizontalLayout_9->addWidget(pushButton_11);


        verticalLayout_8->addWidget(widget_13);

        widget_14 = new QWidget(vertexLocationTab);
        widget_14->setObjectName(QStringLiteral("widget_14"));
        horizontalLayout_10 = new QHBoxLayout(widget_14);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        widget_15 = new QWidget(widget_14);
        widget_15->setObjectName(QStringLiteral("widget_15"));
        formLayout_7 = new QFormLayout(widget_15);
        formLayout_7->setSpacing(6);
        formLayout_7->setContentsMargins(11, 11, 11, 11);
        formLayout_7->setObjectName(QStringLiteral("formLayout_7"));
        formLayout_7->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        thresholdLabel = new QLabel(widget_15);
        thresholdLabel->setObjectName(QStringLiteral("thresholdLabel"));
        sizePolicy2.setHeightForWidth(thresholdLabel->sizePolicy().hasHeightForWidth());
        thresholdLabel->setSizePolicy(sizePolicy2);

        formLayout_7->setWidget(0, QFormLayout::LabelRole, thresholdLabel);

        thresholdDoubleSpinBox = new QDoubleSpinBox(widget_15);
        thresholdDoubleSpinBox->setObjectName(QStringLiteral("thresholdDoubleSpinBox"));

        formLayout_7->setWidget(0, QFormLayout::FieldRole, thresholdDoubleSpinBox);

        pushButton_12 = new QPushButton(widget_15);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        formLayout_7->setWidget(1, QFormLayout::SpanningRole, pushButton_12);

        groupBox_6 = new QGroupBox(widget_15);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        sizePolicy2.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy2);
        formLayout_8 = new QFormLayout(groupBox_6);
        formLayout_8->setSpacing(6);
        formLayout_8->setContentsMargins(11, 11, 11, 11);
        formLayout_8->setObjectName(QStringLiteral("formLayout_8"));
        pushButton_13 = new QPushButton(groupBox_6);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));

        formLayout_8->setWidget(0, QFormLayout::SpanningRole, pushButton_13);

        pushButton_14 = new QPushButton(groupBox_6);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));

        formLayout_8->setWidget(1, QFormLayout::SpanningRole, pushButton_14);

        pushButton_15 = new QPushButton(groupBox_6);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));

        formLayout_8->setWidget(2, QFormLayout::SpanningRole, pushButton_15);


        formLayout_7->setWidget(2, QFormLayout::SpanningRole, groupBox_6);


        horizontalLayout_10->addWidget(widget_15);

        widget_16 = new QWidget(widget_14);
        widget_16->setObjectName(QStringLiteral("widget_16"));
        verticalLayout_9 = new QVBoxLayout(widget_16);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        frame_6 = new QFrame(widget_16);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        sizePolicy1.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy1);
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);

        verticalLayout_9->addWidget(frame_6);


        horizontalLayout_10->addWidget(widget_16);


        verticalLayout_8->addWidget(widget_14);

        tabWidget->addTab(vertexLocationTab, QString());
        cellSegmentationTab = new QWidget();
        cellSegmentationTab->setObjectName(QStringLiteral("cellSegmentationTab"));
        verticalLayout_10 = new QVBoxLayout(cellSegmentationTab);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        widget_17 = new QWidget(cellSegmentationTab);
        widget_17->setObjectName(QStringLiteral("widget_17"));
        horizontalLayout_11 = new QHBoxLayout(widget_17);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_8 = new QLabel(widget_17);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_11->addWidget(label_8);

        horizontalSlider_4 = new QSlider(widget_17);
        horizontalSlider_4->setObjectName(QStringLiteral("horizontalSlider_4"));
        horizontalSlider_4->setOrientation(Qt::Horizontal);

        horizontalLayout_11->addWidget(horizontalSlider_4);

        pushButton_16 = new QPushButton(widget_17);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));

        horizontalLayout_11->addWidget(pushButton_16);


        verticalLayout_10->addWidget(widget_17);

        widget_18 = new QWidget(cellSegmentationTab);
        widget_18->setObjectName(QStringLiteral("widget_18"));
        horizontalLayout_12 = new QHBoxLayout(widget_18);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        widget_19 = new QWidget(widget_18);
        widget_19->setObjectName(QStringLiteral("widget_19"));
        formLayout_9 = new QFormLayout(widget_19);
        formLayout_9->setSpacing(6);
        formLayout_9->setContentsMargins(11, 11, 11, 11);
        formLayout_9->setObjectName(QStringLiteral("formLayout_9"));
        formLayout_9->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        levelSetThresholdLabel = new QLabel(widget_19);
        levelSetThresholdLabel->setObjectName(QStringLiteral("levelSetThresholdLabel"));

        formLayout_9->setWidget(0, QFormLayout::LabelRole, levelSetThresholdLabel);

        levelSetThresholdDoubleSpinBox = new QDoubleSpinBox(widget_19);
        levelSetThresholdDoubleSpinBox->setObjectName(QStringLiteral("levelSetThresholdDoubleSpinBox"));

        formLayout_9->setWidget(0, QFormLayout::FieldRole, levelSetThresholdDoubleSpinBox);

        pushButton_17 = new QPushButton(widget_19);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));

        formLayout_9->setWidget(1, QFormLayout::SpanningRole, pushButton_17);


        horizontalLayout_12->addWidget(widget_19);

        widget_20 = new QWidget(widget_18);
        widget_20->setObjectName(QStringLiteral("widget_20"));
        verticalLayout_11 = new QVBoxLayout(widget_20);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        frame_7 = new QFrame(widget_20);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        sizePolicy1.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy1);
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);

        verticalLayout_11->addWidget(frame_7);


        horizontalLayout_12->addWidget(widget_20);


        verticalLayout_10->addWidget(widget_18);

        tabWidget->addTab(cellSegmentationTab, QString());
        cellTrackingTab = new QWidget();
        cellTrackingTab->setObjectName(QStringLiteral("cellTrackingTab"));
        tabWidget->addTab(cellTrackingTab, QString());
        analyticsTab = new QWidget();
        analyticsTab->setObjectName(QStringLiteral("analyticsTab"));
        tabWidget->addTab(analyticsTab, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        TTTMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TTTMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        TTTMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TTTMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TTTMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TTTMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TTTMainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);

        retranslateUi(TTTMainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(TTTMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TTTMainWindow)
    {
        TTTMainWindow->setWindowTitle(QApplication::translate("TTTMainWindow", "TTTMainWindow", 0));
        actionNew->setText(QApplication::translate("TTTMainWindow", "New", 0));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("TTTMainWindow", "New Project", 0));
#endif // QT_NO_TOOLTIP
        actionNew->setShortcut(QApplication::translate("TTTMainWindow", "Ctrl+N", 0));
        actionOpen->setText(QApplication::translate("TTTMainWindow", "Open", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("TTTMainWindow", "Open project", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("TTTMainWindow", "Ctrl+O", 0));
        actionFrameUp->setText(QApplication::translate("TTTMainWindow", "Frame Up", 0));
        actionFrameUp->setShortcut(QApplication::translate("TTTMainWindow", "Ctrl+Up", 0));
        actionFrameDown->setText(QApplication::translate("TTTMainWindow", "Frame Down", 0));
        actionFrameDown->setShortcut(QApplication::translate("TTTMainWindow", "Ctrl+Down", 0));
        actionAddFrame->setText(QApplication::translate("TTTMainWindow", "Add Frame", 0));
        actionAddFrame->setShortcut(QApplication::translate("TTTMainWindow", "Ctrl+Up", 0));
        actionRemoveFrame->setText(QApplication::translate("TTTMainWindow", "Remove Frame", 0));
        actionRemoveFrame->setShortcut(QApplication::translate("TTTMainWindow", "Ctrl+Down", 0));
        groupBox_3->setTitle(QApplication::translate("TTTMainWindow", "Name", 0));
        groupBox->setTitle(QApplication::translate("TTTMainWindow", "Spatial Properties", 0));
        xLabel->setText(QApplication::translate("TTTMainWindow", "X:", 0));
        yLabel->setText(QApplication::translate("TTTMainWindow", "Y:", 0));
        zLabel->setText(QApplication::translate("TTTMainWindow", "Z:", 0));
        label->setText(QApplication::translate("TTTMainWindow", "Size: ", 0));
        groupBox_2->setTitle(QApplication::translate("TTTMainWindow", "Temporal Properties", 0));
        incrementLabel->setText(QApplication::translate("TTTMainWindow", "Increment:", 0));
        label_2->setText(QApplication::translate("TTTMainWindow", "Length:", 0));
        pushButton_5->setText(QApplication::translate("TTTMainWindow", "Save", 0));
        pushButton_2->setText(QApplication::translate("TTTMainWindow", "Remove Frame", 0));
        pushButton->setText(QApplication::translate("TTTMainWindow", "Add Frame...", 0));
        pushButton_4->setText(QApplication::translate("TTTMainWindow", "Move down", 0));
        pushButton_3->setText(QApplication::translate("TTTMainWindow", "Move Up", 0));
        tabWidget->setTabText(tabWidget->indexOf(projectTab), QApplication::translate("TTTMainWindow", "Project", 0));
        tabWidget->setTabText(tabWidget->indexOf(preprocessingTab), QApplication::translate("TTTMainWindow", "Preprocessing", 0));
        label_4->setText(QApplication::translate("TTTMainWindow", "Frame: ", 0));
        pushButton_6->setText(QApplication::translate("TTTMainWindow", "Apply To All", 0));
        scaleLabel->setText(QApplication::translate("TTTMainWindow", "Scale:", 0));
        pushButton_7->setText(QApplication::translate("TTTMainWindow", "Preview", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QApplication::translate("TTTMainWindow", "Lowest", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("TTTMainWindow", "Highest", 0));
        label_3->setText(QApplication::translate("TTTMainWindow", "Steps:", 0));
        pushButton_8->setText(QApplication::translate("TTTMainWindow", "Preview", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("TTTMainWindow", "Range", 0));
        tabWidget->setTabText(tabWidget->indexOf(scaleSelectionTab), QApplication::translate("TTTMainWindow", "Scale Selection", 0));
        label_5->setText(QApplication::translate("TTTMainWindow", "Frame:", 0));
        pushButton_9->setText(QApplication::translate("TTTMainWindow", "Apply To All", 0));
        groupBox_4->setTitle(QApplication::translate("TTTMainWindow", "Before", 0));
        groupBox_5->setTitle(QApplication::translate("TTTMainWindow", "After", 0));
        label_6->setText(QApplication::translate("TTTMainWindow", "Iterations:", 0));
        pushButton_10->setText(QApplication::translate("TTTMainWindow", "Enhance", 0));
        tabWidget->setTabText(tabWidget->indexOf(enhancementTab), QApplication::translate("TTTMainWindow", "Enhancement", 0));
        label_7->setText(QApplication::translate("TTTMainWindow", "Frame:", 0));
        pushButton_11->setText(QApplication::translate("TTTMainWindow", "Apply to All", 0));
        thresholdLabel->setText(QApplication::translate("TTTMainWindow", "Threshold:", 0));
        pushButton_12->setText(QApplication::translate("TTTMainWindow", "Apply", 0));
        groupBox_6->setTitle(QApplication::translate("TTTMainWindow", "Validation:", 0));
        pushButton_13->setText(QApplication::translate("TTTMainWindow", "Select", 0));
        pushButton_14->setText(QApplication::translate("TTTMainWindow", "Add", 0));
        pushButton_15->setText(QApplication::translate("TTTMainWindow", "Delete", 0));
        tabWidget->setTabText(tabWidget->indexOf(vertexLocationTab), QApplication::translate("TTTMainWindow", "Vertex Location", 0));
        label_8->setText(QApplication::translate("TTTMainWindow", "Frame:", 0));
        pushButton_16->setText(QApplication::translate("TTTMainWindow", "Apply to All", 0));
        levelSetThresholdLabel->setText(QApplication::translate("TTTMainWindow", "LevelSet Threshold", 0));
        pushButton_17->setText(QApplication::translate("TTTMainWindow", "Segment", 0));
        tabWidget->setTabText(tabWidget->indexOf(cellSegmentationTab), QApplication::translate("TTTMainWindow", "Cell Segmentation", 0));
        tabWidget->setTabText(tabWidget->indexOf(cellTrackingTab), QApplication::translate("TTTMainWindow", "Cell Tracking", 0));
        tabWidget->setTabText(tabWidget->indexOf(analyticsTab), QApplication::translate("TTTMainWindow", "Analytics", 0));
        menuFile->setTitle(QApplication::translate("TTTMainWindow", "File", 0));
        menuView->setTitle(QApplication::translate("TTTMainWindow", "View", 0));
        menuHelp->setTitle(QApplication::translate("TTTMainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class TTTMainWindow: public Ui_TTTMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TTTMAINWINDOW_H
