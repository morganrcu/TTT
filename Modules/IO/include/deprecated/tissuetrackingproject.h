#ifndef TISSUETRACKINGPROJECT_H
#define TISSUETRACKINGPROJECT_H
#include <vector>
#include <string>
#include <QtXml>
#include <iostream>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/format.hpp>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImage.h>
#include <itkPointSet.h>
#include <Ellipse.h>
#include <tectonics.h>
#define VINODTH_STYLE

template<class TImage> void StoreImage(const typename TImage::Pointer & image,const QString & basePath, const QString & type, unsigned int numFrame){
    typedef itk::ImageFileWriter<TImage> WriterType;

    typename WriterType::Pointer writer =WriterType::New();

    QString filename(basePath+"/"+type + "-%1" + ".mha");
    filename=filename.arg(numFrame,4,10,QLatin1Char('0'));

    writer->SetFileName(filename.toStdString());
    writer->SetInput(image);
    writer->Update();
}

template<class TImage> typename TImage::Pointer ReadImage(const QString & basePath,const QString & type,int numFrame){

    typedef itk::ImageFileReader<TImage> ReaderType;
    typename ReaderType::Pointer reader = ReaderType::New();

    QString filename(basePath+"/"+type + "-%1" + ".mha");
    filename=filename.arg(numFrame,4,10,QLatin1Char('0'));
    reader->SetFileName(filename.toStdString());;
    reader->Update();

    return reader->GetOutput();
}

template<class TRawImage,
	class TSegmentedImage,
	class TDiffusedImage,
	class TPlatenessImage,
	class TOrientationImage,
	class TVertexnessImage,
	class TVertexLocations,
#ifndef VINODTH_STYLE
	class TLevelSetImage, class TBinaryImage, class TSkeletonImage,
#endif
	class TTissueDescriptor,
	class TTrackedTissueDescriptor>
class TissueTrackingProject{



public:

    typedef TRawImage RawImageType;
    typedef TSegmentedImage SurfaceSegmentedImageType;
    typedef TDiffusedImage DiffusedImageType;
    typedef TPlatenessImage PlatenessImageType;
    typedef TOrientationImage OrientationImageType;
    typedef TVertexnessImage VertexnessImageType;
    typedef TVertexLocations VertexLocationsType;
#ifndef VINODTH_STYLE
    typedef TLevelSetImage LevelSetImageType;
    typedef TBinaryImage BinaryImageType;
    typedef TSkeletonImage SkeletonImageType;
#endif
    typedef TTissueDescriptor TissueDescriptorType;
    typedef TTrackedTissueDescriptor TrackedTissueDescriptorType;


    typedef typename RawImageType::SpacingType SpacingType;

	typedef Ellipse<double> EllipseType;
    typedef std::map<typename TissueDescriptorType::DualGraphVertexDescriptorType,EllipseType >   EllipseMapType;
	typedef boost::shared_ptr<EllipseMapType > EllipseMapTypePointer;

    TissueTrackingProject(){

        this->m_TotalFrames=0;
#ifndef VINODTH_STYLE
        this->m_BinaryImages.clear();
        this->m_BinaryImagesReady.clear();
        this->m_LevelSetImages.clear();
        this->m_LevelSetImagesReady.clear();
#endif
        this->m_DiffusedImages.clear();
        this->m_DiffusedImagesReady.clear();


        this->m_OrientationImages.clear();
        this->m_OrientationImagesReady.clear();
        this->m_PlatenessImages.clear();
        this->m_PlatenessImagesReady.clear();
        this->m_RawImages.clear();
        this->m_RawImagesReady.clear();
        this->m_SurfaceSegmentedImages.clear();
        this->m_SurfaceSegmentedImagesReady.clear();
    }


    void Create(const QString & basePath){
        m_BasePath=basePath;

        m_ConfigDoc.clear();

        m_ConfigRoot = m_ConfigDoc.createElement("config");
        m_ConfigRoot.setAttribute("name","foo");
        m_ConfigDoc.appendChild(m_ConfigRoot);

        StoreConfig();
    }

