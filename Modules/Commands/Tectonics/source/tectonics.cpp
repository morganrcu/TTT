#include <vnl/vnl_matrix_fixed.h>
#include <vnl/algo/vnl_amoeba.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/vnl_inverse.h>
#include <vnl/algo/vnl_real_eigensystem.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <algorithm>
#include <itkPoint.h>
#include "tectonics.h"



#define DTOR ( M_PI/180.0)

void parametricEllipseToMatrixEllipse(const double Rx, const double Ry, const double angle, vnl_matrix_fixed<double,2,2> & result){
	vnl_matrix_fixed<double,2,2> V,D;

	V(0,0)=cos(angle);
	V(1,0)=sin(angle);
	V(0,1)=-sin(angle);
	V(1,1)=cos(angle);

	D(0,0)=Rx;
	D(0,1)=0;
	D(1,0)=0;
	D(1,1)=Ry;

	result = V*D*V.transpose();
}
void polarToRect(double radius, double phase, Vector & r) {

	r(0) = radius * cos(phase);
	r(1) = radius * sin(phase);
}

#if 0
;convert eigenvalues to exponents
function ExpSymmTensor, ppTensor
Evecs = Tensor_to_Evecs(ppTensor)
Evecs[[0,3]] = EXP(Evecs[[0,3]])
Tensor = Evecs_to_Tensor(Evecs)
return, Tensor
end
#endif

Tensor expSymmTensor(const Tensor & ppTensor) {

	vnl_symmetric_eigensystem<double> eigensystem(ppTensor);

	Tensor D(2, 2);

	D(0, 0) = eigensystem.D(0, 0);
	D(0, 1) = 0;
	D(1, 0) = 0;
	D(1, 1) = eigensystem.D(1, 1);

	D(0, 0) = exp(D(0, 0));
	D(1, 1) = exp(D(1, 1));

	return eigensystem.V * D * vnl_inverse(eigensystem.V);
}

#if 0
;convert eigenvalues to logs
function LogSymmTensor, Tensor
	Evecs = Tensor_to_Evecs(Tensor)
	Evecs[[0,3]] = ALOG(Evecs[[0,3]])
	ppTensor = Evecs_to_Tensor(Evecs)
	return, ppTensor
end
#endif

Tensor logSymmTensor(const Tensor & ppTensor) {
	vnl_symmetric_eigensystem<double> eigensystem(ppTensor);
	vnl_matrix<double> D(2, 2);

	D(0, 0) = eigensystem.D(0, 0);
	D(0, 1) = 0;
	D(1, 0) = 0;
	D(1, 1) = eigensystem.D(1, 1);

	D(0, 0) = log(D(0, 0));
	D(1, 1) = log(D(1, 1));

	return eigensystem.V * D * vnl_inverse(eigensystem.V);
}

#if 0

;converts from x,y coordinates to radius,theta coordinates and sorts by theta
function Convert_rxy_to_radth, rxy,DEG=deg
n = N_ELEMENTS(rxy[0,*])
radth = FLTARR(2,n)

radth[0,*] = SQRT(rxy[0,*]^2 + rxy[1,*]^2);radii
radth[1,*] = ATAN(rxy[1,*],rxy[0,*]); theta
if KEYWORD_SET(deg) then begin
radth[1,*] = ROUND(radth[1,*] / !DTOR)
idx = WHERE(radth[1,*] LT 0)
radth[1,idx] += 360;from 0 -> 360 East, anticlockwise
radth[1,*] *= !DTOR
endif else begin
idx = WHERE(radth[1,*] LT 0)
radth[1,idx] += 2*!PI;from 0 -> 2PI East, anticlockwise
endelse

mn = MIN(radth[1,*],mnidx)
for j=0,1 do radth[j,*] = SHIFT(radth[j,*],-mnidx)
radth = [[radth],[radth[*,0]]]
radth[1,n] += 2*!PI
return, radth
end
#endif

struct MyComparator{
	bool operator()(const std::pair<double, double> & a ,const std::pair<double, double> & b) const{
		return a.second < b.second;
	}
}mycomparator;

std::vector<std::pair<double,double> > Convert_rxy_to_radth(const std::vector<vnl_vector<double> > & rxy,
		const bool deg = false) {

	int n = rxy.size();
	std::vector<std::pair<double, double> > radth(n);

	for (int i = 0; i < n; i++) {
		radth[i].first = rxy[i].magnitude();
		radth[i].second = atan2(rxy[i](1), rxy[i](0));
		//std::cout << rxy[i] << "--> " << "(" << radth[i].first << "," << radth[i].second << ")" << std::endl;
	}

	if (deg) {
		for (int i = 0; i < n; i++) {
			radth[i].second = round(radth[i].second / DTOR);
			if (radth[i].second < 0) {
				radth[i].second += 360;
				//radth[i].second *= DTOR;
			}
			radth[i].second *=DTOR;
		}
	} else {
		for (int i = 0; i < n; i++) {
			if (radth[i].second < 0) {
				radth[i].second += 2 * M_PI;
			}
		}
	}
	std::sort(radth.begin(),radth.end(),mycomparator);
#if 0
	for(int i=0;i<n;i++){
		std::cout << radth[i].first << " " << radth[i].second << std::endl;
	}
#endif
#if 0
	mn = MIN(radth[1,*],mnidx)
	for j=0,1 do radth[j,*] = SHIFT(radth[j,*],-mnidx)
	radth = [[radth],[radth[*,0]]]
	radth[1,n] += 2*!PI
	return, radth
#endif
	//TODO END

	return radth;

}
#if 0
;convert a vector to its direction in radians from 0->2Pi, from East, anticlockwise
function Vector_to_Radians, v
return, (ATAN(v[1],v[0]) + 2*!PI) MOD (2*!PI)
end
#endif

double vectorToRadians(const vnl_vector<double> & v) {
	return atan2(v(1), v(0)) + M_PI;
}
#if 0
;Construct a 2x2 rotation matrix, for use in rotating coordinates
;by Rot radians anticlockwise around the z-axis
function RotationMatrix, Rot
return, [[COS(Rot),SIN(Rot)],[-SIN(Rot),COS(Rot)]]
end
#endif

vnl_matrix<double> rotationMatrix(double rotation) {
	vnl_matrix<double> result(2, 2);
	result(0, 0) = cos(rotation);
	result(0, 1) = -sin(rotation);
	result(1, 0) = sin(rotation);
	result(1, 1) = cos(rotation);
	return result;
}

