#include <stdio.h>
#include <locale.h>
#include <math.h>

double discr_4(double a, double b, double c) {
	auto qvD = b * b - a * c;
	return qvD < 0 ? -sqrt(abs(qvD)) : sqrt(qvD);
}

int main() {
	double a, b, c;

	setlocale(LC_ALL, "Russian");
	printf("Введите коэф. a, b, c (в одну строку через пробел): ");
	scanf("%lf %lf %lf", &a, &b, &c);
	
	double d;
	if (!a && !b && !c) {
		printf("forall X");
	}
	else if (!a) {
		printf("X = %.2lf", !c ? 0 : -c / b);
	}
	else if (!(d = discr_4(a, -b / 2, c) / a)) {
		printf("X = %.2lf", b/ (2 * a));
	}
	else if (d < 0) {
		printf("X1 = %.2lf + i%.2lf | X2 = %lf - i%.2lf", -b / (2 * a), -d, -b / (2 * a), -d);
	}
	else {
		printf("X1 = %.2lf | X2 = %.2lf", b/ (2 * a) + d, b/ (2 * a) - d);
	}
	printf("\n");


	return 0;
}