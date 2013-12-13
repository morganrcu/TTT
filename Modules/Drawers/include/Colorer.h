#ifndef COLORER_H_
#define COLORER_H_

template<class T> class Colorer{
public:
#if 0
	typedef Colorer Self;
	typedef std::auto_ptr<Self> Pointer;

	Pointer New(){
		return Pointer(new Self);
	}
#endif
	virtual void GetObjectColor(const T & , double color[3])=0;
	virtual ~Colorer(){

	}
};

#endif /* COLORER_H_ */
