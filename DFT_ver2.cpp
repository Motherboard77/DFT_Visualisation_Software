#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<graphics.h>

using namespace std;

#define SAM  10
#define FREQ_MAX 10
#define PI 3.14159265359
#define SAMPLE_SIZE 10

int euler_computer();     //compute the euler matrix and laod it into the structure
int matrix_project();      //output the euler matrix for DFT computation
int func_sampler(int);
int matrix_mult();         //function for multiplying the euler matrix with the sampled signal values
int dft_project();
int freq_mag_project();
int struct_flush();

struct euler_angle
 {
     float sin_comp[FREQ_MAX][SAM];
     float cos_comp[FREQ_MAX][SAM];

 }angle;

 struct sample
{
    float x_samp;
    float y_samp;
}point[SAMPLE_SIZE];

struct dft_matrix
{
   float real;
   float img;
   double absolut;
}dft[SAM];

 int euler_computer()
 {
   for(int i=0;i<FREQ_MAX;i++)           //i --> frequency unit
        {
            for(int j=0;j<SAM;j++)              //for every frequency unit, get SAM-1 samples of the EULER CIRCLE
            {
                angle.sin_comp[i][j] = sin((2*PI*i*j)/SAM);      //N = #samples = 10
                angle.cos_comp[i][j]= cos((2*PI*i*j)/SAM);
            }

        }

   return 1;
 }

  int matrix_project()
        {
            for(int i = 0; i<=FREQ_MAX-1;i++)
            {
                cout<<"ROW"<<" "<<i<<endl;
                for(int j = 0;j<= SAM-1;j++)
                {
                    cout<<"("<<angle.cos_comp[i][j]<<")"<<"-j "<<"("<<angle.sin_comp[i][j]<<")"<<"\n";
                }
                cout<<endl<<endl;
            }
            return 1;
        }

int func_sampler(int i)
{
    int j;

     for(j=0;j<SAMPLE_SIZE;j++)
    {
        switch(i)
        {
            case 0:   //DC Signal
            {
                point[j].x_samp = j;
                point[j].y_samp = 5;         //DC value = 5
                break;
            }
            case 1:   //St. Line
            {
                point[j].x_samp = j;
                point[j].y_samp = 5 * j + 10 ;       //slope=5, intercept = 10
                break;
            }
            case 2: //Parabola
            {
                point[j].x_samp = j;
                point[j].y_samp = 1 * pow(j,2) ;      //y = 1 * x^2
                break;
            }
            case 3: //Sin Wave
            {
              point[j].x_samp = j;
              point[j].y_samp = 50*sin((2*PI*3*j)/SAM) ;           //Asin(wt)   w ---> anglular frequency , here j = n ,replace time domain with sampled value of time, replace "t" with "n"
              break;
            }
           case 4: //Cos Wave
            {
              point[j].x_samp = j;
              point[j].y_samp = 50*cos((2*PI*3*j)/SAM) ;           //Acos(wt)   w ---> anglular frequency , here j = n ,replace time domain with sampled value of time, replace "t" with "n"
              break;
            }
            case 5: //POLY
            {
              point[j].x_samp = j;       //A0 + A1x + A2 x^2 + A3x^3 + A4x^4 + A5 x^5
              //point[j].y_samp = 50*sin((2*PI*3*j)/SAM) ;           //Asin(wt)   w ---> anglular frequency , here j = n ,replace time domain with sampled value of time, replace "t" with "n"
              point[j].y_samp = 0.1 + 0.1*pow(j,2) + 0.2*pow(j,3) ;
              break;
            }
            default: //
            {
                cout<<"that's it!"<<endl;
                break;
            }
        }
    }
     return 1;
}

int matrix_mult()
{
  for(int i=0;i<SAMPLE_SIZE;i++)
  {
    for(int j=0;j<=SAMPLE_SIZE;j++)
    {
      dft[i].real = dft[i].real + angle.cos_comp[i][j] * point[j].y_samp;    //multiply index wise elements of first row of euler with sampled function and add them together (REAL PART)
      dft[i].img = dft[i].img + angle.sin_comp[i][j] * point[j].y_samp;    //same for IMG PART
    }
    dft[i].absolut = sqrt(pow(dft[i].real,2)+pow(dft[i].img,2));
  }

  return 1;
}

int dft_project()
{
  for(int i=0;i<SAMPLE_SIZE;i++)
  {
    //display the DFT matrix
    cout<<dft[i].real<<" + "<<" j "<<dft[i].img<<endl;
  }

    return 1;
}

int freq_mag_project()
{
  for(int i=0;i<SAMPLE_SIZE;i++)
  {
    cout<<"FREQUENCY UNIT "<<i<<"  "<<dft[i].absolut<<endl;
  }
  return 1;
}

int struct_flush()     //flush the strructure storing the freq values
{
  for(int i=0;i<SAM;i++)
  {
    dft[i].real = 0;
    dft[i].img = 0;
    dft[i].absolut = 0;
  }
  return 1;
}

int main()
{
   int gd = DETECT, gm;
   initgraph(&gd, &gm, "");
    int i;
    int bottom = 400;
    int left = 40;
    int temp2;
    int j = 0;

    //Function types
    //straignt line : Ax + By - C = 0
    //parabola : y - Ax^2 = 0
    //sin wave : sin(Ax + phi)
    //cos wave: cos(Ax + phi)
    //n-degree POLY: (A0)x^0 + (A1)x^1 + (A2)x^2 + (A3)x^3 +...........+ (N-1) terms   , N ---> sample size
    while(1)
    {

    cout<<"select i in range 0 to 5 for function selection"<<"\n";
    cout<<"0: DC Signal"<<endl;
    cout<<"1: St Line"<<endl;
    cout<<"2: Parabola"<<endl;
    cout<<"3: Sin Wave"<<endl;
    cout<<"4: Cos Wave"<<endl;
    cout<<"5: POLY "<<endl;             //here poly of MAX DEGREE N-1, N=10
    cout<<"6: ABORT PROCESS"<<endl;
    cin>>i;

     cleardevice();

    if(i == 6)
    break;
    else
    {
     //sample the desired function of choice. The no of samples must be the same as the sample size of the euler circle
    func_sampler(i);
    //generate the euler matrix
    euler_computer();
    //display the euler matrix
    //  matrix_project();
    //multiply the sampled function and the euler marix and load in the structure dft_matrix
    matrix_mult();
    //display the final DFT matrix
    dft_project();
    //display the frequency magnitude
    freq_mag_project();
    }

     for(int i=0; i<SAMPLE_SIZE;i++)    //plot bar for each sample ranging from 0 to SAMPLE_SIZE -1
    {
       temp2 = dft[i].absolut;
       bar(left+j,bottom-temp2 ,left+20 + j,bottom);
       line(left+j,bottom,left+20+j,bottom);
       // settextstyle(0, HORIZ_DIR, 1);
		   //outtextxy(35 + j, bottom-10, string()i);
       j = j + 50;
     }
       j =  0;
       temp2 = 0;
       struct_flush();
    }

getch();
closegraph();
return 0;
}
