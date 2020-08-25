#include<iostream.h>
#include<string.h>
#include<ctype.h>
#include<fstream.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
class item
{
	int ino,price;
	char iname[20];
	public:
	void additem();
	void dispitem();
	void changeprice();
	void removeitem();
	int getprice() { return price; }
	char * getiname() { return iname; }
	int getino() { return ino; }
	void chngorderno(int orderno) { ino=orderno; }
};

class shop
{
	int sno,qty,billno,cost;
	char itemname[30];
	public:
		shop()
		{
			sno=0;
		}
		void getdata(int);
		void cal();
		void dispbill(int);
		void dispitem();
		void changeprice();
		int getbno();
		int getcost(){return cost;}
};

void cafe();
void create();
void readd();
void printbill();
void item::additem()
{
	int i=0,orderno=1;
	char pr[6];
	clrscr();
	ifstream fi;
	fi.open("item.dat");
	while(fi.read((char *)this,sizeof(item)))
	{
		orderno=ino+1;
	}
	fi.close();
	ofstream fo;
	fo.open("item.dat",ios::app|ios::binary);
	char ch;
	do
	{
		gotoxy(12,2+10*i);
		cout<<"Item no: "<<orderno;
		ino=orderno++;
		gotoxy(12,4+10*i);
		cout<<"Enter item name: ";
		gets(iname);
		gotoxy(12,6+10*i);
		cout<<"Enter price: ";
		cin>>pr;
		for(int j=0;pr[j]!='\0';j++)
		{
			if(!isdigit(pr[j]))
		{
			gotoxy(12,6+13*i);
			cout<<"Enter correct price.";
			getch();
			return;
		}
		}
		price=atoi(pr);
		fo.write((char *)this,sizeof(item));
		gotoxy(12,8+10*i++);
		cout<<"Enter Y to add more items: ";
		cin>>ch;
		if(i%2==0)
		{
			clrscr();
			i=0;
		}
	}while(ch=='Y'||ch=='y');
	fo.close();
}

void item::dispitem()
{
	clrscr();
	int x=11,y=10,recno=0;
	ifstream fi;
	fi.open("item.dat",ios::in|ios::binary);
	while(fi.read((char *)this,sizeof(item)))
	{
		if(recno%8==0)
		{
			if(recno!=0)
			{   x=11;
				y=10;
				getch();
			}
			clrscr();
			gotoxy(22,2);
			cout<<"###############################";
			gotoxy(22,3);
			cout<<"## C  A  F  E  T  A  R  I  A ##";
			gotoxy(22,4);
			cout<<"###############################";
			gotoxy(11,7);
			cout<<"====================================================";
			gotoxy(11,8);
			cout<<"| ITEM NO.		ITEM NAME		COST |";
			gotoxy(11,9);
			cout<<"====================================================";
		}
		recno++;
		gotoxy(11,y);
		cout<<"| "<<ino;
		gotoxy(33,y);
		cout<<iname;
		gotoxy(57,y);
		cout<<price;
		gotoxy(60,y++);
		cout<<"  |";
		gotoxy(x,y++);
		cout<<"----------------------------------------------------";
	}
	if(recno==0)
	{	gotoxy(x,y);
		cout<<"No items present. Please add some items.";
	}
	fi.close();
	getch();
}
void item::changeprice()
{
	clrscr();
	char itemname[20];
	int iprice,pos=0;
	char pr[6];
	fstream f;
	f.open("item.dat",ios::in|ios::out|ios::binary);
	gotoxy(12,4);
	cout<<"Enter item name of price to be changed: ";
	gets(itemname);
	gotoxy(12,6);
	cout<<"Enter new price: ";
	cin>>pr;

	for(int i=0;pr[i]!='\0';i++)
	{
		if(!isdigit(pr[i]))
		{
			gotoxy(12,9);
			cout<<"Enter correct price.";
			getch();
			return;
		}
	}
	iprice=atoi(pr);
	gotoxy(12,9);
	while(f.read((char *)this,sizeof(item)))
	{
		if(strcmpi(itemname,this->iname)==0)
		{
			this->price=iprice;
			pos=f.tellg();
			f.seekp(pos-sizeof(item),ios::beg);
			f.write((char *)this,sizeof(item));
		}
	}
	if(pos==0)
	{
		cout<<"Wrong item name entered.Enter correct item name.";
		getch();
		changeprice();
	}
	f.close();
}

