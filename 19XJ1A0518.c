#include<stdio.h>
#include<stdlib.h>

int num = 0;
int cmp_avail_need(int * arr, int *avail, int n)
{
	int count = 0;
	for(int i = 0; i < n; i++)
	{
		if(avail[i] >= arr[i]) 
		count ++;
	}
	if(count == n) return 1;
	else return 0;
}

void add_array(int *arr, int *avail, int n) 
{
	for(int i = 0; i < n; i++)
	{
		avail[i] += arr[i];
	}
}

void printArray(int **arr,int n, int m) {
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{	
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
}

void printSeq(int *arr, int n) {
	
	for(int i = 0; i < n; i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

void get_need_matrix(int **need, int **max, int **alloc, int *sum, int n, int m)
{
	for(int i = 0; i < n; i++) 
	{
		for(int j = 0; j < m; j++) 
		{
			sum[j] += alloc[i][j];
			need[i][j] = max[i][j] - alloc[i][j];
		}
		
	}
}

void get_matrix_input(FILE *input, int **matrix, int n, int m) 
{
	for(int i = 0; i < n; i++) 
	{
		for(int j = 0; j < m; j++) 
		{
			fscanf(input,"%d",&matrix[i][j]);
		}
	}
}

int check_finish(int *finish, int n)
{
	int count = 0;
	for(int i = 0; i < n; i++)
	{
		if(finish[i] == 1) count ++;
	}
	if(count == n) return 1;
	else return 0;

}

void check_safe_seq(int **need, int **alloc, int **initial_alloc, int *init_avail, int *avail, int *finish, int *seq, int n, int m, int check) 
{

	for(int i = 0; i < n; i++)
	{
		if(cmp_avail_need(need[seq[i]],avail,m) == 1 && finish[seq[i]] != 1) {
			finish[seq[i]] = 1;
			add_array(alloc[seq[i]],avail,m);
		}
	}
	
	
	if(check_finish(finish,n) == 1 && check == 0) 
	{
		//printSeq(seq , n);
		num++;
	}
	
	if(check_finish(finish,n) == 1 && check == 1) 
	{
		printSeq(seq , n);
	}
	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			alloc[i][j] = initial_alloc[i][j];
		}
		finish[i] = 0;
		avail[i] = init_avail[i];
	}
	
}


// functions used to generate permutations
int compare(const void *a, const void * b)
{
	return (*(int*)a - *(int*)b);
}


void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int findCeil (int* seq, char first, int l, int h)
{
    // initialize index of ceiling element
    int ceilIndex = l;
 
    // Now iterate through rest of the elements and find
    // the smallest character greater than 'first'
    for (int i = l+1; i <= h; i++)
    if (seq[i] > first && seq[i] < seq[ceilIndex])
            ceilIndex = i;
 
    return ceilIndex;
}

void generate_permutations(int *seq, int start, int end, int **need, int **alloc, int **initial_alloc,int *init_avail, int *avail, int *finish, int m, int check)  
{
	qsort(seq, end, sizeof(int),compare);
	
	int isFinished = 0;
	while(isFinished != 1)
	{
		check_safe_seq(need, alloc, initial_alloc, init_avail, avail, finish, seq, end, m, check);
		
		
		int i;
		for ( i = end - 2; i >= 0; --i )
		{
			if (seq[i] < seq[i+1])
			{
				break;
			}
		}
		
		if(i == -1) isFinished = 1;
		
		else 
		{
			int ceil = findCeil(seq, seq[i], i+1, end-1);
			swap(&seq[i], &seq[ceil]);
			qsort(seq+i+1, end-i-1, sizeof(int), compare);
		}
	}
}




///////////////////// Main
int main(void) {
	
	
	FILE *input;
	int num_res;
	int num_procs;
    input = fopen("input.txt","r");
    
    // Taking input
    fscanf(input,"%d",&num_res);
    fscanf(input,"%d",&num_procs);
	
	int *tot_res;
	int *avail_res;
	int *init_avail;
	int *sum;
	int *finish;
	int *initial;
	
	
	// Memeory allocation for all 1D arrays
	tot_res = (int*)malloc(num_res*sizeof(int));
	avail_res = (int*)malloc(num_res*sizeof(int));	
	sum = (int*)malloc(num_res*sizeof(int));
	finish = (int*)malloc(num_procs*sizeof(int));
	initial = (int*)malloc(num_procs*sizeof(int));
	init_avail = (int*)malloc(num_procs*sizeof(int));
	
	for(int i = 0; i < num_res; i++)
	{
		fscanf(input,"%d",&tot_res[i]);
		avail_res[i] = 0;
		sum[i] = 0;
	}
	
	
	/// Memory allocation for all matrices
	int **max_matrix;
	int **alloc_matrix;
	int **need_matrix;
	int **initial_alloc;
	
	max_matrix = (int**)malloc(num_procs*sizeof(int*));
	alloc_matrix = (int**)malloc(num_procs*sizeof(int*));
	initial_alloc = (int**)malloc(num_procs*sizeof(int*));
	need_matrix = (int**)malloc(num_procs*sizeof(int*));
	
	for(int i = 0; i < num_procs; i++) 
	{
		max_matrix[i] = (int*)malloc(num_res*sizeof(int));
		alloc_matrix[i] = (int*)malloc(num_res*sizeof(int));
		need_matrix[i] = (int*)malloc(num_res*sizeof(int));
		initial_alloc[i] = (int*)malloc(num_res*sizeof(int));
		finish[i] = 0;
		initial[i] = i;
	}

	// All memory allocated
	
	
	get_matrix_input(input, max_matrix, num_procs, num_res);
	get_matrix_input(input, alloc_matrix, num_procs, num_res);

	for(int i = 0; i < num_procs; i++)
	{
		for(int j = 0; j < num_res; j++)
		{
			initial_alloc[i][j] = alloc_matrix[i][j];
		}
	}

	get_need_matrix(need_matrix,max_matrix,alloc_matrix,sum,num_procs,num_res);

	for(int i = 0; i < num_res; i++) avail_res[i] = tot_res[i] - sum[i];
	for(int i = 0; i < num_res; i++) init_avail[i] = avail_res[i];
	
	int check = 0;
	
	// Used to get number of safe sequences
	generate_permutations(initial,0,num_procs,need_matrix, alloc_matrix, initial_alloc, init_avail, avail_res, finish,	 num_res, check);
	printf("%d\n",num);
	
	// Used to print all safe sequences in lexographic order
	check = 1;
	generate_permutations(initial,0,num_procs,need_matrix, alloc_matrix, initial_alloc, init_avail, avail_res, finish,	 num_res, check);
	return 0;    
}
