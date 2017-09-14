#pragma once
#include <string>
#include<vector>
#include<iostream>
#include<map>
#include<limits>

using namespace std;


class Book {
private:
	string isbn;
	string title;
	string author;
	int edition = -1; //a positive integer
	string public_date;
    string department;

	map<string, float> type_price; // store the price for each type

public:
	Book(string i, string t): isbn(i), title(t){}
	Book(const Book &b){isbn = b.isbn; title = b.title; author = b.author; edition = b.edition; public_date = b.public_date;}

	void set_isbn(string s) { isbn = s; }
	void set_title(string s) { title = s; }
	void set_author(string s) { author = s; }
	void set_edition(int i) { edition = i; }
	void set_date(string s) { public_date = s; }
	void set_type_price(float price, string type) { type_price[type] = price; }
 
	string get_isbn() { return isbn; }
	string get_title() { return title; }
	string get_author() { return author; }
	int get_edition() { return edition; }
	string get_date() { return public_date; }
	float get_price(string type){return type_price[type];}
    
    float get_min_price(){
		float min = numeric_limits<float>::max();
		
		for(map<string,float>::iterator it = type_price.begin(); it != type_price.end(); ++it){
			if(it->second < min){
				min = it->second;
            }
		}
		if(min == numeric_limits<float>::max()) // it means no cost associated with book
			min = 0;
		return min;
	}
    
	float get_max_price(){
		float max = 0;
		
		for(auto it = type_price.begin(); it != type_price.end(); ++it){
			if(it->second > max)
				max = it->second;
		}
		return max;
	}
	
	void print_price_all(){
		for(auto it = type_price.begin(); it != type_price.end(); ++it){
			cout<<"type: "<<it->first << "  price: " <<it->second << endl;
		}
	}
    

	
    void print_book()
    {
        cout<<"ISBN: " << isbn <<endl
            <<"title: " << title << endl
            <<"author: " << author <<endl
            <<"public_date: " << public_date << endl;
        if(edition > 0)
            cout<<"edition: " << edition << endl;
        else
            cout<<"edition: " << endl;
        print_price_all();
        cout << endl;
    }
};



int find_book_index(vector<Book> books, string isbn);



class Course {
private:
	string department;
	int number; //three digits
	string name;

	map<int, vector<pair<string, string>>> sec_books;  //store the books for each section, section, isbn, required or optional
	
public:
	Course(string c, int num, string n):department(c), number(num), name(n){}

	string get_department(){return department;}
	int get_number(){return number;}
	
    bool contain_book(int section, string isbn)
    {
        for(size_t i = 0; i < sec_books[section].size(); ++i){
            if(sec_books[section].at(i).first.compare(isbn) == 0)
                return true;
        }
        return false;
    }
	
	void set_book(int section, string isbn, string r_or_q)
	{
		sec_books[section].push_back(make_pair(isbn, r_or_q));
	}
	
	void print_books()
	{
		string s;
		for(auto it = sec_books.begin(); it != sec_books.end(); it++){
			for(size_t i = 0; i < it->second.size(); ++i){
				cout << "section: " << it->first << " ISBN: " << it->second.at(i).first <<" (R or O): "<< it->second.at(i).second<<endl << endl;
			}
		}
	}
    
    void print_books(vector<Book> bs)
    {
        for(auto it = sec_books.begin(); it != sec_books.end(); it++){
            cout << "section: " << it->first << endl;
            
            for(size_t i = 0; i < it->second.size(); ++i){
                int book_index = find_book_index(bs, it->second.at(i).first);
                if(book_index >= 0){
                    bs.at(book_index).print_book();
                    cout <<"(Required or Optional): "<< it->second.at(i).second << endl << endl;
                }
            }
        }
    }
	
	void print_books_sec(int sec)
	{
		if(sec_books.find(sec) != sec_books.end()){
			for(size_t i = 0; i < sec_books[sec].size(); ++i){
				cout<<"ISBN: "<<sec_books[sec][i].first <<" (Required or Optional): " <<sec_books[sec][i].second << endl;
			}
            cout << endl;
		}
			
	}
    
    void print_books_department(vector<Book>& bs)
    {
        cout<< "Department: " << department <<" number: " << number <<" name: " << name <<endl;
        
        for(auto it = sec_books.begin(); it != sec_books.end(); it++){
            for(size_t i = 0; i < it->second.size(); ++i){
                int book_index = find_book_index(bs, it->second.at(i).first);
                if(book_index >= 0){
                    bs.at(book_index).print_book();
                    cout <<" Required or Optional: " << it->second.at(i).second << endl << endl;
                }
            }
        }
    }
    
    void print_course()
    {
        cout<<"course: " << department <<" " << number <<"    " << name << endl;
    }
    
    float get_min_section_cost(vector<Book>& bs, int section)
    {
        float price = 0;
        
        if(sec_books.find(section) != sec_books.end()){
            for(size_t i = 0; i < sec_books[section].size(); ++i){
        //for(auto it = sec_books.begin(); it != sec_books.end(); it++){
            ///for(size_t i = 0; i < it->second.size(); ++i){
                if(sec_books[section].at(i).second == "R"){
                    int book_index = find_book_index(bs, sec_books[section].at(i).first);
                    if(book_index >= 0){
                        price += bs.at(book_index).get_min_price();
                        //bs.at(book_index).print_book();
                    }
                }
            }
        }
        return price;
    }
    
    float get_max_section_cost(vector<Book>& bs, int section)
    {
        float price = 0;
        
        if(sec_books.find(section) != sec_books.end()){
            for(size_t i = 0; i < sec_books[section].size(); ++i){
                int book_index = find_book_index(bs, sec_books[section].at(i).first);
                if(book_index >= 0){
                    price += bs.at(book_index).get_max_price();
                    //bs.at(book_index).print_book();
                }
            }
        }
        return price;
    }
	
	float get_min_course_cost(vector<Book>& bs)
	{
		float cost = 0;
		
		for(auto it = sec_books.begin(); it != sec_books.end(); ++it){
			cost += get_min_section_cost(bs, it->first);
		}
		
		return cost;
	}
    
    float get_max_course_cost(vector<Book>& bs)
    {
        float cost = 0;
        
        for(auto it = sec_books.begin(); it != sec_books.end(); ++it){
            cost += get_max_section_cost(bs, it->first);
        }
        
        return cost;
    }
    
    int count_section(vector<Book>& bs, string max_or_min)
    {
        int count = 0;
        
        for(auto it = sec_books.begin(); it != sec_books.end(); ++it){
            for(size_t i =0; i < it->second.size(); ++i){
                if(max_or_min == "min"){                //count the number of section for min cost
                    if(it->second.at(i).second =="R"){  //only count required book
                        int book_index = find_book_index(bs, it->second.at(i).first);
                        if(book_index >= 0 && bs.at(book_index).get_min_price() > 0){
                            ++count;
                            break;
                        }
                    }
                }else{      //count the number of section for max cost
                    int book_index = find_book_index(bs, it->second.at(i).first);
                    if(book_index >= 0 && bs.at(book_index).get_min_price() > 0){
                        ++count;
                        break;
                    }
                }
            }
        }
        return count;
    }
};



int find_book_index(vector<Book> books, string isbn);

vector<int> find_department_index(vector<Course> courses, string department);

int find_course_index(vector<Course> courses, string department, int num);


