#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

/*******************************************************************************
macro and data type defination
********************************************************************************/

#define RET_OK      0
#define RET_ERR     -1
#define RET_EXIT    1

#define RET_DRIVER_NOT_FOUND        100
#define RET_DRIVER_ID_INVALID       101

#define RET_PARKING_SPACE_FULL      110
#define RET_PARKING_SPACE_NOTEXIST  111
#define RET_PARKING_SPACE_INVALID   112

#define RET_ACCOUNT_INSUFFICIENT    120
#define RET_ACCOUNT_INVALID         121

#define DRIVER_ROLE_STAFF       1
#define DRIVER_ROLE_STUDENT     2
#define DRIVER_ROLE_INVALID     -1

#define TYPE_PLACE_CAR	                1
#define TYPE_PLACE_EBIKE_WITHCHARGER 	2
#define TYPE_PLACE_EBIKE_NOCHARGER		3
#define TYPE_PLACE_INVALID              -1

#define PLACE_STAE_IDLE         0
#define PLACE_STATE_OCCUPIED    1

#define DEF_CAR_RATE                0.5
#define DEF_EBIKE_WITHCHARGER_RATE  0.3
#define DEF_EBIKE_NOCHARGER_RATE    0.3

#define MAX_NUM_DRIVERS         100
typedef struct
{
    float     car_rate;                    /* Pounds per hour */
    float     ebike_withcharger_rate;      /* Pounds per hour */
    float     ebike_nocharger_rate;        /* Pounds per hour */
} PARKING_FEE_RATE;

typedef struct
{
    int     id;
    int     type;         /* 1-car 2-ebike with charger 3-ebike no charger */
    int     state;        /* 0:free 1:occupied -1:invalid */
	char 	name[20];
    char    driver_id[20];  /* driver's id */
    time_t  time_start;     /* parking start time */
   
} PARKING_PLACE;

typedef struct
{
    char    name[20];       /* name of driver */
    char    id[20];         /* university id number */
    char    phone[20];      /* phone number */
    double  balance;        /* account balance */
    int     role;           /* 1: staff; 2: student; -1: invalid */    
	PARKING_PLACE	place[MAX_NUM_DRIVERS]; 
} DRIVER_INFO;

/*******************************************************************************
variables declaration 
********************************************************************************/
#define MAX_CAR_FLOORS  10
#define MAX_CAR_ROWS    15
#define MAX_CAR_COLS    15
#define MAX_EBIKE_C_ROWS        8
#define MAX_EBIKE_C_COLS        2
#define MAX_EBIKE_NC_ROWS       10
#define MAX_EBIKE_NC_COLS       4

PARKING_PLACE parking_car[MAX_CAR_FLOORS][MAX_CAR_ROWS][MAX_CAR_COLS];           /* multi-story carpark, where each of the 10 floors is a 15x15 rectangular grid of car parking spaces */
PARKING_PLACE parking_ebike_withcharger[MAX_EBIKE_C_ROWS][MAX_EBIKE_C_COLS];   /* 8x2 An 8x2 rectangular grid of e-bike parking spaces each with an electrical recharge plug is provided */
PARKING_PLACE parking_ebike_nocharger[MAX_EBIKE_NC_ROWS][MAX_EBIKE_NC_COLS];    /* A 10x4 rectangular grid of e-bike parking spaces with no recharge facility */
DRIVER_INFO   drivers_info[MAX_NUM_DRIVERS];                        /* drivers including staffes and students */
PARKING_FEE_RATE    parking_fee_rate;
    
/*******************************************************************************
function declaration 
********************************************************************************/

/*******************************************************************************
************** MODULE for common ***********************************************
*******************************************************************************/
int wait_for_kbhit()
{
    int key;
    
    printf("\r\nPress any key to coninue, ESC to exit ... \r\n"); /*we use this function to check whether the user want to continue or go back to the interface*/

    _kbhit();
    key = _getch();
    
    if ( 27 == key )
    {
        return RET_EXIT;
    }
    
    return RET_OK;
}
/*******************************************************************************
*************** MODULE for Administrator ***************************************
********************************************************************************/
int add_driver_info()/*this function is used to add new driver information*/
{
    DRIVER_INFO driver, *p_driver; /*declare a structure of type driver, declare the pointer is of type structure*/

    int ret_val, i,b,loop,error=0,error_stat=0;
    printf("***You need to help driver register. Obtain driver information.\n\r ");
    printf("Input Id:");
    scanf("%s", &driver.id );   
    ret_val = search_driver_info( driver.id,  &p_driver); /*look through the whole array to find the information of the dirver */
    if( ret_val == RET_OK ) /*if we find the information of the driver, enter this if*/
    {
        printf("This driver already existed!\r\n");
        return RET_OK;
    }

    for( i=0; i<MAX_NUM_DRIVERS; i++)
    {
        if( 0 == strcmp( drivers_info[i].id, "" ) )/*look through the whole array to find an empty structure to store the new driver information*/
        {
            p_driver = &drivers_info[i];
            break;
        }
    }
    if ( i==MAX_NUM_DRIVERS )/*if there is no empty structures, enter this if*/
    {
        printf("Drivers space was full!\r\n");
        return RET_ERR;
    }
    strcpy(p_driver->id, driver.id); /*scan the driver information and store them in the empty structure*/
    while(1){
    error=0;
    printf("please print your name with space:");
    fflush(stdin);
    gets(p_driver->name);/*read name*/
    for(loop=0;loop<20;loop++){
/*To make sure the user enters the right characters */
    if(!(((p_driver->name[loop]>=97)&&(p_driver->name[loop]<=122))|| \
    ((p_driver->name[loop]>=65)&&(p_driver->name[loop]<=90))||\
    (p_driver->name[loop]==32)||(p_driver->name[loop]==0)))
    error=1;
    }
    if((p_driver->name[0]==32)||(error==1)){ /*if the administrator enters space first enter this if*/
    printf("name error\n"); 
    error=0;
    continue;
    } 
    break;
    }
    while(1){/*Infinite loop*/
		printf("Please input a 11-digit number: ");
		scanf("%s", &p_driver->phone );
		for(loop=0;loop<11;loop++){/*Scan each character to determine if it is an Arabic numeral*/
			if((p_driver->phone[loop]<48)||(p_driver->phone[loop]>57))
			error_stat=1;
		}
		if((p_driver->phone[10]==0)||(p_driver->phone[11]!='\0')||(error_stat==1)){/*judge if it's an eleven digit number*/
			printf("phone error\n");
			error_stat=0;
			continue;
		}
		break;
	}
    printf("Input Role(1-Staff 2-Student):");
     while (1) {
        fflush(stdin);
        b = scanf("%d", &p_driver->role );    /*Receive user input data*/
        if (b == 1)        /*When input data is valid*/
        {
            if ((p_driver->role==1)||(p_driver->role==2)) {
                break;  /*Return the function sequence number selected by the user*/
            }
            else {
                printf("  There is an error in the input data, please re-enter:");
            }
        }
        else
        {
            rewind(stdin);
            printf("      The input data is not valid. Please re-enter:");
        }
    }
    p_driver->balance = 0;
    printf("Adding driver success!\r\n");
    return RET_OK;  
}

