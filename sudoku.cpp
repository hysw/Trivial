#include<iostream>
using namespace std;

struct Dlk{
	Dlk():u(this),d(this),l(this),r(this),c(this),n(0),k(0),s(0){}
	void hu(){l->r=r;r->l=l;}
	void hl(){r->l=l->r=this;}
	void vu(){u->d=d;d->u=u;}
	void vl(){u->d=d->u=this;}
	Dlk *u,*d,*l,*r;
	Dlk *c; // label
	int n,k,s; // cordnate & value & subnodes(label only)
};
Dlk* choose(Dlk* h){
	Dlk* c=h->r;
	int s=c->s;
	for(Dlk*j=c;j!=h;j=j->r)
		if(j->s < s)
			s=((c=j)->s);
	return c;
}

void cover(Dlk*c){
	c=c->c;
	c->hu();
	for(Dlk*i=c->d;i!=c;i=i->d){
		for(Dlk*j=i->r;j!=i;j=j->r){
			j->vu();
			--j->c->s;
		}
	}
}

void uncover(Dlk*c){
	c=c->c;
	for(Dlk*i=c->u;i!=c;i=i->u)
		for(Dlk*j=i->l;j!=i;j=j->l){
			++j->c->s;
			j->vl();
		}
	c->hl();

}

void fill(Dlk*r,int a[]){
	cover(r->c);
	a[r->n]=r->k;
	for(Dlk*j=r->r;j!=r;j=j->r)
		cover(j);
}
void search(Dlk*h,int a[]){
	if(h==h->r){
		for(int i=0;i!=81;++i)
			cout<<a[i]<<((i+1)%9?"":"\n");
			cout<<"\n";
	}else{
		Dlk* c=choose(h);
		if(c->s){
			cover(c);
			for(Dlk*r=c->d;r!=c;r=r->d){
				a[r->n]=r->k;
				for(Dlk*j=r->r;j!=r;j=j->r)
					cover(j);
				search(h,a);
				for(Dlk*j=r->l;j!=r;j=j->l)
					uncover(j);
			}
			uncover(c);
		}
	}
}
Dlk* create(Dlk* lk,Dlk* col,Dlk* row){
	col=col?col:lk;
	row=row?row:lk;
	lk->u=col;
	lk->d=col->d;
	lk->c=col;
	lk->l=row;
	lk->r=row->r;
	lk->vl();
	lk->hl();
	return lk;
}
Dlk root;
Dlk constraints[4][9*9];
Dlk nodes[9][9][9][4];
char m[9][9];
int main(){
	for(int r=0;r!=9;++r)
		for(int c=0;c!=9;++c)
			cin>>m[r][c];
	int n[90];
	for(int i=0;i!=4;++i)
		for(int j=0;j!=9*9;++j){
			constraints[i][j].s=9;
			create(&constraints[i][j],0,&root);
		}
	for(int r=0;r!=9;++r)
		for(int c=0;c!=9;++c)
			for(int n=0;n!=9;++n){
				nodes[r][c][n][3].n=nodes[r][c][n][2].n=nodes[r][c][n][1].n=nodes[r][c][n][0].n=r*9+c;
				nodes[r][c][n][3].k=nodes[r][c][n][2].k=nodes[r][c][n][1].k=nodes[r][c][n][0].k=n+1;
				create(&nodes[r][c][n][0],&constraints[0][r*9+n],&nodes[r][c][n][0]);
				create(&nodes[r][c][n][1],&constraints[1][c*9+n],&nodes[r][c][n][0]);
				create(&nodes[r][c][n][2],&constraints[2][(((r/3)*3)+c/3)*9+n],&nodes[r][c][n][0]);
				create(&nodes[r][c][n][3],&constraints[3][r*9+c],&nodes[r][c][n][0]);
			}
	for(int r=0;r!=9;++r)
		for(int c=0;c!=9;++c)
			if('1'<=m[r][c]&&m[r][c]<='9')
				fill(&nodes[r][c][m[r][c]-'1'][3],n);
	search(&root,n);
}
