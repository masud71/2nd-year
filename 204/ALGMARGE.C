/*------------------Algorighm of marge & quick sort ----------------*/

Procedure link_merge(left , right : Integer)  Returns Start
Coment : Merge sort using linked list
Begin
	mid,head1,head2 : Integer;

	if (left = right) then
		return left;		Comment : Single Data
	mid = (left + right) / 2;
	Call link_merge(left,mid) returns head1;
	Call link_merge(mid+1,right) returns head2;
	return (combine(head1,head2));
End;

Procedure combine(h1,h2 : Integer)  Returns Start
Begin
	temp1,temp2,dummy : Integer;

	if (store[h1] < store[h2]) then
		temp1 = h1;
		temp2 = h2;
	Endif
	else
		temp1 = h2;
		temp2 = h1;
	Endif
	store[0] = temp1;
	while (temp1 <> 0) do
		while (store[temp1] <= store[temp2] AND temp1) do
			dummy = temp1;
			temp1 = link[temp1];
		End while
		link[dummy] = temp2;
		if (temp1) then
			dummy = temp1;
			temp1 = temp2;
			temp2 = dummy;
		End if
	End While
	return store[0];
End;

Procedure mergesort(left,right : Integer)
Coment : Margesort using array
Begin
	mid : Integer;

	if (left < right) then
		mid = (left + right) / 2;
		call mergesort(left,mid);
		call mergesort(mid+1,right);
		call merge(left,mid,right);
	Endif
End;

Procedure merge(left,mid,right : Integer)
Begin
	i,j,dummy,k : Integer;

	i = left;
	j = mid+1;
	while (i <= j AND j <= right) Do
		if (data[i] > data[j]) then
			dummy = data[j];
			for k = j downto i+1
				data[k] = data[k-1];
			data[i] = dummy;
			inc(j);
		Endif
		Inc(i);
	End While
End;

Procedure qksort(start,end : Integer)
Coment : Quicksort
Begin
	mid : Integer;

	if (start < end) then
		mid = partition(start,end);
		qksort(start,mid-1);
		qksort(mid+1,end);
	End if
End;

Procedure partition(start,end : Integer) returns middle
Begin
	left,right,dummy : Integer;

	left = start+1;
	right = end;
	while (left <= right) do
		while (data[left] <= data[start]) do
			Inc(left);
		while (data[right] > data[start]) do
			Inc(right);
		if (left < right) then
			dummy = data[right];
			data[right] = data[left];
			data[left] = dummy;
		Endif
	End While
	dummy = data[right];
	data[right] = data[start];
	data[start] = dummy;
	return right;
End;