#if 0
;Convert from major/minor axis (=Eigenvector) description of ellipse to perimeter xy
;coordinates, sampled at theta intervals
function Get_EllipsePerimxy, Evecs,theta
cossqth = COS(theta) ^ 2
eccentricity = SIN(ACOS(Evecs[3] / Evecs[0]))
rad = Evecs[3] / SQRT(1.0 - eccentricity^2 * cossqth)
r0 = CV_COORD(FROM_POLAR = TRANSPOSE([[theta],[rad]]),/TO_RECT)
phi = Vector_to_Radians(Evecs[[1,2]]) MOD !PI;orientation not direction
r0
##= RotationMatrix(phi)
return, r0
end
#endif

vnl_vector<double> GetEllipsePerimXY(const vnl_symmetric_eigensystem<double> & Evecs, double theta) {

	double cossqrth = pow(cos(theta), 2);

	//std::cout << Evecs.D << std::endl;
	double eccentricity = sin(acos(Evecs.D(0, 0) / Evecs.D(1, 1)));

	double rad = Evecs.D(0, 0)/ sqrt(1.0 - pow(eccentricity, 2) * cossqrth);

	Vector r0;
	polarToRect(rad, theta, r0);

	double phi = vectorToRadians(Evecs.V.get_column(1));
	if(phi>M_PI) phi=phi-M_PI;

	r0 =  rotationMatrix(phi)* r0;

	return r0;

}
#if 0
;Calculates the proportion area difference between Ellipse0 deformed
;by the StrainTensor and Ellipse1. Optimized mappings result in a pp
;area difference very close to zero.
;To calculate the area overlap between two shapes, the shapes
;are broken down into 360 radial sectors, and the area of
;each pair of sectors with the same theta is compared.
;This is straightforward for Ellipse1, but the deformation StrainTensor
# Ellipse0
;alters theta as well as the radii, so it must be resampled for the
;same theta as Ellipse1.
function EvaluateEllipseMapping, Ellipse0,Ellipse1,StrainTensor

end

#endif

vnl_matrix<double> EvalOrient_To_Tensor(double eval1, double eval2, double orient){
	vnl_matrix<double> V(2,2);

	V(0,0)=cos(orient);
	V(0,1)=-sin(orient);
	V(1,0)=sin(orient);
	V(1,1)=cos(orient);


	vnl_matrix<double> D(2,2);

	D(0,0)=eval1;
	D(0,1)=0;
	D(1,0)=0;
	D(1,1)=eval2;
	return V*D*vnl_inverse(V);
}
#if 0
;convert 2 eigenvalues and orientation of 1st eigenvector to a symmetric
;tensor
function EvalOrient_to_Tensor, eval1,eval2,orient
v = Radians_to_Vector(orient)
Evecs = [eval1,v,eval2,-v[1],v[0]]
return, Evecs_to_Tensor(Evecs)
end
#endif



#if 0
function AMB_Minimize_2Ellipse_AreaDiff, args;[eval0,orient]
COMMON Min2E_data, Ellipse0,Ellipse1,Tr
StrainTensor = EvalOrient_to_Tensor(args[0],Tr-args[0],args[1])
return, EvaluateEllipseMapping(Ellipse0,Ellipse1,StrainTensor)
end
#endif

