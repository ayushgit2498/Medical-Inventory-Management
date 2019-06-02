#include<iostream>
#include<string>
#include<ctime>
#include<fstream>
#include<stdlib.h>
#include<iomanip>
void view_stock();
void update_stock();
void renew_stock();
void new_med();
int check();
int check_expiry(int, int);
using namespace std;
class node
{
public:
	string name;
	int quan;
	int total;
	int price;
	node *next;
};
class bill
{
private:
	string med;
	int q, price10;
public:
	node *head;
	bill()
	{
		head = NULL;
	}
	friend class node;
	void add();
	void delmid();
	void change();
	void display();
	int displayfinal();
	int eval()
	{
		node *ptr;
		ptr = head;
		int sum = 0;
		while (ptr != NULL)
		{
			sum += ptr->total;
			ptr = ptr->next;
		}
		return sum;
	}
	int check()
	{
		string name;
		int h = 1, h1 = 1;
		int n, x, p, n1, n2;
		int check1 = 1;
		fstream finout;
		finout.open("stock.txt", ios::in);
		while (finout >> name >> n >> p >> n1 >> n2)
		{
			if (name == med)
			{
				if (n == 0) //not available
				{
					cout << "Medicine is out of stock.\n";
					h = 0;
					h1 = 1;
				}
				else if (q >= n) //stock is less than ordered 
				{
					check1 = check_expiry(n1, n2);
					if (check1 == 0)
					{
						cout << "THE MEDICINE HAS EXPIRED.\n";
						h = 0;
						h1 = 1;
						break;
					}
					else
					{
						cout << "Sorry for the inconvineince but we are left with only " << n << " tablets.\n";
						q = n;
						x = 0;
						h = 1;
						h1 = 1;
					}

				}
				else  // stock is surplus
				{
					check1 = check_expiry(n1, n2);
					if (check1 == 0)
					{
						cout << "THE MEDICINE HAS EXPIRED.\n";
						h = 0;
						h1 = 1;
						break;
					}
					x = n - q;
					h = 1;
					h1 = 1;
				}
				break;
			}
			else
			{  //medicine still not found
				h = 0;
				h1 = 0;
			}
		}
		finout.close();
		if (h == 0 && h1 == 0)
		{
			cout << "Medicine is not present in stock.\n";
			return 0;
		}
		else if (h == 0)
		{
			return 0;
		}
		else
		{
			fstream fin;
			fstream fout;
			fin.open("stock.txt", ios::in);
			fout.open("temp.txt", ios::out);
			while (fin >> name >> n >> p >> n1 >> n2)
			{
				fout << name << " ";
				if (med == name)
				{
					fout << x << " ";
					price10 = q*p;
				}
				else
				{
					fout << n << " ";
				}
				fout << p << " ";
				fout << n1 << " ";
				fout << n2 << "\n";
			}
			fin.close();
			fout.close();
			remove("stock.txt");
			rename("temp.txt", "stock.txt");
			return 1;
		}
	}
	int check_expiry(int n1, int n2)
	{
		time_t a = time(0);
		tm *all = localtime(&a);
		int month = all->tm_mon + 1;
		int	year = all->tm_year + 1900;
		if (n2>year)
		{
			return 1;
		}
		else  //expired
		{
			if (n2 == year)
			{
				if (n1 > month)//consumable
					return 1;
				else //expired
					return 0;
			}
			else
				return 0;
		}
	}
};
void bill::add()
{
	int c1 = 1;
	cout << "Enter the name of the medicne:";
	cin >> med;
	cout << "Enter the amount of medicine you want:";
	cin >> q;
	c1 = check();
	if (c1 == 1)
	{
		node *nodeptr = new node;
		node *ptr;
		nodeptr->name = med;
		nodeptr->quan = q;
		nodeptr->price = price10 / q;
		nodeptr->total = price10;
		nodeptr->next = NULL;
		ptr = head;
		if (head == NULL)
		{
			head = nodeptr;
			ptr = head;
		}
		else
		{
			while (ptr->next != NULL)
			{
				ptr = ptr->next;
			}
			ptr->next = nodeptr;
		}
	}

}
void bill::delmid()
{
	node *ptr;
	node *ptr1;
	node *prev = NULL, *prev1 = NULL;
	int dq;
	int flag = 0;
	if (head == NULL)
	{
		cout << "Medicines are not present in the bill.\n";
	}
	else
	{
		ptr1 = head;
		while (ptr1->next != NULL)
		{
			prev = ptr1;
			ptr1 = ptr1->next;
		}
		cout << "Enter the name of the medicine which you want to delete.";
		string x1;
		cin >> x1;
		ptr = head;
		if (ptr->name == x1)
		{
			dq = ptr->quan;
			head = ptr->next;
			ptr->next = NULL;
		}
		else if (ptr1->name == x1)
		{
			dq = prev->quan;
			prev->next = NULL;
		}
		else
		{
			while (ptr != NULL && ptr->name != x1)
			{
				prev1 = ptr;
				ptr = ptr->next;
			}
			if (ptr == NULL)
			{
				cout << "There is no such medicine in the bill.\n";
			}
			else
			{
				dq = ptr->quan;
				prev1->next = ptr->next;
				ptr->next = NULL;
			}
		}
		int n1, p, n2, n3;
		string name;
		fstream fin;
		fstream fout;
		fin.open("stock.txt", ios::in);
		fout.open("temp.txt", ios::out);
		while (fin >> name >> n1 >> p >> n2 >> n3)
		{
			fout << name << " ";
			if (x1 == name)
			{
				n1 = n1 + dq;
				fout << n1 << " ";
			}
			else
			{
				fout << n1 << " ";
			}
			fout << p << " ";
			fout << n2 << " ";
			fout << n3 << "\n";
		}
		fin.close();
		fout.close();
		remove("stock.txt");
		rename("temp.txt", "stock.txt");
	}
}
void bill::change()
{
	node *ptr, *ptr2 = head;
	int dq1, count = -1;
	node *prev1 = NULL, *prev2 = NULL;
	if (head == NULL)
	{
		cout << "Medicines are not present in the bill.\n";
	}
	else
	{
		cout << "Enter the name of the medicine which you want to change.";
		string x1;
		cin >> x1;
		ptr = head;
		while (ptr2->next != NULL)
		{
			prev1 = ptr2;
			ptr2 = ptr2->next;
		}
		if (ptr->name == x1)
		{
			dq1 = ptr->quan;
			count = 1;
		}
		else if (ptr2->name == x1)
		{
			dq1 = prev1->quan;
			count = 0;
		}
		else
		{
			while (ptr != NULL && ptr->name != x1)
			{
				prev1 = ptr;
				ptr = ptr->next;
			}
			if (ptr == NULL)
			{
				cout << "There is no such medicine in the bill.\n";
			}
			else
			{
				dq1 = ptr->quan;
				count = 2;
			}
		}
		cout << "Enter the new quantity of the medicine.";
		int x2, x3;
		cin >> x2;
		if (count == 0)
		{
			ptr = ptr2;
			ptr->next->quan = x2;
		}
		if (count == 1)
			ptr->quan = x2;
		if (count == 2)
			ptr->quan = x2;
		int n1, p, n2, n3;
		string name;
		fstream fin;
		fstream fout;
		fin.open("stock.txt", ios::in);
		fout.open("temp.txt", ios::out);
		while (fin >> name >> n1 >> p >> n2 >> n3)
		{
			fout << name << " ";
			if (x1 == name)
			{
				n1 = n1 + dq1 - x2;
				fout << n1 << " ";
				x3 = p;
			}
			else
			{
				fout << n1 << " ";
			}
			fout << p << " ";
			fout << n2 << " ";
			fout << n3 << "\n";
		}
		fin.close();
		fout.close();
		remove("stock.txt");
		rename("temp.txt", "stock.txt");
		if (count == 0)
			ptr->next->total = x2*x3;
		if (count == 1)
			ptr->total = x2*x3;
	}

}
void bill::display()
{
	node *ptr;
	node *nodeptr;
	ptr = head;
	cout << "-------------------------------------\nNAME 	 QUANTITY    RATE   COST" << endl;
	cout<<"--------------------------------------\n";
	if (head == NULL)
	{
		return;
	}
	else
	{
		while (ptr != NULL)
		{
			cout << ptr->name << "    ";
			cout << ptr->quan << "       ";
			cout << ptr->price << "        ";
			cout << ptr->total << "       ";
			ptr = ptr->next;
			cout << endl;
		}
	}
}
int bill::displayfinal()
{
	static int i;
	node *ptr;
	node *nodeptr;
	ptr = head;
	string fname, fname1;
	long number;
	cout << "Entter customer name:";
	cin >> fname;
	cout << "ENter customer number:";
	cin >> number;
	int sum = eval();
	time_t a = time(0);
	tm *all = localtime(&a);
	int	min = all->tm_min;
	int	hour = all->tm_hour;
	int day = all->tm_mday;
	int month = all->tm_mon;
	int	year = all->tm_year;
	cout << "\n\n\n***************HYA******************" << "\n";
	cout << "**********Medical Store*************" << "\n";
	cout << "====================================\n";
	cout << "	Bill number:" << i << "	Time-" << hour << ":" << min << "\n";
	cout << "	Date-" << day << "." << month + 1 << "." << 1900 + year << "\n";
	cout << "	Customer name:" << fname << "\n";
	cout << "	Phone number:" << number << "\n";
	cout << "====================================\n";
	cout << "MEDICINE   QUANTITY   PRICE   TOTAL\n";
	cout << "====================================\n";
	if (head == NULL)
	{
		return 0;
	}
	else
	{
		while (ptr != NULL)
		{
			if(ptr->name=="combiflam"||ptr->name=="cinarest")
			{
				cout << ptr->name <<"\t";
				cout << ptr->quan << "\t";
				cout << ptr->price << "\t";
				cout << ptr->total;
			}
			else
			{
				cout << ptr->name <<"\t\t";
				cout << ptr->quan << "\t";
				cout << ptr->price << "\t";
				cout << ptr->total;
			}
			ptr = ptr->next;
			cout << endl;
		}
		i++;
	}
	cout << "===================================\n";
	cout << "		    GRAND-TOTAL:" << sum << "\n";
	cout << "***********************************\n";
	cout << "	THANK YOU " << fname;
	cout << "\n 	Do visit again.\n";
	cout << "***********************************\n\n\n";
	fstream fout;
	fname1 = fname;
	fname = fname + ".txt";
	fout.open(fname, ios::out | ios::app);
	ptr = head;
	fout<<"------------------------"<<"\n";
	fout << fname1 << " ";
	fout<<"BILL ON->"<<day<<":"<<month+1<<":"<<year+1900<<"\n";
	while (ptr != NULL)
	{
		fout << ptr->name << " ";
		fout << ptr->quan << " ";
		fout << ptr->price << " ";
		fout << ptr->total << "\n";
		ptr = ptr->next;
	}
	fout<<"------------------------"<<"\n";
	fout.close();
	return sum;
}
int main()
{
	int n, final;
	char ch1;
	int sum = 0, sum1;
	cout << "************WELCOME****************\n";
	do
	{
		system("clear");
		cout<<"******************************\n";
		cout << "1.)Enter 1 to view the stock.\n";
		cout << "2.)Enter 2 to update the stock.\n";
		cout << "3.)Enter 3 to create a new bill.\n";
		cout<<"******************************\n";
		cout << "Enter your choice:";
		cin >> n;
		switch (n)
		{
		case 1:
		{
			view_stock();
			break;
		}
		case 2:
		{
			update_stock();
			break;
		}
		case 3:
		{
			cout << "***********HYA*********\n";
			bill b;
			int no;
			char ch;
			cout << " 1)Add medicine.\n 2)Remove medicine.\n 3)Change amount of medicine.\n 4)Display.\n";
			do
			{
				cout << "Enter your choice.";
				cin >> no;
				do
				{
					if ((no==1||no==2||no==3||no==4))
					{
						break;
					}
					else
					{
						cout << "Enter correct choice:";
						cin >> no;
					}
				} while (1);
				switch (no)
				{
				case 1:
				{
					cout << "ENTER THE NUMBER OF MEDICINES YOU WANT TO ADD:";
					int n9;
					cin >> n9;
					while (n9)
					{
						b.add();
						n9--;
					}
					cout << "Your bill till now.\n";
					b.display();
					break;
				}
				case 2:
				{
					b.delmid();
					break;
				}
				case 3:
				{
					b.change();
					break;
				}
				case 4:
				{
					b.display();
					final = b.eval();
					cout << "----------------------------\nThe Total amount till now:" << final << "\n";
					break;
				}
				}
				cout << "DO YOU WANT TO EDIT THE BILL?.";
				cin >> ch;
				do
				{
					if (ch == 'y' || ch == 'n')
					{
						break;

					}
					else
					{
						cout << "Enter either y or n:";
						cin >> ch;
					}
				} while (1);
			} while (ch == 'y');
			system("clear");
			sum1 = b.displayfinal();
			sum = sum + sum1;
			break;
		}
		}
		cout << "DO YOU WANT TO continue AGAIN.";
		cin >> ch1;
		do
		{
			if (ch1 == 'y' || ch1 == 'n')
			{
				break;

			}
			else
			{
				cout << "Enter either y or n:";
				cin >> ch1;
			}
		} while (1);
	} while (ch1 == 'y');
	cout<<"==================xxxx===================\n";
	cout << "TOTAL SALE OF THE DAY IS:" << sum << "\n";
	cout<<"==================xxxx===================\n";
	return 0;
}
void view_stock()
{
	string name;
	int n, p, n1, n2;
	fstream finout;
	finout.open("stock.txt", ios::in);
	while (finout >> name >> n >> p >> n1 >> n2)
	{
		cout << "Medicine name:" << name << "			Stock available:" << n << "\n";
	}
	finout.close();
}
void update_stock()
{
	int n;
	char ch;
	do
	{
		cout << "1.)Enter 1 to renew stock.\n";
		cout << "2.)Enter 2 to add new Medicine.\n";
		cout << "Enter your choice:";
		cin >> n;
		switch (n)
		{
		case 1:
		{
			renew_stock();
			break;
		}
		case 2:
		{
			new_med();
			break;
		}
		}
		cout << "Do you want update your stock again?";
		cout << "Enter (y/n):";
		cin >> ch;
	} while (ch == 'y');
}
void renew_stock()
{
	char ch, ch1;
	int n;
	int n1, n2, n3, p, flag = 1, flag1;
	string name1, name;
	fstream fin;
	fstream fout;
	do
	{
		fin.open("stock.txt", ios::in);
		fout.open("temp.txt", ios::out);
		cout << "Enter the medicine whose stock you want to update:";
		cin >> name1;
		while (fin >> name >> n1 >> p >> n2 >> n3)
		{
			fout << name << " ";
			if (name1 == name)
			{
				cout << "Enter the new quantity of medicine:";
				cin >> n;
				fout << n << " ";
				flag = 0;
			}
			else
			{
				flag1 = -1;
				fout << n1 << " ";
			}
			fout << p << " ";
			fout << n2 << " ";
			fout << n3 << "\n";
		}
		fin.close();
		fout.close();
		remove("stock.txt");
		rename("temp.txt", "stock.txt");
		if (flag1 == -1 && flag != 0)
		{
			cout << name1 << " does not exist in stock.\n";
			cout << "Do you want to add this medicine?\n";
			cout << "Enter (y/n):";
			cin >> ch1;
			if (ch1 == 'y');
			{
				new_med();
			}

		}
		view_stock();
		cout << "Do you want to update stock of another medicine?";
		cout << "Enter (y/n):";
		cin >> ch;
	} while (ch == 'y');
}
void new_med()
{
	char ch;
	int n;
	int n1, n2, n3, p;
	string name1;
	fstream fout;
	do
	{
		fout.open("stock.txt", ios::out | ios::app);
		cout << "Enter the name of medicine you want to add:";
		cin >> name1;
		fout << name1 << " ";
		cout << "Enter the quantity of medicine:";
		cin >> n1;
		fout << n1 << " ";
		cout << "Enter the price of medicine:";
		cin >> p;
		fout << p << " ";
		cout << "Enter the expiry month:";
		cin >> n2;
		fout << n2 << " ";
		cout << "Enter the expiray year:";
		cin >> n3;
		fout << n3 << "\n";
		fout.close();
		view_stock();
		cout << "Do you want to add another medicine?";
		cout << "Enter (y/n):";
		cin >> ch;
	} while (ch == 'y');
}