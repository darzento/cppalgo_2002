#include <stdio.h>
#include <math.h>			// sqrt �Լ� ����� ���� �ʿ� 

int is_prime(int n)
{
	int i;
	for (i = 2; i < n; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

int is_prime2(int n)
{
	int i, sqrn;
	sqrn = (int)sqrt(n);
	for (i = 2; i <= sqrn; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

void main(void) 
{
	int n;

	n = 1483;

	// 2 ~ 1482 ���� �˻� 
	printf("(is_prime) %d is %s prime number\n", n, is_prime(n) ? "" : "not");
	// sqrt(1483) = 38.5  �� 2 ~ 38������ �˻� 
	printf("(is_prime2) %d is %s prime number\n", n, is_prime2(n) ? "" : "not");
	
	n = 529;
	printf("(is_prime) %d is %s prime number\n", n, is_prime(n) ? "" : "not");
	printf("(is_prime2) %d is %s prime number\n", n, is_prime2(n) ? "" : "not");
}	
