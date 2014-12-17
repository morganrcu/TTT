#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <tttHessianToRidgenessMeasureImageFilter.h>
#include <tttHessianToVertexnessMeasureImageFilter.h>
#include <itkMultiScaleHessianBasedMeasureImageFilter.h>
#include <itkDivideImageFilter.h>
#include <itkLocalMaximumImageFilter.h>
#include <AdherensJunctionSegmentationDijkstraCommand.h>
#include <PrimalGraphDrawer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <qtsqltissuetrackingproject2.h>
#include "DefaultColorer.h"
#include "CellGraphCommand.h"
#include "DualGraphDrawer.h"
#include <vnl/algo/vnl_amoeba.h>
#include <vnl/algo/vnl_powell.h>
#include <vnl/vnl_cost_function.h>

void findMinima(const vnl_matrix<double> & matrix, int * r, int * c){
	double minima=std::numeric_limits<double>::infinity();;
	int minR=-1;
	int minC=-1;

	for(int i=0;i<matrix.rows();i++){
		for(int j=0;j<matrix.columns();j++){

			if(matrix(i,j)<minima){
				minR=i;
				minC=j;
				minima=matrix(i,j);
			}
		}
	}

	r[0]=minR;
	c[0]=minC;
}
template<class T1, class T2> double computeF1Score(const typename T1::Pointer & detection, const typename T2::Pointer & reference,double * precision, double * recall, double * F1){

	int numReferenceCells=reference->GetNumCells();
	int numDetectedCells=detection->GetNumCells();

	double matchingThreshold=0.25;

	unsigned long int minLoc = -1;
	vnl_matrix<double> D(numDetectedCells,numReferenceCells);

	int hits=0;
	int retrieved=numDetectedCells;
	int total=numReferenceCells;

	if(retrieved>0){
		BGL_FORALL_VERTICES_T(v1,detection->GetCellGraph(),typename T1::DualGraphType){
			typename ttt::TissueDescriptorTraits<T1>::CellType cellA = boost::get(typename ttt::TissueDescriptorTraits<T1>::CellPropertyTagType(), detection->GetCellGraph(),v1);

			BGL_FORALL_VERTICES_T(v2,reference->GetCellGraph(),typename T2::DualGraphType){
				typename ttt::TissueDescriptorTraits<T2>::CellType cellB = boost::get(typename ttt::TissueDescriptorTraits<T2>::CellPropertyTagType(), reference->GetCellGraph(),v2);
				double dist=sqrt(pow(cellA.GetCentroid()[0] - cellB.GetCentroid()[0],2) +pow(cellA.GetCentroid()[1] - cellB.GetCentroid()[1],2));
				D(v1,v2)=dist;
			}
		}



		bool continueMatching=true;

		while(continueMatching){
			int minR, minC;

			findMinima(D,&minR,&minC);

			if(minR!=-1 && minC!=-1 && D(minR,minC)<matchingThreshold){
				hits++;
				for(int r=0;r<numDetectedCells;r++){
					D(r,minC)=std::numeric_limits<double>::infinity();

				}

				for(int c=0;c<numReferenceCells;c++){
					D(minR,c)=std::numeric_limits<double>::infinity();
				}
			}else{
				continueMatching=false;
			}
		}
		precision[0]=(double)hits/retrieved;
		recall[0] = (double)hits/total;
		if(precision[0]==0 && recall[0]==0){
			F1[0]=0;
		}else{
			F1[0]= (2*precision[0]*recall[0])/(precision[0]+recall[0]);
		}
	}else{
		precision[0]=0;
		recall[0]=0;
		F1[0]=0;
	}




}

