/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _real.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


/* This file has been automatically generated from "real.w"
   by CTANGLE (Version 3.1 [km2c]) */
/*12:*/

#include <LEDA/real.h>
/*16:*/

#include <cmath>



/*:16*/

/*14:*/

enum constructortype{DOUBLE= 0,BIGFLOAT= 1,NEGATION= 2,
SQUAREROOT= 3,ADDITION= 4,SUBTRACTION= 5,MULTIPLICATION= 6,
DIVISION= 7};

struct app_bf
{bigfloat NUM_BF;
bigfloat ERROR_BF;
app_bf(){}
app_bf(const bigfloat&x,const bigfloat&error_x)
{NUM_BF= x;ERROR_BF= error_x;}
};

/*:14*/

/*3:*/

const double eps= ldexp(1,-53);/* $eps = 2^{-53}$ */
const double correction= 1+8*eps;
const double MaxError= ldexp(1,900);/* $2^{900}$ */
const double MinDbl= ldexp(1,-1022);
const double MaxDbl= ldexp(2-ldexp(1,-52),1023);

/*:3*//*4:*/

const bigfloat zero= double(0);
const bigfloat one= double(0);
const bigfloat two= double(2);
const bigfloat onehalf= double(1)/double(2);
const bigfloat onefourth= double(1)/double(4);
const bigfloat oneeighth= double(1)/double(8);
const bigfloat eps_prec= eps;
const bigfloat correction_bf= correction;

/*:4*/

/*15:*/

class real_rep
{
friend class real;
/*9:*/

friend real operator+(const real&,const real&);
/*{\Mbinop addition
}*/
friend real operator-(const real&,const real&);
/*{\Mbinop subtraction
}*/
friend real operator*(const real&,const real&);
/*{\Mbinop multiplication
}*/
friend real operator/(const real&,const real&);
/*{\Mbinop division
}*/
friend real operator-(const real&);
/*{\Munop negation
}*/
friend real sqrt(const real&);
/*{\Mop squareroot operation
}*/

/*:9*/


double NUM;
double ERROR;
app_bf*APP_BF_PTR;
constructortype CON;
real_rep*OP1,*OP2;
int count;

real_rep(double= 0);
real_rep(const bigfloat&);
real_rep(const bigrational&);

~real_rep();

LEDA_MEMORY(real_rep)

inline bigfloat&num_bf() const {return APP_BF_PTR->NUM_BF;}
inline bigfloat&error_bf() const {return APP_BF_PTR->ERROR_BF;}
void init_app_bf();
void adjust_dbl();

int sign();
int sign(const bigfloat&);
inline void improve(const bigfloat&);
inline void improve(long k);
inline void compute(long);

/*43:*/

friend void Mignotte_parameters(const real_rep&,integer&,integer&);

/*:43*/

};

/*:15*/

// INSERTED
GenPtr real::copy() const { PTR->count++; return PTR; }
void   real::clear()      { if (PTR && --(PTR->count)==0) delete PTR; }


/*17:*/

real::real(){PTR= new real_rep(0);}
real::real(double x){PTR= new real_rep(x);}
real::real(int x){PTR= new real_rep(double(x));}
real::real(const bigrational&x){PTR= new real_rep(x);}
real::real(const bigfloat&x){PTR= new real_rep(x);}
real::real(const integer&x){PTR= new real_rep(bigfloat(x));}
real::real(real_rep&x_rep){PTR= &x_rep;}
real::real(const real&x){PTR= x.PTR;x.PTR->count++;}
real&real::operator= (const real&x)
{x.PTR->count++;
if(PTR && --PTR->count==0)delete PTR;
PTR= x.PTR;
return(*this);
}

/*:17*//*18:*/

