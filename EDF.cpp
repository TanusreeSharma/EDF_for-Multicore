#include<stdio.h>
#include<string.h>
#include <stdlib.h>

int processesNumber;
int ProcessesExecutionTime[1000];
int ProcessesDeadline[1000];
int ProcessesPeriode[1000];
int PeriodsLCM;

char help[255];
int i,j,h;
float f;

void getProcesses()
{
	FILE *InputFile;
	InputFile = fopen("input.txt", "r");
	fgets(help, 255, InputFile);
	processesNumber = atoi(help);
	// getting processes from the file input.txt;
	for(i = 0; i<processesNumber; i++)
	{
		fgets(help, 255, InputFile);
		ProcessesExecutionTime[i]= atoi(help);
		
		fgets(help, 255, InputFile);
		ProcessesDeadline[i]= atoi(help);
		
		fgets(help, 255, InputFile);
		ProcessesPeriode[i]= atoi(help);
	}
	fclose(InputFile);
	// showing the processes;
	printf("Pi| Ci, Di, pi\n");
	printf("----------------\n");
	for(i=0; i<processesNumber ; i++)
	{
		printf("P%d | %d, %d, %d\n",i,ProcessesExecutionTime[i], ProcessesDeadline[i], ProcessesPeriode[i]);
	}
	
	
}
void getPeriodsLCM()
{
	int help[1000];
	for(i=0 ; i<processesNumber; i++)
	{
		help[i] = ProcessesPeriode[i];
	}
	int h = 0;
	int theLowestValueInHelp;
	int theLowestValueIndex;
	while(h == 0)
	{
		h = 1;
		theLowestValueInHelp = help[0];
		theLowestValueIndex = 0;
		
		for(i=0 ; i<processesNumber; i++)
		{
			if(theLowestValueInHelp != help[i])
			{
				h = 0;
				if(theLowestValueInHelp>help[i])
				{
					theLowestValueInHelp = help[i];
					theLowestValueIndex = i;
					
				}
				
			}	
		}
		if(h == 0)
		{
			help[theLowestValueIndex] = theLowestValueInHelp + ProcessesPeriode[theLowestValueIndex];
		}	
	}
	PeriodsLCM = help[0];
	printf("We Can Schedule That Real System In %d Units(%d is the Lcm of our processes periods)",PeriodsLCM,PeriodsLCM);
	
}
float schedulability()
{
	float Condition = 0;
	float x,y;
	printf("\n The Sum Of :");
	for(i=0 ; i<processesNumber ; i++)
	{
		x = ProcessesExecutionTime[i];
		y = ProcessesPeriode[i];
		Condition = Condition + (x/y);
		printf(" (%d/%d) ",ProcessesExecutionTime[i],ProcessesPeriode[i]);
	}
	printf("is equal : %f", Condition);
	return Condition;
}

void schedule()
{
	int earliestDeadline;
	int earliestDeadlineIndex;
	int schedulingTable[PeriodsLCM];
	int remainsCapacity[1000];
	int nextDeadline[1000];
	int processOnlineNewPeriod[1000];
	for(i=0 ; i<processesNumber ; i++)
	{
		nextDeadline[i] = ProcessesDeadline[i];
		remainsCapacity[i] = ProcessesExecutionTime[i];
		processOnlineNewPeriod[i] = 0;
	}
	// scheduling time milestone...
	FILE *OutputtFile;
	OutputtFile = fopen("output.txt", "w");
	fprintf(OutputtFile, "Periods LCM = %d\n",PeriodsLCM);
	for(i=0; i<PeriodsLCM; i++)
	{
		printf("\n(%d,%d) : ",i,i+1);
			
			//getting the earliest deadline
			earliestDeadline = PeriodsLCM;
			earliestDeadlineIndex = -1;
			for(j=0 ; j<processesNumber ; j++)
			{
				 if(remainsCapacity[j] > 0)
				 {
				 	
				 	if(earliestDeadline > nextDeadline[j])
				 	{
				 		
				 		earliestDeadline = nextDeadline[j];
				 		earliestDeadlineIndex = j;
					 }
				 }
			}
			printf("     [exc = %d] ",earliestDeadlineIndex);
			fprintf(OutputtFile, "(%d,%d) :   [exc = %d]\n",i,i+1,earliestDeadlineIndex);
			remainsCapacity[earliestDeadlineIndex]--;
			
			//get the next deadline distance	
			for(j=0 ; j<processesNumber ; j++)
			{
				
				 if(processOnlineNewPeriod[j] == (ProcessesPeriode[j] - 1 ) )
				 {
				 	nextDeadline[j] =  ProcessesDeadline[j];
				 	remainsCapacity[j] = ProcessesExecutionTime[j];
				 	processOnlineNewPeriod[j] = 0;
				 }
				 else
				 {
				 	if(nextDeadline[j] >0)
				 	{
				 		nextDeadline[j]--;
					 }
					 else
					 {
					 	if(remainsCapacity[j] > 0)
					 	{
					 		printf("\nthe process %d has no chance to complete its Capacity",j);
					 		fprintf(OutputtFile, "\nthe process %d has no chance to complete its Capacity",j);
						 }
					 }
				 	 processOnlineNewPeriod[j]++; 
				 }
				 
				 
			}
	}
	fclose(OutputtFile);


}

main()
{
	
	
	printf("EDF Programme \n");
	for(i=0; i<1024000; i++);
	printf("done \n");
	
	
	printf("\n\nOur System :\n");
	getProcesses();
	
	
	
	printf("\n\nPeriods LCM :\n");
	getPeriodsLCM();
	
	
	
	printf("\n\nSchedulability Test :\n");
	f = schedulability();
	if(f <= 1)
	{
		printf("\n This Real Time System Is Schedulable because %f <= 1", f);
		schedule();
	}
	else
	{
		printf("\nThis Real Time System Is Not Schedulable because %f > 1",f);
		FILE *OutputtFile;
		OutputtFile = fopen("output.txt", "w");
		fprintf(OutputtFile, "This Real Time System Is Not Schedulable because %f > 1",f);
		fclose(OutputtFile);
		exit(0);
	}
}