template<class T> typename ttt::TissueDescriptor<2>::Pointer DoSegmentation(typename T::Pointer & inputImage,double threshold,double limit,double platenessMin, double platenessRange,double vertexnessMin, double vertexnessRange,double radiusSize){


	typedef itk::Image<float,2> VertexnessImageType;
	typedef itk::Image<float,2> RidgenessImageType;
	typedef itk::SymmetricSecondRankTensor< float, 2 > HessianPixelType;
	typedef itk::Image< HessianPixelType, 2 >           HessianImageType;


	typedef itk::ImageFileWriter<VertexnessImageType> FileWriterType;


	typedef ttt::HessianRidgenessMeasureImageFilter< HessianImageType, RidgenessImageType > RidgenessFilterType;
	typename RidgenessFilterType::Pointer ridgenessFilter = RidgenessFilterType::New();
	double sigmaMinimum=platenessMin-platenessRange;
	double sigmaMaximum=platenessMin+platenessRange;
	int numberOfSigmaSteps=4;
	typedef itk::MultiScaleHessianBasedMeasureImageFilter< T, HessianImageType, RidgenessImageType > MultiScaleEnhancementFilterType;
	typename MultiScaleEnhancementFilterType::Pointer ridgenessMultiScaleEnhancementFilter =  MultiScaleEnhancementFilterType::New();


	ridgenessMultiScaleEnhancementFilter->SetInput( inputImage );
	ridgenessMultiScaleEnhancementFilter->SetHessianToMeasureFilter( ridgenessFilter );
	ridgenessMultiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
	ridgenessMultiScaleEnhancementFilter->SetSigmaMinimum( sigmaMinimum );
	ridgenessMultiScaleEnhancementFilter->SetSigmaMaximum( sigmaMaximum );
	ridgenessMultiScaleEnhancementFilter->SetNumberOfSigmaSteps( numberOfSigmaSteps );


	ridgenessMultiScaleEnhancementFilter->Update();

	typename FileWriterType::Pointer ridgenessWriter = FileWriterType::New();

	ridgenessWriter->SetFileName("ridgeness.tif");
	ridgenessWriter->SetInput(ridgenessMultiScaleEnhancementFilter->GetOutput());
	ridgenessWriter->Update();

	//2. Extract verteness
	typedef ttt::HessianVertexnessMeasureImageFilter< HessianImageType, VertexnessImageType >  VertexnessFilterType;
	VertexnessFilterType::Pointer vertexnessFilter = VertexnessFilterType::New();
	double vertexnessSigmaMinimum=vertexnessMin-vertexnessRange;
	double vertexnessSigmaMaximum=vertexnessMin+vertexnessRange;
	int vertexnessNumberOfSigmaSteps=4;



	typedef itk::MultiScaleHessianBasedMeasureImageFilter< T, HessianImageType, VertexnessImageType > MultiScaleEnhancementFilterType;

	typename MultiScaleEnhancementFilterType::Pointer vertexnessMultiScaleEnhancementFilter =  MultiScaleEnhancementFilterType::New();
	vertexnessMultiScaleEnhancementFilter->SetInput( inputImage );
	vertexnessMultiScaleEnhancementFilter->SetHessianToMeasureFilter( vertexnessFilter );
	vertexnessMultiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
	vertexnessMultiScaleEnhancementFilter->SetSigmaMinimum( vertexnessSigmaMinimum );
	vertexnessMultiScaleEnhancementFilter->SetSigmaMaximum( vertexnessSigmaMaximum );
	vertexnessMultiScaleEnhancementFilter->SetNumberOfSigmaSteps( vertexnessNumberOfSigmaSteps );

	vertexnessMultiScaleEnhancementFilter->Update();


	typename FileWriterType::Pointer vertexnessWriter = FileWriterType::New();

	vertexnessWriter->SetFileName("vertexness.tif");
	vertexnessWriter->SetInput(vertexnessMultiScaleEnhancementFilter->GetOutput());
	vertexnessWriter->Update();

	 typedef itk::LocalMaximumImageFilter<VertexnessImageType> LocalMaxFilter;

	typename LocalMaxFilter::Pointer localmax=LocalMaxFilter::New();

	localmax->SetInput(vertexnessMultiScaleEnhancementFilter->GetOutput());
	  localmax->SetThreshold(threshold);
	  itk::Size<2> radius;
	  radius.Fill(radiusSize);
	  localmax->SetRadius(radius);
	  localmax->GenerateData();

	  typename LocalMaxFilter::LocalMaximaContainerPointerType output = localmax->GetOutput();


	  ttt::AdherensJunctionSegmentationDijkstraCommand<2> segmentator;
	  segmentator.SetPlatenessImage(ridgenessMultiScaleEnhancementFilter->GetOutput());
	  segmentator.SetVertexnessImage(vertexnessMultiScaleEnhancementFilter->GetOutput());
	  segmentator.SetVertexLocations(output);
	  segmentator.SetLimit(limit);
	  segmentator.Do();



	  ttt::CellGraphCommand<2> dual;

	  dual.SetPrimalGraph(segmentator.GetTissueDescriptor());
	  dual.Do();

#if 0
	  ttt::PrimalGraphDrawer<ttt::TissueDescriptor<2> > drawer;
	  std::cout << "END" << std::endl;

	  ttt::DualGraphDrawer<ttt::TissueDescriptor<2> > dualDrawer;

	  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> > ::SkeletonVertexType  > vertexColorer;
	  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> >::SkeletonEdgeType  > edgeColorer;

	  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> > ::CellVertexType  > cellVertexColorer;
	  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> >::CellEdgeType  > cellEdgeColorer;

	  dualDrawer.SetVertexColorer(&cellVertexColorer);
	  dualDrawer.SetEdgeColorer(&cellEdgeColorer);

	  drawer.SetVertexColorer(&vertexColorer);
	  drawer.SetEdgeColorer(&edgeColorer);


	  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	  drawer.SetRenderer(renderer);
	  drawer.SetTissueDescriptor(dual.GetGraphs());
	  drawer.Draw();
	  drawer.Show();

	  dualDrawer.SetRenderer(renderer);
	  dualDrawer.SetTissueDescriptor(dual.GetGraphs());
	  dualDrawer.Draw();
	  dualDrawer.Show();

	  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

	  renderWindow->AddRenderer(renderer);

	  vtkSmartPointer<vtkRenderWindowInteractor> irenWin = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	  irenWin->SetRenderWindow(renderWindow);

	  renderWindow->Render();
	  irenWin->Start();
#endif



	  return dual.GetGraphs();

}



