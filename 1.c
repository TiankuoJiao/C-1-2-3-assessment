#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#define arraylength 1000
/*Function declaration*/
void name();
void phone();
void d2b(int n);/*A function that changes from decimal to binary*/
void d2b_main();
void zuobiao();
void Select_Function();
int select=0;
/*Call function*/
int main()
{
    name();
	phone();
	d2b_main();
	zuobiao();
 	return 0;
}
/*The following is the first exercise*/
void name()
{
   char name[arraylength]={0};/*Define an array*/
   int sum=0,length=0, count=0, loop=0, error=0;/*int the variables*/
   
   while(1){
   		error=0;
        printf("please print your name with space: ");
	    gets(name);/*read name*/
    	for(loop=0;loop<arraylength;loop++){
/*The first line is the range of A-z, the second line is the range of A-z, and the third line is 0 (the value of the array is 0) and the ASCII code of Spaces */
            if(!(((name[loop]>=97)&&(name[loop]<=122))|| \
   		      ((name[loop]>=65)&&(name[loop]<=90))||\
				 (name[loop]==32)||(name[loop]==0))){
				error=1;
				printf("name error\n"); 
				break;
				}
		}
		if(error==1) continue;
		else break;
		
   }  
	length=strlen(name);
  	printf("length=%d\n",length);
	for(loop=0;loop<length;loop++){
		if(name[loop]==32)/*Count the number of spaces*/
		count +=1; 
		printf("name[%d]=%d\n",loop,name[loop]);
	    sum=sum+name[loop];
	}
  	printf("There is %d Space(s)\n",count);
	sum-=count*32;/*Subtract the value represented by the space*/
    printf("The result minus the space(s) is equal to %d\n",sum);
    system("PAUSE");/*Pause the code*/
    system("cls");/*Remove the interface*/ 
   }    
/*The following is the second exercise*/
void phone()
{
	char  phone[12]={0};
	int loop=0,error_stat=0;/*Add a variable that will increase during the loop and error_stat of 0 means no input error and a 1 means an input error*/
	float head=0;/*the first six bits as head*/
	float tail=0;/*the final five bits as tail*/
	float result=0;/*the initial value is 0*/
	while(1){/*Infinite loop*/
		printf("Please input a 11-digit number: ");
		scanf("%s",&phone);
		for(loop=0;loop<11;loop++){/*Scan each character to determine if it is an Arabic numeral*/
			if((phone[loop]<48)||(phone[loop]>57))
			error_stat=1;
		}
		if((phone[10]==0)||(phone[11]!='\0')||(error_stat==1)){/*judge if it's an eleven digit number*/
			printf("phone error\n");
			error_stat=0;
			continue;
		}
		break;
	}
	for(loop=0;loop<6;loop++){
		head+=(phone[loop]-48)*pow(10,5-loop);/*Turn each ASCII value into a true number and multiply it by a multiple of ten*/
	} 
	for(loop=6;loop<11;loop++){
		tail+=(phone[loop]-48)*pow(10,10-loop);/*Turn each ASCII value into a true number and multiply it by a multiple of ten*/
	}
	result=head/tail;/*get the result*/
	printf("%lf\n",result);
	system("PAUSE");/*Pause the code*/
    system("cls");/*Remove the interface*/
}
/*The following is the third exercise*/
void d2b_main()
{
	int number=0;
	while(1){/*Infinite loop*/
		printf("print a three-digit positive integer number£º");
		scanf("%d",&number);/*Assign the input number to "number"*/
		if((number<100)||(number>999)){/*Digital range*/
			printf("Input error, please enter again\n");
			fflush(stdin);
			continue;
		}
		break;
	}
	
	d2b(number);
	system("PAUSE");/*Pause the code*/
    system("cls");/*Remove the interface*/
}
/*The function that the decimal becomes a binary*/
void d2b(int n)
{
	int c[32],i=0; /*32-bit signed integer range*/
    do{
	c[i++]=n%2;/*Take the remainder*/
	n/=2;/*n is divided by 2*/
	  }
	while(n!=0);	
	for(i--;i>=0;i--)/*Until i = 0*/	
	printf("%d",c[i]);
	printf("\n");
}

void zuobiao()
{
	while(1){
		printf("1:Rectangular to polar conversion 2:Polar to rectangular conversion\n");/*choose function*/
		printf("Choose function: ");
		scanf("%d", &select);
		if((select!=1)&&(select!=2)){/*Judge whether the input is 1 or 2*/
			printf("select error!!\n");/*Report an error*/
			fflush(stdin);
			continue;
		}
		else{
			Select_Function();
		}
		fflush(stdin);/*Clear buffer*/
		break;
	}
	
}

void Select_Function()
{
	int ax=0, ay=0;/*variables*/
	float moA=0, citaA=0;	
	switch(select){
		case 1:
			printf("Please print Ax,Ay, for example 2,3:\n");
			scanf("%d,%d",&ax,&ay);
			printf("Ax=%d,Ay=%d\n",ax,ay);	  
			moA= sqrt(ax*ax + ay*ay);
			printf("moA=%f\n",moA);
			printf("ay/ax=%f\n",ay*1.0/ax);
			citaA= atan(ay*1.0/ax); 
			printf("citaA=%f\n",citaA);
			break;	  	  	  
		case 2:
			printf("Please enter the magnitude of vector A and citaA£¬for example 3,0.9:\n");
			scanf("%f,%f",&moA,&citaA);
			printf("moA=%f,citaA=%f\n",moA,citaA);
			ax= moA*cos(citaA);
			ay= moA*sin(citaA);
			printf("Ax=%f,Ay=%f\n",ax,ay);
			break;	  	  	  
		default: break;/*stop*/
              	  }
}

