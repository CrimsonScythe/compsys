#include "io.c"
#include "alloc.c" 

inline void merge_run(long size_a, long size_b, long from[], long to[]) {
  long* p_a = from;
  long* p_a_limit = from + size_a;
  long* p_b = p_a_limit;
  long* p_b_limit = p_a_limit + size_b;
  long* target = to;
  long* target_limit = target + size_a + size_b;
  while (target < target_limit) {
    if (p_a < p_a_limit) {
      if (p_b < p_b_limit) {
        if (*p_a < *p_b) { *target = *p_a++; }
        else { *target = *p_b++; }
      }
      else { *target = *p_a++; }
    }
    else { *target = *p_b++; }
    ++target;
  }
}

void merge_runs(long total_size, long run_size, long from[], long to[]) {
  while (total_size) {

    long run_size_a = run_size;
    long run_size_b = run_size;
    if (total_size < run_size_a + run_size_b) 
      run_size_b = total_size - run_size_a;
    merge_run(run_size_a, run_size_b, from, to);
    from = from + run_size_a + run_size_b;
    to = to + run_size_a + run_size_b;
    total_size = total_size - run_size_a - run_size_b;
  }
}

void mergeArrays(long arr1[], long arr2[], long n1, long n2, long arr3[]) { 

    long i = 0;
    long j = 0;
    long k = 0; 

    while (1) 
    { 
      if (i < n1)
      {
        if (j < n2)
        {
        if (*arr1 < *arr2){ 
            *arr3 = *arr1;
            arr3++;
            arr1++;
            i++;
            k++;
            }
        else{
            *arr3=*arr2;
            arr3++;
            arr2++;
            k++;
            j++;
            }

        } else
        {
          break;
        }
        

      } else
      {
        break;
      }
            
    }

  
  
    while (i < n1) {
        *arr3=*arr1;
        arr3++;
        arr1++;
        k++;
        i++;

      }

    while (j < n2){ 
        *arr3=*arr2;
        arr3++;
        arr2++;
        k++;
        j++;

        }
        
} 

void merge_sort(long num_elem, long array[]) {

  long* Array = allocate(num_elem);
  for (long index = 0; index < num_elem; index++)
  {
    Array[index] = array[index];
  }
  

  long* array_l=Array;
  long* array_lb;
  long num_left = num_elem/2;
  long num_right = num_elem - num_left;

  
  array_lb = allocate(num_left);

  long run_size = 1;
  

  while (run_size < num_left) {

    merge_runs(num_left, run_size, array_l, array_lb);

    long* tmp = array_l;
    array_l = array_lb;
    array_lb = tmp;

    run_size <<= 1;
    
    // run size 2 last if total size 2^2
    // run size 4 last if total size 2^3
  }

  run_size=1;

  long* array_r;
  long* array_rb;

  for (long pp = 0; pp < num_right; pp++)
  {
    Array++;
  }
  array_r=Array;
  
  array_rb = allocate(num_right);

  while(run_size < num_right){
    
    merge_runs(num_right, run_size, array_r, array_rb);
    long* tmp2 = array_r;
    array_r = array_rb;
    array_rb = tmp2;
    

    run_size <<= 1;
  }


 

  mergeArrays(array_r, array_l, num_right, num_left, array);
  

}

long* run() {
  init_allocator();
  // Read it the program should print
  // 0: Silent mode
  // 1: Print sorted list
  // 2: Get input from stdin
  // 3: Test mode (print + input)
  long mode = read_long();
  long is_printing = mode & 1;
  long get_input = mode & 2;
  // Read number of elements in array
  long num_entries = read_long();
  long* p;
  // Run the algorithm
  if (get_input) {
    p = read_array(num_entries);
  }
  else {
    p = get_random_array(num_entries);
  }

  // Run the algorithm
  merge_sort(num_entries, p);

  if (is_printing) {
    print_array(num_entries, p);
  }
  return p; // <-- prevent optimization
}