void interpolate(const std::vector<std::pair<double,double> > & data, std::vector<std::pair<double, double> > & Pts){
	double p,a,b,fa,fb,fp,d;
	for(int i=0;i<Pts.size();i++){
		p=Pts[i].second;
		bool found=false;
		for(int j=0;j<data.size()-1;j++){
			if( data[j].second <= p && p <= data[j+1].second ){
				found =true;
				a=data[j].second;fa=data[j].first;
				b=data[j+1].second; fb=data[j+1].first;
				break;
			}
		}
		if(!found){
			a= data.back().second;
			fa= data.back().first;
			b=data[0].second;
			fb= data[0].first;
			d = (fb -fa)/(2*M_PI - (b-a));
		}else{
			d= (fb-fa)/(b-a);

		}
//		std::cout << a << " < " << p << " < " << b  << std::endl;
		fp= fa + d*(p-a);
		Pts[i].first=fp;
	}
}
double evaluateEllipseMapping(const EllipseMatrix & ellipse0,
		const EllipseMatrix & ellipse1, vnl_matrix<double> strainTensor) {
#if 0
	std::cout << "ellipse0" << std::endl;
	std::cout << ellipse0 << std::endl;
	std::cout << "ellipse1" << std::endl;
	std::cout << ellipse1 << std::endl;

	std::cout << "strainTensor" << std::endl;
	std::cout << strainTensor << std::endl;
#endif

	vnl_symmetric_eigensystem<double> evecs0(ellipse0); //Evecs0 = Tensor_to_Evecs(Ellipse0) ;unpack ellipse tensor description to major and minor axis information
#if 0
	std::cout << "evecs0.D" << std::endl;
	std::cout << evecs0.D << std::endl;

	std::cout << "evecs0.V" << std::endl;
	std::cout << evecs0.V << std::endl;
#endif

	vnl_symmetric_eigensystem<double> evecs1(ellipse1); //Evecs1 = Tensor_to_Evecs(Ellipse1)
#if 0
	std::cout << "evecs1.D" << std::endl;

	std::cout << evecs1.D << std::endl;

	std::cout << "evecs1.V" << std::endl;
	std::cout << evecs1.V << std::endl;
#endif
	int n = 360; //n = 360

	//th = FINDGEN(n) * !DTOR

	std::vector<vnl_vector<double> > Perimxy0(n), Perimxy1(n), defPerimxy0(n);
#if 0
	std::cout << "evecs1.Vreal" << std::endl;

	std::cout << evecs1.V << std::endl;
#endif
	double phi1 = vectorToRadians(evecs1.V.get_column(1)); // phi1 = Vector_to_Radians(Evecs1[[1,2]]) MOD !PI ;major axis orientation not direction

	for (int i = 0; i < n; i++) {
		double th = DTOR * i;

		Perimxy0[i] = GetEllipsePerimXY(evecs0, th); //    Perimxy0 = Get_EllipsePerimxy(Evecs0,th)
		//std::cout << Perimxy0[i] << std::endl;
		double anglefrac = (phi1 / DTOR);   //anglefrac = (phi1 / !DTOR) MOD 1.0

		//std::cout << anglefrac << std::endl;
		th = (i - anglefrac) * DTOR; //th = (FINDGEN(n) - anglefrac) * !DTOR

		//std::cout << th << std::endl;
		Perimxy1[i] = GetEllipsePerimXY(evecs1, th); // //Perimxy1 = Get_EllipsePerimxy(Evecs1,th)

		//std::cout << Perimxy1[i] << std::endl;

		//  ;deform ellipse0 by strain tensor
		defPerimxy0[i] = expSymmTensor(strainTensor).transpose() * Perimxy0[i]; //defPerimxy0 = ExpSymmTensor(StrainTensor) # Perimxy0
		//std::cout << defPerimxy0[i] << std::endl;
		// ;convert perim xy coords to radii for each theta

		//std::cout << Perimxy0[i] << " " << Perimxy1[i] << " " << defPerimxy0[i] << std::endl;
	}
#if 0
	std::cout << "PerimXY0=[";
	for(int i=0;i<Perimxy0.size();i++){
		std::cout << Perimxy0[i] << ";";
	}
	std::cout<<"]" << std::endl;
	std::cout << "===================" << std::endl;
	std::cout << "PerimXY1=[";
	for(int i=0;i<Perimxy1.size();i++){
			std::cout << Perimxy1[i] << ";";
	}
	std::cout<<"]" << std::endl;
	std::cout << "===================" << std::endl;

	std::cout << "defperimxy0=[";
		for(int i=0;i<defPerimxy0.size();i++){
				std::cout << defPerimxy0[i] << ";";
		}
		std::cout<<"]" << std::endl;
		std::cout << "===================" << std::endl;
#endif
	std::vector<std::pair<double, double> > radth1;
	std::vector<std::pair<double, double> > radthdef0;

	radth1=Convert_rxy_to_radth(Perimxy1,true);

#if 0
	radth1 = Convert_rxy_to_radth(Perimxy1,/DEG);all th +ve, increasing from 0
	radthdef0 = Convert_rxy_to_radth(defPerimxy0)
#endif


	radthdef0=Convert_rxy_to_radth(defPerimxy0);



	std::vector<std::pair<double,double> > resampled_radth1=radthdef0;
	interpolate(radth1,resampled_radth1);
#if 0
	for(int i=0;i<radth1.size();i++){
		std::cout << radthdef0[i].first << " " << radthdef0[i].second << " " << resampled_radth1[i].first << " " << resampled_radth1[i].second << std::endl;
	}

#endif
#if 0
	;Resample radii and theta of Perimxy1 to intervals of defPerimxy0
	Resampled_radth1 = radth1
	Resampled_radth1[0,0:n-1] = INTERPOLATE(radth1[0,*],radthdef0[1,0:n-1] / !DTOR)
	Resampled_radth1[0,n] = Resampled_radth1[0,0]
	Resampled_radth1[1,*] = radthdef0[1,*]
#endif
	std::vector<double> arcangles(n-1);
	std::vector<double> areas1(n-1);
	std::vector<double> areasdef0(n-1);
	for(int i=1;i<n;i++){
		arcangles[i-1]=resampled_radth1[i].second - resampled_radth1[i-1].second;
		//std::cout << arcangles[i-1] << std::endl;
		areas1[i-1]=resampled_radth1[i].first * resampled_radth1[i-1].first*sin(arcangles[i-1])/2;
		//std::cout << areas1[i-1] << std::endl;
		areasdef0[i-1]=radthdef0[i].first*radthdef0[i-1].first*sin(arcangles[i-1])/2;

		//std::cout << areasdef0[i-1] << std::endl;
	}

	double area1=M_PI*evecs1.D(1,1)*evecs1.D(0,0);

	double areaOlap=0;
	for(int i=0;i<n;i++){
		areaOlap += areasdef0[i]< areas1[i]?areasdef0[i]:areas1[i];
	}
	if(areaOlap!=areaOlap){
		std::cout << ":(" << std::endl;
	}
	double ppAreaDiff = 1.0 - (areaOlap / area1);
//	std::cout << "Area: " <<  ppAreaDiff << std::endl;

	return ppAreaDiff;
#if 0
	;Convert radii to areas
	ArcAngles = Resampled_radth1[1,1:n] - Resampled_radth1[1,0:n-1]
	Areas1 = Resampled_radth1[0,0:n-1] * Resampled_radth1[0,1:n] * SIN(ArcAngles) / 2.0
	Areasdef0 = radthdef0[0,0:n-1] * radthdef0[0,1:n] * SIN(ArcAngles) / 2.0

	AreaOlap = TOTAL(Areasdef0 < Areas1)
	Area1 = !PI * Evecs1[0] * Evecs1[3]
	ppAreaDiff = 1.0 - AreaOlap / Area1
	return, ppAreaDiff
#endif
}

class ellipse_mapping_cost_function: public vnl_cost_function {
private:
	EllipseMatrix ellipse0;
	EllipseMatrix ellipse1;
	double trace;
public:
	ellipse_mapping_cost_function(): vnl_cost_function(2),trace(0){}
	ellipse_mapping_cost_function(const EllipseMatrix & ellipse0, const EllipseMatrix & ellipse1,double trace) : vnl_cost_function(2){
		this->ellipse0=ellipse0;
		this->ellipse1=ellipse1;
		this->trace=trace;
	}
	double f(const vnl_vector<double>& x)  {


//		std::cout << "Trace: " << trace << "Eval: " << x(0) << " " << trace -x(0) << " " << x(1) << std::endl;

		//if(x(0)>=trace) return 1;
		vnl_matrix<double> tensor = EvalOrient_To_Tensor(x(0),trace-x(0),x(1));

		//std::cout << "Tensor: " << tensor << std::endl;
		double area = evaluateEllipseMapping(ellipse0,ellipse1,tensor);
		//std::cout << "Area: " << area << std::endl;
		return area;
	}
};


#if 0
;Unpack 2x2 tensor into the eigenvectors and eigenvalues of its
;symmetric component, and the rotation of its anitsymmetric component
function Tensor_to_Evecs, Tensor,ROTATION = Rot
TrTensor = TRANSPOSE(Tensor)
SymmTensor = (Tensor + TrTensor) / 2;symmetric component
evals = EIGENQL(SymmTensor, EIGENVECTORS = Evecs, /DOUBLE)
Evecs = [Evals[0],Evecs[*,0],Evals[1],Evecs[*,1]]

Spin = (Tensor - TrTensor) / 2;antisymmetric component
Rot = Spin[0, 1];in radians anticlockwise
return, Evecs
end

double tensorToEvecs(vnl_matrix<double> tensor) {

}
#endif

