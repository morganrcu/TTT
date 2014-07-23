#include "tttsqlnewprojectdialog.h"
#include "ui_tttsqlnewprojectdialog.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <qfiledialog.h>


SQLNewProjectDialog::SQLNewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLNewProjectDialog)
{
	m_Accepted=false;
    ui->setupUi(this);

}

SQLNewProjectDialog::~SQLNewProjectDialog()
{
    delete ui;
}
void SQLNewProjectDialog::accept(){

	typedef typename MySQLTissueTrackingProject::RawImageType ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer reader=ReaderType::New();

    m_NewProject = new MySQLTissueTrackingProject;

    std::string name = this->ui->projectNameLabel->text().toStdString();
    std::string projectPath = this->ui->wdLineEdit->text().toStdString();

    double spacingX = (double)atof(this->ui->xSpacingLineEdit->text().toStdString().c_str());
    double spacingY = (double)atof(this->ui->yLineEdit->text().toStdString().c_str());
    double spacingZ = (double)atof(this->ui->zLineEdit->text().toStdString().c_str());

    double timeDelta = atof(this->ui->timeDeltaLineEdit->text().toStdString().c_str());

    int sizeX= atoi(this->ui->selectedFilesTable->item(0,4)->text().toStdString().c_str());
    int sizeY= atoi(this->ui->selectedFilesTable->item(0,5)->text().toStdString().c_str());
    int sizeZ= atoi(this->ui->selectedFilesTable->item(0,6)->text().toStdString().c_str());

	m_NewProject->openDB();
	m_NewProject->NewProject(name,projectPath,spacingX,spacingY,spacingZ,timeDelta,sizeX,sizeY,sizeZ);
	for(int row=0; row<this->ui->selectedFilesTable->rowCount();row++){
		m_NewProject->NewFrame(row);
		m_NewProject->SetFrame(row);
		QString file= this->ui->selectedFilesTable->item(row,0)->text();

		reader->SetFileName(file.toStdString());

		reader->Update();

		m_NewProject->SetRawImage(reader->GetOutput());

	}
	m_Accepted=true;
	QDialog::accept();
}
void SQLNewProjectDialog::selectWorkingDirectory(){
	QString dir=QFileDialog::getExistingDirectory(this,tr("Select woking directory..."),QDir::currentPath(),QFileDialog::ShowDirsOnly);

	this->ui->wdLineEdit->setText(dir);
}
void SQLNewProjectDialog::addFiles(){
	typedef itk::Image<unsigned char,3> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;

	ReaderType::Pointer reader=ReaderType::New();

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Tiff Files to add..."), QDir::currentPath(),"Images (*.mha *.tif *.tiff )");

    QStringList list = files;
    QStringList::Iterator it = list.begin();
    int row=this->ui->selectedFilesTable->rowCount();
    while(it != list.end()) {
    	this->ui->selectedFilesTable->setRowCount(row+1);
    	QTableWidgetItem *newItem = new QTableWidgetItem(*it);
    	this->ui->selectedFilesTable->setItem(row,0 , newItem);

    	reader->SetFileName(it->toStdString());
        reader->Update();


        ImageType::SpacingType spacing= reader->GetOutput()->GetSpacing();

        newItem = new QTableWidgetItem(tr("%1").arg(spacing[0]));
    	this->ui->selectedFilesTable->setItem(row,1 , newItem);

    	newItem = new QTableWidgetItem(tr("%1").arg(spacing[1]));
    	this->ui->selectedFilesTable->setItem(row,2 , newItem);

    	newItem = new QTableWidgetItem(tr("%1").arg(spacing[2]));
    	this->ui->selectedFilesTable->setItem(row,3 , newItem);

    	ImageType::SizeType size= reader->GetOutput()->GetLargestPossibleRegion().GetSize();

        newItem = new QTableWidgetItem(tr("%1").arg(size[0]));
    	this->ui->selectedFilesTable->setItem(row,4 , newItem);

    	newItem = new QTableWidgetItem(tr("%1").arg(size[1]));
    	this->ui->selectedFilesTable->setItem(row,5 , newItem);

    	newItem = new QTableWidgetItem(tr("%1").arg(size[2]));
    	this->ui->selectedFilesTable->setItem(row,6 , newItem);

        it++;
        row++;

    }

}