int rmv_driver_info()/*this functionis used to remove the specific driver information*/
{
    DRIVER_INFO     driver, *p_driver;/*use this pointer to point to the structure we want to remove*/
    int     ret_val;
    printf("Driver Id:");
    scanf("%s", &driver.id ); 
    ret_val = search_driver_info( &driver.id, &p_driver);/*look through the whole array to find the information of the dirver */
    if( RET_OK != ret_val )/*if the user information does not exist,enter this if*/
    {
        printf("This driver not existing!\r\n");
        return RET_OK;
    }
    strcpy( p_driver->id, ""); /*when we find the structure store the driver information, we set all the element of the element to '0' which is equivalent to remove the user information*/
    strcpy( p_driver->name, "");
    strcpy( p_driver->phone, "");
    p_driver->role = DRIVER_ROLE_INVALID;
    p_driver->balance = 0;
    printf("Driver % removed!\r\n", driver.id );      
    return RET_OK;
}

int view_one_driver_info()/*this function is used to view the information of specific driver*/
{
    DRIVER_INFO driver, *p_driver;
    int  ret_val;   
    printf("Driver Id:");
    scanf("%s", &driver.id );    
    ret_val = search_driver_info( &driver.id, &p_driver);/*look through the whole array to find the information of the dirver */
    if( ret_val != RET_OK )/*if the user information does not exist,enter this if*/
    {
        printf("This driver does not exist!\r\n");
        return RET_OK;
    }
    if( 0 == strcmp(p_driver->id, "") )     /* to make safe more */ 
    {
        printf("Failed to locate driver.\r\n");
        return RET_ERR;
    }
    printf("Driver found!\r\n");
    printf( "\tId:%s\r\n", p_driver->id );/*if we find the driver information, print it*/
    printf( "\tName:%s\r\n", p_driver->name );
    printf( "\tPhone:%s\r\n", p_driver->phone ); 
    printf( "\tRole:%s\r\n", (p_driver->role == DRIVER_ROLE_STAFF)? 
    "Staff" : ((p_driver->role == DRIVER_ROLE_STUDENT)?"Student":"Invalid" ) ); 
    printf( "\tBalance:%f\r\n", p_driver->balance );
    printf("\r\n");    
    return RET_OK;
}

int view_all_drivers_info()/*this function is used to view all the drivers' information stored in the array*/
{
    int     i, total_num;
    char	place_name[50];
    DRIVER_INFO     *p_driver;
    PARKING_PLACE   *p_place;

    printf("\r\n");
    printf("\tId        Name      Phone          Role      Balance   Position\r\n");
    printf("\t---------------------------------------------------------------------------\r\n");		/*Position to be added*/

    total_num = 0;
    for( i=0; i<MAX_NUM_DRIVERS; i++ )
    {
        p_driver = &drivers_info[i];

        if ( 0 == strcmp(p_driver->id, "") )/*if we come across empty structures, skip them*/
        {
            continue;
        }
        total_num ++;
        /*get_driver_place( p_driver->id, &p_place);*/
        if( RET_OK== get_driver_place( p_driver->id, &p_place))
		{
			/*printf("\tPlace:%s\r\n",p_place->name);*/
			strcpy(place_name,p_place->name);
		}
		else
		{
			strcpy(place_name,"None");
		}
		printf( "\t%-10s%-10s%-15s%-10s%-10.2f%-15s  \r\n", 
            p_driver->id,
            p_driver->name,
            p_driver->phone, 
            (p_driver->role == DRIVER_ROLE_STAFF)? "Staff" : ((p_driver->role == DRIVER_ROLE_STUDENT)?"Student":"Invalid" ),/*if the user is a stuff,print "stuff",else print "student"*/
            p_driver->balance, place_name );					
    }
    printf("\r\n\tTotal num of drivers:%d \r\n", total_num);
}

int search_driver_info( char *p_id, DRIVER_INFO **p_driver)/*we use a pointer *P_driver to point to the address of the structure DRIVER_INFO*/
{
    int i;   
    for(i=0; i<MAX_NUM_DRIVERS; i++)
    {
        if( 0 == strcmp(drivers_info[i].id, p_id) )
        {
            *p_driver = &drivers_info[i];/*if we find the driver information, return 0*/
            return RET_OK;
        }
    }

    return RET_DRIVER_NOT_FOUND;/*return 100 if we do not find the driver information*/
}

int credit_driver_account() /*this function is used to credit the user*/
{
	DRIVER_INFO   *p_driver;  
    char id[50];
    float money;
    int b;
    printf("***If a driver intends to charge the account balance, you should manually charge the user's money into the account.\r\n"); 
    printf("\r\nDriver ID num:");
    scanf("%s", id);
    if( RET_OK != search_driver_info(id, &p_driver) )/*look through the whole array to find the information of the dirver,if the user information does not exist,enter this if*/
    {
        printf("Driver not found!\r\n");
        return RET_ERR;
    }
    else
    {
        printf("Driver found!\r\n");
    }   
    while (1) {
        fflush(stdin);
        printf("Please input the money to credit, in pounds:");
        b = scanf("%f", &money);    /*Receive user input data*/
        if (b == 1)   /*Determine if the user entered the number*/
        {
            
         break; 
        }
        else
        {
            fflush(stdin);
            printf("      The input data is not valid. Please re-enter:");
        }
    }
    if( RET_OK != search_driver_info(id, &p_driver) )
    {
        printf("Driver not found!\r\n");
        return RET_ERR;
    }
    else
    {
        printf("Driver found!\r\n");
    }
    p_driver->balance += money;

    printf("Credit succeeded!\r\n");
    
}