#if 0
;calculate the area of an ellipse from its tensorial description
function Ellipse_Area, EllipseTensor
if TOTAL(FINITE(EllipseTensor)) NE 4 then Message, 'Invalid EllipseTensor argument'
 Evecs = Tensor_to_Evecs(EllipseTensor)
return, !DPI * Evecs[0] * Evecs[3]
end

#endif

double ellipseArea(const EllipseMatrix & ellipse) {
	//vnl_real_eigensystem eigensystem(ellipse);
	vnl_symmetric_eigensystem<double> eigensystem(ellipse);
	return 2*M_PI*(eigensystem.D(0, 0) * eigensystem.D(1, 1));//TODO
}

#if 0
void calculateIntercalationSRT( const SRT & tissue, const SRT & cellShape, SRT & intercalation, double * errorEst) {
	intercalation = tissue - cellShape;

	errorEst=0;

	//TODO compute error

//    errorEst[0]=abs(boost::numeric::ublas::diag(intercalation))/2;
//  errorEst[0] = abs(boost::numeric::ublas::trace(intercalation))/2;
}

#endif

#if 0
;finds best mapping of ellipse at t-dt onto ellipse at t+dt
;a numerical method is used to optimize the mapping, by minimizing the
;proportional area difference between the two cell shapes
function OptimizeCellShapeStrain, Ellipse0,Ellipse1

end
#endif

void optimizeCellShapeStrain(const EllipseMatrix & a, const EllipseMatrix & b,vnl_matrix<double> & tensor) {

	double area0 = ellipseArea(a); //Area0 = Ellipse_Area(Ellipse0)
	double area1 = ellipseArea(b); // Area1 = Ellipse_Area(Ellipse1)

	//std::cout << area0 << " " << area1 << std::endl;
	double tr = 2.0 * log(sqrt(area1 / area0)); //Tr = 2.0 * ALOG(SQRT(Area1 / Area0)) ;area change which must be preserved by strains i.e.eval1+eval2 must equal Tr

	vnl_matrix<double> initStrainTensor = logSymmTensor(b) - logSymmTensor(a); //     init_StrainTensor = LogSymmTensor(Ellipse1) - LogSymmTensor(Ellipse0)
	//std::cout << "InitStrainTensor" << std::endl;
	//std::cout << initStrainTensor << std::endl;
	//init_StrainTensor incorrect if axes of ellipses not in same orientations,
	//so must optimize below, but good first guess

	double init_ppAreaDiff = evaluateEllipseMapping(a, b, initStrainTensor); //init_ppAreaDiff = EvaluateEllipseMapping(Ellipse0,Ellipse1,init_StrainTensor)

	//std::cout << "InitPPAreaDiff" << std::endl;
	//std::cout << init_ppAreaDiff << std::endl;

	vnl_symmetric_eigensystem<double> eigensystem(initStrainTensor);
	double orient = vectorToRadians(eigensystem.V.get_column(1));

#if 0
	TIPO init_ppAreaDiff = EvaluateEllipseMapping(a,b,initStrainTensor);

	TIPO init_Evecs = Tensor_to_Evecs(init_strainTensor); //init_Evecs = Tensor_to_Evecs(init_StrainTensor)
	TIPO orient = Vector_to_Radians(init_Evecs[1:2]) % M_PI;//orient = Vector_to_Radians(init_Evecs[1:2]) MOD !PI ;orient not direc

#endif

	ellipse_mapping_cost_function func(a,b,tr);

	vnl_amoeba optim(func);
	vnl_vector<double> x0(2);


	x0(0)=eigensystem.D(1,1);
	x0(1)=orient;
	//std::cout << "Start: " << x0(0) << " " << x0(1) << std::endl;


	optim.set_f_tolerance(0.01);
	optim.set_max_iterations(200);

	optim.minimize(x0);
	//std::cout << "Result: " << x0(0) << " " << x0(1) << std::endl;


#if 0

	COMMON Min2E_data, EE_Ellipse0,EE_Ellipse1,EE_Tr;global data blocks to be accessed during minimization by AMOEBA()
	EE_Ellipse0 = Ellipse0
	EE_Ellipse1 = Ellipse1
	EE_Tr = Tr

	args = [init_Evecs[0],orient];1st eigenvalue and orientation of 1st eigenvector
	Scales = [0.1,!PI];args can vary +/- scale
	MaxCalls = 50;converges quickly: proportional area difference landscapes simple with one minimum

#endif

#if 0
	dum = AMOEBA(0.01, $;precision
			FUNCTION_NAME = 'AMB_Minimize_2Ellipse_AreaDiff', $
			P0 = args, $
			SCALE = Scales, $
			NMAX = MaxCalls, $
			FUNCTION_VALUE = fvalues, $;returns final simplex pp area difference
			SIMPLEX = Sres);returns final simplex function arguments

	ppAreaDiff = MIN(fvalues,ith);best result
	eval = Sres[0,ith]
	orient = Sres[1,ith]
	return, EvalOrient_to_Tensor(eval,Tr-eval,orient)
#endif
			tensor= EvalOrient_To_Tensor(x0(0),tr-x0(0),x0(1));

}

void calculateIntercalationSRT( const vnl_matrix<double> & tissueSRT, const vnl_matrix<double> & cellShapeSRT, Tensor & intercalationSRT ){
	intercalationSRT= tissueSRT - cellShapeSRT;
}

#if 0
#if 0
function Calculate_Intercalation_SRT, Tissue_SRT,CellShape_SRT,ERROR = ErrorEst
Intercalation_SRT = Tissue_SRT - CellShape_SRT
ErrorEst = ABS(TRACE(Intercalation_SRT)) / 2
return, Intercalation_SRT
end
#endif

#if 0
function Calculate_CellShape_SRT, Ellipses,Tissue_Rotation,mins,ERROR = ErrorEst
nCells = N_ELEMENTS(Ellipses[0,0,*,0])

AllCellShape_SRTs = FLTARR(2,2,nCells)
CellAreas = FLTARR(nCells)
cumulative_SRT = FLTARR(2,2)
for i=0,nCells-1 do begin
;rotate t-dt cell shapes to mid time Pt (t) of domain
rMatrix = RotationMatrix(Tissue_Rotation * mins/2)
UnRotEllipse0 = rMatrix
# Ellipses[*,*,i,0] # TRANSPOSE(rMatrix)
;un-rotate t+dt cell shapes to mid time Pt (t) of domain
rMatrix = RotationMatrix(-Tissue_Rotation * mins/2)
UnRotEllipse1 = rMatrix
# Ellipses[*,*,i,1] # TRANSPOSE(rMatrix)


