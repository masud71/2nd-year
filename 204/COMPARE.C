#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SIZE 5000

int store[SIZE],data[SIZE+2],link[SIZE];

void bubble(void);
void cocktail(void);
int partition(int,int);
void swap(int *,int *);
void rquick(int,int);
void squick(int,int);
void heapsort(void);
void createheap(void);
void restore(int);
void shell(void);
void binsort(void);
void inssort(void);
int find(int *,int);
void adselect();
void selsort(int,int);
void mergesort(int,int);
void merge(int,int,int);
int link_merge(int,int);
int combine(int,int);
int linkins(int,int);
void radix(void);
void initptr(int [][],int *,int *);
void radixsort(int,int [][],int *,int *);

main()
{
	register int i;
	clock_t start,end;

	clrscr();
	printf("Number of data = %d\n\n",SIZE);
	randomize();
	for(i=0;i<SIZE;i++) store[i] = random(INT_MAX);
	for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	selsort(0,SIZE-1);
	end = clock();
	printf("Time required for Selection sort : %f seconds.\n",(end-start)/CLK_TCK);
	for(i=0;i<SIZE;i++) data[i+1] = store[i];
	start = clock();
	adselect();
	end = clock();
	printf("Time required for Advanced Selection sort : %f seconds.\n",(end-start)/CLK_TCK);
	for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	bubble();
	end = clock();
	printf("Time required for Bubble sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	cocktail();
	end = clock();
	printf("Time required for Cocktail sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	inssort();
	end = clock();
	printf("Time required for Straight insertion sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	binsort();
	end = clock();
	printf("Time required for Binary insertion sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	shell();
	end = clock();
	printf("Time required for Shell sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	mergesort(0,SIZE-1);
	end = clock();
	printf("Time required for Normal Merge sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	link_merge(0,SIZE-1);
	end = clock();
	printf("Time required for Linked Merge sort : %f seconds.\n",(end-start)/CLK_TCK);
	for(i=0;i<SIZE;i++) data[i] = store[i];
	data[SIZE] = INT_MAX;
	start = clock();
	rquick(0,SIZE-1);
	end = clock();
	printf("Time required for Recursive Quick sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	data[SIZE] = INT_MAX;
	start = clock();
	squick(0,SIZE-1);
	end = clock();
	printf("Time required for Itaretive Quick sort : %f seconds.\n",(end-start)/CLK_TCK);
	for(i=0;i<SIZE;i++) data[i] = store[i];
	data[SIZE]=0;
	start = clock();
	heapsort();
	end = clock();
	printf("Time required for Heap sort : %f seconds.\n",(end-start)/CLK_TCK);
    for(i=0;i<SIZE;i++) data[i] = store[i];
	start = clock();
	radix();
	end = clock();
	printf("Time required for Radix sort : %f seconds.\n",(end-start)/CLK_TCK);
	printf("\nPress any key to exit ......\n");
	getch();
}

void bubble()
{
	register int i,indic;
	int last;

	last = SIZE-1;
	do {
		indic = 0;
		for (i=0;i<last;i++)
			if (data[i] > data[i+1]) {
				indic = i;
				swap(&data[i],&data[i+1]);
			}
		last = indic;
	} while (indic);
}

void cocktail()
{
	register int i,indic;
	int lastl,lastr,left=1;

	lastr = SIZE-1;
	lastl = 0;
	do {
		indic = 0;
		if (left) {
			for (i=lastl;i<lastr;i++)
				if (data[i] > data[i+1]) {
					indic = i;
					swap(&data[i],&data[i+1]);
				}
			left =0;
			lastr = indic;
		}
		else {
			for (i=lastr;i>lastl;i--)
				if (data[i] < data[i-1]) {
					indic = i;
					swap(&data[i],&data[i-1]);
				}
			left = 1;
			lastl = indic;
		}
	} while (indic);
}

int partition(int left,int right)
{
	register int i,j;

	i=left+1;
	j=right;
	while (i <= j) {
		while(data[i] <= data[left]) i++;
		while(data[j] > data[left]) j--;
		if (i < j) swap(&data[i],&data[j]);
	}
	swap(&data[left],&data[j]);
	return j;
}

void swap(int *d1,int *d2)
{
	int dummy;

	dummy = *d1;
	*d1 = *d2;
	*d2 = dummy;
}

void rquick(int left,int right)
{
	int part;

	if (left<right) {
		part = partition(left,right);
		rquick(left,part-1);
		rquick(part+1,right);
	}
}

void squick(int left,int right)
{
	int stack[20],ptr,part;

	ptr=0;
	for(;;) {
		while (left<right) {
			part=partition(left,right);
			if (part-left < right-part) {
				stack[ptr+1] = part+1;
				stack[ptr+2] = right;
				right = part-1;
			}
			else {
				stack[ptr+1] = left;
				stack[ptr+2] = part-1;
				left = part+1;
			}
			ptr += 2;
		}
		if (ptr) {
			left = stack[ptr-1];
			right =stack[ptr];
			ptr -= 2;
		} else return;
	}
}

void heapsort()
{
	int i;
	clock_t start,end;

	createheap();
	for(i=SIZE;i>1;i--) {
		swap(&data[1],&data[i]);
		restore(i-1);
	}
}

void createheap()
{
	register int i,dummy,ok;

	do {
		ok=1;
		for(i=SIZE/2;i>=1;i--) {
			dummy = (data[i*2] > data[i*2+1]) ? i*2 : i*2+1;
			if (data[i] < data[dummy]) {
				ok=0;
				swap(&data[i],&data[dummy]);
			}
		}
	}while (!ok);
}

void restore(int index)
{
	int i = 1,j;

	for(;2*i<=index;i=j) {
		if (i*2 < index) j = (data[i*2]>data[i*2+1]) ? i*2 : i*2+1;
		else if (i*2 == index) j = i*2;
		if (data[i] < data[j]) swap(&data[i],&data[j]);
		else return;
	}
}

void inssort()
{
	register int i,j;
	int dummy;

	for (i=1;i<SIZE;i++) {
		dummy = data[i];
		for (j=i-1;(data[j]>dummy) && j>=0;j--)
			data[j+1] = data[j];
		data[j+1] = dummy;
	}
}

void binsort()
{
	register int i,j;
	int low,high,dummy;

	for (i=1;i<SIZE;i++) {
		dummy = data[i];
		low=0;
		high=i-1;
		while (low<=high) {
			j=(low+high)/2;
			if (data[j] > dummy) high = j-1;
			else low = j+1;
		}
		for (high=i;high>low;high--) data[high]=data[high-1];
		data[low]=dummy;
	}
}

void shell(void)
{
	register int i,j,k;
	int size,dummy;

	for (size=SIZE/2;size;size=size/2)
		for (k=0;k<size;k++)
		for (i=k+size;i<SIZE;i += size) {
			dummy = data[i];
			for (j=i-size;(data[j]>dummy) && j>=k;j -= size)
				data[j+size] = data[j];
			data[j+size] = dummy;
		}
}

void selsort(int st,int end)
{
	register int i,j,min;

	for (i=st;i<end;i++) {
		min = i;
		for (j=i+1;j<=end;j++)
			if (data[j] < data[min]) min = j;
		swap(&data[i],&data[min]);
	}
}

void adselect()
{
	int size,i,ni=0,newdata[SIZE],index[50];

	size = SIZE/50;
	for (i=0;i<50;i++) index[i] =i*size+1;
	for (i=0;i<50;i++) selsort(i*size+1,(i+1)*size);
	do {
		newdata[ni++]= find(index,size);
	}while (ni<SIZE);
}

int find(int index[],int size)
{
	int i,min,ndata;

	for (min=0;!index[min];min++);
	for (i=min+1;i<50;i++)
		if (data[index[i]] < data[index[min]] && index[i]) min = i;
	ndata = data[index[min]];
	index[min]++;
	if (index[min] % size == 1)	index[min] = 0;
	return ndata;
}

void radix()
{
	int i,avail,ptr[10][2],link[SIZE+10];

	for (i=1;i<1000;i*=10) {
		initptr(ptr,link,&avail);
		radixsort(i,ptr,link,&avail);
	}
}

void initptr(int ptr[][2],int link[],int *avail)
{
	register int i;

	for (i=0;i<10;i++) ptr[i][0] = ptr[i][1] = i;
	for (i=10;i<SIZE+9;i++) link[i] = i+1;
	*avail = 10;
}

void radixsort(int key,int ptr[][2],int link[],int *avail)
{
	register int i,j;
	int temp,list[SIZE+10];

	for (i=0;i<SIZE;i++) {
		temp = ptr[(data[i]/key) % 10][1];
		list[temp] = data[i];
		ptr[(data[i]/key) % 10][1] = link[temp] = *avail;
		*avail = link[*avail];
	}
	temp = 0;
	for (i=0;i<10;i++)
		for (j=ptr[i][0];j != ptr[i][1];j = link[j]) data[temp++] = list[j];
}

void mergesort(int start,int end)
{
	int mid;

	if (start<end) {
		mid = (start+end)/2;
		mergesort(start,mid);
		mergesort(mid+1,end);
		merge(start,mid,end);
	}
}

void merge(int start,int mid,int end)
{
	int i,j,k;
	int dummy[SIZE];

	i=start;
	j=mid+1;
	k=0;
	while (i<=mid && j<=end) {
		if (j<=end)
			while (data[i]<=data[j] && i<=mid) dummy[k++] = data[i++];
		if (i<=mid)
			while (data[j]<data[i] && j<=end) dummy[k++] = data[j++];
	}
	if (i>mid)
		while (j<=end) dummy[k++] = data[j++];
	else while (i<=mid) dummy[k++] = data[i++];
	for (i=0;i<k;i++) data[start+i] = dummy[i];
}

int link_merge(int start,int end)
{
	int mid,l1,l2;

	if (start<end) {
		if (end-start <= 16) return (linkins(start,end));
		mid = (start+end)/2;
		l1 = link_merge(start,mid);
		l2 = link_merge(mid+1,end);
		return (combine(l1,l2));
	}
}

int combine(int h1,int h2)
{
	int temp,dummy=-1;

	if (data[h1] < data[h2]) temp = h1;
	else temp = h2;
	while (h1 != -1 && h2 != -1) {
		if (h2 != -1) {
			while (data[h1] <= data[h2] && h1 !=-1) {
				dummy = h1;
				h1= link[h1];
			}
			link[dummy] = h2;
		}
		if (h1 != -1) {
			while (data[h2] < data[h1] && h2 !=-1) {
				dummy = h2;
				h2= link[h2];
			}
			link[dummy] = h1;
		}
	}
	return temp;
}

int linkins(int start,int end)
{
	register int i,j;
	int dummy;

	for (i=start+1;i<=end;i++) {
		dummy = data[i];
		for (j=i-1;(data[j]>dummy) && j>=start;j--)
			data[j+1] = data[j];
		data[j+1] = dummy;
	}
	for (i=start;i<end;i++) link[i] = i+1;
	link[end] = -1;
	return start;
}