class segmentation_cost_function: public vnl_cost_function {


public:
	typedef itk::Image<float,2> InputImageType;
	typename InputImageType::Pointer divided;
	typename  ttt::TissueDescriptor<3>::Pointer   m_Reference ;
	std::ofstream  m_Output;

	segmentation_cost_function(char * outputName): vnl_cost_function(7){
		m_Output.open(outputName,std::ifstream::out);
		//m_Output << "distanceWeight,areaWeight,ellipseWeight,creationWeight,terminationWeight,associationWeight,mitosisWeight,trueAssociations,wrongAssociations,associationAsTermination,associationAsMitosis,trueTermination,terminationAsAssociation,terminationAsMitosis,trueCreation,creationAsAssociation,creationAsMitosis,trueMitosis,wrongMitosis,mitosisAsAssociation,mitosisAsTermination,precisionAssociations,recallAssociations,F1Associations,precisionTermination,recallTermination,F1Termination,precisionCreation,recallCreation,F1Creation,precisionMitosis,recallMitosis,F1Mitosis,F1" << std::endl;
		//m_Output << "distanceWeight,areaWeight,perimeterWeight,xxWeight,xyWeight,yyWeight,creationWeight,terminationWeight,associationWeight,mitosisWeight,apoptosisWeight,F1" << std::endl;


	}
	~segmentation_cost_function(){
		m_Output.close();
	}

	void SetReference(ttt::TissueDescriptor<3>::Pointer & reference){
		m_Reference=reference;

	}

