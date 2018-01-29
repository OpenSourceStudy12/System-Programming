#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

inline bool cmpstr(string a, string b)
{
	return a < b;
}

inline bool cmpSize(string a, string b)
{
	return a.size() < b.size();
}

void Sort(string arr[], int len, bool (*pf)(string, string))
{
	for (int i = 0; i < len-1; i++) {
		for (int j = 0; j < len - i - 1; j++) {
			if (pf(arr[j], arr[j+1])) {
				auto t = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = t;
			}
		}
	}
}

void show(string a[], int len, bool (*p)(string))
{
	for (int i = 0; i < len; i++) {
		if (p(a[i])) {
			cout << a[i] << " ";
		}
	}
	cout << endl;
}

int main()
{
	string array[5] = {"asd", "sfse", "qerwt", "lkji", "mnbv"};

	int n = 3;
	//捕获列表， 捕获之前存在的变量， 拷贝进函数体内
	auto p = [&n/*只读*/](int a, int b) /*mutable：捕获进来的值可以改变*/{
		n = 10;//如果使用引用捕获， 则可以更改
		return a*b - n;
	};

//	auto p = [=](int a, int b) {}; : 把前面全部变量都通过拷贝捕获进来。
//	auto p = [&](int a, int b) {}; : 把前面的全部变量通过引用捕获进来。
//	auto p = [&, r](int a, int b) {}; : 除了r是拷贝之外， 其他的都是引用。
	cout << p(4, 5) << endl;
	cout << n << endl;
#if 0
	Sort(array, 5, cmpstr);

	for (auto m : array) {
		cout << m << " " ;
	}
	cout << endl;

	1.简洁
	2.距离
	3.提高了效率

	Sort(array, 5, [](string a, string b)->bool { return a > b; });

	for (auto m : array) {
		cout << m << " " ;
	}
	cout << endl;

	sort(array, array + 5, 
			[](string a, string b)->bool {
			return a < b;
			});

	for (auto m : array) {
		cout << m << " " ;
	}
	cout << endl;

	vector<int> v;
	int num;
	while (cin >> num) {
		v.push_back(num);
	}

	sort(v.begin(), v.end(), 
			[](int a, int b)->bool {
			return a > b;
			});

	for (auto m : v) {
		cout << m << " ";
	}
	cout << endl;
#endif
	return 0;
}