int set_parking_feerate()/*this function is designed for the conservator to change the fee rate*/
{
	int a,b,c;
    printf("Car place fee rate:\n");  
    while (1) {
        a = scanf("%f", &parking_fee_rate.car_rate);    /*Receive user input data*/
        if (a == 1)        
        {
            if (parking_fee_rate.car_rate >= 0 && parking_fee_rate.car_rate <= 1000) {/*User input range*/
                break;  
            }
            else {
                printf("There is an error in the input data, please re-enter:");
            }
        }
        else
        {
            rewind(stdin);
            printf("There is an error in the input data, please re-enter:");
        }
    }	
    printf("Ebike place with charger fee rate(Pounds/hour):");
    while (1) {
        b = scanf("%f", &parking_fee_rate.ebike_withcharger_rate);    /*Receive user input data*/
        if (b == 1)       
        {
            if (parking_fee_rate.ebike_withcharger_rate >= 0 && parking_fee_rate.ebike_withcharger_rate <= 1000) { /*User input range*/
                break;  
            }
            else {
                printf("There is an error in the input data, please re-enter:");
            }
        }
        else
        {
            rewind(stdin);
            printf("There is an error in the input data, please re-enter:");
        }
    }					
	printf("Ebike place without charger fee rate(Pounds/hour):");
        while (1) {
        c = scanf("%f", &parking_fee_rate.ebike_nocharger_rate);    /*Receive user input data*/
        if (c == 1)       
        {
            if (parking_fee_rate.ebike_nocharger_rate >= 0 && parking_fee_rate.ebike_nocharger_rate <= 1000) {/*User input range*/
                break;  
            }
            else {
                printf("There is an error in the input data, please re-enter:");
            }
        }
        else
        {
            rewind(stdin);
            printf("There is an error in the input data, please re-enter:");
        }
    }
	printf("Parking fee rate set finished!\r\n");
    return RET_OK;
}

int view_parking_feerate()/*this function is designed for the administrator to see the fee rate*/
{
    printf("\r\n");
    printf("\tParking fee rate:\r\n");
    printf("\tCar place:%.3f per second\r\n", parking_fee_rate.car_rate );
    printf("\tEbike place with charger:%.3f per second\r\n", parking_fee_rate.ebike_withcharger_rate);
    printf("\tEbike place without charger:%.3f per second\r\n", parking_fee_rate.ebike_nocharger_rate );
    return RET_OK;
}

/*******************************************************************************
********************* MODULE for Entry/Exit system Worker **********************
********************************************************************************/

int ask_for_place_type(int *p_type)/*ask the user which kind of place they want to park*/
{
    char input[50];  
	while(1)
    {
        printf("What type of place would you like?\r\n");
        printf("\t1-car\r\n");
        printf("\t2-ebike without charger \r\n");
        printf("\t3-ebike with charger \r\n");
        printf("\t4-No thanks \r\n");
        scanf("%s", &input);
        if ( 0 == strcmp( input, "1") )
        {
        
            *p_type = TYPE_PLACE_CAR;
            break;
        }
        else if ( 0 == strcmp( input, "2") )
        {
            *p_type = TYPE_PLACE_EBIKE_NOCHARGER;
            break;
        }
        else if ( 0 == strcmp( input, "3") )
        {
            *p_type = TYPE_PLACE_EBIKE_WITHCHARGER;
            break;
        }
        else if ( 0 == strcmp( input, "4") )
        {
            break;
        }
        else
        {
            printf("Try again.\r\n");
            continue;
        }
    }
    return  RET_OK;   
}

int get_idle_car_space( PARKING_PLACE **p_place)/*Get spare space for car*/ 
{
    int i,j,k;   
    for(i=0; i<MAX_CAR_FLOORS; i++)/*search through the whole car parking places to see which place is empty*/
    {
        for(j=0; j<MAX_CAR_ROWS; j++)
        {
            for(k=0; k<MAX_CAR_COLS; k++)
            {
                if( parking_car[i][j][k].state == PLACE_STAE_IDLE )/*if we find an empty place, enter this if*/
                {
                    *p_place = &parking_car[i][j][k];/*store all the empty parking places*/
                    return RET_OK;
                }
            }
        }
    }
    return RET_PARKING_SPACE_FULL;/*if there is no empty slot, return this to tell that the parking slot is full*/
}

int get_idle_ebike_c_space( PARKING_PLACE **p_place)	
{
    int i,j;    
    for(i=0; i<MAX_EBIKE_C_ROWS; i++)/*search through the whole e-bike parking places with charger to see which place is empty*/
    {
        for(j=0; j<MAX_EBIKE_C_COLS; j++)
        {
            if( parking_ebike_withcharger[i][j].state == PLACE_STAE_IDLE )/*if we find an empty place, enter this if*/
            {
                *p_place = &parking_ebike_withcharger[i][j];/*store all the empty parking places*/
                return RET_OK;
            }
        }
    }
    return RET_PARKING_SPACE_FULL;/*if there is no empty slot, return this to tell that the parking slot is full*/
}

int get_idle_ebike_nc_space( PARKING_PLACE **p_place)
{
    int i,j;   
    for(i=0; i<MAX_EBIKE_NC_ROWS; i++)/*search through the whole e-bike parking places without charger to see which place is empty*/
    {
        for(j=0; j<MAX_EBIKE_NC_COLS; j++)
        {
            if( parking_ebike_nocharger[i][j].state == PLACE_STAE_IDLE )/*if we find an empty place, enter this if*/
            {
                *p_place = &parking_ebike_nocharger[i][j];/*store all the empty parking places*/
                return RET_OK;
            }
        }
    }
    return RET_PARKING_SPACE_FULL;/*if there is no empty slot, return this to tell that the parking slot is full*/
}