    void StoreConfig(){

        QFile file( m_BasePath+"/config.xml" );
        file.remove();

        if( !file.open(QIODevice::ReadWrite | QIODevice::Text) ){
            std::cerr << "Can't open config file for writting: " << file.fileName().toStdString() <<std::endl;
            exit(-1);
        }else{
            QTextStream ts( &file );
            std::cout << m_ConfigDoc.toString().toStdString() << std::endl;
            ts << m_ConfigDoc.toString();
            file.close();
        }
    }


    void NewFrame(){

        m_TotalFrames++;

        m_RawImages.resize(m_TotalFrames);
        m_RawImagesReady.resize(m_TotalFrames);
        m_RawImagesReady[m_TotalFrames-1]=false;

        m_SurfaceSegmentedImages.resize(m_TotalFrames);
        m_SurfaceSegmentedImagesReady.resize(m_TotalFrames);
        m_SurfaceSegmentedImagesReady[m_TotalFrames-1]=false;

        m_SCImages.resize(m_TotalFrames);
        m_SCImagesReady.resize(m_TotalFrames);
        m_SCImagesReady[m_TotalFrames-1]=false;
       
        m_DiffusedImages.resize(m_TotalFrames);
        m_DiffusedImagesReady.resize(m_TotalFrames);
        m_DiffusedImagesReady[m_TotalFrames-1]=false;

        m_PlatenessImages.resize(m_TotalFrames);
        m_PlatenessImagesReady.resize(m_TotalFrames);
        m_PlatenessImagesReady[m_TotalFrames-1]=false;

        m_OrientationImages.resize(m_TotalFrames);
        m_OrientationImagesReady.resize(m_TotalFrames);
        m_OrientationImagesReady[m_TotalFrames-1]=false;

        m_VertexnessImages.resize(m_TotalFrames);
        m_VertexnessImagesReady.resize(m_TotalFrames);
        m_VertexnessImagesReady[m_TotalFrames-1]=false;

        m_VertexLocations.resize(m_TotalFrames);
        m_VertexLocationsReady.resize(m_TotalFrames);
        m_VertexLocationsReady[m_TotalFrames-1]=false;
#ifndef VINODTH_STYLE

        m_LevelSetImages.resize(m_TotalFrames);
        m_LevelSetImagesReady.resize(m_TotalFrames);
        m_LevelSetImagesReady[m_TotalFrames-1]=false;

        m_BinaryImages.resize(m_TotalFrames);
        m_BinaryImagesReady.resize(m_TotalFrames);
        m_BinaryImagesReady[m_TotalFrames-1]=false;

        m_SkeletonImages.resize(m_TotalFrames);
        m_SkeletonImagesReady.resize(m_TotalFrames);
        m_SkeletonImagesReady[m_TotalFrames-1]=false;
#endif
        m_TissueDescriptions.resize(m_TotalFrames);
        m_TissueDescriptionsReady.resize(m_TotalFrames);
        m_TissueDescriptionsReady[m_TotalFrames-1]=false;

        m_TrackedTissueDescriptions.resize(m_TotalFrames);
        m_TrackedTissueDescriptionsReady.resize(m_TotalFrames);
        m_TrackedTissueDescriptionsReady[m_TotalFrames-1]=false;


        m_Ellipses.resize(m_TotalFrames);
        m_EllipsesReady.resize(m_TotalFrames);
        m_EllipsesReady[m_TotalFrames-1]=false;

        m_DomainStrainRates.resize(m_TotalFrames);
        m_DomainStrainRatesReady.resize(m_TotalFrames);
        m_DomainStrainRatesReady[m_TotalFrames-1]=false;
    }

