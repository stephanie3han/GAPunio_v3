main()
{
int i,j;
float a,b;

a = 0.786;
b = 0.897;


for (i=0;i<100000;i++)
          for (j=0;j<100000;j++)
          	{
          	b = a * b;
          	a = a * 0.89;
               }

printf("End %f %f\n",a,b);
}

