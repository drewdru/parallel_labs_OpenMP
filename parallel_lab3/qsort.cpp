void quickSort(int *arr, int l, int r){
    int mid = (r + l) / 2;
    int pivot = arr[mid];
    int tl = l, tr = r;
    while(tl<=tr){
        while(arr[tl]<pivot) tl++;
        while(arr[tr]>pivot) tr--;
        if(tl<=tr){
			swap(arr[tl],arr[tr]);
            tr--;
            tl++;
        }
    }
    if(l<tr)quickSort(arr, l, tr); 
    if(tl<r)quickSort(arr, tl, r); 
}
double quickSort(int* arr, int size){
	int i;
	int *a = new int [size];
	for(i=0;i<size;i++) a[i] = arr[i];
	double start = omp_get_wtime();
	quickSort(a,0,size-1);
	delete []a;
	return omp_get_wtime()-start;
}
void quickParallelSort(int *arr, int l, int r){
    int mid = (r + l) / 2;
    int pivot = arr[mid];
    int tl = l, tr = r;
    while(tl<=tr){
		while(arr[tl]<pivot) tl++;
		while(arr[tr]>pivot) tr--;
        if(tl<=tr){
			swap(arr[tl],arr[tr]);
            tr--;
            tl++;
        }
    }
	#pragma omp critical
	if(l<tr)quickParallelSort(arr, l, tr);
	#pragma omp critical
	if(tl<r)quickParallelSort(arr, tl, r);
}
double quickParallelSort(int* arr, int size){
	int i;
	int *a = new int [size];
	for(i=0;i<size;i++) a[i] = arr[i];
	double start = omp_get_wtime();
	#pragma omp parallel
	#pragma omp single nowait
	quickParallelSort(arr, 0, size-1);
	delete []a;
	return omp_get_wtime()-start;
}