    void Load(QString & basePath){
        m_BasePath=basePath;
        m_ConfigDoc.clear();
        std::cout << "Tratando de abrir: " << m_BasePath.toStdString() << std::endl;
        QFile file( m_BasePath+"/config.xml" );
        if(!file.open(QIODevice::ReadOnly)) return;
        std::cout << "Fichero abierto" << std::endl;
        QString error;
        int errorLine;
        int errorColumn;
        if(!m_ConfigDoc.setContent(&file,&error,&errorLine,&errorColumn)){
            file.reset();
            std::cout << "No puedo parsear: " <<error.toStdString() << errorLine <<":"<< errorColumn<< std::endl;
            file.close();
            return;
        }
        file.close();
        std::cout << "Cargado!" << std::endl;
        m_ConfigRoot = m_ConfigDoc.documentElement();

        QDomNode n = m_ConfigRoot.firstChild();

        int frame=0;

        LoadSpacing();

        while(!n.isNull()){

            QDomElement e = n.toElement();
            if(!e.isNull()){

                this->NewFrame();



                QDomAttr surfacesegmented = e.attributeNode("surfacesegmented");
                QDomAttr diffused = e.attributeNode("diffused");
                QDomAttr plateness = e.attributeNode("plateness");
                QDomAttr orientation = e.attributeNode("orientation");
                QDomAttr vertexness = e.attributeNode("vertexness");
                QDomAttr vertexlocations = e.attributeNode("vertexlocations");
                QDomAttr levelset =e.attributeNode("levelset");
                QDomAttr thresholded=e.attributeNode("thresholded");
                QDomAttr skeleton=e.attributeNode("skeleton");
                QDomAttr descriptor = e.attributeNode("descriptor");
                QDomAttr trackedDescriptor = e.attributeNode("trackedDescriptor");
                QDomAttr ellipses = e.attributeNode("ellipses");

                QDomAttr secondchannel = e.attributeNode("sc");

                typename RawImageType::Pointer rawImage=ReadImage<RawImageType>(m_BasePath,QString("raw"),frame);
                assert(rawImage);
                this->SetRaw(frame,rawImage);

                if(surfacesegmented.value().toInt() == 1){
                	typename RawImageType::Pointer surfaceSegmentedImage=ReadImage<RawImageType>(m_BasePath,QString("surfacesegmented"),frame);
                	assert(surfaceSegmentedImage);
                    this->SetSurfaceSegmented(frame,surfaceSegmentedImage);
                }

                if(secondchannel.value().toInt() == 1){

                    this->SetSC(frame,ReadImage<RawImageType>(m_BasePath,QString("sc"),frame));
                }
                if(diffused.value().toInt()==1){
                    this->SetDiffused(frame,ReadImage<DiffusedImageType>(m_BasePath,QString("diffused"),frame));
                }
                if(plateness.value().toInt()==1){
                    this->SetPlateness(frame,ReadImage<PlatenessImageType>(m_BasePath,QString("plateness"),frame));
                }
                if(orientation.value().toInt()==1){
                    this->SetOrientation(frame,ReadImage<OrientationImageType>(m_BasePath,QString("orientation"),frame));
                }

                if(vertexness.value().toInt()==1){
                    this->SetVertexness(frame,ReadImage<VertexnessImageType>(m_BasePath,QString("vertexness"),frame));
                }

                if(vertexlocations.value().toInt()==1){
                    //typedef itk::Image<float,3> VertexnessImageType;
                    //typedef itk::PointSet<typename VertexnessImageType::PixelType, 3> PointsetType;
                    typedef typename VertexLocationsType::PointsContainer PointsContainer;

                    typename VertexLocationsType::Pointer vlt = VertexLocationsType::New();
                    //typename PointsetType::Pointer copy = PointsetType::New();
                    boost::format vlfmt = boost::format("/vl%i.txt")%frame;
                    std::string vlfilestr = m_BasePath.toStdString()+vlfmt.str();
                    const char* vlfileName = vlfilestr.c_str();
                    std::ifstream ifs(vlfileName);     
                    boost::archive::text_iarchive ar(ifs);
                    int np;
                    ar & np;
                    typename PointsContainer::Pointer newPoints = PointsContainer::New();
                    typedef typename VertexLocationsType::PointType PointType;

                    for (int i = 0; i < np; i++) {

                        itk::Index<3> pIndex;
                        ar & pIndex[0] & pIndex[1] & pIndex[2];
                        PointType p;
                        p[0] = pIndex[0];
                        p[1] = pIndex[1];
                        p[2] = pIndex[2];
                        newPoints->push_back(p);
                    }

                    vlt->SetPoints(newPoints);
                    //vlt = copy;

                    this->SetVertexLocations(frame, vlt );

                }
#ifndef VINODTH_STYLE
                if(levelset.value().toInt()==1){
                    this->SetLevelSet(frame,ReadImage<LevelSetImageType>(m_BasePath,QString("levelset"),frame));

                }

                if(thresholded.value().toInt()==1){
                    this->SetThresholded(frame,ReadImage<BinaryImageType>(m_BasePath,QString("thresholded"),frame));
                }

                if(skeleton.value().toInt()==1){
                    this->SetSkeleton(frame,ReadImage<SkeletonImageType>(m_BasePath,QString("skeleton"),frame));
                }
#endif
                if(descriptor.value().toInt()==1){
                  

                   boost::format descfmt = boost::format("/desc%i.txt")%frame;
                   std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
                   const char* descfileName = descfilestr.c_str();
                   typename TissueDescriptorType::Pointer rpt = TissueDescriptorType::New();

                   std::ifstream ifs(descfileName);
                   boost::archive::text_iarchive ar(ifs);
                   ar & *rpt;
                   assert(rpt);
                   this->SetTissueDescriptor(frame,rpt); 

                }
                if(trackedDescriptor.value().toInt()==1){


                   boost::format descfmt = boost::format("/trackedDesc%i.txt")%frame;
                   std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
                   const char* descfileName = descfilestr.c_str();
                   typename TrackedTissueDescriptorType::Pointer rpt = TrackedTissueDescriptorType::New();

                   std::ifstream ifs(descfileName);
                   boost::archive::text_iarchive ar(ifs);
                   ar & *rpt;
                   assert(rpt);
                   this->SetTrackedTissueDescriptor(frame,rpt);

                }
                if(ellipses.value().toInt()==1){

                	boost::format descfmt = boost::format("/ellipses%i.txt")%frame;

                	std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
                    const char* descfileName = descfilestr.c_str();

                    EllipseMapTypePointer ellipses = boost::shared_ptr<EllipseMapType>(new EllipseMapType);


                    std::ifstream ifs(descfileName);
                    boost::archive::text_iarchive ar(ifs);
                    ar & *ellipses;
                    assert(ellipses);
                    this->SetEllipses(frame,ellipses);
                }
            }
            n=n.nextSibling();
            frame++;
        }

    }



