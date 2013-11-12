#ifndef __itkVertexnessMeasurementFunction_h
#define __itkVertexnessMeasurementFunction_h

#include "tttObjectnessMeasurementFunction.h"

namespace ttt
{


class VertexnessMeasurementFunction:public ObjectnessMeasurementFunction
{
public:
  /** Standard class typedefs. */
  typedef VertexnessMeasurementFunction   Self;
  typedef LightObject                Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(VertexnessMeasurementFunction, ObjectnessMeasurementFunction);

  itkNewMacro(Self);
  
  /** Set/Get macros for Alpha */
  inline void SetAlpha(double alpha){
	  m_Alpha=alpha;
  }

  itkGetMacro(Alpha, double);
  

  /** Set/Get macros for Beta */
  inline void SetBeta(double beta){
  	  m_Beta=beta;
    }

  itkGetMacro(Beta, double);

  /** Set/Get macros for Gamma */
  inline void SetGamma(double gamma){
	  m_Gamma=gamma;
  }
  itkGetMacro(Gamma, double);


  /** Set/Get macros for C */
  inline void SetC(double C){
	  m_C=C;
  }

  itkGetMacro(C, double);
  


  /** Macro to scale the vesselness measure with the
      largest eigenvalue or not */
  inline void SetScaleVertexnessMeasure(bool scale){
	  m_ScaleVertexnessMeasure=scale;
  }
  itkGetMacro( ScaleVertexnessMeasure, bool );
  itkBooleanMacro(ScaleVertexnessMeasure);


  virtual double ComputeObjectProperty(const FixedArray< double,3> & eigenValue) {
		//

		double result = 0;
		// Find the smallest eigenvalue
		double smallest = vnl_math_abs(eigenValue[0]);
		double Lambda1 = eigenValue[0];
		for (unsigned int i = 1; i <= 2; i++) {
			if (vnl_math_abs(eigenValue[i]) < smallest) {
				Lambda1 = eigenValue[i];
				smallest = vnl_math_abs(eigenValue[i]);
			}
		}

		// Find the largest eigenvalue
		double largest = vnl_math_abs(eigenValue[0]);
		double Lambda3 = eigenValue[0];

		for (unsigned int i = 1; i <= 2; i++) {
			if (vnl_math_abs(eigenValue[i] > largest)) {
				Lambda3 = eigenValue[i];
				largest = vnl_math_abs(eigenValue[i]);
			}
		}

		//  find Lambda2 so that |Lambda1| < |Lambda2| < |Lambda3|
		double Lambda2 = eigenValue[0];

		for (unsigned int i = 0; i <= 2; i++) {
			if (eigenValue[i] != Lambda1 && eigenValue[i] != Lambda3) {
				Lambda2 = eigenValue[i];
				break;
			}
		}

		assert(vnl_math_abs(Lambda3) > vnl_math_abs(Lambda2));
		assert(vnl_math_abs(Lambda2) > vnl_math_abs(Lambda1));
		if (Lambda3 < 0 && Lambda2 < 0 && Lambda1 < 0) {
			return 0;
		} else {

			double S = vcl_sqrt(Lambda1*Lambda1 + Lambda2*Lambda2 * Lambda3*Lambda3);
			double A = Lambda3/Lambda2;
			double B = Lambda2/Lambda1;

			double Lambda3Sqr=vnl_math_sqr(Lambda3);

			double GammaSqr=m_Gamma*m_Gamma;
			double AlphaSqr=m_Alpha*m_Alpha;
			double BetaSqr=m_Beta*m_Beta;
			double term1 =  ( vcl_exp( -1.0 * (( vnl_math_sqr(S)))  / (2.0*GammaSqr)));
			double term2  = ( 1 - vcl_exp( -1.0 * (( vnl_math_sqr( A )) / (2.0*AlphaSqr))));
			double term3  = ( 1 - vcl_exp( -1.0 * (( vnl_math_sqr( B )) / (2.0*BetaSqr))));
			double term4 = vcl_exp(-1.0 * (2.0 * vnl_math_sqr(m_C)) / (Lambda3Sqr));
	//			double term1 =  ( 1 - vcl_exp( -1.0 * (S)));
	//			double term2  = ( 1 - vcl_exp( -1.0 * (A)));
	//			double term3  = ( 1 - vcl_exp( -1.0 * (B)));

				double vertexnessMeasure=term1*term2*term3*term4;

			if (m_ScaleVertexnessMeasure) {
				double Lambda3Abs=vcl_abs(Lambda3);
				result = Lambda3Abs * vertexnessMeasure;
			}else{
				result=vertexnessMeasure;
			}
			return result;
		}
	}

protected:
  VertexnessMeasurementFunction(){
  	  m_Alpha = 0.5;
  	  m_Beta  = 0.5;
  	  m_Gamma = 8;
  	  m_C = 10e-6;
  	  m_ScaleVertexnessMeasure=false;
    }
  virtual ~VertexnessMeasurementFunction() {

  }

  void PrintSelf(std::ostream & os, Indent indent) const{

	  os << indent << "Alpha: " << m_Alpha << std::endl;
	  os << indent << "Beta:  " << m_Beta  << std::endl;
	  os << indent << "Gamma: " << m_Gamma << std::endl;

	  os << indent << "C: " << m_C << std::endl;;
  }

private:
  VertexnessMeasurementFunction(const Self &); //purposely not implemented
  void operator=(const Self &);           //purposely not implemented

  double                                        m_Alpha;
  double                                        m_Beta;
  double                                        m_Gamma;

  double                                        m_C;

  bool                                          m_ScaleVertexnessMeasure;

};
} // end namespace itk

#endif
