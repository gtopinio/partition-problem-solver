/*
    CMSC 142 Project - EF5L
    The Partition Problem Solver
    Group Members:
    1. Rodriguez, Paulo Rodriguez
    2. Topinio, Mark Genesis

    Description:
    This program solves the partition problem using the brute force method. The partition problem is a problem
    where you are given a set of integers and you are tasked to find a partitioning of the set into two subsets
    such that the sum of the integers in the first subset is equal to the sum of the integers in the second subset.
    If there are no such partitionings, then the program should produce a message saying so.
*/

// NOTE: Must have the "input.txt" file in the same directory as this file. Please don't leave a new line for at the end of the file or else it won't work

// ================================= FILE AND SET INITIALIZATION FUNCTIONS =================================
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

// Function for finding the number of sets from the file
int get_num_of_sets(FILE* file){
    int number_of_sets = 0;
    file = fopen("input.txt", "r");
    fscanf(file, "%d", &number_of_sets);
    fclose(file);
    return number_of_sets;
}

// Function for finding the number of integers in the current set
int get_num_of_ints_curr_set(FILE* file, int multiplier, int current_set_num, int total_num_of_sets){
    
    int number_of_integers_in_current_set = 0;
    int temp;
    file = fopen("input.txt", "r");
    char charTemp[200];
    int counter = 0;
    // Skip the characters before the current set
    for(int i = 0; i < multiplier; i++){
        fgets(charTemp, 200, file);
        // fscanf(file, "%d", &temp); // keeps scanning until the counter reaches the multiplier
        // if(counter == multiplier){
            // break; // if we break, that means we are at the beginning of the current set that we want
        
    }
    if(current_set_num+1 == total_num_of_sets){ // if the current set is the last set, count the integers until EOF
        do{
            fscanf(file, "%d", &temp);
            number_of_integers_in_current_set++;
        } while(fgetc(file) != EOF);
        fclose(file);
        return number_of_integers_in_current_set;
    } else { // if the current set is not the last set, count the integers until the next line
        
        // This code segment is for Linux
        char delim[2] = " ";
        fgets(charTemp, 200, file);
        char *num;
        num = strtok(charTemp, delim);

        while(num != NULL){
            number_of_integers_in_current_set++;
            num = strtok(NULL, delim);
        } 

        // Code segment below only works in Windows
        // char ch = fgetc(file);
        // do{
        //     fscanf(file, "%d", &temp);
        //     printf("%cHi", ch);

        //     number_of_integers_in_current_set++;
        // } while(ch != '\n');
        fclose(file);
        return number_of_integers_in_current_set;
    }
}

// Function for initializing the current set
int* initialize_curr_set(FILE* file, int number_of_integers_in_current_set, int multiplier){

    int* curr_set_integers;
    char charTemp[200];
    int temp;
    file = fopen("input.txt", "r");
    
    int counter = 0;
    // Skip the characters before the current set
    for(int i = 0; i < multiplier; i++){
        fgets(charTemp, 200, file);
        // fscanf(file, "%d", &temp);
        // if(counter == multiplier){
        //     break;
        // }
    }

    // Allocate memory for the current set
    curr_set_integers = (int*) malloc(number_of_integers_in_current_set * sizeof(int));
    // Read the integers in the current set
    for(int i = 0; i < number_of_integers_in_current_set; i++){
        fscanf(file, "%d", &curr_set_integers[i]);
    }
    fclose(file);
    return curr_set_integers;
}

// Function for bubble sorting the set in increasing order
void bubble_sort(int* set, int size){
    for(int i=0;i<size-1; i++){
        for(int j = i+1;j<size;j++){
            if(set[i]>set[j]){
                int temp = set[i];
                set[i] = set[j];
                set[j] = temp;
            }
        }
    }
}