    inline void AddRaw(unsigned int frame,const typename RawImageType::Pointer & rawImage){
        if(frame>=m_TotalFrames){
            this->NewFrame();
            QDomElement frameElement = m_ConfigDoc.createElement("frame");

            frameElement.setAttribute("id",frame);
            frameElement.setAttribute("surfacesegmented",false);
            frameElement.setAttribute("diffused",false);
            frameElement.setAttribute("plateness",false);
            frameElement.setAttribute("orientation",false);
            frameElement.setAttribute("vertexness",false);
            frameElement.setAttribute("vertexlocations",false);
            frameElement.setAttribute("levelset",false);
            frameElement.setAttribute("thresholded",false);
            frameElement.setAttribute("skeleton",false);
            frameElement.setAttribute("descriptor",false);
            frameElement.setAttribute("trackedDescriptor",false);
            frameElement.setAttribute("domainStrainRates",false);
            m_ConfigRoot.appendChild(frameElement);

        }

        SetRaw(frame,rawImage);

        StoreImage<RawImageType>(rawImage,m_BasePath,"raw",frame);

        StoreConfig();
    }



    inline void AddSC(unsigned int frame,const typename RawImageType::Pointer & rawImage){
        
        SetSC(frame,rawImage);
        StoreImage<RawImageType>(rawImage,m_BasePath,"sc",frame);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("sc",true);
        StoreConfig();
    }



    inline void AddSurfaceSegmented(unsigned int frame, const typename SurfaceSegmentedImageType::Pointer & surfaceSegmentedImage){

        SetSurfaceSegmented(frame,surfaceSegmentedImage);

        StoreImage<SurfaceSegmentedImageType>(surfaceSegmentedImage,m_BasePath,"surfacesegmented",frame);

        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("surfacesegmented",true);
        StoreConfig();
    }


    inline void AddDiffused(unsigned int frame, const typename DiffusedImageType::Pointer & diffusedImage){

        SetDiffused(frame,diffusedImage);

        StoreImage<DiffusedImageType>(diffusedImage,m_BasePath,"diffused",frame);

        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("diffused",true);
        StoreConfig();
    }



