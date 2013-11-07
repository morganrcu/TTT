#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileReader.h"
#include "itkNumericSeriesFileNames.h"
#include "itkSCIFIOImageIO.h"
#include <QFileDialog>
NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}


void NewProjectDialog::on_buttonBox_accepted(){
	m_NewProject.Create(this->ui->basedirttxt->text());
    QString imageDir = this->ui->imgdir->text();
    QString imgprefix = this->ui->imgprefixtxt->text();
    unsigned int  nframes = this->ui->nframestxt->text().toUInt();
    unsigned int  zslices = this->ui->zslicestxt->text().toUInt();
    unsigned int  nchans = this->ui->nchanstxt->text().toUInt();
    double xspacing =this->ui->xspacingtxt->text().toDouble();
    double yspacing = this->ui->yspacingtxt->text().toDouble();
    double zspacing = this->ui->zspacingtxt->text().toDouble();
    TissueTracker::ProjectType::SpacingType spacing;

    //spacing[0]=xspacing;
    spacing[0]=0.1022727; //TODO Fix units in readed frame
    //spacing[1]=yspacing;
    spacing[1]=0.1022727;
    //spacing[2]=zspacing;
    spacing[2]=1.0192918;

    m_NewProject.AddSpacing(spacing);
    QDir directory=imageDir;

    //QString format("%1/%2_t%3");

    for(int i = 1; i <= nchans; i++){

        if(i == 1){
            for(unsigned int currentFrame=0;currentFrame<=nframes;currentFrame++){
#if 0
                typedef itk::ImageSeriesReader<giaa::TissueSegmentation::RawImageType> ReaderType;

                 itk::NumericSeriesFileNames::Pointer numericSeriesFileNames = itk::NumericSeriesFileNames::New();

                numericSeriesFileNames->SetStartIndex(1);
                numericSeriesFileNames->SetEndIndex(zslices);
                numericSeriesFileNames->SetIncrementIndex(1);
                //QString prefix = format.arg(directory.path()).arg(imgprefix).arg(currentFrame,3,10,QLatin1Char('0')) + QString("_z%03d") + QString("_c001.tif");
                QString prefix = format.arg(directory.path()).arg(imgprefix).arg(currentFrame,3,10,QLatin1Char('0')) + QString("_z%03d.tif");
                std::cout << prefix.toStdString() << std::endl;
                numericSeriesFileNames->SetSeriesFormat(prefix.toStdString());

                ReaderType::Pointer reader = ReaderType::New();
                reader->SetDebug(true);

                std::vector<std::string > filenames = numericSeriesFileNames->GetFileNames();

                reader->SetFileNames(filenames);
                reader->Update();

                reader->GetOutput()->SetSpacing(m_NewProject.GetSpacing());

                m_NewProject.AddRaw(currentFrame-1,reader->GetOutput());
#endif

                itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();

                typedef itk::ImageFileReader<giaa::TissueSegmentation::RawImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetDebug(true);

                reader->SetImageIO(io);

                //QString file = format.arg(directory.path()).arg(imgprefix).arg(currentFrame,3,10,QLatin1Char('0')) + QString("_z%03d.tif");
                //QString file("/home/morgan/Notum Movies.mdb/denoised/notum_T0.ome.tif");
                //QString file = format.arg(directory.path()).arg(imgprefix).arg(currentFrame,3,10,QLatin1Char('0')) + QString("_z%03d.tif");
                QString file = imageDir.arg(currentFrame);// format.arg(directory.path()).arg(imgprefix).arg(currentFrame,3,10,QLatin1Char('0')) + QString("_z%03d.tif");
                std::cout <<file.toStdString() << std::endl;
                reader->SetFileName(file.toStdString());
                reader->Update();

                std::cout << reader->GetOutput()->GetSpacing() << std::endl;

                reader->GetOutput()->SetSpacing(m_NewProject.GetSpacing());
                m_NewProject.AddRaw(currentFrame,reader->GetOutput());

              }
        }
        else if (i == 2){
#if 0
            for(unsigned int currentFrame=1;currentFrame<=nframes;currentFrame++){

                typedef itk::ImageSeriesReader<giaa::TissueSegmentation::RawImageType> ReaderType;

                 itk::NumericSeriesFileNames::Pointer numericSeriesFileNames = itk::NumericSeriesFileNames::New();

                numericSeriesFileNames->SetStartIndex(1);
                numericSeriesFileNames->SetEndIndex(zslices);
                numericSeriesFileNames->SetIncrementIndex(1);
                QString prefix = format.arg(directory.path()).arg(imgprefix).arg(currentFrame,3,10,QLatin1Char('0')) + QString("_z%03d") + QString("_c002.tif");
                std::cout << prefix.toStdString() << std::endl;
                numericSeriesFileNames->SetSeriesFormat(prefix.toStdString());

                ReaderType::Pointer reader = ReaderType::New();
                reader->SetDebug(true);

                std::vector<std::string > filenames = numericSeriesFileNames->GetFileNames();

                reader->SetFileNames(filenames);
                reader->Update();

                reader->GetOutput()->SetSpacing(m_NewProject.GetSpacing());

                std::cout<<"Got here in accept button"<<std::endl;
                m_NewProject.AddSC(currentFrame-1,reader->GetOutput());

              }
#endif
        }

    }
}
void NewProjectDialog::on_buttonBox_rejected(){
    std::cout<<"in rejected"<<std::endl;
}
void NewProjectDialog::on_BrowseProject_clicked()
{
    QString baseDir = QFileDialog::getExistingDirectory (this, tr("Directory"), QDir::currentPath());
    if ( baseDir.isNull() == true )
    {
        baseDir = QDir::currentPath();
    }

    this->ui->basedirttxt->clear();
    this->ui->basedirttxt->insert(baseDir);
}

void NewProjectDialog::on_BrowseImage_clicked()
{
    QString imageDir = QFileDialog::getExistingDirectory (this, tr("Directory"), QDir::currentPath());
    if ( imageDir.isNull() == true )
    {
        imageDir = QDir::currentPath();
    }

    this->ui->imgdir->clear();
    this->ui->imgdir->insert(imageDir);
}

void NewProjectDialog::on_NewProjectDialog_accepted()
{

}
