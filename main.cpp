#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "spline.h"
#include "quanc8.h"
#include "lagrange.h"
#include <math.h>
#include <iostream>
#include "Tlagrange_2/spline.h"

using namespace std;

double x = 0.0; //от этой переменной зависит функци€ f
const int N = 11; //кол-во точек, по которым строим п.Ћагранжа и —плайн
double step = 0.2; //шаг точек интерпол€ции
double lower = 0.1;//перва€ точка в которой будем находить значение(не узел в построении)
int  numFlags = 0;//отслеживаем плохиеслучаи срабатывани€ quanc8

double f(double t){
    return exp(x*t);
}

void main(void){
    double a = 0, b = 1, abserr = 1.0e-10, relerr = 0, errest, flag;
    int nofun;

    //ищем значени€ в точках-узлах
    x = 0.0;
    double QuancForLaglange[N];
    for (int i = 0; i < N; i++) {
        quanc8(f, a, b, abserr, relerr, &QuancForLaglange[i], &errest, &nofun, &flag);
        x += step;
        if (flag != 0.0)
            numFlags++;
    }

    //ищем значени€ в точках интерпол€ции, хэти точки тоже собираем в массив
    x = 0.1;
    double PerfectValues[N];
    double lagrangeX[N];
    for (int i = 0; i < N; i++) {
        quanc8(f, a, b, abserr, relerr, &PerfectValues[i], &errest, &nofun, &flag);
        x += step;
        lagrangeX[i] = i * step;
    }
    
    double LagrangeResults[N];
    double x = 0.1;
    for (int i = 0; i < N; i++){
        LagrangeResults[i] = lagrange(N - 1, lagrangeX, QuancForLaglange, x);
        x += step;
    }

    //преобразование массив->vector. кор€во, но тут юзаетс€ сторонн€€ библиотека. автор https://github.com/ttk592
    int size = sizeof(QuancForLaglange) / sizeof(QuancForLaglange[0]);
    vector<double> Y(QuancForLaglange, QuancForLaglange + size);
    vector<double> X(lagrangeX, lagrangeX + size);

    double SplineResults[N];
    spline s;
    s.set_points(X, Y);    
     
    //просто не люблю форматирование :—
    cout  << "x" << "          QUANC" << "           Lagrange" << "           Spline" << "             El" << "                  Es" << endl;
   
    for (int i = 0; i < N; i++){
       cout << lower + step * i << "       f=" << PerfectValues[i] << "        L=" << LagrangeResults[i] << "        S=" << s(lower + step * i);
       cout << "       L-f=" << LagrangeResults[i] - PerfectValues[i] << "       S-f=" << s(lower + step * i) - PerfectValues[i] << endl;
       if (numFlags != 0)
           cout << "WARNING" << endl;
    }
}