    void AddPlateness(unsigned int frame, const typename PlatenessImageType::Pointer & platenessImage){
        SetPlateness(frame,platenessImage);
        StoreImage<PlatenessImageType>(platenessImage,m_BasePath,"plateness",frame);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("plateness",true);
        StoreConfig();
    }
    void AddOrientation(unsigned int frame, const typename OrientationImageType::Pointer & orientationImage){
        SetOrientation(frame,orientationImage);
        StoreImage<OrientationImageType>(orientationImage,m_BasePath,"orientation",frame);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("orientation",true);
        StoreConfig();
    }


    void AddVertexness(unsigned int frame, const typename VertexnessImageType::Pointer & vertexnessImage){
        m_VertexnessImages[frame]=vertexnessImage;
        m_VertexnessImagesReady[frame]=true;

        StoreImage<VertexnessImageType>(vertexnessImage,m_BasePath,"vertexness",frame);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("vertexness",true);
        StoreConfig();
    }



    void AddVertexLocations(unsigned int frame,const typename VertexLocationsType::Pointer & vertexLocations ){
        SetVertexLocations(frame,vertexLocations);
        boost::format vlfmt = boost::format("/vl%i.txt")%frame;
        std::string vlfilestr = m_BasePath.toStdString()+vlfmt.str();


        //typedef itk::Image<double,3> VertexnessImageType;
        typedef itk::PointSet<typename VertexnessImageType::PixelType, 3> PointsetType;
        typedef typename PointsetType::PointsContainer PointsContainer;
        typename PointsContainer::Pointer localMaxPoints = vertexLocations->GetPoints();
        typedef  typename PointsContainer::Iterator PointIterator;
        typedef typename PointsetType::PointType PointType;
        

        std::ofstream ofs(vlfilestr.c_str());
        boost::archive::text_oarchive ar(ofs);
        int np = vertexLocations->GetNumberOfPoints();
        ar & np;

        for (PointIterator it = localMaxPoints->Begin(); it != localMaxPoints->End(); it++) {
        
            PointType p= it.Value();
            //itk::Index<3> pIndex;
            //pIndex[0]=p[0];
            //pIndex[1]=p[1];
            //pIndex[2]=p[2];
           
            ar & p[0] & p[1] & p[2];
        }

        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("vertexlocations",true);
        StoreConfig();

    }

#ifndef VINODTH_STYLE
    void AddLevelSet(unsigned int frame, const typename LevelSetImageType::Pointer & levelsetImage){
        SetLevelSet(frame,levelsetImage);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("levelset",true);
        StoreImage<LevelSetImageType>(levelsetImage,m_BasePath,"levelset",frame);
        StoreConfig();
    }


    void AddThresholded(unsigned int frame, const typename BinaryImageType::Pointer & thresholdedImage){
        SetThresholded(frame,thresholdedImage);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("thresholded",true);
        StoreImage<BinaryImageType>(thresholdedImage,m_BasePath,"thresholded",frame);
        StoreConfig();
    }



    void AddSkeleton(unsigned int frame,const typename SkeletonImageType::Pointer & skeletonImage){
        SetSkeleton(frame,skeletonImage);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("skeleton",true);
        StoreImage<SkeletonImageType>(skeletonImage,m_BasePath,"skeleton",frame);
        StoreConfig();
    }
#endif
    void AddTissueDescriptor(unsigned int frame, const typename TissueDescriptorType::Pointer & tissueDescriptor){
        SetTissueDescriptor(frame,tissueDescriptor);

        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("descriptor",true);
        boost::format descfmt = boost::format("/desc%i.txt")%frame;
        std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
        const char* descfileName = descfilestr.c_str();

        std::ofstream ofs(descfileName);     
        boost::archive::text_oarchive ar(ofs);
        ar & *tissueDescriptor;       
        StoreConfig();
    }

