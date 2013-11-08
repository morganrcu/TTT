#include <algorithm>
#include "giaaCellTracker.h"

using namespace boost;

typedef std::pair<ttt::CellVertexType,ttt::TrackedCellVertexType> CellAndTrack;
typedef std::pair<CellAndTrack, double> CellAndTrackDistance;

struct CompareSecond
            {
                bool operator()(const CellAndTrackDistance& left, const CellAndTrackDistance& right) const
                {
                    return left.second < right.second;
                }
            };

void ttt::CellTracker::Track() {

	std::vector< std::map<int,TrackedCellVertexType> > idsToSkeletonVertex;
	idsToSkeletonVertex.resize(m_Observations.size());

	//First frame: Init a track per observed cell
    m_Tracks.resize(m_Observations.size());
    m_Tracks[0]=TrackedTissueDescriptor::New();

    m_Tracks[0]->m_SkeletonGraph=m_Observations[0]->m_SkeletonGraph;
    m_Tracks[0]->m_CellGraph=boost::shared_ptr<ttt::TrackedCellGraph>(new ttt::TrackedCellGraph);

    std::map<CellVertexType,TrackedCellVertexType> obsToTrack;

    BGL_FORALL_VERTICES(v,*m_Observations[0]->m_CellGraph,CellGraph) {


            TrackedCellVertexType n= boost::add_vertex(*m_Tracks[0]->m_CellGraph);
            boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID=m_NextID++;
            boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_SkeletonNodes=
            		boost::get(ttt::CellPropertyTag(),*m_Observations[0]->m_CellGraph,v).m_SkeletonNodes;
            boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_Centroid=
                        		boost::get(ttt::CellPropertyTag(),*m_Observations[0]->m_CellGraph,v).m_Centroid;
            boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ObservedCell=v;
            std::cout << "Init track: " << boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID << std::endl;

            idsToSkeletonVertex[0][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[0]->m_CellGraph,n).m_ID]=n;

            obsToTrack[v]=n;

    }

    BGL_FORALL_EDGES(e,*m_Observations[0]->m_CellGraph,CellGraph){
    	CellVertexType source = boost::source(e,*m_Observations[0]->m_CellGraph);
    	CellVertexType target = boost::target(e,*m_Observations[0]->m_CellGraph);

    	boost::add_edge(obsToTrack[source],obsToTrack[target],*m_Tracks[0]->m_CellGraph);

    }

    //Forward pass;
    for(unsigned int t=1; t<m_Observations.size(); t++) {
    	obsToTrack.clear();

    	m_Tracks[t]=TrackedTissueDescriptor::New();

        m_Tracks[t]->m_CellGraph=boost::shared_ptr<ttt::TrackedCellGraph>(new ttt::TrackedCellGraph);

    	m_Tracks[t]->m_SkeletonGraph=m_Observations[t]->m_SkeletonGraph;
        //Assign each observation to nearest track
        int currentTracks=m_Tracks[t-1]->GetNumCells();

        std::map<ttt::TrackedCellVertexType,bool> updatedTracks;

        BGL_FORALL_VERTICES(v,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph){
        	updatedTracks[v]=false;
        }

        int currentObs = m_Observations[t]->GetNumCells();
        std::map<ttt::CellVertexType,bool> associatedObs;

        BGL_FORALL_VERTICES(v,*m_Observations[t]->m_CellGraph,CellGraph){
        	associatedObs[v]=false;
        }
        std::cout << "# of current Tracks: " << currentTracks << " " << t << " " << m_Observations.size() << std::endl;
        std::cout << "# of current observations: " << currentObs << " " << t << std::endl;

        if (currentTracks > 0) {

            std::map<CellAndTrack,double> distances;


            BGL_FORALL_VERTICES(track,*m_Tracks[t-1]->m_CellGraph,TrackedCellGraph) {

                itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,track).m_Centroid;

                BGL_FORALL_VERTICES(obs,*m_Observations[t]->m_CellGraph,CellGraph) {
                    itk::Point<double,3> observationCentroid = boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,obs).m_Centroid;
                    itk::Vector<double,3> diff = trackCentroid - observationCentroid;
                    double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));

                    std::pair<CellVertexType,TrackedCellVertexType> loc (obs,track);
                    distances[loc] = distance;

                }
            }

            bool endAssociation = false;
            double maxDistance = 2;
            int associations = 0;
            int numTracksUpdated=0 ;

            while (!endAssociation && associations < currentTracks && associations < currentObs) {

                CellVertexType minO;
                TrackedCellVertexType minT;
                double minVal = INFINITY;

                CellAndTrackDistance minimum = *std::min_element(distances.begin(),distances.end(),CompareSecond());

                minVal=minimum.second;
                minO=minimum.first.first;
                minT=minimum.first.second;

                if (minVal < maxDistance) {
                	associations++;
                	numTracksUpdated++;
                	TrackedCellVertexType n= boost::add_vertex(*m_Tracks[t]->m_CellGraph);

                	boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID=
                			boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,minT).m_ID;

                	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_Centroid=
                			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,minO).m_Centroid;

                	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes=
                			boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,minO).m_SkeletonNodes;

                    boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell=minO;


                	idsToSkeletonVertex[t][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID]=n;

                    obsToTrack[minO]=n;

                	assert(!updatedTracks[minT]);
                	assert(!associatedObs[minO]);
                	updatedTracks[minT] = true;
                	associatedObs[minO] = true;
                	std::map<CellAndTrack,double>::iterator it=distances.begin();

                	while(it!=distances.end()){
                		if(it->first.first==minO || it->first.second==minT){
                			distances.erase(it++);
                		}else{
                			++it;
                		}
                	}
                	std::cout << "Associated obs :" << minO << " to track " << minT << " dist: " << minVal << "ID: " << boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID << std::endl;
                } else {
                	endAssociation = true;
                }
            }
        }

        //Init a track with each unassociated observation
        for(std::map<ttt::CellVertexType,bool>::iterator it= associatedObs.begin();it!=associatedObs.end();++it){

        	if(!it->second){

				TrackedCellVertexType n = boost::add_vertex(*m_Tracks[t]->m_CellGraph);


				boost::get(TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID=m_NextID++;

				boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_Centroid=
						boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_Centroid;

				boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_SkeletonNodes=
						boost::get(ttt::CellPropertyTag(),*m_Observations[t]->m_CellGraph,it->first).m_SkeletonNodes;
				boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ObservedCell=it->first;

				idsToSkeletonVertex[t][ boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,n).m_ID]=n;

				obsToTrack[it->first]=n;

        	}
        }

        BGL_FORALL_EDGES(e,*m_Observations[t]->m_CellGraph,CellGraph){
        	CellVertexType source = boost::source(e,*m_Observations[t]->m_CellGraph);
        	CellVertexType target = boost::target(e,*m_Observations[t]->m_CellGraph);

        	boost::add_edge(obsToTrack[source],obsToTrack[target],*m_Tracks[t]->m_CellGraph);

        }    }



    //Compute velocity vectors

    for(unsigned int t=0;t<m_Tracks.size();t++){

        BGL_FORALL_VERTICES(track,*m_Tracks[t]->m_CellGraph,TrackedCellGraph) {
        	itk::Point<double,3> t_1,t0,t1;
        	int id=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_ID;
        	t0=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_Centroid;
        	bool previous=false;
        	bool next=false;

        	if(t>0 && idsToSkeletonVertex[t-1].count(id)>0 ){
        		previous=true;
        		t_1=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t-1]->m_CellGraph,idsToSkeletonVertex[t-1][id]).m_Centroid;
        	}

        	if(t<m_Tracks.size()-1 && idsToSkeletonVertex[t+1].count(id)>0 ){
        		next=true;
        	    t1=boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t+1]->m_CellGraph,idsToSkeletonVertex[t+1][id]).m_Centroid;
        	}

        	itk::Vector<double,3> vel;
        	if(previous && next){
        		itk::Vector<double,3> vl=t0 - t_1;
        		itk::Vector<double,3> vr=t1 - t0;
        		vel=(vl +vr)/2;
        	}else if(previous && !next){
        		itk::Vector<double,3> vl=t0 - t_1;
        		vel=vl;
        	}else if(!previous && next){
        		itk::Vector<double,3> vr=t1 - t0;
        		vel=vr;
        	}else{
        		vel.Fill(0);
        	}
        	boost::get(ttt::TrackedCellPropertyTag(),*m_Tracks[t]->m_CellGraph,track).m_Velocity=vel;
        }
    }



