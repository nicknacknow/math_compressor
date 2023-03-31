#include <Windows.h>
#include <iostream>
#include <string>

#include "Matrix.h"


long double f(long double x) {
	long double a = 435;
	long double b = -94112./105;
	long double c = 40826./45;
	long double d = -164203./360;
	long double h = 18169./144;
	long double j = -14057./720;
	long double k = 1139./720;
	long double l = -263./5040;
	
	return a + (b * x) + (c * pow(x, 2)) + (d * pow(x, 3)) + (h * pow(x, 4)) + (j * pow(x, 5)) + (k * pow(x, 6)) + (l * pow(x, 7));
}

long double f(Matrix<long double> m, long double x) {
	long double ret = 0;
	static bool done = false;

	std::string equation = "y = ";

	for (int i = 0; i < m.rows(); i++) {
		if (!done)
			equation.append(std::to_string(m(i, 0)) + std::string("x^{") + std::to_string(i) + std::string("} + "));
		ret += (m(i, 0) * pow(x, i));
	}

	if (!done)
		printf("equation : %s\n", equation.c_str());
	done = true;

	return ret;
}

int main() {
	printf("input message to be compressed: ");
	std::string inp;
	std::getline(std::cin, inp);

	/*int* msg = new int[8];

	for (int i = 1; i <= 8; i++) {
		msg[i - 1] = std::round(f(i));
		printf("[%d] %d => %c\n", i, msg[i - 1], (char)msg[i - 1]);
	}*/

	Matrix<long double> m1(inp.size(), inp.size());
	for (int i = 0; i < m1.rows(); i++) {
		for (int j = 0; j < m1.cols(); j++) {
			m1(i, j) = pow(i + 1, j);
		}
	}

	/*for (int i = 0; i < m1.rows(); ++i) {
		for (int j = 0; j < m1.cols(); ++j) {
			std::cout << m1(i, j) << " ";
		}
		std::cout << "\n";
	}*/

	printf("\n\n");

	Matrix<long double> m2 = m1.inverse();

	/*for (int i = 0; i < m2.rows(); ++i) {
		for (int j = 0; j < m2.cols(); ++j) {
			std::cout << m2(i, j) << " ";
		}
		std::cout << "\n";
	}*/

	Matrix<long double> b(inp.size(), 1);
	for (int i = 0; i < inp.size(); i++) {
		printf("%d\n", inp[i]);
		b(i, 0) = (char)inp[i];
	}
	/*b(0, 0) = 98;
	b(1, 0) = 111;
	b(2, 0) = 111;
	b(3, 0) = 107;
	b(4, 0) = 108;
	b(5, 0) = 101;
	b(6, 0) = 116;
	b(7, 0) = 115;*/
	//b(8, 0) = 111;
	//b(9, 0) = 111;

	Matrix<long double> matrix_X = m2 * b;
	/*for (int i = 0; i < matrix_X.rows(); ++i) {
		for (int j = 0; j < matrix_X.cols(); ++j) {
			std::cout << matrix_X(i, j) << " ";
		}
		std::cout << "\n";
	}*/

	for (int i = 1; i <= inp.size(); i++) {
		int n = std::round(f(matrix_X, i));
		printf("[%d] %d => %c\n", i, n, (char)n);
	}

	//for (int i = 0; i < 8; i++) {
	//	printf("%d %c\n", msg[i], (char)msg[i]);
	//}

	std::cin.get();
	return 0;
}