    void AddTrackedTissueDescriptor(unsigned int frame, const typename TrackedTissueDescriptorType::Pointer & tissueDescriptor){
        SetTrackedTissueDescriptor(frame,tissueDescriptor);

        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("trackedDescriptor",true);
        boost::format descfmt = boost::format("/trackedDesc%i.txt")%frame;
        std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
        const char* descfileName = descfilestr.c_str();

        std::ofstream ofs(descfileName);
        boost::archive::text_oarchive ar(ofs);
        ar & *tissueDescriptor;
        StoreConfig();
    }
    void AddEllipses(unsigned int frame, const EllipseMapTypePointer & ellipses){
    	SetEllipses(frame,ellipses);
    	m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("ellipses",true);
    	boost::format descfmt = boost::format("/ellipses%i.txt")%frame;
    	std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
    	const char* descfileName = descfilestr.c_str();

    	std::ofstream ofs(descfileName);
    	boost::archive::text_oarchive ar(ofs);
    	ar & *ellipses;
    	StoreConfig();
    }
    void AddDomainStrainRates(unsigned int frame, const DomainStrainRatesMapTypePointer & domainStrainRates){
    	SetDomainStrainRates(frame,domainStrainRates);
    	m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("domainStrainRates",true);
    	boost::format descfmt = boost::format("/tectonics%i.txt")%frame;

    	std::string descfilestr =  m_BasePath.toStdString()+descfmt.str();
    	const char* descfileName = descfilestr.c_str();

    	std::ofstream ofs(descfileName);
    	boost::archive::text_oarchive ar(ofs);
    	//ar & *domainStrainRates;
    	StoreConfig();
    }
#if 0
    void AddTissueDescriptorD(unsigned int frame, const typename TissueDescriptorType::Pointer & tissueDescriptor){
        SetTissueDescriptor(frame,tissueDescriptor);
        m_ConfigRoot.childNodes().at(frame).toElement().setAttribute("descriptor",true);

    }

#endif
    inline typename RawImageType::Pointer GetRawImage(unsigned int frame){
        return m_RawImages[frame];
    }
    inline typename RawImageType::Pointer GetSurfaceSegmentedImage(unsigned int frame){
        return m_SurfaceSegmentedImages[frame];
    }
    inline typename RawImageType::Pointer GetSCImage(unsigned int frame){
        return m_SCImages[frame];
    }
    inline typename DiffusedImageType::Pointer GetDiffusedImage(unsigned int frame){
        return m_DiffusedImages[frame];
    }

    inline typename PlatenessImageType::Pointer GetPlatenessImage(unsigned int frame){
        return m_PlatenessImages[frame];
    }
    inline typename OrientationImageType::Pointer GetOrientationImage(unsigned int frame){
        return m_OrientationImages[frame];
    }

    inline typename VertexnessImageType::Pointer GetVertexnessImage(unsigned int frame){
        return m_VertexnessImages[frame];
    }

    inline typename VertexLocationsType::Pointer GetVertexLocations(unsigned int frame){
        return m_VertexLocations[frame];
    }
#ifndef VINODTH_STYLE
    inline typename LevelSetImageType::Pointer GetLevelSetImage(unsigned int frame){
        return m_LevelSetImages[frame];
    }

    inline typename BinaryImageType::Pointer GetThresholdedImage(unsigned int frame){
        return m_BinaryImages[frame];
    }
    inline typename SkeletonImageType::Pointer GetSkeletonImage(unsigned int frame){
        return m_SkeletonImages[frame];
    }
#endif
    inline typename TissueDescriptorType::Pointer GetTissueDescriptor(unsigned int frame){
        return m_TissueDescriptions[frame];
    }
    inline std::vector<typename TissueDescriptorType::Pointer> GetTissueDescriptors(){
        return m_TissueDescriptions;
    }

    inline typename TrackedTissueDescriptorType::Pointer GetTrackedTissueDescriptor(unsigned int frame){
    	return m_TrackedTissueDescriptions[frame];
    }

    inline EllipseMapTypePointer GetEllipses(unsigned int frame){
    	return m_Ellipses[frame];
    }

    inline DomainStrainRatesMapTypePointer GetDomainStrainRates(unsigned int frame){
    	return m_DomainStrainRates[frame];
    }

    inline bool IsRawReady(unsigned int frame){
        return m_RawImagesReady[frame];
    }

    inline bool IsSCReady(unsigned int frame){
        return m_SCImagesReady[frame];
    }

    inline bool IsSurfaceSegmentedReady(unsigned int frame){
        return m_SurfaceSegmentedImagesReady[frame];
    }

    inline bool IsDiffusedReady(unsigned int frame){
        return m_DiffusedImagesReady[frame];
    }

