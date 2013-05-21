#include <deque>
#include "geogrid.hpp"
using namespace std;

namespace hysw{namespace geogrid{
  class Node_prep{
	public:
		int getlink(Node_prep*n){
			for(int i=0;i!=6;++i)
				if(link[i]==n)return i;
			throw "";
		}
			Node_prep(double x_=0,double y_=0,double z_=0):next(0),x(x_),y(y_),z(z_),link(),rear(){}
			int id;
			double x,y,z;
			Node_prep* next;
			Node_prep* link[7];
			Node_prep* rear[7];
			void project(){
			double m=sqrt(x*x+y*y+z*z);
			x/=m;y/=m;z/=m;
		}
		void create(int i,deque<Node_prep>& nodes){
			if(!rear[i]&&link[i]){
				int j=link[i]->getlink(this);
				rear[i]=link[i];
				nodes.push_back(Node_prep((x+rear[i]->x)/2,(y+rear[i]->y)/2,(z+rear[i]->z)/2));
				link[i]=&nodes.back();
				link[i]->project();
				link[i]->link[0]=this;
				link[i]->link[3]=rear[i];
				rear[i]->link[j]=link[i];
				rear[i]->rear[j]=this;
			}
		}
	};
	
	std::vector<Node> GenerateGrid(int iteration){
		deque<Node_prep> nodes;
		
		double sl=sqrt(1+(1.0+sqrt(5))*(1.0+sqrt(5))/4);
		double hs=(1.0+sqrt(5))/2/sl/sl*2;
		double vdf=1-sqrt(1-hs*hs);
		double hsq2=hs;
		
		for(int i=0;i!=10;++i){
			nodes.push_back(Node_prep(
				hsq2*cos(i/5.0*3.1415926),
				i&1?-vdf:vdf,
				hsq2*sin(i/5.0*3.1415926)
			));
		}
		nodes.push_back(Node_prep(0,+1,0));
		nodes.push_back(Node_prep(0,-1,0));
		
		for(int i=0;i!=5;++i){
			nodes[10].link[i]=&nodes[i*2];
			nodes[11].link[i]=&nodes[i*2+1];
		}
		for(int i=0;i!=10;++i){
			nodes[i].link[0]=&nodes[10+(i&1)];
			nodes[i].link[1]=&nodes[(10+i-2)%10];
			nodes[i].link[2]=&nodes[(10+i-1)%10];
			nodes[i].link[3]=&nodes[(10+i+1)%10];
			nodes[i].link[4]=&nodes[(10+i+2)%10];
		}
		for(int i=0;i!=12;++i)
			nodes[i].project();
		while(iteration--){
			int limit=nodes.size();
			for(int i=0;i!=limit;++i)
				for(int j=0;j!=6;++j)
					nodes[i].create(j,nodes);
			
			for(int i=limit;i!=nodes.size();++i){
				Node_prep* p=&nodes[i];
				
				int j=p->link[0]->getlink(p);
				int k=p->link[3]->getlink(p);
				p->link[1]=p->link[0]->link[j+1]?p->link[0]->link[j+1]:p->link[0]->link[0];
				p->link[2]=p->link[3]->link[k+1]?p->link[3]->link[k+1]:p->link[3]->link[0];
				p->link[4]=k>0?p->link[3]->link[k-1]:(p->link[3]->link[5]?p->link[3]->link[5]:p->link[3]->link[4]);
				p->link[5]=j>0?p->link[0]->link[j-1]:(p->link[0]->link[5]?p->link[0]->link[5]:p->link[0]->link[4]);
				
				if(
						p->link[1]->link[0]==p->link[4]->link[0] ||
						p->link[1]->link[3]==p->link[4]->link[0] ||
						p->link[1]->link[0]==p->link[4]->link[3] ||
						p->link[1]->link[3]==p->link[4]->link[3]
						){
					std::swap(p->link[2],p->link[4]);
				}
			}
			for(int i=0;i!=nodes.size();++i)
				for(int i=0;i!=6;++i)
					nodes[i].rear[i]=0;
		}
		vector<Node> tnodes;
		for(int i=0;i!=nodes.size();++i)
			nodes[i].id=i;
		for(int i=0;i!=nodes.size();++i){
			tnodes.push_back(Node());
			tnodes.back().id=i;
			tnodes.back().x=nodes[i].x;
			tnodes.back().y=nodes[i].y;
			tnodes.back().z=nodes[i].z;
			for(int j=0;j!=6;++j)
				tnodes.back().neighbor[j]=
					nodes[i].link[j]?nodes[i].link[j]->id:-1;
		}
		return tnodes;
	}
}}