end
#endif

#endif
void calculateCellShapeSRT(const std::vector<EllipseMatrix> & ellipses0,
		const std::vector<EllipseMatrix> & ellipses1, double rotation, double mins,Tensor & cellShapeSRT) {

	assert(ellipses0.size() == ellipses1.size());

	int ncells = ellipses0.size();
	std::vector<vnl_matrix<double> > AllCellShape_SRTs(ncells);
	std::vector<double> areas(ncells);
	vnl_matrix<double> cumulative_SRT(2, 2);

	vnl_matrix<double> rMatrix;
	double totalArea=0;
	for (int i = 0; i < ncells; i++) {
		rMatrix = rotationMatrix(rotation * mins / 2);
		vnl_matrix<double> unRotEllipse0 = rMatrix * ellipses0[i]
				* rMatrix.transpose();

		rMatrix = rotationMatrix(-rotation * mins / 2);
		vnl_matrix<double> unRotEllipse1 = rMatrix * ellipses1[i]
				* rMatrix.transpose();
		vnl_matrix<double> StrainTensor;
		optimizeCellShapeStrain(unRotEllipse0, unRotEllipse1,StrainTensor);

		AllCellShape_SRTs[i]=StrainTensor/mins;
		areas[i]=ellipseArea(ellipses0[i]);
		cumulative_SRT+= AllCellShape_SRTs[i]*areas[i];
		totalArea+=areas[i];
#if 0
		StrainTensor = OptimizeCellShapeStrain(UnRotEllipse0,UnRotEllipse1)
		AllCellShape_SRTs[*,*,i] = StrainTensor / mins;convert strain to strain rate (per minute)
		CellAreas[i] = Ellipse_Area(Ellipses[*,*,i,0])
		cumulative_SRT += AllCellShape_SRTs[*,*,i] * CellAreas[i];area-weighted cell contribution
		endfor
#endif
	}

	vnl_matrix<double> AvCellShape_SRT = cumulative_SRT/totalArea;
	double wssq = 0.0;
	cellShapeSRT=AvCellShape_SRT;

#if 0
		AvCellShape_SRT = cumulative_SRT / TOTAL(CellAreas);area-weighted average strain rate

		wssq = 0.0;weighted sum of squares
		for i=0,nCells-1 do wssq += TOTAL((AllCellShape_SRTs[*,*,i] - AvCellShape_SRT)^2 * CellAreas[i])
		sd = SQRT(wssq / (4*TOTAL(CellAreas)));standard deviation
		ErrorEst = sd / SQRT(nCells);standard error
		return, AvCellShape_SRT
#endif


}

#if 0
;Calculate Tissue strain rate tensor from centroid trajectories
;returns tensor and optional domain translation and an average error estimate for
;each of the tensor components
function Calculate_Tissue_SRT, Centroids,mins,TRANSLATION = Translation,ERROR = ErrorEst

dx = REFORM((Centroids[0,*,1] - Centroids[0,*,0]) / mins)
dy = REFORM((Centroids[1,*,1] - Centroids[1,*,0]) / mins)
nCells = N_ELEMENTS(dx)
xy = REFORM(Centroids[*,*,0] - REBIN(Centroids[*,0,0],2,nCells))
weights = FLTARR(nCells) + 1.0;no differential weighting

du = REGRESS_PLANE(xy,dx,weights,CONF_INTERVALS = duCIs,INTERCEPT = duIcpt)
dv = REGRESS_PLANE(xy,dy,weights,CONF_INTERVALS = dvCIs,INTERCEPT = dvIcpt)

Tissue_SRT = [[du],[dv]]
;Error estimate is mean of confidence intervals associated with each of the 4 elements of SRT
ErrorEst = MEAN([duCIs,dvCIs])
Translation = Centroids[*,0,1] - Centroids[*,0,0] + [duIcpt,dvIcpt]
return, Tissue_SRT
end

#endif

void calculateTissueSRT(const std::vector<Centroid> & centroids0,
		const std::vector<VelocityVector> & velocities, int mins,
		Tensor & TissueSRT) { //, TRANSLATION & translation, ERROR & ErrorEst){
	//std::cout << "->calculateTissueSRT" << std::endl;

	assert(centroids0.size() == velocities.size());
	int ncells = centroids0.size();

#if 0
	std::vector<Pt> d(ncells);
	for (int i = 0; i < ncells; i++) {
		Pt speed;
		speed[0] = (centroids1[i][0] - centroids0[i][0]) / mins;
		speed[1] = (centroids1[i][1] - centroids0[i][1]) / mins;
		d[i] = speed;
	}
#endif
	vnl_matrix<double> X(ncells, 3);
	vnl_matrix<double> Y(ncells, 2);

	for (int i = 0; i < ncells; i++) {
		X(i, 0) = centroids0[i][0];
		X(i, 1) = centroids0[i][1];
		X(i, 2) = 1;
		Y(i, 0) = velocities[i][0];
		Y(i, 1) = velocities[i][1];
	}
	//std::cout << "Reg" << std::endl;
	vnl_matrix_fixed<double,3,2> beta = vnl_inverse(X.transpose() * X) * X.transpose() * Y;

	//std::cout << "X:" << std::endl;
	//std::cout << X << std::endl;
	//std::cout << "Y:" << std::endl;
	//std::cout << Y << std::endl;
	//std::cout << "beta:" << std::endl;
	//std::cout << beta << std::endl;

	vnl_matrix<double> Yest = X*beta;

	vnl_matrix<double> Yerr = Y - Yest;

	//	for(int i=0; i<ncells;i++){
	//		std::cout << Y.get_row(i) << " " << Yest.get_row(i) << " " << Yerr.get_row(i) << std::endl;
	// }


	TissueSRT(0,0)=beta(0,0);
	TissueSRT(0,1)=beta(0,1);
	TissueSRT(1,0)=beta(1,0);
	TissueSRT(1,1)=beta(1,1);
	//TissueSRT = beta;w
	//TODO compute translation
	//TODO compute error
}

#if 0
function Domain_Strain_Rates, $;returns tissue, cell shape and intercalation strain rate tensors
Centroids, $;FLOAT[2,nCells,2] Centroid (x,y) coordinates for a domain of cells at t-dt and t+dt
;First cell must be focal cell
Ellipses, $;FLOAT[2,2,nCells,2] Tensor (2x2) descriptions of ellipses of same cells at t-dt and t+dt
mins, $;FLOAT duration of domain in minutes
TRANSLATION = Translation, $;FLOAT[3] returns the translation of the domain
ERRORS = ErrorEstimates;FLOAT[3] returns error estimates for each of the 3 strain rate tensors