#if 0
        std::vector<int> UAcells;
        std::vector<int> Acells;

        // collect unassigned and assigned
        BGL_FORALL_VERTICES(obs,m_Observations[t]->m_CellGraph,CellGraph) {
            if (boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).trackID == -1){
                UAcells.push_back(obs);
            }
            else{
                Acells.push_back(boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).trackID);
            }
        }

        std::cout<<"num uass " << UAcells.size() << std::endl;
        // go through previous frames for each cell in unassigned

        while(UAcells.size() > 0){

            int minD = INFINITY;
            int asstrackID = -1;
            int curridx = 0;
            int chosenframe = 0;

            for(int UAidx = 0; UAidx < UAcells.size(); UAidx++){
                CellVertexType curr = UAcells[UAidx];
                std::cout<< "Looking at curr " << curr << std::endl;
                Cell currcell = boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,curr);

                for(int prevframe = 0; prevframe < t; prevframe++){
                    bool notassigned = true;
                    BGL_FORALL_VERTICES(obs,m_Observations[prevframe]->m_CellGraph,CellGraph){
                        for(int i = 0; i < Acells.size(); i++){
                            if(Acells[i] == boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).trackID){
                                notassigned = false;
                                break;
                            }
                        }
                        if (notassigned){
                            std::cout <<" Potential track id "<< boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).trackID  << " frame " << prevframe << std::endl;
                            itk::Point<double,3> currCentroid = currcell.m_Centroid;
                            itk::Point<double,3> prevCentroid = boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).m_Centroid;
                            itk::Vector<double,3> diff = currCentroid - prevCentroid;
                            double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
                            if (distance < minD){
                                minD = distance;
                                asstrackID = boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).trackID;
                                curridx = UAidx;
                                chosenframe = prevframe;
                            }
                        }
                    }
                }
            }

            if (asstrackID != -1){
                boost::get(ttt::CellPropertyTag(),m_Observations[chosenframe]->m_CellGraph,UAcells[curridx]).trackID = asstrackID;
                UAcells.erase(UAcells.begin()+curridx);
            }
            else{
                std::cout<<"Assigning new ids to UA cells"<<std::endl;
                for(int i = 0; i < UAcells.size(); i++){
                  maxid++;
                  CellVertexType n= boost::add_vertex(m_Tracks[t]->m_TrackedCellGraph);
                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_ID=maxid;
                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_Centroid=  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,UAcells[i]).m_Centroid;
                  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,UAcells[i]).trackID = maxid;
                  GenColor gen;
                  float seed = (float)rand()/RAND_MAX;
                  //std::cout << " seed is " << seed << std::endl;
                  std::vector<double> color = gen.generateRandomColor(seed);

                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_Color = color;
                  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,UAcells[i]).m_Color =
                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_Color;
                }
                break;
            }
        }
    }