void item::removeitem()
{
	clrscr();
	ifstream fi;
	ofstream fo;
	fi.open("item.dat",ios::in|ios::binary);
	fo.open("change.dat",ios::out|ios::binary);
	int orderno=0,flag=0;
	char itemname[20];
	gotoxy(12,6);
	cout<<"Enter item name to be removed: ";
	gets(itemname);

	while(fi.read((char *)this,sizeof(item)))
	{
		if(strcmpi(this->iname,itemname)==0)
			flag=1;
		else
		{
			++orderno;
			ino=orderno;
			fo.write((char *)this,sizeof(item));
		}
	}
	gotoxy(12,9);
	if(flag==0)
		cout<<"Item not present.";
	else
		cout<<itemname<<" deleted.";
	getch();
	fi.close();
	fo.close();
	remove("item.dat");
	rename("change.dat","item.dat");
}

void shop::getdata(int bno)
{
	char qt[6];
	cout<<"\n\n\t\tEnter item name: ";
	gets(itemname);
	cout<<"\n\t\tEnter quantity: ";
	cin>>qt;
	for(int i=0;qt[i]!='\0';i++)
	{
		if(!isdigit(qt[i]))
		{
			cout<<"\n\n\t\tEnter correct quantity.";
			getch();
			cafe();
		}
	}
	qty=atoi(qt);
	cal();
	sno++;
	billno=bno;
}

void shop::cal()
{
	cost=0;
	ifstream fi;
	fi.open("item.dat",ios::in|ios::binary);
	item itm;
	if(isdigit(qty)==0)
	{
		while(fi.read((char *)&itm,sizeof(itm)))
		{

			if(strcmpi(itemname,itm.getiname())==0)
			{
			cost=itm.getprice()*qty;
			}
		}
		if(cost==0)
		{
			cout<<"\n\t\tWrong input";
			getdata(billno);
		}
	}
	else
	{
		cout<<"\n\t\tWrong input";
		getdata(billno);
	}
}

void shop::dispbill(int i)
{
	gotoxy(18,12+i);
	cout<<itemname;
	gotoxy(33,12+i);
	cout<<qty;
	gotoxy(49,12+i);
	cout<<cost;
}
void shop::dispitem()
{
	cout<<"\n\t\t"<<billno<<"	"<<itemname<<"		"<<qty<<"		"<<cost;
}
int shop::getbno()
{
	return(billno);
}

void cafe()
{
	char a;
	shop s;
	item i;
	do
	{   clrscr();
		gotoxy(22,2);
		cout<<"###############################";
		gotoxy(22,3);
		cout<<"## C  A  F  E  T  A  R  I  A ##";
		gotoxy(22,4);
		cout<<"###############################";
		gotoxy(24,8);
		cout<<"01.Display Menu";
		gotoxy(24,10);
		cout<<"02.Order Items";
		gotoxy(24,12);
		cout<<"03.Display Items Ordered";
		gotoxy(24,14);
		cout<<"04.Print Bill";
		gotoxy(24,16);
		cout<<"05.Exit";
		gotoxy(24,19);

		cout<<"Enter The Choice: ";
		cin>>a;
		switch(a)
		{
			case '1':i.dispitem();break;
			case '2':create();break;
			case '3':readd(); break;
			case '4':printbill();break;
			case '5':gotoxy(24,24);cout<<"E X I T I N G !!";break;
			default:gotoxy(24,22);cout<<"Wrong Choice";
		}
	}while (a!='5');
	getch();
	exit(1);
}

void create()
{
	clrscr();
	char ch;
	shop s;
	int bno,flag=0;
	ifstream fin;
	fin.open("shop.dat",ios::in|ios::binary);
	while(fin.read((char*)&s,sizeof s))
	{
		flag=1;
	}
	if(flag==1)

	{
		fin.seekg(-sizeof(s),ios::end);
		fin.read((char*)&s,sizeof s);
		bno=s.getbno();
		bno++;
	}
	else
	{
		bno=1;
	}
	ofstream fout;
	fout.open("shop.dat",ios::app|ios::binary);
	do
	{
		s.getdata(bno);
		fout.write((char*)&s,sizeof s);
		cout<<"\n\t\tEnter Y to continue\n\t\t";
		ch=getche();
	}while(toupper(ch)=='Y');
	fout.close();
	getch();
	cafe();
}