    inline bool IsPlatenessReady(unsigned int frame){
        return m_PlatenessImagesReady[frame];
    }

    inline bool IsOrientationReady(unsigned int frame){
        return m_OrientationImagesReady[frame];
    }

    inline bool IsVertexnessReady(unsigned int frame){
        return m_VertexnessImagesReady[frame];
    }


    inline bool IsVertexLocationsReady(unsigned int frame){
        return m_VertexLocationsReady[frame];
    }
#ifndef VINODTH_STYLE
    inline bool IsLevelSetReady(unsigned int frame){
        return m_LevelSetImagesReady[frame];
    }

    inline bool IsThresholdedReady(unsigned int frame){
        return m_BinaryImagesReady[frame];
    }

    inline bool IsSkeletonReady(unsigned int frame){
        return m_SkeletonImagesReady[frame];
    }
#endif
    inline bool IsTissueDescriptorReady(unsigned int frame){
        return m_TissueDescriptionsReady[frame];
    }

    inline bool IsTrackedTissueDescriptorReady(unsigned int frame){
            return m_TrackedTissueDescriptionsReady[frame];

    }

    inline bool IsEllipsesReady(unsigned int frame){
    	return m_Ellipses[frame];
    }

    inline bool IsDomainStrainRatesReady(unsigned int frame){
    	return m_DomainStrainRatesReady[frame];
    }
    inline unsigned int GetNumFrames(){
        return m_RawImages.size();
    }


    inline void AddSpacing(const SpacingType & value){
        m_Spacing=value;
        boost::format spfmt = boost::format("/spacing.txt");
        std::string spfilestr =  m_BasePath.toStdString()+spfmt.str();
        const char* spfileName = spfilestr.c_str();
        std::ofstream ofs(spfileName);
        boost::archive::text_oarchive ar(ofs);
        ar & m_Spacing[0] & m_Spacing[1] & m_Spacing[2];

    }



    inline SpacingType GetSpacing(){
        return m_Spacing;
    }

private:

    inline void SetRaw(unsigned int frame,const typename RawImageType::Pointer & rawImage){
     m_RawImages[frame]=rawImage;
     m_RawImagesReady[frame]=true;
    }
    inline void SetSurfaceSegmented(unsigned int frame, const typename SurfaceSegmentedImageType::Pointer & surfaceSegmentedImage){
        m_SurfaceSegmentedImages[frame]=surfaceSegmentedImage;
        m_SurfaceSegmentedImagesReady[frame]=true;
    }
    inline void SetSC(unsigned int frame,const typename RawImageType::Pointer & rawImage){
         m_SCImages[frame]=rawImage;
         m_SCImagesReady[frame]=true;
    }
    inline void SetDiffused(unsigned int frame, const typename DiffusedImageType::Pointer & diffusedImage){
        m_DiffusedImages[frame]=diffusedImage;
        m_DiffusedImagesReady[frame]=true;

    }
    inline void SetPlateness(unsigned int frame, const typename PlatenessImageType::Pointer & platenessImage){
        m_PlatenessImages[frame]=platenessImage;
        m_PlatenessImagesReady[frame]=true;
    }
    inline void SetOrientation(unsigned int frame, const typename OrientationImageType::Pointer & orientationImage){
        m_OrientationImages[frame]=orientationImage;
        m_OrientationImagesReady[frame]=true;
    }
    inline void SetVertexness(unsigned int frame, const typename VertexnessImageType::Pointer & vertexnessImage){
        m_VertexnessImages[frame]=vertexnessImage;
        m_VertexnessImagesReady[frame]=true;
    }
    inline void SetVertexLocations(unsigned int frame,const typename VertexLocationsType::Pointer & vertexLocations ){
        m_VertexLocations[frame]=vertexLocations;
        m_VertexLocationsReady[frame]=true;
    }
#ifndef VINODTH_STYLE
    inline void SetLevelSet(unsigned int frame, const typename LevelSetImageType::Pointer & levelsetImage){
            m_LevelSetImages[frame]=levelsetImage;
            m_LevelSetImagesReady[frame]=true;
    }
    inline void SetThresholded(unsigned int frame, const typename BinaryImageType::Pointer & thresholdedImage){
        m_BinaryImages[frame]=thresholdedImage;
        m_BinaryImagesReady[frame]=true;
    }