	void SetInputImage(typename InputImageType::Pointer & image){
		divided=image;
	}
public:
	double f(const vnl_vector<double>& x)  {

		double x0 =std::abs(x(0));
		double x1 =std::abs(x(1));
		double x2 =std::abs(x(2));
		double x3 =std::abs(x(3));
		double x4 =std::abs(x(4));
		double x5 =std::abs(x(5));
		double x6 =std::abs(x(6));
#if 0
		if(x0<0) x0=0;
		if(x1<0) x1=0;
		if(x2<0) x2=0;
		if(x3<0) x3=0;
		if(x4<0) x4=0;
		if(x5<0) x5=0;
		if(x6<0) x6=0;

#endif
#if 0
		if(x0<0.00001) x0=0.00001;
		if(x1<1/100) x1=1/100;
		if(x2<0.05) x2=0.05;
		if(x3<0) x3=0;
		if(x4<0.05) x4=0.05;
		if(x5<0) x5=0;
		if(x6<1/10) x6=1/10;
#endif

		double threshold=0.00001+x0;
		double limits=1+x1*100;
		double platenessMin=0.05+x2;
		double platenessRange=x3;
		double vertexnessMin=0.05+x4;
		double vertexnessRange=x5;
		double radius=1+x6*10;

		std::cout << threshold << "," << limits << "," << platenessMin << "," << platenessRange << ","<< vertexnessMin << "," << vertexnessRange << "," << radius << std::endl;
		typename ttt::TissueDescriptor<2>::Pointer descriptor =DoSegmentation<InputImageType>(divided,threshold,limits,platenessMin,platenessRange,vertexnessMin,vertexnessRange,radius);
		//
		double precision,recall,F1;
		computeF1Score<ttt::TissueDescriptor<2>, ttt::TissueDescriptor<3> >(descriptor,m_Reference,&precision,&recall,&F1);
		std::cout << precision << "," << recall << "," << F1 <<std::endl;

		m_Output << threshold << "," << limits << "," << platenessMin << "," << platenessRange << "," << vertexnessMin << "," << vertexnessRange << "," << radius << "," << precision << "," << recall  << "," << F1 <<   std::endl;
		return -F1;

	}
};