real_rep::real_rep(double x)
{APP_BF_PTR= nil;count= 1;
OP1= OP2= nil;
NUM= x;ERROR= 0;CON= DOUBLE;
}
real_rep::real_rep(const bigfloat&x)
{APP_BF_PTR= new app_bf(x,0);count= 1;
OP1= nil;OP2= nil;CON= BIGFLOAT;
adjust_dbl();
}
real_rep::real_rep(const bigrational&x)
{
APP_BF_PTR= nil;count= 1;
OP1= new real_rep(bigfloat(x.numerator()));
OP2= new real_rep(bigfloat(x.denominator()));
CON= DIVISION;
adjust_dbl();
}

/*:18*//*19:*/

real::~real()
{PTR->count--;
if(PTR->count==0)
delete PTR;
}

real_rep::~real_rep()
{
if(APP_BF_PTR!=nil)
delete APP_BF_PTR;
if((OP1!=nil)&&(--OP1->count==0))
delete OP1;
if((OP2!=nil)&&(--OP2->count==0))
delete OP2;
#ifdef DEBUG_REAL
APP_BF_PTR= nil;
OP1= OP2= nil;
#endif
}


/*:19*/

/*20:*/

void real_rep::init_app_bf()
{
if(APP_BF_PTR==nil)
if(ERROR!=Infinity)
APP_BF_PTR=
new app_bf(bigfloat(NUM),fabs(bigfloat(ERROR*NUM*correction)));
else
compute(52);
}


/*:20*//*21:*/

void real_rep::adjust_dbl()
{
if((fabs(num_bf())>bigfloat(MaxDbl))
||(error_bf()>bigfloat(MaxDbl))
)
{
ERROR= Infinity;NUM= 1;return;
}
if((num_bf()==zero)&&(error_bf()==zero))
{
NUM= ERROR= 0;return;
}
double n_head= num_bf().todouble();
double e_head= error_bf().todouble();
if(n_head!=0)
{NUM= n_head;
ERROR= eps+Max(MinDbl,e_head)/fabs(n_head);
}
else
{NUM= e_head+2*MinDbl;
ERROR= 2;
}
ERROR= ERROR*correction;
if(ERROR>=MaxError)
{NUM= 1;ERROR= Infinity;
}
}

/*:21*//*22:*/


double real::todouble()const
{return PTR->NUM;}
double real::get_double_error()const
{return PTR->ERROR;}
bigfloat real::tobigfloat()const
{PTR->init_app_bf();return PTR->num_bf();}
integer real::get_precision()const
{PTR->init_app_bf();return-PTR->error_bf().get_exponent();}

std::ostream&operator<<(std::ostream&out, const real&x)
{out<<x.tobigfloat();return out;}

std::istream&operator>>(std::istream&in,real&x)
{
double x_num;in>>x_num;
x= real(x_num);return in;
}



/*:22*/

/*23:*/

real operator+(const real&x,const real&y)
{
real_rep&z_rep= *new real_rep();
/* this represents the new |real| element |z| that we return later */

z_rep.CON= ADDITION;
z_rep.OP1= x.PTR;z_rep.OP2= y.PTR;
z_rep.OP1->count++;z_rep.OP2->count++;
/* the new element |z| now points to |x| and |y|, so their counters have
       to be increased by one
    */

double&x_num= x.PTR->NUM;
double&y_num= y.PTR->NUM;
double&NUM= z_rep.NUM;
NUM= x_num+y_num;/* approximation of $x+y$ */

if(fabs(NUM)>MinDbl)
{/* regular case */
z_rep.ERROR= fabs(x_num/NUM)*x.PTR->ERROR
+fabs(y_num/NUM)*y.PTR->ERROR
+eps;
}
else
{/* underflow is possible */

NUM= fabs(x_num)*x.PTR->ERROR+fabs(y_num)*y.PTR->ERROR+4*MinDbl;
z_rep.ERROR= 2;
}

z_rep.ERROR*= correction;

if(!((fabs(NUM)+x.PTR->ERROR+y.PTR->ERROR<Infinity)
&&(z_rep.ERROR<MaxError)
)
)
/* if one of the quantities fabs(NUM), x.PTR->ERROR, y.PTR->ERROR, and
        z\_rep.ERROR is either Infinity or NaN, we set the error of the
        result to Infinity
     */
z_rep.ERROR= Infinity;

return real(z_rep);
}

