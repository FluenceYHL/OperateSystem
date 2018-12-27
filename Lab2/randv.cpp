#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
#include<stdlib.h>
double get_rand(){  //generate a random variable on U[0,1] 
	return rand()/(double)	RAND_MAX;
}

double p[]={0.7,0.1,0.2};
void gen_accumulate_pr(double p[],int n,double accp[])
{
	accp[0]=p[0];
	for(int i=1;i<n-1;i++)
		accp[i]=p[i]+accp[i-1];
	accp[n-1]=1.0;
}
int uniform(double accp[],int n){ 
	// random sample for probability p[]
	// get i with probability p[i]
	double rv=get_rand();
	int i=0;
	if(rv< accp[0])
		return 0;
	do{
		i++;
	}while(rv>accp[i]);	
	if(i==n) i=n-1;
	//std::cout<<rv<<" "<<i<<std::endl;
	return i;
}

int main(int argc, char** argv) {
	for(int i=0;i<10;i++)
		std::cout<<"  "<<get_rand()<<std::endl;
	double accp[3];
	gen_accumulate_pr(p,3,accp);
	for(int i=0;i<3;i++)
		std::cout<<" "<< " "<<accp[i]<<std::endl;	
	
	for(int i=0;i<30;i++)
		std::cout<<" "<< " "<<uniform(accp,3)<<std::endl;	
	return 0;
}