// Function for solving the partition problem. It uses backtracking to find the solution and prints the solution. It also prints the time it took to solve the problem.
int partition(int* set, int size){
    clock_t start_time, end_time; // variables for the timer
    int partition_counter = 0; // counter for the number of partitionings

    //checks if the set can be partitioned
    int set_sum = 0;
    for(int i = 0; i < size; i++) set_sum += set[i];
    if(set_sum % 2 == 1){ // if the sum of the set is odd, then it cannot be partitioned
        return 0;
    }

    int *subset = (int*) malloc(size * sizeof(int));
	int start, move;
	int *nopts = (int*) malloc((size+2) * sizeof(int)); //array of top of stacks
	int **option = (int**) malloc((size+2) * sizeof(int*)); //array of stacks of options
    for(int i =0; i <size+2; i++) option[i] = (int*) malloc((size+2) * sizeof(int));
	int i, j, candidate;

	move = start = 0; 
	nopts[0]= 1;

    // Start the timer
    start_time = clock();
	
	while (nopts[start] >0) //while dummy stack is not empty
	{
		if(nopts[move]>0)
		{
			move++;
			nopts[move]=0; //initialize new move
			if(move==size+1) continue;
			else if(move == 1){
				for(i = size; i>=1; i--) 
				{
                    candidate = set[i-1];
					nopts[move]++;
					option[move][nopts[move]] = candidate;
				}
			}
			else{
				for(i=size;i>=1;i--)
				{
                    candidate = set[i-1];
					for(j=move-1;j>=1;j--)
						if(candidate<=option[j][nopts[j]]) break;
					if(!(j>=1))
						option[move][++nopts[move]] = candidate;
				}
			}
		}
		else
		{
            int subset_sum = 0, subset_size=0;
            for(i=1;i<move;i++){
				if(i!= 1 && option[i][nopts[i]] < option[i-1][nopts[i-1]]) continue;
                subset[subset_size++] = option[i][nopts[i]];
                subset_sum += subset[subset_size-1];
            }

            if((set_sum / 2) == subset_sum){ // if the sum of the subset is equal to the sum of the set divided by 2, then we have found a partitioning
                partition_counter++; // increase the counter

                printf("{ ");
                for(i = 0;i < subset_size; i++) printf("%d ",subset[i]);
                printf("} { ");
                for(i = 0; i < size;i++) {
                    for(j = 0; j < subset_size;j++){
                        if(set[i] == subset[j]) break;
                    }
                    if(j == subset_size) printf("%d ", set[i]);
                }
                printf("}\n");
            }

			move--;
			nopts[move]--;
            if(move == 1) break;
		}
	}

    // Stop the timer
    end_time = clock();
    printf("Running time: %f\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    // Free the allocated memory
    free(subset);
    free(nopts);
    for(i=0;i<size+2;i++) free(option[i]);
    free(option);

    return partition_counter;
}

int main(){
    int number_of_sets = 0;                    // number of sets in the input file
    int set_size = 0;                          // number of integers in the current set
    int** sets;                                // array of sets
    int counter = 1;                           // for reading lines
    int partition_counter = 0;                 // for counting the number of partitions
    int current_set_num = 0;                   // for knwoing the current set
    

    // start timer using 
    FILE* input_file;                          // input file pointer
    
    printf("\n============== Partition Problem Solver ==============\n");

    // First we get the number of sets using the get_num_of_sets function
    number_of_sets = get_num_of_sets(input_file);
    printf("Number of sets: %d\n", number_of_sets);
    sets = (int**) malloc (number_of_sets*sizeof(int*));

    for(int i = 0; i<number_of_sets; i++){
        printf("\n================== Set %d ==================\n", i+1);
        // Then we get the number of integers in the current set using the get_num_of_ints_curr_set function
        set_size = get_num_of_ints_curr_set(input_file, counter, current_set_num, number_of_sets);
        printf("Number of integers in current set: %d\n", set_size);

        // Then we get the integers in the current set using the initialize_curr_set function
        sets[i] = initialize_curr_set(input_file, set_size, counter);

        // Printing the integers in the current set
        printf("Current set: ");
        for(int j=0; j<set_size; j++) printf("%d ", sets[i][j]);
        printf("\n");

        // Sorting the integers in the current set
        bubble_sort(sets[i], set_size);
        printf("Sorted set: ");
        for(int j=0; j<set_size; j++) printf("%d ", sets[i][j]);
        
        printf("\n================ Solving... ================\n");
        partition_counter = partition(sets[i],set_size);        // Solving the partition problem for the current set
        if(partition_counter == 0) printf("The set has no valid partitions\n");
        else printf("Number of partitions: %d\n", partition_counter);

        free(sets[i]);
        counter ++;                // for reading lines in the file
        current_set_num++;                  // for knowing the current set
    }
    
    free(sets);
    return 0;
}