real operator-(const real&x,const real&y)
{
real_rep&z_rep= *new real_rep();
z_rep.CON= SUBTRACTION;
z_rep.OP1= x.PTR;z_rep.OP2= y.PTR;
z_rep.OP1->count++;z_rep.OP2->count++;
if((x.PTR->ERROR==Infinity)||(y.PTR->ERROR==Infinity))
{z_rep.ERROR= Infinity;return real(z_rep);
}
double x_num= x.PTR->NUM,y_num= y.PTR->NUM;
double&NUM= z_rep.NUM;
NUM= x_num-y_num;/* approximation of $x-y$ */
if(fabs(NUM)<=MinDbl)
{
NUM= fabs(x_num)*x.PTR->ERROR+fabs(y_num)*y.PTR->ERROR+4*MinDbl;
z_rep.ERROR= 2;
}
else
{z_rep.ERROR= fabs(x_num/NUM)*x.PTR->ERROR
+fabs(y_num/NUM)*y.PTR->ERROR
+eps;
}
z_rep.ERROR*= correction;

if(!((fabs(NUM)+x.PTR->ERROR+y.PTR->ERROR<Infinity)
&&(z_rep.ERROR<MaxError)
)
)
z_rep.ERROR= Infinity;

return real(z_rep);
}

/*:23*//*24:*/

real operator*(const real&x,const real&y)
{
real_rep&z_rep= *new real_rep();
z_rep.CON= MULTIPLICATION;
z_rep.OP1= x.PTR;z_rep.OP2= y.PTR;
z_rep.OP1->count++;z_rep.OP2->count++;
double&x_num= x.PTR->NUM;
double&y_num= y.PTR->NUM;
double&qx= x.PTR->ERROR;
double&qy= y.PTR->ERROR;
double&NUM= z_rep.NUM;
NUM= x_num*y_num;

if(!(fabs(NUM)<MinDbl))
{
z_rep.ERROR= qx+qy+qx*qy+eps;
}
else
{if((x_num!=0)&&(y_num!=0))
{/* underflow occurred in the computation of |z_rep.NUM| */
z_rep.NUM= MinDbl*(1+qx)*(1+qy);
z_rep.ERROR= 2;
}
else
z_rep.ERROR= 0;
}
z_rep.ERROR*= correction;

if(!((fabs(NUM)+qx+qy<Infinity)&&(z_rep.ERROR<MaxError)))
z_rep.ERROR= Infinity;

return real(z_rep);
}

/*:24*//*25:*/

real operator/(const real&x,const real&y)
{
real_rep&z_rep= *new real_rep();
z_rep.CON= DIVISION;
z_rep.OP1= x.PTR;z_rep.OP2= y.PTR;
z_rep.OP1->count++;z_rep.OP2->count++;
double x_num= x.PTR->NUM,y_num= y.PTR->NUM;
double&NUM= z_rep.NUM;
if(!(y.PTR->ERROR<1))
{z_rep.ERROR= Infinity;return real(z_rep);
}
if(y_num==0)
error_handler(1,"real::operator/:Division by zero");
NUM= x_num/y_num;
if(fabs(NUM)>=MinDbl)
z_rep.ERROR= (x.PTR->ERROR+y.PTR->ERROR)/(1-y.PTR->ERROR)
+eps;
else
{
if(x_num!=0)
{
NUM= MinDbl*(1+x.PTR->ERROR)/(1-y.PTR->ERROR);
z_rep.ERROR= 2;
}
else z_rep.ERROR= 0;
}
z_rep.ERROR*= correction;

if(!((fabs(NUM)+x.PTR->ERROR!=Infinity)||(z_rep.ERROR<MaxError)))
z_rep.ERROR= Infinity;
return real(z_rep);
}

/*:25*//*26:*/