end

#endif

void domainStrainRates(const std::vector<Centroid> & centroids0,
		const std::vector<VelocityVector> & centroids1,
		const std::vector<EllipseMatrix> & ellipse0,
		const std::vector<EllipseMatrix> & ellipse1,
		double mins,
		Tensor & Tissue_SRT,
		Tensor & CellShape_SRT,
		Tensor & Intercalation_SRT) { //, SRT & Tissue_SRT, SRT & CellShape_SRT, SRT & Intercalation_SRT, TRANSLATION & translation, ERRORS & errors){


	//ERROR Tissue_ErrorEst;

	// calculateTissueSRT(centroids,mins,Tissue_SRT,translation,Tissue_ErrorEst); //Tissue_SRT = Calculate_Tissue_SRT(Centroids,mins,TRANSLATION = Translation,ERROR = Tissue_ErrorEst)
	 //(2,2);
	calculateTissueSRT(centroids0, centroids1, mins, Tissue_SRT); //,Tissue_SRT,translation,Tissue_ErrorEst);
	//std::cout << Tissue_SRT << std::endl;

	Tensor Spin = (Tissue_SRT - Tissue_SRT.transpose()) * (0.5); //Spin = (Tissue_SRT - TRANSPOSE(Tissue_SRT)) / 2
	double Tissue_Rotation = Spin(1,0); //Tissue_Rotation = Spin[0,1]   ;in radians per minute anticlockwise
	//std::cout << "Spin" << std::endl;
	//std::cout << Spin << std::endl;

	//ERROR CellShape_ErrorEst;
	//calculateCellShapeSRT(ellipses,Tissue_Rotation,mins,CellShape_SRT,CellShape_ErrorEst);  //CellShape_SRT = Calculate_CellShape_SRT(Ellipses,Tissue_Rotation,mins,ERROR = CellShape_ErrorEst)
	calculateCellShapeSRT(ellipse0, ellipse1, Tissue_Rotation, mins,CellShape_SRT);

	CellShape_SRT += Spin;


	calculateIntercalationSRT(Tissue_SRT,CellShape_SRT,Intercalation_SRT);
#if 0
	CellShape_SRT +=Spin; //CellShape_SRT += Spin         ;assume tissue rotation is equal to cell rotation (see Methods in "Tissue Tectonics")

	ERROR Intercalation_ErrorEst;

	calculateIntercalationSRT(Tissue_SRT,CellShape_SRT,Intercalation_SRT,Intercalation_ErrorEst);//Intercalation_SRT = Calculate_Intercalation_SRT(Tissue_SRT,CellShape_SRT,ERROR = Intercalation_ErrorEst)

	//ErrorEstimates = [Tissue_ErrorEst,CellShape_ErrorEst,Intercalation_ErrorEst]

	//SRTs = [[[Tissue_SRT]],[[CellShape_SRT]],[[Intercalation_SRT]]]

	//return, SRTs
#endif
}

void tensorToPlot(const vnl_matrix<double> & tensor, vnl_vector<double> & majorDir, double * majorLength, vnl_vector<double> & minorDir, double * minorLength,double * rotation){

	vnl_symmetric_eigensystem<double> eigensystem(tensor);

	majorDir= eigensystem.V.get_column(1);
	majorLength[0]=eigensystem.D(1,1);
	minorDir = eigensystem.V.get_column(0);
	minorLength[0]=eigensystem.D(0,0);
	vnl_matrix<double> Spin = (tensor - tensor.transpose()) / 2;

	assert(majorLength[0]>minorLength[0]);
	rotation[0] = tensor(1,0);
}