int allocate_space_randomly(int place_type, DRIVER_INFO *p_driver, PARKING_PLACE **p_place)
{
    int i,j,k;

    PARKING_PLACE       *place;
    
    switch (place_type)
    {
        case TYPE_PLACE_CAR:
            if ( RET_OK == get_idle_car_space( &place ) )/*if the user want to park car enter this if*/
            {
                *p_place = place;/*assign the parking slot*/
                return RET_OK;
            }
            break;
        case TYPE_PLACE_EBIKE_NOCHARGER:
            if( RET_OK == get_idle_ebike_nc_space( &place ) )/*if the user want to park e-bike without charger enter this if*/
            {
                *p_place = place;
                return RET_OK;
            }
            break;
        case TYPE_PLACE_EBIKE_WITHCHARGER:
            if( RET_OK == get_idle_ebike_c_space( &place ) )/*if the user want to park e-bike with charger enter this if*/
            {
                *p_place = place;
                return RET_OK;
            }
            break;
        default:
            return RET_ERR;           
    }
    return RET_ERR;
}

int free_a_space( PARKING_PLACE *p_place )/*this function is designed to free the space after the user is left so that the parking slot can be assigned again*/
{  
    p_place->state = PLACE_STAE_IDLE;
    strcpy(p_place->driver_id, "");
    p_place->time_start = -1;
    return RET_OK;
}

int free_specific_space( )/*this function is used to locate the specific place the worker wants to free */
{
    int  place_type, floor_id, row_id, col_id;
    PARKING_PLACE *p_place;         
    while(1)
    {
        printf("Parking type(1-Car 2-Ebike with charger 3-Ebike no charger):");
        scanf("%d", &place_type );
        switch( place_type )
        {
            case TYPE_PLACE_CAR:
                printf("Floor id(1~10):");
                scanf("%d", &floor_id);
                if( (floor_id<1) || (floor_id>10) ) { printf("incorrect value,try again.\r\n"); continue; }
                printf("Row id(1-15):");
                scanf("%d", &row_id);
                if( (row_id<1) || (row_id>15) ) { printf("incorrect value,try again.\r\n"); continue; }
				printf("Col id(1-15):");
                scanf("%d", &col_id);
                if( (col_id<1) || (col_id>15) ) { printf("incorrect value,try again.\r\n"); continue; }

                p_place = & parking_car[ floor_id-1 ][row_id-1][col_id-1];
                p_place->state = PLACE_STAE_IDLE;
                strcpy(p_place->driver_id, "");
                p_place->time_start = -1;

                printf("free specified car space ok! (floor %d, row %d, col %d )\r\n", floor_id, row_id, col_id );                
                return RET_OK;
                
            case TYPE_PLACE_EBIKE_WITHCHARGER:
                printf("Row id(1-8):");
                scanf("%d", &row_id);
                if( (row_id<1) || (row_id>8) ) { printf("incorrect value,try again.\r\n"); continue; }
				printf("Col id(1-2):");
                scanf("%d", &col_id);
                if( (col_id<1) || (col_id>2) ) { printf("incorrect value,try again.\r\n"); continue; }

                p_place = & parking_ebike_withcharger[row_id-1][col_id-1];
                p_place->state = PLACE_STAE_IDLE;
                strcpy(p_place->driver_id, "");
                p_place->time_start = -1;

                printf("free specified ebike-charger space ok! (row %d, col %d )\r\n", row_id, col_id );               
                return RET_OK;
                
            case TYPE_PLACE_EBIKE_NOCHARGER:
                printf("Row id(1-10):");
                scanf("%d", &row_id);
                if( (row_id<1) || (row_id>10) ) { printf("incorrect value,try again.\r\n"); continue; }
				printf("Col id(1-4):");
                scanf("%d", &col_id);
                if( (col_id<1) || (col_id>4) ) { printf("incorrect value,try again.\r\n"); continue; }

                p_place = & parking_ebike_nocharger[row_id-1][col_id-1];
                p_place->state = PLACE_STAE_IDLE;
                strcpy(p_place->driver_id, "");
                p_place->time_start = -1;

                printf("free specified ebike-no-charger space ok! (row %d, col %d )\r\n", row_id, col_id );                
                return RET_OK;                
            default:
                printf("incorrect, try again.\r\n");
                continue;
        }        
        if( RET_EXIT == wait_for_kbhit() )
        {
            break;
        }
    }
    return RET_OK;
}

int text_space_type( int place_type, char *p_text )/*this function is used to store the type of vehicle*/
{
    if ( NULL == p_text )
    {
        return RET_ERR;
    }
    switch (place_type)
    {
        case TYPE_PLACE_CAR:
            strcpy( p_text, "CAR");
            break;
        case TYPE_PLACE_EBIKE_NOCHARGER:
            strcpy( p_text, "EBIKE-CHARGER");
            break;
        case TYPE_PLACE_EBIKE_WITHCHARGER:
            strcpy( p_text, "EBIKE-NOCHARGER");
            break;
        default:
            return RET_ERR;
    }
    return RET_OK;
}

int get_driver_place( char *driver_id, PARKING_PLACE **p_place)/*this function is used to get which place the specific driver is parking now*/
{
    int i,j,k;
    /* search in car places */
    for (i=0; i<MAX_CAR_FLOORS; i++)
    {
        for (j=0; j<MAX_CAR_ROWS; j++)
        {
            for (k=0; k<MAX_CAR_COLS; k++)
            {
                if (0==strcmp( parking_car[i][j][k].driver_id, driver_id) )/*if we find the driver is parking in the car slot, enter this if*/
                {
                    *p_place = &parking_car[i][j][k];
                    return RET_OK;
                }
            }
        }
    }
    /* search in ebike-c places */
    for (i=0; i<MAX_EBIKE_C_ROWS; i++)
    {
        for (j=0; j<MAX_EBIKE_C_COLS; j++)
        {
            if (0==strcmp( parking_ebike_withcharger[i][j].driver_id, driver_id) )/*if we find the driver is parking in the e-bike with charger slot, enter this if*/
                {
                    *p_place = &parking_ebike_withcharger[i][j];
                    return RET_OK;
                }
        }
    }
    /* search in ebik-nc places */
    for (i=0; i<MAX_EBIKE_NC_ROWS; i++)
    {
        for (j=0; j<MAX_EBIKE_NC_COLS; j++)
        {
            if (0==strcmp( parking_ebike_nocharger[i][j].driver_id, driver_id) )/*if we find the driver is parking in the e-bike without charger slot, enter this if*/
                {
                    *p_place = &parking_ebike_nocharger[i][j];
                    return RET_OK;
                }
        }
    }   
    return RET_DRIVER_NOT_FOUND;   
}