real sqrt(const real&x)
{real_rep&z_rep= *new real_rep();
z_rep.CON= SQUAREROOT;
z_rep.OP1= x.PTR;z_rep.OP1->count++;
z_rep.OP2= nil;
double x_num= x.PTR->NUM;
if(x.PTR->ERROR>=1)
{z_rep.ERROR= Infinity;return real(z_rep);
}
if(x_num==0)
{z_rep.NUM= z_rep.ERROR= 0;return real(z_rep);
}
if(x_num<0)
error_handler(-1,"real::sqrt: sqrt of negative number");
z_rep.NUM= sqrt(x_num);
z_rep.ERROR= x.PTR->ERROR+eps;
z_rep.ERROR*= correction;
return real(z_rep);
}

/*:26*//*27:*/

real operator-(const real&x)
{real_rep&z_rep= *new real_rep();
z_rep.CON= NEGATION;
z_rep.OP1= x.PTR;z_rep.OP1->count++;z_rep.OP2= nil;
z_rep.NUM= -x.PTR->NUM;z_rep.ERROR= x.PTR->ERROR;
return real(z_rep);
}

/*:27*//*28:*/

real operator+= (real&x,real&y){x= x+y;return x;}
real operator-= (real&x,real&y){x= x-y;return x;}
real operator*= (real&x,real&y){x= x*y;return x;}



/*:28*/

/*32:*/

long needed_places(const bigfloat&epsilon)
{return-epsilon.get_exponent().tolong();}

/*:32*/

/*29:*/

void real::improve(const integer&prec){PTR->improve(pow2(-prec));}
void real::improve(long p){PTR->improve(pow2(-p));}
void real::compute_in(long p){PTR->compute(p);}
void real::compute_up_to(long p)
{PTR->init_app_bf();
if(PTR->sign()==0)return;
bigfloat q= (fabs(tobigfloat())+pow2(-get_precision()))*bigfloat(pow2(-p-1));
PTR->improve(q);
PTR->APP_BF_PTR->NUM_BF.round(p);
PTR->APP_BF_PTR->ERROR_BF= q;
PTR->adjust_dbl();
}

/*:29*//*30:*/

void real_rep::improve(const bigfloat&q)
{
if(q<=zero)
error_handler(-1,"real_rep::improve: quality <= 0");
int signum;
long p;
bigfloat needed_precision,
easy_value,
qx,qy,
y_low,
x_high,x_low,
qsquare;
init_app_bf();
if(error_bf()<=q)return;

switch(CON)
{
case DOUBLE:case BIGFLOAT:return;
case NEGATION:
OP1->improve(q);
num_bf()= -OP1->num_bf();
break;
case ADDITION:/*33:*/

OP1->improve(onefourth*q);
OP2->improve(onefourth*q);
easy_value= OP1->num_bf()+OP2->num_bf();
if(easy_value==zero)
num_bf()= zero;
else
{
needed_precision= onefourth*q/fabs(easy_value);
p= needed_places(needed_precision);
num_bf()= add(OP1->num_bf(),OP2->num_bf(),p);
}
/*:33*/
;break;
case SUBTRACTION:/*34:*/

OP1->improve(onefourth*q);
OP2->improve(onefourth*q);
easy_value= OP1->num_bf()+OP2->num_bf();
if(easy_value==zero)
num_bf()= 0;
else
{
needed_precision= onefourth*q/fabs(easy_value);
p= needed_places(needed_precision);
num_bf()= sub(OP1->num_bf(),OP2->num_bf(),p);
}

/*:34*/
;break;
case MULTIPLICATION:/*35:*/

OP2->init_app_bf();
qx= q*oneeighth/(fabs(OP2->num_bf())+OP2->error_bf());
OP1->improve(qx);
qy= q*oneeighth/fabs(OP1->num_bf());
OP2->improve(qy);
easy_value= OP1->num_bf()*OP2->num_bf();
if(easy_value==zero)
num_bf()= zero;
else
{
needed_precision= q*onefourth/fabs(easy_value);
p= needed_places(needed_precision);
num_bf()= mul(OP1->num_bf(),OP2->num_bf(),p);
}


/*:35*/
;break;
case DIVISION:/*36:*/

signum= OP2->sign();
if(signum==0)error_handler(0,"real::improve: division by zero");
OP2->init_app_bf();
y_low= fabs(OP2->num_bf())-error_bf();
qx= q*y_low*oneeighth;
OP1->improve(qx);
if(OP1->num_bf()==zero)
num_bf()= 0;
else
{
qy= q*y_low*y_low*oneeighth/fabs(OP1->num_bf());
OP2->improve(qy);
needed_precision= q*onefourth*fabs(OP2->num_bf()/OP1->num_bf());
p= needed_places(needed_precision);
num_bf()= div(OP1->num_bf(),OP2->num_bf(),p);
}

/*:36*/
;break;
case SQUAREROOT:/*37:*/

OP1->init_app_bf();
x_low= OP1->num_bf()-OP1->error_bf();
x_high= OP1->num_bf()+OP1->error_bf();
qsquare= q*q;

if((x_low<=onefourth*qsquare)&&(x_high>=onehalf*qsquare))
{
OP1->improve(oneeighth*qsquare);
x_low= OP1->num_bf()-OP1->error_bf();
x_high= OP1->num_bf()+OP1->error_bf();
}
if(x_high<zero)
error_handler(-1,"real::improve: sqrt needs an argument >= 0");
if(x_low>onefourth*qsquare)
{
qx= onefourth*q*sqrt(x_low);
OP1->improve(qx);
needed_precision= onefourth*q/sqrt(x_high);
p= needed_places(needed_precision);
num_bf()= sqrt(fabs(OP1->num_bf()),p);
}
else
num_bf()= 0;


/*:37*/
;break;
}
error_bf() = q;
adjust_dbl();
}