/*
int main() {
	std::vector<Pt> centroids0;

	Pt p;
	p[0] = 68.1649;
	p[1] = 154.556;
	centroids0.push_back(p);

	p[0] = 68.6106;
	p[1] = 148.847;
	centroids0.push_back(p);

	p[0] = 63.5622;
	p[1] = 142.632;
	centroids0.push_back(p);

	p[0] = 59.8756;
	p[1] = 156.818;
	centroids0.push_back(p);

	p[0] = 73.5348;
	p[1] = 156.924;
	centroids0.push_back(p);

	p[0] = 66.5087;
	p[1] = 163.363;
	centroids0.push_back(p);

	p[0] = 71.1074;
	p[1] = 165.953;
	centroids0.push_back(p);

	std::vector<Pt> centroids1;
	p[0] = 70.9150;
	p[1] = 150.351;
	
	centroids1.push_back(p);

	p[0] = 65.8085;
	p[1] = 145.642;
	centroids1.push_back(p);

	p[0] = 70.9426;
	p[1] = 139.045;
	centroids1.push_back(p);

	p[0] = 61.6778;
	p[1] = 153.120;
	centroids1.push_back(p);

	p[0] = 76.1005;
	p[1] = 152.091;
	centroids1.push_back(p);

	p[0] = 68.4807;
	p[1] = 158.268;
	centroids1.push_back(p);

	p[0] = 73.3548;
	p[1] = 160.798;
	centroids1.push_back(p);

#if 0
	pro Demo
	Centroids = $;Data from Drosophila 022WTCad t=82,name=3694,gidx=3385,cidx=181,nCor=1,dt=+/-2,2 min intervals
	[[[, ], $;focal cell, t-dt
	[, ], $
	[, ], $
	[, ], $
	[, ], $
	[, ], $
	[, ]], $
	[[, ], $;focal cell, t+dt
	[, ], $
	[, ], $
	[, ], $
	[, ], $
	[, ], $
	[, ]]]

#endif

	int ncells = centroids1.size(); //nCells = N_ELEMENTS(Centroids[0,*,0])

	vnl_matrix<double> a0(2, 2);
	a0(0, 0) = 3.66930;
	a0(0, 1) = -0.757964;
	a0(1, 0) = -0.757964;
	a0(1, 1) = 6.12269;
	vnl_matrix<double> a1(2, 2);
	a1(0, 0) = 3.56877;
	a1(0, 1) = 0.122487;
	a1(1, 0) = 0.122488;
	a1(1, 1) = 6.02515;
	vnl_matrix<double> a2(2, 2);
	a2(0, 0) = 4.67563;
	a2(0, 1) = 1.48467;
	a2(1, 0) = 1.48467;
	a2(1, 1) = 6.29152;
	vnl_matrix<double> a3(2, 2);
	a3(0, 0) = 3.75063;
	a3(0, 1) = 0.0691266;
	a3(1, 0) = 0.0691264;
	a3(1, 1) = 3.93438;
	vnl_matrix<double> a4(2, 2);
	a4(0, 0) = 3.10386;
	a4(0, 1) = -0.231607;
	a4(1, 0) = -0.231607;
	a4(1, 1) = 5.98518;
	vnl_matrix<double> a5(2, 2);
	a5(0, 0) = 2.88924;
	a5(0, 1) = -0.0887103;
	a5(1, 0) = -0.0887103;
	a5(1, 1) = 4.43233;
	vnl_matrix<double> a6(2, 2);
	a6(0, 0) = 3.61653;
	a6(0, 1) = -0.622872;
	a6(1, 0) = -0.622873;
	a6(1, 1) = 3.88820;

	std::vector<Ellipse> ellipses0, ellipses1;
	ellipses0.push_back(a0);
	ellipses0.push_back(a1);
	ellipses0.push_back(a2);
	ellipses0.push_back(a3);
	ellipses0.push_back(a4);
	ellipses0.push_back(a5);
	ellipses0.push_back(a6);

	vnl_matrix<double> b0(2, 2);
	b0(0, 0) = 3.58847;
	b0(0, 1) = -0.404760;
	b0(1, 0) = -0.404760;
	b0(1, 1) = 6.14627;
	vnl_matrix<double> b1(2, 2);
	b1(0, 0) = 3.48517;
	b1(0, 1) = -0.0674718;
	b1(1, 0) = -0.0674718;
	b1(1, 1) = 5.93491;
	vnl_matrix<double> b2(2, 2);
	b2(0, 0) = 4.83835;
	b2(0, 1) = 1.53995;
	b2(1, 0) = 1.53995;
	b2(1, 1) = 5.91973;
	vnl_matrix<double> b3(2, 2);
	b3(0, 0) = 3.65555;
	b3(0, 1) = 0.213053;
	b3(1, 0) = 0.213053;
	b3(1, 1) = 4.26899;
	vnl_matrix<double> b4(2, 2);
	b4(0, 0) = 2.67217;
	b4(0, 1) = -0.182167;
	b4(1, 0) = -0.182167;
	b4(1, 1) = 5.91113;
	vnl_matrix<double> b5(2, 2);
	b5(0, 0) = 2.96638;
	b5(0, 1) = 0.532873;
	b5(1, 0) = 0.532873;
	b5(1, 1) = 4.50105;
	vnl_matrix<double> b6(2, 2);
	b6(0, 0) = 3.22826;
	b6(0, 1) = -0.603982;
	b6(1, 0) = -0.603982;
	b6(1, 1) = 3.91057;
	ellipses1.push_back(b0);
	ellipses1.push_back(b1);
	ellipses1.push_back(b2);
	ellipses1.push_back(b3);
	ellipses1.push_back(b4);
	ellipses1.push_back(b5);
	ellipses1.push_back(b6);

#if 0

	Ellipses = FLTARR(2,2,nCells,2)
	Ellipses[*,*,*,0] = $
	[[[, ], $;focal cell, t-dt
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]]]
	Ellipses[*,*,*,1] = $
	[[[, ], $;focal cell, t+dt
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]], $
	[[, ], $
	[, ]]]
#endif

#if 0
	dt = 2
	frames = dt * 2;duration of domain time window in frames
	frame_interval_mins = 2.0
	mins = frames * frame_interval_mins;duration of domain time window in minutes

	SRTs = Domain_Strain_Rates(Centroids,Ellipses,mins,TRANSLATION = Translation,ERRORS = ErrorEstimates)
#endif
	double mins = 1;

	vnl_matrix<double> tissueSRT;
	vnl_matrix<double> cellShapeSRT;
	vnl_matrix<double> intercalationSRT;
	domainStrainRates(centroids0, centroids1, ellipses0, ellipses1, mins,tissueSRT,cellShapeSRT,intercalationSRT);

#if 0
	PRINT, 'Error Estimates:'
 PRINT, '  Tissue:',ErrorEstimates[0]
	PRINT, '  Cell Shape:',ErrorEstimates[1]
	PRINT, '  Intercalation:',ErrorEstimates[2]

	WINDOW, 1, XSIZE = 1000,YSIZE = 500
	!P.MULTI = [0,4,2]
	xr = [MIN(Centroids[0,*,*]),MAX(Centroids[0,*,*])]
	yr = [MIN(Centroids[1,*,*]),MAX(Centroids[1,*,*])]
	mxr = (xr[1]-xr[0]) > (yr[1]-yr[0])
	xr = MEAN(xr) + [-mxr,mxr]*3/4
	yr = MEAN(yr) + [-mxr,mxr]*3/4

	th = FINDGEN(360) * !DTOR
	circle = [TRANSPOSE(COS(th)),TRANSPOSE(SIN(th))]

	Titles = ['Domain t=0 mins',STRING(FORMAT='(%"Domain t=%d mins")',mins)]
	for t=0,1 do begin
	PLOT, Centroids[0,*,t],Centroids[1,*,t],PSYM = 1,TITLE = Titles[t],CHARSIZE = 1.5,$
	XRANGE = xr,YRANGE = yr,XSTYLE = 1,YSTYLE = 1,XTICKLEN = 1,YTICKLEN = 1, $
	XGRIDSTYLE = 1,YGRIDSTYLE = 1,XTICKINTERVAL = 10,YTICKINTERVAL = 10,XMINOR = 1,YMINOR = 1, $
	XTITLE = 'X (\B5m)',YTITLE = 'Y (\B5m)
	XYOUTS, Centroids[0,*,t]+.5,Centroids[1,*,t]+.5,STRTRIM(STRING(INDGEN(nCells)+1),2)
	for i=0,nCells-1 do begin
	ell = Ellipses[*,*,i,t]
# circle
	OPLOT, ell[0,*] + Centroids[0,i,t],ell[1,*] + Centroids[1,i,t]
	endfor
	endfor

	PLOT, [0,0],[0,0],/NODATA,TITLE = 'Centroid trajectories',CHARSIZE = 1.5,XRANGE = xr,YRANGE = yr, $
	XSTYLE = 1,YSTYLE = 1,XTICKLEN = 1,YTICKLEN = 1,XGRIDSTYLE = 1,YGRIDSTYLE = 1, $
	XTICKINTERVAL = 10,YTICKINTERVAL = 10,XMINOR = 1,YMINOR = 1, XTITLE = 'X (\B5m)',YTITLE = 'Y (\B5m)
	ARROW, Centroids[0,*,0],Centroids[1,*,0],Centroids[0,*,1],Centroids[1,*,1],/DATA,HSIZE = !D.X_SIZE / 128

	rg = MAX(ABS(Translation))
	PLOT, [0,0],[0,0],/NODATA,TITLE = 'Domain translation (\B5m/min)',XRANGE = [-rg,rg],YRANGE = [-rg,rg], $
	CHARSIZE = 1.5,XSTYLE = 4,YSTYLE = 4
	AXIS, 0,0,/XAXIS,CHARSIZE = 1.5 & AXIS, 0,0,/YAXIS,CHARSIZE = 1.5
	ARROW, 0,0,Translation[0],Translation[1],/DATA,THICK = 2

	PLOT, [Centroids[0,*,0],Centroids[0,*,1]-Translation[0]], $
	[Centroids[1,*,0],Centroids[1,*,1]-Translation[1]],PSYM = 3,TITLE = 'Domain vector field', $
	CHARSIZE = 1.5,/ISOTROPIC,YSTYLE = 3,XTICKLEN = 1,YTICKLEN = 1,XGRIDSTYLE = 1,YGRIDSTYLE = 1, $
	XTICKINTERVAL = 10,YTICKINTERVAL = 10,XMINOR = 1,YMINOR = 1,XTITLE = 'X (\B5m)',YTITLE = 'Y (\B5m)'
 ARROW, Centroids[0,*,0],Centroids[1,*,0], $
	Centroids[0,*,1]-Translation[0],Centroids[1,*,1]-Translation[1],/DATA,HSIZE = !D.X_SIZE / 256

	MaxRadius = MAX(ABS(SRTs)) * 1.3
	Tissue_SRT = REFORM(SRTs[*,*,0])
	CellShape_SRT = REFORM(SRTs[*,*,1])
	Intercalation_SRT = REFORM(SRTs[*,*,2])
	Draw_SRT, Tissue_SRT,'Tissue Strain Rates',MaxRadius
	Draw_SRT, CellShape_SRT,'Cell Shape Strain Rates',MaxRadius
	Draw_SRT, Intercalation_SRT,'Intercalation Strain Rates',MaxRadius
	!P.MULTI = 0
	end
#endif
}*/