double get_fee_rate( int place_type )/*this function is used to get the fee rate of different type of the parking slot*/
{
    switch( place_type )
    {
        case TYPE_PLACE_CAR:
            return parking_fee_rate.car_rate;
            break;
        case TYPE_PLACE_EBIKE_NOCHARGER:
            return parking_fee_rate.ebike_nocharger_rate;
            break;
        case TYPE_PLACE_EBIKE_WITHCHARGER:
            return parking_fee_rate.ebike_withcharger_rate;
            break;
    }    
    return RET_ERR;
}

int charge_driver( DRIVER_INFO *p_driver )/*this function is used to charge the drivrer when they leave*/
{
    char    text[20];
    int     place_type;
    int     ret_val;
    double  fee;
    double  duration;
    double  left_fee;
    time_t  time_checkin, time_checkout;
    PARKING_PLACE *p_place;
    /* function entry check */
    if (NULL==p_driver)
    {
        return RET_ERR;
    }     
    /* look up driver in place */
    if( RET_OK != get_driver_place( p_driver->id, &p_place) )
    {
        printf("Driver %s has no parking place found!\r\n", p_driver->id );
        return RET_ERR;
    }  
    printf("Driver parking place found!\r\n");    
    /* parking type */    
    text_space_type( p_place->type, text );
    printf("\tYour parking type : %s \r\n", text);       
    /* work out parking time duration */
    time_checkin = p_place->time_start;
    time_checkout = time(NULL);
    duration = (double)difftime(time_checkout, time_checkin );
    printf("\tYour parking start time:  %s", ctime(&time_checkin) );
    printf("\tYour parking end time:    %s", ctime(&time_checkout) );
    printf("\tYour parking time duration: %.2f hours.\r\n", duration/3600);    
    if (duration>72)
    	duration=72;		/*Roof the fee charging.*/    
	fee = get_fee_rate( p_place->type ) * duration/3600;/*calculate the total charge for the user*/ 
    printf("\tYou should pay: %.2f pounds.\r\n", fee);   
    /* update drivers account */
    if ( p_driver->balance < fee )
    {
    	left_fee=fee-p_driver->balance;
    	p_driver->balance==0;
        printf("Your balance is insufficient! Your balance is 0. You should still pay %.2f .\r\n", left_fee );
    }
    /* free space */
    return RET_OK;    
}

int get_driver_info( char *id, DRIVER_INFO **p_driver )
{
    int i;    
    for(i=0; i<MAX_NUM_DRIVERS; i++)
    {
        if( 0==strcmp(id, drivers_info[i].id) )
        {
            *p_driver = &drivers_info[i];
            return RET_OK;
        }
    }
    return RET_DRIVER_NOT_FOUND;
}

int driver_checkin( )	/*This part should be initiatied by the entry/exit worker, to offer the drivers their choice and specify parking regulations*/
{
    DRIVER_INFO     *p_driver;
    PARKING_PLACE   *p_place;   
    char        id[50];
    int         place_type;   
    printf("Now checking in...Please ask the driver for the information.\r\n");
    printf("Please input driver ID:");
    scanf("%s", id);    
    if( RET_OK != get_driver_info( id, &p_driver) )/*if we do not find the user, enter this if*/
    {
        printf("The driver not registed!\r\n");
        return RET_ERR;
    }
    if( RET_OK == get_driver_place(id, &p_place) )
    {
        printf("The driver has checked in!\r\n");
        return RET_ERR;
    }
    printf("Id:%s, Name:%s, Balance:%f \r\n",
        p_driver->id, p_driver->name, p_driver->balance );
    /* ask for place type */
    if( RET_OK != ask_for_place_type( &place_type ) )
    {
        printf("Place type incorrect.\r\n");
        return RET_ERR;
    }
    /* allocate place */
    if( RET_OK != allocate_space_randomly( place_type, p_driver, &p_place) )
    {
        printf("Failed to allocate parking space!\r\n");
        return RET_ERR;
    }
    /* occupy the place */
    p_place->state = PLACE_STATE_OCCUPIED;
    p_place->time_start = time(NULL);
    strcpy( p_place->driver_id, p_driver->id);      
    printf("Check in finished!\r\n");   
    return RET_OK;
}

int driver_checkout()/*this function is used for the user to checkout*/
{
    DRIVER_INFO     *p_driver;
    PARKING_PLACE   *p_place;
    char    id[50];
    int     ret_val;  
    printf("Now checking out...\r\n");
    printf("Please input your ID:\r\n");
    scanf("%s", id);
    /* get driver */
    if( RET_OK != get_driver_info( id, &p_driver) )/*if we do not find the user, enter this if*/
    {
        printf("Get driver info failed!\r\n");
        return RET_ERR;
    }
    printf("Get driver info ok.\r\n");
    /* get place */
    if( RET_OK != get_driver_place( p_driver->id, &p_place) )/*if the user do not have any vehicle parking in the slot, enter this if*/
    {
        printf("Driver %s has no parking place found!\r\n", p_driver->id );
        return RET_ERR;
    }  
    /* charge */
    if (RET_OK != charge_driver( p_driver ) )
    {
        printf("Charging driver failed!\r\n");
        return RET_ERR;
    }
    printf("Charging drive ok.\r\n");
    /* free place */
    if (RET_OK != free_a_space( p_place ) )
    {
        printf("Free driver space failed!\r\n");
        return RET_ERR;
    }  
    printf("Check out finished!\r\n");
    return RET_OK;
}