#endif

#ifdef OUT_OF_CONTROL
    double r_color[10] = {0, .5, 1, 0, 0, .5, .5,  .2,  1,  0};
    double g_color[10] = {0, .5, 0, 1, 0, .5,  0,  .8, .5, .5}; 
    double b_color[10] = {0, .5, 0, 0, 1,  0, .5,  .6,  0,  1};
    int index = 0;
    int maxid = m_Observations[0]->GetNumCells() - 1;
    std::cout<<"Max id: "<< maxid << std::endl;

    //1. Allocate structures



    //2. First frame
    m_Tracks[0]=TrackedTissueDescriptor::New();
    BGL_FORALL_VERTICES(v,m_Observations[0]->m_CellGraph,CellGraph) {

            CellVertexType n= boost::add_vertex(m_Tracks[0]->m_TrackedCellGraph);
            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_ID=m_NextID;
           
            boost::get(ttt::CellPropertyTag(),m_Observations[0]->m_CellGraph,v).GetFourierTransform(m_Observations[0]->m_SkeletonGraph);

            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).fft = boost::get(ttt::CellPropertyTag(),m_Observations[0]->m_CellGraph,v).fft;

            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_Centroid=
                boost::get(ttt::CellPropertyTag(),m_Observations[0]->m_CellGraph,v).m_Centroid;
           
            boost::get(ttt::CellPropertyTag(),m_Observations[0]->m_CellGraph,v).trackID =
            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_ID;

            index = boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_ID;

            GenColor gen;
            float seed = (float)rand()/RAND_MAX;
            //std::cout << " seed is " << seed << std::endl;
            std::vector<double> color = gen.generateRandomColor(seed);

            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_Color = color;
            boost::get(ttt::CellPropertyTag(),m_Observations[0]->m_CellGraph,v).m_Color =
            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_Color;
            m_NextID++;

            //std::cout<< seed << " " << color[0] << " " << color[1] << " " << color[2] << std::endl;
            //std::cout << "Initializing ID: " << boost::get(ttt::CellPropertyTag(),m_Observations[0]->m_CellGraph,v).trackID << " " << boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[0]->m_TrackedCellGraph,n).m_ID << std::endl;
    }
        //Init first frame with positions in first observation and initializa IDs



    // 3. A partir del segundo frame forward-pass


    for(int t=1; t<m_Observations.size(); t++) {

        // clone descriptor
        m_Tracks[t]=cloneTrackedTissueDescriptor(m_Tracks[t-1]);

        //OPTIONAL: PREDICT????

        //ASIGNACIÓN GREEDY


        int currentTracks=m_Tracks[t]->GetNumCells();
        bool updatedTracks[currentTracks];

        for (int i = 0; i < currentTracks; i++) {
            updatedTracks[i] = false;
        }

        int currentObs= m_Observations[t]->GetNumCells();
        bool associatedObs[currentObs];
        for (int i = 0; i < currentObs; i++) {
            associatedObs[i] = false;
        }

        std::cout << "# of current Tracks: " << currentTracks << " " << t << " " << m_Observations.size() << std::endl;
        std::cout << "# of current observations: " << currentObs << " " << t << std::endl;

        /*BGL_FORALL_VERTICES(track,m_Tracks[t]->m_TrackedCellGraph,TrackedCellGraph){
            if(boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,track).m_ID > maxid)
               maxid = boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,track).m_ID;
        }*/
        
        //std::cout << "Max id is " << maxid << std::endl;
        
        if (currentTracks > 0) {
            //boost::numeric::ublas::matrix<double> distances(currentTracks, currentObs);
            std::map<std::pair<int,int>,double> distances;

            int i=0;
            BGL_FORALL_VERTICES(track,m_Tracks[t]->m_TrackedCellGraph,TrackedCellGraph) {
                //std::cout<< track << " " << std::endl;
                itk::Point<double,3> trackCentroid = boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,track).m_Centroid;
                //std::cout<< trackCentroid[0] << std::endl; 
                int j=0;
                BGL_FORALL_VERTICES(obs,m_Observations[t]->m_CellGraph,CellGraph) {
                    itk::Point<double,3> observationCentroid = boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).m_Centroid;
                    itk::Vector<double,3> diff = trackCentroid - observationCentroid;
                    double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
                    //double distance = sqrt(pow(diff[0],2)+pow(diff[1],2));

                    /*boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).GetFourierTransform(m_Observations[t]->m_SkeletonGraph);
                    fftw_complex *orig = boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).fft;
                    fftw_complex *trac = boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,track).fft;
                    
                    itk::Vector<double,10> origV;
                    itk::Vector<double,10> trackV;

                    for(int idx = 0; idx < 10; idx++){
                        origV[idx] = orig[0][idx]; 
                        trackV[idx] = trac[0][idx];
                    }

                    itk::Vector<double,10> fftdiff = trackV - origV;

                    double fftdistsquare = 0.0;
                    for(int idx = 0; idx < 10; idx++){
                        fftdistsquare += pow(fftdiff[idx],2);
                    }

                    double fftdistance = sqrt(fftdistsquare);
                    //double score = exp(-1*distance)*exp(-1*fftdistance);*/
                    double score = distance;
                    //distances.at_element(i,j)=score;
                    std::pair<int,int> loc (j,i);
                    distances[loc] = distance;
                    /*std::cout << " Scores " << i << " " << j << " " << score << std::endl;
                    std::cout << " observ " << observationCentroid[0] << " " << observationCentroid[1] << " " << observationCentroid[2] << std::endl;
                    std::cout << " tracks " << trackCentroid[0] << " " << trackCentroid[1] << " " << trackCentroid[2] << std::endl;*/
                    j++;
                }
                i++;
            }

             //Asociar
            bool endAssociation = false;
            double maxDistance = 1000;
            int associations = 0;


            boost::graph_traits<ttt::CellGraph>::vertex_iterator Ti, Ti_end;

            boost::graph_traits<ttt::TrackedCellGraph>::vertex_iterator  Oi, Oi_end;

            boost::tie(Ti, Ti_end) = boost::vertices(m_Tracks[t]->m_TrackedCellGraph);

            boost::tie(Oi, Oi_end) = boost::vertices(m_Observations[t]->m_CellGraph);

            int numtracks =0 ;

            while (!endAssociation && associations < currentTracks && associations < currentObs) {

                int minO, minT;
                double minVal = INFINITY;

                for (int i = 0; i < currentObs; i++) {
                    for (int j = 0; j < currentTracks; j++) {
                        std::pair<int,int> loc (i,j);
                        double currentValue = distances[loc];
                        //double currentValue = distances.at_element(i, j);
                        if (currentValue < minVal) {
                            minVal = currentValue;
                            minO = i;
                            minT = j;
                        }

                        //std::cout << i << " " << j << ": " << currentValue << " " << minVal << " " << minO << " " << minT << std::endl;

                    }
                }

                    //std::cout<< "min val " << minVal << " " << currentTracks << " " << currentObs << std::endl;

                    if (minVal < maxDistance) {

                        /*if((boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,Ti[minT]).m_ID) > maxid){
                            maxid++;
                            //std::cout<< "GaveTrack new " << Oi[minO] << " track id " << maxid << " " << std::endl;
                            boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,Ti[minT]).m_Centroid=  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,Oi[minO]).m_Centroid;
                            boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,Oi[minO]).trackID = maxid;
                        }
                        else{*/
                        numtracks++;
                        //std::cout<< "GaveTrack " << Oi[minO] << " track id " << boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,Ti[minT]).m_ID << " " << minVal << " " << numtracks <<std::endl;
                        boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,Ti[minT]).m_Centroid=  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,Oi[minO]).m_Centroid;
                        boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,Oi[minO]).trackID = boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,Ti[minT]).m_ID;
                        boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,Oi[minO]).m_Color = boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,Ti[minT]).m_Color;

                        //}
                        updatedTracks[minO] = true;
                        associatedObs[minT] = false;
                        //1. Actualizar track
                        
                        for (int i = 0; i < currentObs; i++) {
                            std::pair<int,int> loc (i,Ti[minT]);
                            distances[loc] = INFINITY;
                            //distances.at_element(i, minT) = INFINITY;
                        }

                        
                        for (int j = 0; j < currentTracks; j++) {
                            
                            std::pair<int,int> loc (Oi[minO],j);
                            //std::cout<< "looking: " << loc.first << " " << loc.second << " " << minO << " " << j << " " << distances[loc] << std::endl;
                            std::pair<int,int> loca (j,j);
                            //std::cout<< "looking: " << loca.first << " " << loca.second << " " << j << " " << j << " " << distances[loca] << std::endl;
                            distances[loc] = INFINITY; 
                            //std::cout<< distances[loc]<<std::endl;
                            //std::cout<< distances[loca]<<std::endl;


                            //distances.at_element(minO, j) = INFINITY;
                        }
                        /*
                        for(int i = 0; i < currentTracks; i++){
                            for(int j = 0; j < currentTracks; j++){
                                 tuple<int,int> loc = (i,j);
                                 std::cout << " Scores " << i << " " << j << " " << distances[loc] << std::endl;
                            }
                        }*/
                    } else {
                        endAssociation = true;
                    }
            }
            //2. Eliminar pistas sin actualizar
        }

        std::vector<int> UAcells;
        std::vector<int> Acells;

        // collect unassigned and assigned
        BGL_FORALL_VERTICES(obs,m_Observations[t]->m_CellGraph,CellGraph) {
            if (boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).trackID == -1){
                UAcells.push_back(obs);
            }
            else{
                Acells.push_back(boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,obs).trackID);
            }
        }

        std::cout<<"num uass " << UAcells.size() << std::endl; 
        // go through previous frames for each cell in unassigned
        
        while(UAcells.size() > 0){

            int minD = INFINITY;
            int asstrackID = -1;
            int curridx = 0;
            int chosenframe = 0;

            for(int UAidx = 0; UAidx < UAcells.size(); UAidx++){
                CellVertexType curr = UAcells[UAidx];
                std::cout<< "Looking at curr " << curr << std::endl;
                Cell currcell = boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,curr);
                
                for(int prevframe = 0; prevframe < t; prevframe++){
                    bool notassigned = true;
                    BGL_FORALL_VERTICES(obs,m_Observations[prevframe]->m_CellGraph,CellGraph){
                        for(int i = 0; i < Acells.size(); i++){
                            if(Acells[i] == boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).trackID){
                                notassigned = false;
                                break;
                            }
                        }
                        if (notassigned){
                            std::cout <<" Potential track id "<< boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).trackID  << " frame " << prevframe << std::endl;
                            itk::Point<double,3> currCentroid = currcell.m_Centroid;
                            itk::Point<double,3> prevCentroid = boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).m_Centroid;
                            itk::Vector<double,3> diff = currCentroid - prevCentroid;
                            double distance = sqrt(pow(diff[0],2)+pow(diff[1],2) + pow(diff[2],2));
                            if (distance < minD){
                                minD = distance;
                                asstrackID = boost::get(ttt::CellPropertyTag(),m_Observations[prevframe]->m_CellGraph,obs).trackID;
                                curridx = UAidx;
                                chosenframe = prevframe;
                            }
                        }
                    }
                }
            }

            if (asstrackID != -1){
                boost::get(ttt::CellPropertyTag(),m_Observations[chosenframe]->m_CellGraph,UAcells[curridx]).trackID = asstrackID;
                UAcells.erase(UAcells.begin()+curridx);   
            }
            else{
                std::cout<<"Assigning new ids to UA cells"<<std::endl;
                for(int i = 0; i < UAcells.size(); i++){   
                  maxid++;
                  CellVertexType n= boost::add_vertex(m_Tracks[t]->m_TrackedCellGraph);
                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_ID=maxid;
                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_Centroid=  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,UAcells[i]).m_Centroid;
                  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,UAcells[i]).trackID = maxid;
                  GenColor gen;
                  float seed = (float)rand()/RAND_MAX;
                  //std::cout << " seed is " << seed << std::endl;
                  std::vector<double> color = gen.generateRandomColor(seed);

                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_Color = color;
                  boost::get(ttt::CellPropertyTag(),m_Observations[t]->m_CellGraph,UAcells[i]).m_Color =
                  boost::get(ttt::TrackedCellPropertyTag(),m_Tracks[t]->m_TrackedCellGraph,n).m_Color;
                }
                break;
            }
        }


    }
    // update second frame
#endif

}