int main(int argc, char ** argv){

	typename ttt::TissueDescriptor<3>::Pointer reference;
	typedef itk::Image<float,2> InputImageType;

	ttt::QTSQLTissueTrackingProject2<3> * projectqtNotum = new ttt::QTSQLTissueTrackingProject2<3>;
	projectqtNotum->SetDBName("TuftsTissueTracker");
	projectqtNotum->SetHost("localhost");
	projectqtNotum->SetUser("root");
	projectqtNotum->SetPassword("ttt1Tracker");

	projectqtNotum->SetProjectID(2);
	ttt::TissueTrackingAbstractProject2<3> * projectNotum=projectqtNotum;
	projectNotum->Open();
	itk::FixedArray<double,3> spacing = projectNotum->GetSpacing();

	reference=projectNotum->GetTissueDescriptor(0);
	std::cout << reference->GetNumCells() << std::endl;







	typedef itk::ImageFileReader<InputImageType> FileReaderType;
	typename FileReaderType::Pointer reader=FileReaderType::New();
	reader->SetFileName(argv[1]);
	reader->Update();
	typedef itk::DivideImageFilter<InputImageType,InputImageType,InputImageType> DivideType;
	typename DivideType::Pointer divider = DivideType::New();
	typedef itk::ImageFileWriter<InputImageType> FileWriterType;

	typename InputImageType::Pointer input = reader->GetOutput();

	InputImageType::SpacingType inputSpacing;
	inputSpacing[0]=spacing[0];
	inputSpacing[1]=spacing[1];
	input->SetSpacing(inputSpacing);
	divider->SetInput1(input);
	divider->SetConstant2(1.0);
	divider->Update();

	typename FileWriterType::Pointer dividedwriter = FileWriterType::New();
	dividedwriter->SetFileName("divided.tif");
	dividedwriter->SetInput(divider->GetOutput());
	dividedwriter->Update();


	InputImageType::Pointer divided= divider->GetOutput();

	vnl_vector<double> x0(7);
	//0.02082,77.3571,0.45,0.190196,0.703413,0,10.8766
	//0.00572289,77.3571,0.45,0.190196,0.703413,0,10.8766,0.483709,0.502604,0.492976
	//0.00792187,79.9644,0.455432,0.174933,0.683341,9.24891e-05,11.3531,0.564865,0.544271,0.554377

	x0(0)=0.00792187-0.00001;
	x0(1)=(79.9644-1)/100; //area
	x0(2)=0.455432-0.05;//0.455673 + 0.380393/2 ; //perimeter
	x0(3)=0.174933; //xxv0.320331
	x0(4)=0.683341-0.05 ; //xy
	x0(5)=9.24891e-05; //yy
	x0(6)=(11.3531 -1 )/10;

	//double threshold=0.00001+x0;
	//		double limits=1+x1*100;
	//double platenessMin=0.05+x2;
	//	double platenessRange=x3;
	//	double vertexnessMin=0.05+x4;
	//	double vertexnessRange=x5;
	//	double radius=1+x6*10;


#if 0
	//0.0280357,77.2093,0.622742,0.318622,0.660706,0.054581,9.49084,0.180645,0.152174,0.165192
	//0.0280357,77.2099,0.62274,0.318624,0.660695,0.0545814,6.00001,0.154696,0.152174,0.153425
	//0.02851,338.995,0.62544,0.313653,0.657064,0,6,0.180124,0.157609,0.168116
	//1e-05,338.995,0.62544,0.313653,0.657064,0,6,0.141026,0.179348,0.157895
	//6.12353e-05,400.602,0.38852,0.272128,0.833157,4.37588e-05,8.09019,0.16092,0.152174,0.156425
	//9.50617e-05,410.506,0.718898,0.235638,0.694323,2.68494e-05,8.09742
	//0.00189634,690.074,0.687557,0.590496,0.690542,0.0955784,8.05081,0.144033,0.190217,0.163934
	//0.00252446,890.087,0.688053,0.591282,0.691332,0.0957158,6.77139,0.172691,0.233696,0.198614
	//0.00252518,891.037,0.688236,0.591452,0.690872,0.0956944,7.34117,0.176955,0.233696,0.201405
	//0.00252518,1091.07,0.683535,0.577802,0.689778,0.0883228,7.23621,0.1875,0.244565,0.212264

	//0.00242518,991.053,0.666944,0.577685,0.689005,0.0883228,7.23621,0,0,0
	//0.00242518,991.053,0.666944,0.577685,0.689139,0.0883228,7.23621,0.194093,0.25,0.218527
	x0(0)=0.00242518 - 0.00001;
	x0(1)=(991.053-1)/100; //area
	x0(2)=0.666944 - 0.05; //perimeter
	x0(3)=0.577685; //xxv0.320331
	x0(4)=0.689005 - 0.05; //xy
	x0(5)=0.0883228; //yy
	x0(6)=(7.23621-1)/10;
#endif

	segmentation_cost_function segmentationFunction("output-notum2D.txt");
	segmentationFunction.SetInputImage(divided);
	segmentationFunction.SetReference(reference);
#if 1
	vnl_vector<double> minimums(7), maximums(7);
	minimums(0)=0.001; maximums(0)=0.1;
	minimums(1)=0.01; maximums(1)=2;
	minimums(2)=0; 	 maximums(2)=1;
	minimums(3)=0; maximums(3)=0.6;
	minimums(4)=0; maximums(4)=1;
	minimums(5)=0; maximums(5)=0.6;
	minimums(6)=0; maximums(6)=2;


	segmentationFunction.f(x0);
	for(int i=0;i<x0.size();i++){
		vnl_vector<double> x1=x0;
		double step = (maximums(i)-minimums(i))/5;

		for(double value=minimums(i);value<=maximums(i);value=value+step){
			x1(i)=value;
			segmentationFunction.f(x1);
		}
	}
#endif

#if 0
	vnl_amoeba optim(segmentationFunction);
	//vnl_powell optim(&segmentationFunction);


	optim.set_relative_diameter(0.1);
	optim.set_f_tolerance(1);
	optim.set_max_iterations(200);
	optim.minimize(x0);
#endif
#if 0
	for(int i=0;i<thresholds.size();i++){
		for(int j=0;j<limits.size();j++){

			typename ttt::TissueDescriptor<2>::Pointer descriptor = DoSegmentation<InputImageType>(divided,thresholds[i],limits[j],0.25,0.1,0.25,0.1,3);
			double precision,recall,F1;
			computeF1Score<ttt::TissueDescriptor<2>, ttt::TissueDescriptor<3> >(descriptor,reference,&precision,&recall,&F1);
			std::cout << precision << "," << recall << "," << F1 <<std::endl;
#if 0
			  ttt::PrimalGraphDrawer<ttt::TissueDescriptor<2> > drawer;
			  std::cout << "END" << std::endl;

			  ttt::DualGraphDrawer<ttt::TissueDescriptor<2> > dualDrawer;

			  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> > ::SkeletonVertexType  > vertexColorer;
			  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> >::SkeletonEdgeType  > edgeColorer;

			  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> > ::CellVertexType  > cellVertexColorer;
			  DefaultColorer<ttt::TissueDescriptorTraits<ttt::TissueDescriptor<2> >::CellEdgeType  > cellEdgeColorer;

			  dualDrawer.SetVertexColorer(&cellVertexColorer);
			  dualDrawer.SetEdgeColorer(&cellEdgeColorer);

			  drawer.SetVertexColorer(&vertexColorer);
			  drawer.SetEdgeColorer(&edgeColorer);


			  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
			  drawer.SetRenderer(renderer);
			  drawer.SetTissueDescriptor(descriptor);
			  drawer.Draw();
			  drawer.Show();

			  dualDrawer.SetRenderer(renderer);
			  dualDrawer.SetTissueDescriptor(descriptor);
			  dualDrawer.Draw();
			  dualDrawer.Show();

			  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

			  renderWindow->AddRenderer(renderer);

			  vtkSmartPointer<vtkRenderWindowInteractor> irenWin = vtkSmartPointer<vtkRenderWindowInteractor>::New();

			  irenWin->SetRenderWindow(renderWindow);

			  renderWindow->Render();
			  irenWin->Start();
#endif
		}
	}

#endif


	  //3. Find localmaxima

	  //4 Extract ridges


	  //4. Compute segmentation

	  //5. Evaluate


	  // PARA CADA CELULA DETECTADA
	  // BUSCAR CORRESPONDENCIA EN GROUND TRUTH





#if 0
		double precision=(double)hit/retrieved;
		double recall = (double)hit/total;
		double f1= (2*precision*recall)/(precision+recall);
		std::cout << precision << "," << recall << "," << f1;


	  typename ttt::AdherensJunctionVertices::Pointer matched = ttt::AdherensJunctionVertices::New();




	  	std::cout << "Threshold,Precision,Recall,F1" << std::endl;
	  	for(vector<PointAndValueType>::reverse_iterator it = detections.rbegin();it!=detections.rend();it++){
	  		count++;
	  		//std::cout << "Trying to match " << (*it).first << std::endl;
	  		double minValue=1e128;
	  		ttt::AdherensJunctionVertices::iterator minLoc;

	  		for(typename ttt::AdherensJunctionVertices::iterator gtIt=gt->begin();gtIt!=gt->end();gtIt++){

	  			IndexType  gtPoint = (*gtIt)->GetPosition();

	  			double dist=sqrt(pow(gtPoint[0] - it->first[0],2) +pow(gtPoint[1] - it->first[1],2));

	  			if(dist<minValue){
	  				minValue=dist;
	  				minLoc=gtIt;
	  			}

	  		}

	  		if(minValue<=THRESHOLD){
	  			//std::cout << "MATCHED TO " << (*minLoc)->GetPosition() << std::endl;
	  			gt->erase(minLoc);
	  			hit++;

	  		}

	  		precision = (double)hit/count;
	  		recall = (double)hit/positive;
	  		f1 = (2*precision*recall)/(precision+recall);
	  		std::cout << it->second << "," << precision << "," << recall << "," << f1 << std::endl;
	  	}


#endif


	  return EXIT_SUCCESS;
	}