int view_places_state()/*this function is used to see the parking state of the whole parking slot*/
{
    int     i,j,k;    
    int     num_car_places_all, num_car_places_occupied, num_car_places_idle;
    int     num_ebike_c_places_all, num_ebike_c_places_occupied, num_ebike_c_places_idle;
    int     num_ebike_nc_places_all, num_ebike_nc_places_occupied, num_ebike_nc_places_idle;
    PARKING_PLACE   *p_place;    
    num_car_places_all = MAX_CAR_FLOORS*MAX_CAR_ROWS*MAX_CAR_COLS;
    num_car_places_occupied = 0;
    num_car_places_idle = 0;
    num_ebike_c_places_all = MAX_EBIKE_C_ROWS*MAX_EBIKE_C_COLS;
    num_ebike_c_places_occupied = 0;
    num_ebike_c_places_idle = 0;
    num_ebike_nc_places_all = MAX_EBIKE_NC_ROWS*MAX_EBIKE_NC_COLS;
    num_ebike_nc_places_occupied = 0;
    num_ebike_nc_places_idle = 0;

	printf("Parking list:\r\n");
    printf("\tPlace\t\t\tDriver\t\tTimeCheckin\r\n");
    printf("\t---------------------------------------------------------\r\n");
    for( i=0; i<MAX_CAR_FLOORS; i++ )
    {
        for( j=0; j<MAX_CAR_ROWS; j++ )
        {
            for( k=0; k<MAX_CAR_COLS; k++ )
            {
                p_place = & parking_car[i][j][k];
                if( p_place->state == PLACE_STAE_IDLE )/*to see how many places are empty*/
                {
                    num_car_places_idle ++;
                }
                if( p_place->state == PLACE_STATE_OCCUPIED )/*to see how many places are occupied*/
                {
                    num_car_places_occupied ++;
                    
                    printf("\tCar F-%02d R-%02d C-%02d\t\t%s\t\t%s", 
                        i+1, j+1, k+1,
                        p_place->driver_id,
                        ctime(&p_place->time_start) );
                }
            }
        }
    }

    /* places ebike-c */
    for( i=0; i<MAX_EBIKE_C_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_C_COLS; j++ )
        {
            p_place = & parking_ebike_withcharger[i][j];
            if( p_place->state == PLACE_STAE_IDLE )
            {
                num_ebike_c_places_idle ++;
               
            }
            if( p_place->state == PLACE_STATE_OCCUPIED )
            {
                num_ebike_c_places_occupied ++;
                printf("\tEbike-C R-%02d C-%02d\t\t%s\t\t%s\r\n", 
                    i+1, j+1, 
                    p_place->driver_id,
                    ctime(&p_place->time_start) );
             
            }
        }
    }

    /* places ebike-nc */
    for( i=0; i<MAX_EBIKE_NC_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_NC_COLS; j++ )
        {
            p_place = & parking_ebike_nocharger[i][j];
            if( p_place->state == PLACE_STAE_IDLE )
            {
                num_ebike_nc_places_idle ++;
            }
            if( p_place->state == PLACE_STATE_OCCUPIED )
            {
                num_ebike_nc_places_occupied ++;
                printf("\tEbike-NC R-%02d C-%02d\t\t%s\t\t%s", 
                    i+1, j+1, 
                    p_place->driver_id,
                    ctime(&p_place->time_start) );
            }
        }
    }

    /* format print */

    printf("\r\nParking places state:\r\n");    
    printf("\tType    \t\tTotal    \tOccupied \tIdle    \r\n");
    printf("\t-----------------------------------------------------------------\r\n");
    printf("\tCar     \t\t%-5d     \t%-10d     \t%-10d     \r\n", 
        num_car_places_all, num_car_places_occupied, num_car_places_idle );
    printf("\tEbike-C \t\t%-5d     \t%-10d     \t%-10d     \r\n", 
        num_ebike_c_places_all, num_ebike_c_places_occupied, num_ebike_c_places_idle);
    printf("\tEbike-NC \t\t%-5d    \t%-10d     \t%-10d \r\n", 
        num_ebike_nc_places_all, num_ebike_nc_places_occupied, num_ebike_nc_places_idle);       
    return RET_OK;
}

/*******************************************************************************
***********************  MODULE menu  ******************************************
********************************************************************************/
#define MENU_MAIN_HEAD      "Welcome to Parking system!\r\nIdentify your role please.\r\n\r\n"
#define MENU_MAIN_ITEM_1    "\t 1-System Administrator \r\n"
#define MENU_MAIN_ITEM_2    "\t 2-Entry/Exit Worker \r\n"
#define MENU_MAIN_EXIT      "\t 3-exit \r\n"
#define MENU_MAIN_OPTION    "\r\n\t Your option:"
#define MENU_MAIN_ID_MIN    1
#define MENU_MAIN_ID_MAX    3

#define MENU_ADMIN_HEAD     "Welcome to System Administration!\r\n\n"
#define MENU_ADMIN_ITEM_1   "\t 1-add driver info \r\n"                     /*these are used in the main interface*/
#define MENU_ADMIN_ITEM_2   "\t 2-view driver info  \r\n"
#define MENU_ADMIN_ITEM_3   "\t 3-remove driver \r\n"
#define MENU_ADMIN_ITEM_4   "\t 4-credit driver \r\n"
#define MENU_ADMIN_ITEM_5   "\t 5-set parking fee rate \r\n"
#define MENU_ADMIN_ITEM_6   "\t 6-view parking fee rate \r\n"
#define MENU_ADMIN_EXIT     "\t 7-exit \r\n"
#define MENU_ADMIN_OPTION   "\r\n\t Your option:"
#define MENU_ADMIN_ID_MIN    1
#define MENU_ADMIN_ID_MAX    7

#define MENU_WORKER_HEAD     "Welcome to Enty/Exit Working!\r\n\n"
#define MENU_WORKER_ITEM_1   "\t 1-check in \r\n"
#define MENU_WORKER_ITEM_2   "\t 2-view parking spaces state \r\n"
#define MENU_WORKER_ITEM_3   "\t 3-check out \r\n"
#define MENU_WORKER_ITEM_4   "\t 4-free a parking space \r\n"
#define MENU_WORKER_EXIT     "\t 5-exit \r\n"
#define MENU_WORKER_OPTION   "\r\n\t Your option:"
#define MENU_WORKER_ID_MIN    1
#define MENU_WORKER_ID_MAX    5