/*:30*//*38:*/

void real_rep::compute(long p)
{
if(p<=0)
error_handler(1,"real_rep::compute needs an argument > 0");
bigfloat*x_ptr= nil,*qx_ptr= nil,*y_ptr= nil,*qy_ptr= nil;
if(OP1!=nil)
{OP1->compute(p);
x_ptr= &(OP1->num_bf());
qx_ptr= &(OP1->error_bf());
}
if(OP2!=nil)
{OP2->compute(p);
y_ptr= &(OP2->num_bf());
qy_ptr= &(OP2->error_bf());
}
bigfloat&x= *x_ptr,&y= *y_ptr,&qx= *qx_ptr,&qy= *qy_ptr,
x_low,x_high,y_low;
if(APP_BF_PTR==nil)APP_BF_PTR= new app_bf();

int signum;
bigfloat prec= pow2(-p);

switch(CON)
{
case DOUBLE:
num_bf()= bigfloat(NUM);
error_bf()= 0;break;
case BIGFLOAT:break;
case NEGATION:
num_bf()= -x;
error_bf()= qx;break;
case ADDITION:
num_bf()= add(x,y,p);
error_bf()= fabs(num_bf())*prec+qx+qy;break;
case SUBTRACTION:
num_bf()= sub(x,y,p);
error_bf()= fabs(num_bf())*prec+qx+qy;break;
case MULTIPLICATION:
num_bf()= mul(x,y,p);
error_bf()= fabs(num_bf())*prec+fabs(x)*qy+fabs(y)*qx+qx*qy;
break;
case DIVISION:
if(qy>=fabs(y))
{signum= OP2->sign();
if(signum==0)error_handler(-2,"real::compute: division by zero");
}
y_low= fabs(OP2->num_bf())-OP2->error_bf();
num_bf()= div(x,y,p);
error_bf()= fabs(num_bf())*prec+(qy*fabs(num_bf())+qx)/y_low;
break;
case SQUAREROOT:
x_low= x-qx;x_high= x+qx;
if(x_high<zero)
error_handler(-1,"real::compute: squareroot needs argument >= 0");
if(x_low>zero)
{
num_bf()= sqrt(x,p);
error_bf()= num_bf()*prec+qx/sqrt(x_low);
}
else
{
num_bf()= sqrt(qx,p);
error_bf()= two*num_bf();
}
}
error_bf()= error_bf()*bigfloat(correction);
adjust_dbl();
}


