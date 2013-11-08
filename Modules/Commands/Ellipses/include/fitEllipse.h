#ifndef FIT_ELLIPSE_H
#define FIT_ELLIPSE_H
#include <itkVariableSizeMatrix.h>
#include <vector>
#include <vnl/algo/vnl_real_eigensystem.h>

template<class T> std::vector<double> fitEllipse(const std::vector<itk::Point <T,3> > & points){



	#define Matrix itk::VariableSizeMatrix<T>

  int npoints = points.size();
  Matrix x(npoints,1);
  Matrix y(npoints,1);

 for(int k=0;k<npoints;k++){
   x(k,0)=points[k][0];
   y(k,0)=points[k][1];
 }


 double mx=0;
 double my=0;
 double maxX=0;
 double minX=100000;
 double maxY=0;
 double minY=100000;

 for(int k=0;k<npoints;k++){
   mx += x(k,0);
   my += y(k,0);

   if(maxX < x(k,0)){
     maxX=x(k,0);
   }
   if(minX > x(k,0)){
     minX=x(k,0);
   }
   if(maxY < y(k,0)){
     maxY= y(k,0);
   }
   if(minY > y(k,0)){
     minY = y(k,0);
   }
 }

 mx = mx/npoints;
 my = my/npoints;
 double sx = (maxX - minX)/2;
 double sy = (maxY - minY)/2;

 for(int k=0;k<npoints;k++){
   x(k,0)=(x(k,0) - mx)/sx;
   y(k,0)=(y(k,0) - my)/sy;
 }

    /*std::cout << "X" << std::endl;
    std::cout << x << std::endl;

    std::cout << "Y" << std::endl;
    std::cout << y << std::endl;*/


Matrix D(npoints,6);

for(int k=0;k<npoints;k++){
  D(k,0)=pow(x(k,0),2);
  D(k,1)=x(k,0)*y(k,0);
  D(k,2)=pow(y(k,0),2);
  D(k,3)=x(k,0);
  D(k,4)=y(k,0);
  D(k,5)=1;
}

    //std::cout << D << std::endl;



vnl_matrix<T> S=D.GetTranspose()*D.GetVnlMatrix();

    //std::cout << "S:" << std::endl;

    //std::cout << S << std::endl;
Matrix C(3,3);
C.Fill(0);
C(0,2)=-2;
C(1,1)=1;
C(2,0)=-2;

    //std::cout << C << std::endl;
//vnl_generalized_eigensystem problem(S,C.GetVnlMatrix());

    Matrix tmpA(3,3);
    Matrix tmpB(3,3);
    Matrix tmpC(3,3);

    vnl_matrix<T> tmpE;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            tmpA(i,j)=S(i,j);
            tmpB(i,j)=S(i,j+3);
            tmpC(i,j)=S(i+3,j+3);
        }
    }
    /*std::cout << "A:" << std::endl;
    std::cout << tmpA << std::endl;
    std::cout << "B:" << std::endl;
    std::cout << tmpB << std::endl;
    std::cout << "C:" << std::endl;
    std::cout << tmpC << std::endl;*/

    tmpE=tmpC.GetInverse()*tmpB.GetTranspose();

    vnl_matrix<T> input = C.GetInverse()*(tmpA.GetVnlMatrix() - tmpB.GetVnlMatrix()*tmpE);

   // std::cout << input << std::endl;

    vnl_real_eigensystem eigensystem(input);

   /* std::cout << "V" << std::endl;
    std::cout << eigensystem.V << std::endl;

    std::cout << "D" << std::endl;
    std::cout << eigensystem.D << std::endl;*/

    int minEig=0;
    for(int k=0;k<3;k++){
        //std::cout << eigensystem.D(k,k) << std::endl;
        if(eigensystem.D(k,k).real()<0){
            minEig=k;
            break;
        }
    }

    vnl_matrix<struct std::complex<T> > A = eigensystem.V.extract(3,1,0,minEig);

    vnl_matrix<T> Areal(3,1);
    Areal(0,0)=A(0,0).real();
    Areal(1,0)=A(1,0).real();
    Areal(2,0)=A(2,0).real();
    vnl_matrix<T > evecY = -tmpE*Areal;

    double p0,p1,p2,p3,p4,p5;
    p0=Areal(0,0);
    p1=Areal(1,0);
    p2=Areal(2,0);
    p3=evecY(0,0);
    p4=evecY(1,0);
    p5=evecY(2,0);

    double a0,a1,a2,a3,a4,a5;
    a0= p0*sy*sy;
    a1= p1*sx*sy;
    a2= p2*sx*sx;
    a3= -2*p0*sy*sy*mx - p1*sx*sy*my + p3*sx*sy*sy;
    a4= -p1*sx*sy*mx - 2*p2*sx*sx*my + p4*sx*sx*sy;
    a5= p0*sy*sy*mx*mx + p1*sx*sy*mx*my + p2*sx*sx*my*my - p3*sx*sy*sy*mx - p4*sx*sx*sy*my + p5*sx*sx*sy*sy;