int print_menu_main()
{
    system("cls");
    printf( MENU_MAIN_HEAD );
    printf( MENU_MAIN_ITEM_1 );
    printf( MENU_MAIN_ITEM_2 );
    printf( MENU_MAIN_EXIT );
    printf( MENU_MAIN_OPTION );
    
    return RET_OK;
}

int print_menu_admin()
{
    system("cls");
    printf( MENU_ADMIN_HEAD );
    printf( MENU_ADMIN_ITEM_1 );
    printf( MENU_ADMIN_ITEM_2 );
    printf( MENU_ADMIN_ITEM_3 );
    printf( MENU_ADMIN_ITEM_4 );
    printf( MENU_ADMIN_ITEM_5 );
    printf( MENU_ADMIN_ITEM_6 );
    printf( MENU_ADMIN_EXIT );
    printf( MENU_ADMIN_OPTION );

    return RET_OK;
}

int print_menu_worker()
{
    system("cls");
    printf( MENU_WORKER_HEAD );
    printf( MENU_WORKER_ITEM_1 );
    printf( MENU_WORKER_ITEM_2 );
    printf( MENU_WORKER_ITEM_3 );
    printf( MENU_WORKER_ITEM_4 );
    printf( MENU_WORKER_EXIT );
    printf( MENU_WORKER_OPTION );

    return RET_OK;
}

/******************************************************************************
******************************************************************************/
int get_menu_input(int num_min, int num_max)
{
    int item_num,b;
    while (1) {
        b = scanf("%d", &item_num);;    /*Receive user input data*/
        if (b == 1)        /*When input data is valid*/
        {
            if ((item_num ==1)||(item_num==2)||(item_num==3)||(item_num==4)||(item_num==5)||(item_num==6)||(item_num==7)){/*make sure the user enters the right thing*/
                return item_num;  
            }
            else {
                printf("There is an error in the input data, please re-enter:");
                fflush(stdin); 
            }
        }
        else
        {
        printf("You must input right number \n" );
        fflush(stdin); 
        }
    }
    return -1;
}

int menu_main_proc()
{
    int ret_val;
    int menu_item_id;

    /* main menu */
    print_menu_main();

    /* get menu input */
    menu_item_id = get_menu_input( MENU_MAIN_ID_MIN, MENU_MAIN_ID_MAX );
    if ( menu_item_id < 0 )
    {
        return RET_ERR;
    }

    /* menu item process */
    switch ( menu_item_id )
    {
        case 1:         /* process for Administrator */
            menu_admin_proc();
            break;
            
        case 2:         /* process for worker */
            menu_worker_proc();
            break;
 
        case 3:         /* exit */
            write_data_to_file();
            return RET_EXIT;           
        default:
            break;
    }
    return RET_OK;
}

int menu_admin_proc()
{
    int ret_val;   
	int menu_item_id;   
	
    while(1)
    {
        print_menu_admin();

        menu_item_id = get_menu_input( MENU_ADMIN_ID_MIN, MENU_ADMIN_ID_MAX);
        if ( menu_item_id < 0 )
        {
            return RET_ERR;
        }

        switch( menu_item_id )/*the switch case in the administrator interface*/
        {
            case 1:
                add_driver_info();
                break;
                
            case 2:
                
                view_all_drivers_info();
                break;
                
            case 3:
                rmv_driver_info();
                break;
                
            case 4:
                credit_driver_account();
                break;
                
            case 5:
                set_parking_feerate();
                break;
            case 6:
                view_parking_feerate();
                break;
            case 7:
                return RET_EXIT;
                break;
            default:
                return RET_ERR;
        }

        ret_val = wait_for_kbhit();
        if ( RET_EXIT == ret_val )
        {
            break;
        }
    }
    
    return RET_OK;
}

int menu_worker_proc()/*the function used in the interface of the worker*/
{
    int ret_val;

    while(1)
    {
        print_menu_worker();

        ret_val = get_menu_input( MENU_WORKER_ID_MIN, MENU_WORKER_ID_MAX);
        switch (ret_val)
        {
            case 1:
                driver_checkin();
                break;
            case 2:
                view_places_state();                
                break;
            case 3:
                driver_checkout();
                break;
            case 4:                
                free_specific_space();
                break;
            case 5:
                write_data_to_file();
                return RET_OK;
                break;
            default:
                return RET_ERR;
                
        }
        ret_val = wait_for_kbhit();
        if ( ret_val == RET_EXIT )
        {
            break;
        }        
    }    
    return RET_OK;
}

/*******************************************************************************
***********************  MODULE main  ******************************************
********************************************************************************/

int read_data_from_file()/*this function is used to read the data from the file*/
{
	int		i,j,k;	
    FILE 	*fp;

    PARKING_FEE_RATE    *p_fee_rate;
    DRIVER_INFO         *p_driver;
    PARKING_PLACE       *P_place;

    fp = fopen("parking.dat", "r");
    if (NULL == fp)/*check if the file is opened successfully*/
    {
        printf("File not exist, will use default value.\r\n");
        return;
    }

    /* read system data */
    printf("reading parking_fee_rate data...");
    if( !feof(fp) )
    {
        if( 0 == fread(&parking_fee_rate, sizeof(PARKING_FEE_RATE), 1, fp ) )
        {
            printf("error when reading parking_fee_rate!\r\n");
        }
    }
    printf("ok\r\n");
    
    /* read drivers info */
    printf("reading drivers_info data...");
    for( i=0; i<MAX_NUM_DRIVERS; i++ )
    {
        if( !feof(fp) )
        {
            if( 0 == fread(&drivers_info[i], sizeof(DRIVER_INFO), 1, fp ) )
            {
                printf("error when reading data drivers_info from file!\r\n");
            }
        }
    }
    printf("ok\r\n");
           
    /* read car places data */
    printf("reading parking_car data...");
    for( i=0; i<MAX_CAR_FLOORS; i++ )
    {
        for( j=0; j<MAX_CAR_ROWS; j++ )
        {
            for( k=0; k<MAX_CAR_COLS; k++)
            {
                 if( !feof(fp) )
                {
                    if( 0 == fread(&parking_car[i][j][k], sizeof(PARKING_PLACE), 1, fp ) )
                    {
                        printf("error when read data parking_car from file!\r\n");
                    }
                }
            }
        }
    }
    printf("ok\r\n");
    
    /* read ebike with charger places data */
    printf("reading ebike_withcharger data...");
    for( i=0; i<MAX_EBIKE_C_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_C_COLS; j++ )
        {
            if( !feof(fp) )
            {
                if( 0 == fread(&parking_ebike_withcharger[i][j], sizeof(PARKING_PLACE), 1, fp ) )
                {
                    printf("error when read data parking_car from file!\r\n");
                }
            }
        }
    }
    printf("ok\r\n");

    /* read ebike without charger places data */
    printf("reading ebike_nocharger data...");
    for( i=0; i<MAX_EBIKE_NC_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_NC_COLS; j++ )
        {
            if( !feof(fp) )
            {
                if( 0 == fread(&parking_ebike_nocharger[i][j], sizeof(PARKING_PLACE), 1, fp ) )
                {
                    printf("error when read data parking_car from file!\r\n");
                }
            }
        }
    }
    /* close file */
    fclose(fp);   
    printf("ok\r\n");   
}

