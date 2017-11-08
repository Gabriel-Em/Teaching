#include <iostream>
#include <fstream>
using namespace std;

void readFile(int &P, int &N, int &V)
{
	ifstream read("cub.in");

	read >> P >> N >> V;

	read.close();
}

void findCoord(int N, int V, int &x, int &y, int &z)
{
	/*
	calculam nivelul pe care suntem si reducem V la valoarea unui bec de pe nivelul 1
	daca nivelul pe care suntem este par, atunci V devine simetricul lui V de pe nivelul 1

	exemplu:
	daca N = 3, atunci becul cu numarul 10 ne va da nivelul 10/(3*3) + 1 adica 2
	apoi 10 va deveni 1 (pentru ca becurile de pe primul nivel merg de la 1 la 9, so 10-18, 19-27, 28,36, etc vor
	fi reduse la 1-9.
	De cate ori suntem pe un nivel par in schimb, spirala porneste din centru spre colt, deci, daca am vrea
	sa o parcurgem din coltul din stanga sus spre centru ca si nivelele impare, atunci ar trebui sa parcurgem
	elementele de la ultimul element spre primul (in cazul nostru, de la 9 la 1)

	Nivel impar:

	1 2 3
	8 9 4
	7 6	5

	Nivel par:

	9 8 7
	2 1 6
	3 4	5

	Se observa ca pe nivelele pare fiecare element este simetricul unui element de pe un nivel impar

	1 - 9
	2 - 8
	3 - 7
	4 - 6
	5 - 5
	etc

	Cum aflam simetricul? N*N+1-element

	daca element = 3, atunci 3*3+1-3 = 7
	element = 5, atunci 3*3+1-5 = 5

	revenind la exemplul cu V = 10. V a fost redus la 1 mai sus. Deoarece suntem pe un nivel par 1 va deveni
	simetricul sau adica 9.

	Pentru reducerea unui element la nivelul 1 se pot folosi formulele
	V = V / z (daca V e multiplu de n*n)
	V = V % (N*N) (daca V nu e multiplu de n*n)

	exemplu, daca N = 3, becul 18 se va reduce la 9 (18/(3*3)), deoarece 18 e multiplu de (3*3)
	becul 13 se va reduce la 4 (28%(3*3)), deoarece 13 nu e multiplu de (3*3)
	becul
	in functie de nivelul pe care suntem, reductia elementelor poate deveni simetricul lor
	18, care a devenit 9, va deveni 1, pentru ca 18 e pe nivelul 2 in acelasi loc in care e 1 pe nivelul 1
	13, care a devenit 4, fiind pe nivelul 2, va deveni 6, pentru ca 13 e pe nivelul 2 in acelasi loc in care e
	6 pe nivelul 1

	pentru a determina nivelul se pot folosi formulele z = V / (N*N) daca V e multiplu de N*N sau z = V/(N*N)+1 altfel
	*/

	if (V % (N*N) == 0)
	{
		z = V / (N*N);
		V = V / z;
	}
	else
	{
		z = V / (N*N) + 1;
		V = V % (N*N);
	}

	if (z % 2 == 0)
		V = (N*N) + 1 - V;

	y = 0, x = 1;

	int pace = N, reach = 0, dir = 0; // 1 - dreapta, 2 - jos, 3 - stanga, 4 - sus
	bool q = true;

	/* parcurgem spirala de pe nivelul 1 dupa regulile de mai jos in cautarea elementului V transformat mai sus

	pentru n = 3 si nivelul 1

	1 2 3
	8 9 4
	7 6	5

	vom parcurge la:

	pasul 1 - 1 2 3
	pasul 2 - 4 5
	pasul 3 - 6 7
	pasul 4 - 8
	pasul 5 - 9

	in timp ce parcurgem nivelul, modificam coordonatele x,y in functie de numarul de elemente pe care le-am parcurs
	la un pas pana la intalnirea unui pas care contine elementul V cautat

	*/
	while (reach < V)
	{
		reach = reach + pace;
		dir++;
		if (dir == 5)
			dir = 1;

		switch (dir)
		{
		case 1: y += pace; break;
		case 2: x += pace; break;
		case 3: y -= pace; break;
		case 4: x -= pace; break;
		}

		if (q)
		{
			q = false;
			pace--;
		}
		else
			q = true;
	}

	/*
	dupa ce am gasit pasul care contine elementele printre care se afla V modificam coordonatele x,y in functie de
	pozitia elementului V intre elementele de la acel pas
	*/

	if (reach != V)
		switch (dir)
		{
		case 1: y -= reach - V; break;
		case 2: x -= reach - V; break;
		case 3: y += reach - V; break;
		case 4: x += reach - V; break;
		}
}

bool isPrime(int nr)
{
	if (nr == 2)
		return true;
	if (nr < 2 || nr % 2 == 0)
		return false;

	int i = 3;
	while (i*i <= nr)
	{
		if (nr%i == 0)
			return false;
		i += 2;
	}

	return true;
}

int getPrime(int N, int f)
{
	int c = 0, nr1, nr2, nr1_ = 0, nr2_ = 0;

	switch (f)
	{
	case 1: nr1 = 1, nr2 = 2 * N*N; break;														// Fata 1
	case 2: nr1 = N, nr2 = 2 * N*N - N + 1; break;												// Fata 2
	case 3: nr1 = 2 * N - 1, nr2 = 2 * N*N - 2 * N + 2; break;									// Fata 3	
	case 4: nr1_ = 1, nr2_ = 2 * N*N; nr1 = 3 * N - 2; nr2 = 2 * N*N - 3 * N + 3; break;		// Fata 4
	}

	while (nr1 < N*N*N || nr2 < N*N*N)
	{
		// nivele impare

		if (f != 4) // fata 1 2 3
		{
			for (int i = 0; i < N; i++)
			{
				// nivele impare

				if (isPrime(nr1))
					c++;

				nr1++;

				// nivele pare

				if (nr2 <= N*N*N)
				{
					if (isPrime(nr2))
						c++;

					nr2--;
				}
			}
			nr1 = nr1 + N*N * 2 - N;
			nr2 = nr2 + N*N * 2 + N;
		}
		else // fata 4
		{
			for (int i = 0; i < N - 1; i++)
			{
				// nivele impare

				if (isPrime(nr1))
					c++;
				nr1++;

				// nivele pare

				if (nr2 <= N*N*N)
				{
					if (isPrime(nr2))
						c++;
					nr2--;
				}
			}

			// nivele impare

			if (isPrime(nr1_))
				c++;

			nr1 = nr1 + N*N * 2 - N + 1;
			nr1_ = nr1_ + N * N * 2;

			// nivele pare

			if (nr2_ <= N*N*N)
				if (isPrime(nr2_))
					c++;

			nr2 = nr2 + N*N * 2 + N - 1;
			nr2_ = nr2_ + N*N * 2;
		}
	}

	return c;
}

void writeToFile(int x, int y, int z)
{
	ofstream write("cub.out");

	write << x << " " << y << " " << z;

	write.close();
}

void writeToFile(int f1, int f2, int f3, int f4)
{
	ofstream write("cub.out");

	write << f1 << endl << f2 << endl << f3 << endl << f4;

	write.close();
}

int main()
{
	int N, P, V;

	readFile(P, N, V);

	if (P == 1)
	{
		int x, y, z;
		findCoord(N, V, x, y, z);
		writeToFile(x, y, z);
	}
	else
	{
		writeToFile(
			getPrime(N, 1),
			getPrime(N, 2),
			getPrime(N, 3),
			getPrime(N, 4));
	}

	//cout << endl << endl;
	//system("pause");
	return 0;
}