/*:38*/

/*39:*/

int sign(double x)
{if(x==0)return 0;else return(x>0?1:-1);}

/*:39*//*40:*/

int real::sign() {return PTR->sign(0);}
int real::sign(const integer&prec) {return PTR->sign(pow2(-prec));}
int real::sign(long p) {return PTR->sign(pow2(-p));}
int real_rep::sign() {return sign(0);}
int real_rep::sign(const bigfloat&q_initial)
{integer M= 0,deg= 1;
bigfloat q= q_initial;
/*41:*/

if((ERROR!=Infinity)&&((NUM==0)||(ERROR<1)))
return::sign(NUM);
if((APP_BF_PTR!=nil)&&
((error_bf()==zero)||(error_bf()<fabs(num_bf())))
)
return::sign(num_bf());

/*:41*/

if(q==zero)/*45:*/

Mignotte_parameters(*this,M,deg);

q= pow2(-M)*correction_bf;

/*:45*/

/*42:*/

bigfloat current_precision= eps_prec;
init_app_bf();
if(error_bf()<one)
current_precision= error_bf()*eps_prec;
improve(current_precision);
while((current_precision>q)&&(error_bf()>=fabs(num_bf())))
{current_precision= current_precision*current_precision;
improve(current_precision);
}
if(fabs(num_bf())>error_bf())
return::sign(num_bf());
return 0;

/*:42*/

}

/*:40*//*44:*/

void Mignotte_parameters(const real_rep&x,integer&M,integer&deg)
{
long exp,M_int;
integer M1,M2,deg1,deg2;
switch(x.CON)
{case DOUBLE:
deg= 1;
exp= bigfloat(x.NUM).get_exponent().tolong();
if(x.NUM==int(x.NUM))/* x.NUM is an integer */
{M= exp+1;break;}
if(exp>=52)
{M_int= exp+1;M= M_int;break;}
if(exp<0){M_int= 52-exp;M= M_int;break;}
M= 53;break;
case BIGFLOAT:if(x.APP_BF_PTR->NUM_BF!=zero)
M= (x.APP_BF_PTR->NUM_BF).get_exponent();
else
M= 0;
deg= 1;break;
case NEGATION:Mignotte_parameters(*x.OP1,M,deg);break;
case MULTIPLICATION:case DIVISION:
Mignotte_parameters(*x.OP1,M1,deg1);
Mignotte_parameters(*x.OP2,M2,deg2);
M= M1*deg2+M2*deg1;
deg= deg1*deg2;
break;
case ADDITION:case SUBTRACTION:
Mignotte_parameters(*x.OP1,M1,deg1);
Mignotte_parameters(*x.OP2,M2,deg2);
M= deg1*deg2+M1*deg2+M2*deg1;
deg= deg1*deg2;
break;
case SQUAREROOT:
Mignotte_parameters(*x.OP1,M1,deg1);
M= M1;deg= deg1<<1;
break;
}
}

/*:44*//*46:*/

int operator==(const real&x,const real&y)
{return((y-x).sign()==0?1:0);}
int operator<(const real&x,const real&y)
{return((y-x).sign()>0?1:0);}
int operator>(const real&x,const real&y)
{return((y-x).sign()<0?1:0);}
int operator<=(const real&x,const real&y)
{return((y-x).sign()>=0?1:0);}
int operator>=(const real&x,const real&y)
{return((y-x).sign()<=0?1:0);}
int operator!=(const real&x,const real&y)
{return((y-x).sign()!=0?1:0);}

/*:46*/

/*47:*/

real fabs(real&x){ if (x.sign()<0) return -x; else return x; }
real sq(const real&x){return x*x;}
real hypot(const real&x,const real&y){return sqrt(x*x+y*y);}
real powi(const real&x,int n)
{real y= x,z= 1;
int n_prefix= n;
while(n_prefix>0)
{
if(n_prefix%2)z= z*y;
n_prefix= n_prefix/2;
y= y*y;
}
return z;
}

/*:47*/


/*:12*/