void readd()
{
	clrscr();
	ifstream fin;
	shop s1;

	fin.open("shop.dat",ios::in|ios::binary);

	int bno;

	gotoxy(16,3);
	cout<<"==============================================";
	gotoxy(16,4);
	cout<<"Billno	Item Name	Quantity	Cost";
	gotoxy(16,5);
	cout<<"==============================================";
	while (fin.read((char*)&s1,sizeof s1))
	{
		s1.dispitem();
	}
	fin.close();
	getch();
	cafe();
}
void printbill()
{
	clrscr();
	ifstream fin;
	fin.open("shop.dat",ios::in|ios::binary);
	shop s1;
	int mybno,flag=0,totcost=0,i=0;
	char bn[6];
	gotoxy(12,4);
	cout<<"Enter bill number to be printed: ";
	cin>>bn;

	for(int j=0;bn[j]!='\0';j++)
	{
		if(!isdigit(bn[i]))
		{
			gotoxy(12,7);
			cout<<"Enter correct bill number.";
			getch();
			return;
		}
	}
	mybno=atoi(bn);
	while (fin.read((char *)&s1,sizeof (s1)))
	{
		if(mybno==s1.getbno())
		{
			if(flag==0)
			{

				gotoxy(16,8);
				cout<<"Billno : "<<s1.getbno();
				gotoxy(16,10);
				cout<<"=======================================";
				gotoxy(16,11);
				cout<<"| Item Name	Quantity	Cost |";
				gotoxy(16,12);
				cout<<"=======================================";
			}
			i++;
			s1.dispbill(i);
			flag=1;
			totcost+=s1.getcost();
		}
	}

	if(flag==0)
	{
		gotoxy(16,8);
		cout<<"Bill Number "<<mybno<<" not avilable";
	}
	cout<<"\n\n\t\tYou have to pay Rupees: "<<totcost;
	getch();
	cafe();
}

void main()
{
	clrscr();
	char USERNAME[20]="VRIDDHI";
	char PASSWORD[20]="123456";
	char p[20],u[20];
	gotoxy(20,5);
	cout<<"Enter username: ";
	cin>>u;
	gotoxy(20,7);
	cout<<"Enter password: ";
	char pwd;
	int i=0;
	
	do
	{
		pwd=getch();
		if(pwd!=13)
		{
			 p[i++]=pwd;
			  gotoxy(35+i,7);
			  cout<<"*";
		}
	}while(pwd!=13);
	p[i]='\0';

	if(strcmpi(u,USERNAME)!=0||strcmp(p,PASSWORD)!=0)
	{
		gotoxy(20,11);
		cout<<"Invalid UserID or password.";
		getch();
		exit(1);
	}
	item it;
	char a;
	do
	{
		clrscr();
		gotoxy(16,4);
		cout<<"#########################################";
		gotoxy(16,5);
		cout<<"## A D M I N I S T R A T O R   M E N U ##";
		gotoxy(16,6);
		cout<<"#########################################";
		gotoxy(24,10);
		cout<<"01.Add item";
		gotoxy(24,12);
		cout<<"02.Display item detail";
		gotoxy(24,14);
		cout<<"03.Change item price";
		gotoxy(24,16);
		cout<<"04.Remove item";
		gotoxy(24,18);
		cout<<"05.Go to Cafetaria";
		gotoxy(24,20);
		cout<<"06.Exit";

		gotoxy(24,23);
		cout<<"Enter your choice(1-6): ";
		cin>>a;
		switch(a)
		{
			case '1':it.additem();break;
			case '2':it.dispitem();break;
			case '3':it.changeprice();break;
			case '4':it.removeitem();break;
			case '5':cafe();break;
			case '6':gotoxy(24,25);cout<<"E X I T I N G !!";getch();break;
			default:gotoxy(24,23);cout<<"Wrong Choice entered.Enter choice between 1-5.";getch();
		}
   }while(a!='6');
}