int write_data_to_file()
{
    FILE *fp;   
    int	i,j,k;
    PARKING_FEE_RATE    *p_fee_rate;
    DRIVER_INFO         *p_driver;
    PARKING_PLACE       *P_place;
    fp = fopen("parking.dat", "w");
    if (NULL == fp)
    {
        printf("File not exist,ignored!\r\n");
        return;
    }

    /* read system data */
    printf("Saving data to file...");
	if( !feof(fp) )/*check if the file is opened successfully*/
    {
        if( 0 == fwrite(&parking_fee_rate, sizeof(PARKING_FEE_RATE), 1, fp ) )
        {
            printf("error when writing parking_fee_rate!\r\n");
        }
    }
    
    /* read drivers info */
    for( i=0; i<MAX_NUM_DRIVERS; i++ )
    {
        if( !feof(fp) )
        {
            if( 0 == fwrite(&drivers_info[i], sizeof(DRIVER_INFO), 1, fp ) )
            {
                printf("error when writing data drivers_info to file!\r\n");
            }
        }
    }
   
        
    /* read car places data */
    for( i=0; i<MAX_CAR_FLOORS; i++ )
    {
        for( j=0; j<MAX_CAR_ROWS; j++ )
        {
            for( k=0; k<MAX_CAR_COLS; k++)
            {
                 if( !feof(fp) )
                {
                    if( 0 == fwrite(&parking_car[i][j][k], sizeof(PARKING_PLACE), 1, fp ) )
                    {
                        printf("error when writing data parking_car to file!\r\n");
                    }
                }
            }
        }
    }
    
    /* read ebike with charger places data */
    for( i=0; i<MAX_EBIKE_C_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_C_COLS; j++ )
        {
            if( !feof(fp) )
            {
                if( 0 == fwrite(&parking_ebike_withcharger[i][j], sizeof(PARKING_PLACE), 1, fp ) )
                {
                    printf("error when writing data parking_car to file!\r\n");
                }
            }
        }
    }

    /* read ebike without charger places data */
    for( i=0; i<MAX_EBIKE_NC_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_NC_COLS; j++ )
        {
            if( !feof(fp) )
            {
                if( 0 == fwrite(&parking_ebike_nocharger[i][j], sizeof(PARKING_PLACE), 1, fp ) )
                {
                    printf("error when writing data parking_car to file!\r\n");
                }
            }
        }
    }
    /* close file */
    fclose(fp);
    printf("Done.\r\n");
}

int init_system()
{
    int i,j,k;   
    PARKING_PLACE   *p_place;
    
    /***************** initiate data variables ************************/
    /* places for car */   
    for( i=0; i<MAX_CAR_FLOORS; i++ )
    {
        for( j=0; j<MAX_CAR_ROWS; j++ )
        {
            for( k=0; k<MAX_CAR_COLS; k++ )
            {
                p_place = &parking_car[i][j][k];
                p_place->id = -1;
                p_place->type = TYPE_PLACE_CAR;
                p_place->state = PLACE_STAE_IDLE;

                strcpy( p_place->driver_id, "");
                sprintf(p_place->name, "CAR F-%02d R-%02d C-%02d", i+1, j+1, k+1);
                p_place->time_start = 0;                
            }
        }
    }
    /* places for ebike without charger */
    for( i=0; i<MAX_EBIKE_NC_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_NC_COLS; j++ )
        {
                p_place = &parking_ebike_nocharger[i][j];
                p_place->id = -1;
                p_place->type = TYPE_PLACE_EBIKE_NOCHARGER;
                p_place->state = PLACE_STAE_IDLE;

                strcpy( p_place->driver_id, "");
                sprintf(p_place->name, "EBIKE-NC R-%02d C-%02d", i+1, j+1 );
                p_place->time_start = 0;                
         }
    }

    /* places for ebike with charger */
    for( i=0; i<MAX_EBIKE_C_ROWS; i++ )
    {
        for( j=0; j<MAX_EBIKE_C_COLS; j++ )
        {
                p_place = &parking_ebike_withcharger[i][j];
                p_place->id = -1;
                p_place->type = TYPE_PLACE_EBIKE_WITHCHARGER;
                p_place->state = PLACE_STAE_IDLE;

                strcpy( p_place->driver_id, "");
                sprintf(p_place->name, "EBIKE-C R-%02d C-%02d", i+1, j+1 );
                p_place->time_start = 0;                
         }
    }
    printf("init vaiables finished.\r\n");   
    read_data_from_file();  /* read file */  
}

int main()
{
    int ret_val;
    printf("Welcome to Parking system!\r\n\r\n");   
    printf("Wait for system initializing...\r\n");   
    init_system();/*initiate the system first*/
    printf("initialize ok!\r\n");
    /* loop to deal with user commands */
    while(1)
    {
        ret_val = menu_main_proc();
        if ( ret_val == RET_EXIT )
        {
            break;
        }
    }
    write_data_to_file(); /* save to file */
    printf("\r\nThank you for using Parking system!\r\n");/* say goodbye */
    return RET_OK;
}