    inline void SetSkeleton(unsigned int frame,const typename SkeletonImageType::Pointer & skeletonImage){
        m_SkeletonImages[frame]=skeletonImage;
        m_SkeletonImagesReady[frame]=true;
    }
#endif

    inline void SetTissueDescriptor(unsigned int frame, const typename TissueDescriptorType::Pointer & tissueDescriptor){
        m_TissueDescriptions[frame]=tissueDescriptor;
        m_TissueDescriptionsReady[frame]=true;
    }
    inline void SetTrackedTissueDescriptor(unsigned int frame, const typename TrackedTissueDescriptorType::Pointer & trackedTissueDescriptor){
            m_TrackedTissueDescriptions[frame]=trackedTissueDescriptor;
            m_TrackedTissueDescriptionsReady[frame]=true;
    }

    inline void SetEllipses(unsigned int frame, const EllipseMapTypePointer & ellipses){
    	m_Ellipses[frame]=ellipses;
        m_EllipsesReady[frame]=true;

    }

    inline void SetDomainStrainRates(unsigned int frame, const DomainStrainRatesMapTypePointer & domainStrainRates){
    	m_DomainStrainRates[frame]= domainStrainRates;
    	m_DomainStrainRatesReady[frame]=true;
    }


    inline void LoadSpacing(){
        boost::format spfmt = boost::format("/spacing.txt");
        std::string spfilestr = m_BasePath.toStdString()+spfmt.str();
        const char* spfileName = spfilestr.c_str();
        std::ifstream ifs(spfileName);
        boost::archive::text_iarchive ar(ifs);
        SpacingType s;
        ar & s[0] & s[1] & s[2];
        m_Spacing = s;
    }



private:



    SpacingType m_Spacing;

    std::vector<typename RawImageType::Pointer> m_RawImages;
    std::vector<typename SurfaceSegmentedImageType::Pointer> m_SurfaceSegmentedImages;
    std::vector<typename RawImageType::Pointer> m_SCImages;
    std::vector<typename DiffusedImageType::Pointer> m_DiffusedImages;
    std::vector<typename PlatenessImageType::Pointer> m_PlatenessImages;
    std::vector<typename OrientationImageType::Pointer> m_OrientationImages;
    std::vector<typename VertexnessImageType::Pointer> m_VertexnessImages;
    std::vector<typename VertexLocationsType::Pointer> m_VertexLocations;
#ifndef VINODTH_STYLE
    std::vector<typename LevelSetImageType::Pointer> m_LevelSetImages;
    std::vector<typename BinaryImageType::Pointer> m_BinaryImages;
    std::vector<typename SkeletonImageType::Pointer> m_SkeletonImages;

    std::vector<bool> m_LevelSetImagesReady;
    std::vector<bool> m_BinaryImagesReady;
    std::vector<bool> m_SkeletonImagesReady;
#endif
    std::vector<typename TissueDescriptorType::Pointer> m_TissueDescriptions;
    std::vector<typename TrackedTissueDescriptorType::Pointer> m_TrackedTissueDescriptions;

    std::vector<EllipseMapTypePointer> m_Ellipses;

    std::vector<DomainStrainRatesMapTypePointer> m_DomainStrainRates;

    unsigned int m_TotalFrames;

    std::vector<bool> m_RawImagesReady;
    std::vector<bool> m_SurfaceSegmentedImagesReady;
    std::vector<bool> m_SCImagesReady;
    std::vector<bool> m_DiffusedImagesReady;
    std::vector<bool> m_PlatenessImagesReady;
    std::vector<bool> m_OrientationImagesReady;
    std::vector<bool> m_VertexnessImagesReady;
    std::vector<bool> m_VertexLocationsReady;
    std::vector<bool> m_TissueDescriptionsReady;
    std::vector<bool> m_TrackedTissueDescriptionsReady;
    std::vector<bool> m_EllipsesReady;

    std::vector<bool> m_DomainStrainRatesReady;

    QString m_BasePath;

    QDomDocument m_ConfigDoc;
    QDomElement m_ConfigRoot;

};

#endif // TISSUETRACKINGPROJECT_H