#if 0

#if 0

;Pack Eigenvectors and eigenvalues into a symmetric 2x2 tensor
function Evecs_to_Tensor, Evecs
EvecM = [[Evecs[1:2]],[Evecs[4:5]]]
EvalM = [[Evecs[0],0],[0,Evecs[3]]]
SymmTensor = EvecM
# EvalM # TRANSPOSE(EvecM)
return, SymmTensor
end

#endif

void evecsToTensor() {

}
#if 0
#endif

#if 0

#if 0
;convert a direction (in radians from East, anticlockwise) to an xy vector
function Radians_to_Vector, direc
return, [COS(direc),SIN(direc)]
end
#endif


#if 0

;Regression of z onto xy plane, returning gradients in x and y, with
;optional intercept and confidence intervals for both gradients
function Regress_Plane, xy, z, weights, CONF_INTERVALS = cis, INTERCEPT = icpt
npts = N_ELEMENTS(z)
mErr = 1 / SQRT(weights)
coeff = REGRESS(xy,z,MEASURE_ERRORS = mErr,YFIT = zfits,CONST = icpt)

;Generate confidence intervals in x and y directions
;CIs have same residuals but differ by spread of x and y data Pts
P = 0.025;5% two-tailed, giving student's t approx = 2.0
ncoeff = 3;x, y and intercept
dfree = npts - ncoeff;degrees of freedom
sumw = TOTAL(weights)
s2 = TOTAL(weights * (z - zfits)^2)/(sumw * dfree)
ci_top = SQRT(s2) * ABS(T_CVF(P, dfree))
ci_x = ci_top / SQRT(TOTAL(weights * (xy[0,*] - MEAN(xy[0,*]))^2)/sumw); Draper & Smith p36 [Reference 30]
ci_y = ci_top / SQRT(TOTAL(weights * (xy[1,*] - MEAN(xy[1,*]))^2)/sumw)
cis = [ci_x,ci_y]
return, REFORM(coeff)
end

#endif



#if 0
;__________________________________________________________________________________
;______________________________________Demo________________________________________
;__________________________________________________________________________________
;Calculates Tissue, Cell Shape and Intercalation strain rates for an
;example domain from the Drosophila germ-band, and displays results as
;in Figure 3 in "Tissue Tectonics".

pro Draw_SRT, SRT,Title,MaxRadius
Evecs = Tensor_to_Evecs(SRT,ROTATION = Rotation)

;Deformations
PLOT, [0,0],[0,0],/NODATA,TITLE = Title +' (pp/min)',CHARSIZE = 1.5,XSTYLE = 4,YSTYLE = 4, $
XRANGE = [-MaxRadius,MaxRadius],YRANGE = [-MaxRadius,MaxRadius]
OPLOT,/POLAR,REPLICATE(MaxRadius/2,360),FINDGEN(360) * !DTOR,LINESTYLE = 1
OPLOT,/POLAR,REPLICATE(MaxRadius,360),FINDGEN(360) * !DTOR,LINESTYLE = 1
RadialAxis = MaxRadius * SIN(!PI/4)
XYOUTS, RadialAxis/2,RadialAxis/2,STRING(FORMAT = '(%"%6.3f")',MaxRadius/2)
XYOUTS, RadialAxis,RadialAxis,STRING(FORMAT = '(%"%6.3f")',MaxRadius)
for i=0,1 do OPLOT, [Evecs[i*3+1],-Evecs[i*3+1]]*Evecs[i*3],[Evecs[i*3+2],-Evecs[i*3+2]]*Evecs[i*3], $
THICK = 3,LINESTYLE = Evecs[i*3] GT 0 ? 0:2

;Rotation
OPLOT, [0,0],[-Rotation,Rotation],THICK = 2
OPLOT,/POLAR,REPLICATE(Rotation,90),!PI/2 * FINDGEN(90) / 90
OPLOT,/POLAR,REPLICATE(Rotation,90),!PI/2 * FINDGEN(90) / 90 + !PI
end

#endif
#endif
#endif