//    A(1)*sy*sy*mx*mx + A(2)*sx*sy*mx*my + A(3)*sx*sx*my*my   ...
//    - A(4)*sx*sy*sy*mx - A(5)*sx*sx*sy*my   ...
 //   + A(6)*sx*sx*sy*sy

    double thetarad = 0.5*atan2(a1,a0 - a2);
    double cost = cos(thetarad);
    double sint = sin(thetarad);
    double sin_squared = sint*sint;
    double cos_squared = cost*cost;
    double cos_sin = sint * cost;

    double Ao = a5;
    double Au =   a3 * cost + a4 * sint;
    double Av = - a3 * sint + a4 * cost;
    double Auu = a0 * cos_squared + a2 * sin_squared + a1 * cos_sin;
    double Avv = a0 * sin_squared + a2 * cos_squared - a1 * cos_sin;

    //% ROTATED = [Ao Au Av Auu Avv]

    double tuCentre = - Au/(2*Auu);
    double tvCentre = - Av/(2*Avv);
    double wCentre = Ao - Auu*tuCentre*tuCentre - Avv*tvCentre*tvCentre;

    double uCentre = tuCentre * cost - tvCentre * sint;
    double vCentre = tuCentre * sint + tvCentre * cost;

    double Ru = -wCentre/Auu;
    double Rv = -wCentre/Avv;

    //OUTPUT IS uCentre, vCentre, Ru, Rv, thetha
    std::vector<double> result;
    result.push_back(uCentre);
    result.push_back(vCentre);
    result.push_back(Ru);
    result.push_back(Rv);
    result.push_back(thetarad);

    return result;

}


/*
int main(){
    double Rx = 300;
    double Ry = 200;
    double Cx = 250;
    double Cy = 150;
    double Rotation = .4; // Radians

    double NoiseLevel = .5; // Will add Gaussian noise of this std.dev. to points

    int npoints=100;

    double step = 2.0/npoints;
    typedef itk::Point<double,3> MyPoint;
    std::vector<MyPoint> points;
    for(int i=0;i<npoints;i++){
        double t=(i)*step;

        MyPoint point;
        point[0]=Rx*cos(t);
        point[1]=Ry*sin(t);

        MyPoint npoint;
        npoint[0]= point[0]*cos(Rotation) - point[1]*sin(Rotation) + Cx;
        npoint[1]= point[0]*sin(Rotation) + point[1]*cos(Rotation) + Cy;
        points.push_back(npoint);
    }

    std::cout << "Input: "<< std::endl;

    for(int i=0;i<npoints;i++){

        std::cout << "(" << points[i][0] << "," << points[i][1] << ")" << std::endl;
    }

    fitEllipse(points);

}*/
#endif
